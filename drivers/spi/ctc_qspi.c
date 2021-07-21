#include <dm.h>
#include <errno.h>
#include <common.h>
#include <spi.h>
#include <malloc.h>
#include <asm/io.h>
#include <fdtdec.h>
#include <ctc_qspi.h>
#include <asm/arch/cpu.h>
#include <asm/arch/sysctl.h>
#include <spi-mem.h>

DECLARE_GLOBAL_DATA_PTR;

//#define CTC_DEBUG 1

#ifndef CONFIG_DM_SPI
struct ctc_qspi {
	struct spi_slave slave;
	struct ctc_qspi_priv priv;
};
#endif

static int reg_write_mask(void __iomem *addr, u32 value, u32 mask)
{
	uint temp;

	temp = readl(addr);
	temp &= ~mask;
	temp |= value;
	writel(temp, addr);
	
	return 0;
}

static int ctc_reg_read(struct ctc_qspi_priv *priv, u32 reg, u32* value)
{
    *value = readl(priv->virtbase + reg);

    return *value;
}

static int ctc_reg_write(struct ctc_qspi_priv *priv, u32 reg, u32 value)
{
	writel(value, priv->virtbase + reg);

	return 0;
}

static int ctc_reg_write_mask(struct ctc_qspi_priv *priv, u32 reg, u32 value, u32 mask)
{
    uint temp;
	
    ctc_reg_read(priv, reg, &temp);
    temp &= ~mask;
    temp |= value;
    ctc_reg_write(priv, reg, temp);

    return 0;
}

static int ctc_write_tx_buf(struct ctc_qspi_priv *priv, u8 offset, u32 value)
{
	writel(value, priv->virtbase + CTC_QSPI_TX_BUFF + offset);

	return 0;
}

static void ctc_fill_tx_buf(struct ctc_qspi_priv *priv, u8* offset, u8* buf, u32 len)
{
    int i = 0;
    u8 off = *offset;

    while(i < len)
    {
        if(buf && (buf+i))
        {
            priv->tx_entry |= buf[i]<<(off % 4)*8;
        }
        
        if(off % 4 == 0)
        {
            ctc_write_tx_buf(priv, off, priv->tx_entry);
            priv->tx_entry = 0;
        }
        i++;
        off--;
    }
	
    *offset = off;
}

static int ctc_read_rx_buf(struct ctc_qspi_priv *priv, u8 offset, u8* value)
{
	*value = readb(priv->virtbase + CTC_QSPI_RX_BUFF + offset);

	return 0;
}

static void ctc_extra_rx_buf(struct ctc_qspi_priv *priv, u8 offset, u8* buf, u32 len)
{
    int i = 0;

    while(i < len) {
        ctc_read_rx_buf(priv, offset, &buf[i++]);
        offset--;
    }
}

static void ctc_select_qspi_mode(struct ctc_qspi_priv *priv, u8 qspi_mode)
{

	if((qspi_mode == PIO_MODE1) || (qspi_mode == PIO_MODE2)){
		reg_write_mask(&sysctl_base->SysQspiBootCfg0, 0x0, 0x3);
		ctc_reg_write_mask(priv, SPI_MODE,  priv->pio, 0xffffffff);
	}
	else if(qspi_mode == PP_MODE){
		reg_write_mask(&sysctl_base->SysQspiBootCfg0, 0x0, 0x3);
		ctc_reg_write_mask(priv, SPI_MODE,  0x100, 0xffffffff);
	}
	else if(qspi_mode == BOOT_MODE){
		reg_write_mask(&sysctl_base->SysQspiBootCfg0, 0x0, 0x3);
		ctc_reg_write_mask(priv, SPI_MODE,  0x0, 0xffffffff);
		reg_write_mask(&sysctl_base->SysQspiBootCfg0, 0x1, 0x3);
	}else if(qspi_mode == XIP_MODE){
		reg_write_mask(&sysctl_base->SysQspiBootCfg0, 0x0, 0x3);
		ctc_reg_write_mask(priv, SPI_MODE,  0x0, 0xffffffff);
		reg_write_mask(&sysctl_base->SysQspiBootCfg0, 0x2, 0x3);
	}
}

static void ctc_stepx_conf_init(struct ctc_qspi_priv *priv)
{
	priv->step = 0;
	ctc_reg_write_mask(priv, PIO_STEP0_CONF(priv->pio), 0, 0xffffffff);
	ctc_reg_write_mask(priv, PIO_STEP1_CONF(priv->pio), 0, 0xffffffff);
	ctc_reg_write_mask(priv, PIO_STEP2_CONF(priv->pio), 0, 0xffffffff);
	ctc_reg_write_mask(priv, PIO_STEP3_CONF(priv->pio), 0, 0xffffffff);
}

static void ctc_qspi_pio_ctrl(struct ctc_qspi_priv *priv)
{
	u32 ctrl = 0;

	ctrl = CTRL_IDLE_CYCLE(priv->idlecycle) | CTRL_PRE_CYCLE(priv->precycle)
	        | CTRL_POST_CYCLE(priv->postcycle) | CTRL_SCLK_DEFAULT(priv->sclkdef)
	        | CTRL_SOUT3_DEFAULT(priv->sout3def) | CTRL_SOUT2_DEFAULT(priv->sout2def) 
	        | CTRL_SOUT1_DEFAULT(priv->sout1def) | CTRL_CS(priv->cs) 
	        | CTRL_DIV_SCLK(priv->clkdiv);
	
	ctc_reg_write_mask(priv, PIO_CTRL(priv->pio), ctrl, 0xffffffff);
}

static void ctc_stepx_conf(struct ctc_qspi_priv *priv, u8 lanes, u32 bytes)
{
	u32 cycle=0;
	u32 stepx_conf = 0;
	 
	cycle = (bytes*8)/lanes;

	if(cycle == 0)
	return;

	stepx_conf = (0xd << 20) | (lanes << 16) | (cycle);

	/* only write data and read data support qual mode and dual mode.
	* For read data mode, step3 is used for receive data.
	* For write data mode, step2 is used for transmit data.
	*/
	if(priv->step == 0){
		ctc_reg_write_mask(priv, PIO_STEP0_CONF(priv->pio), stepx_conf, 0xffffffff);
		priv->step++;
	}else if(priv->step == 1){
		ctc_reg_write_mask(priv, PIO_STEP1_CONF(priv->pio), stepx_conf, 0xffffffff);
		priv->step++;
	}else if(priv->step == 2){
		ctc_reg_write_mask(priv, PIO_STEP2_CONF(priv->pio), stepx_conf, 0xffffffff);
		priv->step++;
	}else if(priv->step == 3){
		ctc_reg_write_mask(priv, PIO_STEP3_CONF(priv->pio), stepx_conf, 0xffffffff);
		priv->step++;
	}
}

static int ctc_transfer_for_PIO(struct ctc_qspi_priv *priv)
{
	u8   offset;
	u8   msg_len=0;
	u32 temp;
	u32 timeout = 0;
	
	ctc_select_qspi_mode(priv, priv->pio);
	ctc_qspi_pio_ctrl(priv);

	msg_len = priv->xfer[0].len + priv->xfer[1].len;
	offset = msg_len - 1;
	priv->tx_entry = 0;
	ctc_fill_tx_buf(priv,  &offset, priv->xfer[0].tx_buf, priv->xfer[0].len); 
	if(priv->xfer[1].tx_buf){
		ctc_fill_tx_buf(priv,  &offset, priv->xfer[1].tx_buf, priv->xfer[1].len); 
	}else{
		ctc_fill_tx_buf(priv,  &offset, priv->xfer[1].rx_buf, priv->xfer[1].len); 
	}
	
	ctc_stepx_conf_init(priv);
	ctc_stepx_conf(priv, 1, priv->xfer[0].len);
	ctc_stepx_conf(priv, 1, priv->xfer[1].len);

	ctc_reg_write_mask(priv, PIO_GO(priv->pio), 0x01, 0xffffffff);

	while(ctc_reg_read(priv, PIO_GO(priv->pio), &temp) & 0x1){
		if(timeout++ > TIMEOUT_COUNT)
			break;
		udelay(1);
	}

	if(priv->xfer[1].rx_buf){
		ctc_extra_rx_buf(priv, priv->xfer[1].len-1, 
			priv->xfer[1].rx_buf, priv->xfer[1].len);
	}

	return 0;
}

int ctc_do_qspi_xfer(struct ctc_qspi_priv *priv, unsigned int bitlen,
		const void *dout, void *din, unsigned long flags)
{
	u32 bytes = bitlen >>3;

	if ((flags & SPI_XFER_BEGIN) && (flags & SPI_XFER_END)){
		memset(priv->xfer, 0, sizeof(priv->xfer));
		priv->xfer[0].tx_buf = (void *)dout;
		priv->xfer[0].len = bytes;
	}else if(flags & SPI_XFER_BEGIN){
		memset(priv->xfer, 0, sizeof(priv->xfer));
		if(dout){
			priv->xfer[0].tx_buf = (void *)dout;
		}else{
			priv->xfer[0].rx_buf = din;
		}
		priv->xfer[0].len = bytes;		
	}else if(flags & SPI_XFER_END){
		if(dout){
			priv->xfer[1].tx_buf = (void *)dout;
		}else{
			priv->xfer[1].rx_buf = din;
		}
		priv->xfer[1].len = bytes;
	}

	if(flags & SPI_XFER_END){
		ctc_transfer_for_PIO(priv);
	}
	
	return 0;
}


#ifndef CONFIG_DM_SPI

static inline struct ctc_qspi *ctc_to_qspi_spi(struct spi_slave *slave)
{
	return container_of(slave, struct ctc_qspi, slave);
}

static int ctc_qspi_set_speed(struct ctc_qspi_priv *priv, uint speed)
{	
	u8 clkdiv;

	priv->speed_hz = speed;
	clkdiv = (priv->bus_clk/(priv->speed_hz*2));
	if(!clkdiv){
		clkdiv = 1;
	}
	priv->clkdiv = clkdiv;

	return 0;
}

static int ctc_qspi_set_mode(struct ctc_qspi_priv *priv, uint mode)
{
	/* only support mode0 and mode3, default mode3 */
	if ((mode & 0x3) == SPI_MODE_0){
		priv->sclkdef = 0;
	}   
	else if ((mode & 0x3) == SPI_MODE_3){
		priv->sclkdef = 1;
	} 
	else{
		priv->sclkdef = 1;
	}

	return 0;
}


struct spi_slave * spi_setup_slave(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int mode)
{
	struct ctc_qspi *qspi;

	qspi = spi_alloc_slave(struct ctc_qspi, bus, cs);
	if (!qspi)
		return NULL;

	qspi->priv.virtbase = (void __iomem *)QSPI_REGS_BASE;
	qspi->priv.bus_clk = CTC_QSPI_DEFAULT_SCK_FREQ;
	qspi->priv.num_chipselect = QSPI_NUM_CS;
	qspi->priv.idlecycle = CTC_IDLE_CYCLE;
	qspi->priv.precycle = CTC_PRE_CYCLE;
	qspi->priv.postcycle = CTC_POST_CYCLE;
	qspi->priv.speed_hz = max_hz;
	qspi->priv.qspi_mode = mode;
	qspi->priv.sout1def = 1 ;
	qspi->priv.sout2def = 1 ;
	qspi->priv.sout3def = 1 ;
	qspi->priv.cs = (1 << cs);
	
	qspi->slave.max_read_size = CTC_QSPI_RX_BUFFER_SIZE-6;        //6 = 1 cmd byte + 4 address bytes + 1 dummy byte
	qspi->slave.max_write_size = CTC_QSPI_TX_BUFFER_SIZE-6;

	if((qspi->priv.qspi_mode == PIO_MODE1) || (qspi->priv.qspi_mode == PIO_MODE2))
	{
		qspi->priv.pio = qspi->priv.qspi_mode;
	}
	else /* if use xip mode or boot mode for read data,  then use pio mode1*/
	{
		qspi->priv.pio = PIO_MODE1;
	}
	
	ctc_qspi_set_speed(&qspi->priv, max_hz);
	ctc_qspi_set_mode(&qspi->priv, qspi->priv.qspi_mode);

	qspi->slave.bus = bus;

	return &qspi->slave;	
}

void spi_free_slave(struct spi_slave *slave)
{
	struct ctc_qspi *qspi = ctc_to_qspi_spi(slave);

	free(qspi);
}


void spi_init(void)
{
	/*nothing to do*/
}

int spi_claim_bus(struct spi_slave *slave)
{
	/*nothing to do*/
	return 0;
}

void spi_release_bus(struct spi_slave *slave)
{
	/*nothing to do*/
}

int spi_xfer(struct spi_slave *slave, unsigned int bitlen, const void *dout,
		void *din, unsigned long flags)
{
	struct ctc_qspi *qspi = ctc_to_qspi_spi(slave);
	return ctc_do_qspi_xfer(&qspi->priv, bitlen, dout, din, flags);
}
#else
static int ctc_qspi_claim_bus(struct udevice *dev)
{
	struct udevice *bus = dev->parent;
	struct dm_spi_slave_platdata *slave_plat = dev_get_parent_platdata(dev);
	struct ctc_qspi_priv *priv;

	bus = dev->parent;
	priv = dev_get_priv(bus);

	priv->cs  = (1 << slave_plat->cs);
	priv->sout1def = 1 ;
	priv->sout2def = 1 ;
	priv->sout3def = 1 ;

	return 0;
}

static int ctc_qspi_release_bus(struct udevice *dev)
{
	return 0;
}

static int ctc_qspi_xfer(struct udevice *dev, unsigned int bitlen,
		const void *dout, void *din, unsigned long flags)
{
	struct ctc_qspi_priv *priv;
	struct udevice *bus;

	bus = dev->parent;
	priv = dev_get_priv(bus);

	return ctc_do_qspi_xfer(priv, bitlen, dout, din, flags);
}

static int ctc_qspi_dm_set_speed(struct udevice *bus, uint speed)
{
	u8 clkdiv;
	struct ctc_qspi_priv *priv;

	priv = dev_get_priv(bus);

	if(speed > MAX_SPEED_HZ){
		printf("Error!The max speed must be under 25Mhz, using default speed 25Mhz!\n");
		priv->speed_hz = MAX_SPEED_HZ;
		clkdiv = (priv->bus_clk/(priv->speed_hz*2));
	}else{
		priv->speed_hz = speed;
		clkdiv = (priv->bus_clk/(priv->speed_hz*2));
	}
	if(!clkdiv){
		clkdiv = 1;
	}
	priv->clkdiv = clkdiv;

	return 0;
}

static int ctc_qspi_dm_set_mode(struct udevice *bus, uint mode)
{
	struct ctc_qspi_priv *priv;

	priv = dev_get_priv(bus);

	/* only support mode0 and mode3, default mode3 */
	if ((mode & 0x3) == SPI_MODE_0){
		priv->sclkdef = 0;
	}   
	else if ((mode & 0x3) == SPI_MODE_3){
		priv->sclkdef = 1;
	} 
	else{
		priv->sclkdef = 1;
	}

	return 0;
}

static int ctc_qspi_probe(struct udevice *bus)
{
	struct ctc_qspi_platdata *plat = dev_get_platdata(bus);
	struct ctc_qspi_priv *priv = dev_get_priv(bus);	
	struct dm_spi_bus *dm_spi_bus;

	dm_spi_bus = bus->uclass_priv;
	dm_spi_bus->max_hz = plat->speed_hz;

	priv->virtbase = plat->virtbase;
	priv->bus_clk  = plat->bus_clk;
	priv->speed_hz = plat->speed_hz;
	priv->num_chipselect = plat->num_chipselect;
	priv->pio = plat->pio;
	priv->idlecycle = plat->idlecycle;
	priv->precycle = plat->precycle;
	priv->postcycle = plat->postcycle;
	priv->qspi_mode = plat->qspi_mode;
	
	return 0;
}

static int ctc_qspi_child_pre_probe(struct udevice *dev)
{
	struct spi_slave *slave = dev_get_parent_priv(dev);
	
	slave->max_read_size = CTC_QSPI_RX_BUFFER_SIZE-6;        //6 = 1 cmd byte + 4 address bytes + 1 dummy byte
	slave->max_write_size = CTC_QSPI_TX_BUFFER_SIZE-6;
		
	return 0;
}

static int ctc_qspi_ofdata_to_platdata(struct udevice *bus)
{
	fdt_addr_t addr;
	struct ctc_qspi_platdata *plat = bus->platdata;
	const void *blob = gd->fdt_blob;
	int node = dev_of_offset(bus);

	addr = dev_get_addr(bus);
	if (addr == FDT_ADDR_T_NONE) {
		debug("SPI: Can't get base address or size\n");
		return -ENOMEM;
	}
	plat->virtbase   = (void __iomem *)addr;
	plat->bus_clk    = fdtdec_get_int(blob, node, "pclk", CTC_QSPI_DEFAULT_SCK_FREQ);
	plat->num_chipselect = fdtdec_get_int(blob, node, "num-cs", 1);
	plat->idlecycle = fdtdec_get_int(blob, node, "idle-cycle", 2);
	plat->precycle = fdtdec_get_int(blob, node, "pre-cycle", 2);
	plat->postcycle = fdtdec_get_int(blob, node, "post-cycle", 1);
	plat->speed_hz   = fdtdec_get_int(blob, node, "spi-max-frequency", 100000);

	plat->qspi_mode = fdtdec_get_int(blob, node, "qspi-mode", 1);
	if((plat->qspi_mode == PIO_MODE1) || (plat->qspi_mode == PIO_MODE2))
	{
		plat->pio = plat->qspi_mode;
	}
	else /* if use xip mode or boot mode for read data,  then use pio mode1*/
	{
		plat->pio = PIO_MODE1;
	}

	return 0;
}

int ctc_qspi_adjust_op_size(struct spi_slave *slave, struct spi_mem_op *op)
{
	int cad_size = 0;

	cad_size = sizeof(op->cmd.opcode) + op->addr.nbytes + op->dummy.nbytes;
	if(op->data.dir == SPI_MEM_DATA_IN){
		if(op->data.nbytes>CTC_QSPI_RX_BUFFER_SIZE-cad_size){
			op->data.nbytes =  CTC_QSPI_RX_BUFFER_SIZE - cad_size;
		}
	}else{
		if(op->data.nbytes>CTC_QSPI_TX_BUFFER_SIZE-cad_size){
			op->data.nbytes =  CTC_QSPI_TX_BUFFER_SIZE - cad_size;
		}
	}
	return 0;
}

int ctc_qspi_exec_op(struct spi_slave *slave,
		       const struct spi_mem_op *op)
{
	return -ENOTSUPP;
}

static const struct spi_controller_mem_ops ctc_mem_ops = {
	.adjust_op_size = ctc_qspi_adjust_op_size,
	.exec_op = ctc_qspi_exec_op,
};

static const struct dm_spi_ops ctc_qspi_ops = {
	.claim_bus	 = ctc_qspi_claim_bus,
	.release_bus = ctc_qspi_release_bus,
	.xfer		 = ctc_qspi_xfer,
	.set_speed	 = ctc_qspi_dm_set_speed,
	.set_mode	 = ctc_qspi_dm_set_mode,
	.mem_ops       = &ctc_mem_ops,
};

static const struct udevice_id ctc_qspi_ids[] = {
	{ .compatible = "ctc, igdaxi001a-qspi" },
	{ }
};

U_BOOT_DRIVER(ctc_qspi) = {
	.name	   = "ctc_qspi",
	.id	       = UCLASS_SPI,
	.of_match  = ctc_qspi_ids,
	.ops	   = &ctc_qspi_ops,
	.ofdata_to_platdata       = ctc_qspi_ofdata_to_platdata,
	.platdata_auto_alloc_size = sizeof(struct ctc_qspi_platdata),
	.priv_auto_alloc_size     = sizeof(struct ctc_qspi_priv),
	.probe	                  = ctc_qspi_probe,
	.child_pre_probe         = ctc_qspi_child_pre_probe,
};
#endif

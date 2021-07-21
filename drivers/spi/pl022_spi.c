#include <dm.h>
#include <errno.h>
#include <common.h>
#include <spi.h>
#include <malloc.h>
#include <asm/io.h>
#include <fdtdec.h>
#include <pl022_spi.h>
#include <asm/arch/cpu.h>
#include <asm/arch/sysctl.h>
#include <linux/kernel.h>
#include <regmap.h>

DECLARE_GLOBAL_DATA_PTR;

#ifndef SPI_GpioMulti
#define SPI_GpioMulti 0x0
#endif

#ifndef CONFIG_DM_SPI
struct ctc_pl022_spi{
	struct spi_slave slave;
	struct pl022_spi_priv priv;
};
#endif

#ifndef CONFIG_DM_SPI
static inline struct ctc_pl022_spi *to_ctc_pl022_spi(struct spi_slave *slave)
{
	return container_of(slave, struct ctc_pl022_spi, slave);
}
#endif

static void readwrite(struct pl022_spi_priv *priv)
{
    	uint width=0;

	while ((readw(SSP_SR(priv->virtbase)) & SSP_SR_MASK_RNE)
	       && (priv->rx < priv->rx_end)) {
    		switch (priv->read) {
    			case READING_NULL:
    				readw(SSP_DR(priv->virtbase));
            			width = 1;
    				break;
    			case READING_U8:
    				*(u8 *) (priv->rx) = readw(SSP_DR(priv->virtbase)) & 0xFFU;
            			width = 1;
    				break;
    			case READING_U16:
    				*(u16 *) (priv->rx) = (u16) readw(SSP_DR(priv->virtbase));
            			width = 2;
    				break;
    			case READING_U32:
    				*(u32 *) (priv->rx) = readl(SSP_DR(priv->virtbase));
            			width = 4;
    				break;
    	}
    		priv->rx += width;
    		priv->exp_fifo_level--;
	}

	while ((priv->exp_fifo_level < priv->fifodepth)
	       && (priv->tx < priv->tx_end)) {
    		switch (priv->write) {
    			case WRITING_NULL:
    				writew(0x0, SSP_DR(priv->virtbase));
            			width = 1;
    				break;
    			case WRITING_U8:
    				writew(*(u8 *) (priv->tx), SSP_DR(priv->virtbase));
            			width = 1;
    				break;
    			case WRITING_U16:
    				writew((*(u16 *) (priv->tx)), SSP_DR(priv->virtbase));
            			width = 2;
    				break;
    			case WRITING_U32:
    				writel(*(u32 *) (priv->tx), SSP_DR(priv->virtbase));
            			width = 4;
    				break;
    		}
    		priv->tx += width;
    		priv->exp_fifo_level++;
		while ((readw(SSP_SR(priv->virtbase)) & SSP_SR_MASK_RNE)
    	       		&& (priv->rx < priv->rx_end)) {
        		switch (priv->read) {
        			case READING_NULL:
        				readw(SSP_DR(priv->virtbase));
                			width = 1;
        				break;
        			case READING_U8:
        				*(u8 *) (priv->rx) = readw(SSP_DR(priv->virtbase)) & 0xFFU;
                			width = 1;
        				break;
        			case READING_U16:
        				*(u16 *) (priv->rx) = (u16) readw(SSP_DR(priv->virtbase));
                			width = 2;
        				break;
        			case READING_U32:
        				*(u32 *) (priv->rx) = readl(SSP_DR(priv->virtbase));
                			width = 4;
        				break;
        		}
        		priv->rx += width;
        		priv->exp_fifo_level--;
    		}
    	}
}

static int pl022_cs_active(uint cs)
{
    	uint sysctrspi = 0xf0;

    	sysctrspi = 0xf0 | cs;
    	sysctrspi &= (~(1 << (cs+4)));
	
	writew(sysctrspi, &sysctl_base->SysSpiSelCfg);
	return 0;
}

static inline u32 spi_rate(u32 rate, u16 cpsdvsr, u16 scr)
{
	return rate / (cpsdvsr * (1 + scr));
}

/**
 * flush - flush the FIFO to reach a clean state
 * @pl022: SSP driver private data structure
 */
static int flush(struct pl022_spi_priv *priv)
{
    	uint timeout = 100;
    
    	while ((readw(SSP_SR(priv->virtbase)) & SSP_SR_MASK_BSY) && timeout--){
        	udelay(1);
    	}

	while (readw(SSP_SR(priv->virtbase)) & SSP_SR_MASK_RNE){
		readw(SSP_DR(priv->virtbase));
    	}   

	priv->exp_fifo_level = 0;

	return timeout;
}

static int pl022_cs_inactive(uint cs)
{
    	uint sysctrspi;

    	sysctrspi = 0xf0;
    
	writew(sysctrspi, &sysctl_base->SysSpiSelCfg);

	return 0;
}

static int pl022_xfer(struct pl022_spi_priv *ctc_priv, uint cs, unsigned int bitlen,
		const void *dout, void *din, unsigned long flags)
{
	struct pl022_spi_priv *priv = ctc_priv;
    	uint len = bitlen >>3;

	if (flags & SPI_XFER_BEGIN){
        	pl022_cs_active(cs);
    	} 

    	priv->tx = dout;
    	priv->tx_end = dout + len;
	priv->rx = din;
	priv->rx_end = din + len;   
    	priv->write = dout ? WRITING_U8 : WRITING_NULL;
    	priv->read  = din ? WRITING_U8 : READING_NULL;

	flush(priv);

    	while((priv->tx < priv->tx_end) || (priv->rx < priv->rx_end)){
        	readwrite(priv);
    	}

	if (flags & SPI_XFER_END){
       	 	pl022_cs_inactive(cs);
    	}  

	return 0;
}


#ifndef CONFIG_DM_SPI

static int ctc_pl022_spi_set_speed(struct ctc_pl022_spi *priv, uint speed)
{
	/* Lets calculate the frequency parameters */
	u16 cpsdvsr = CPSDVR_MIN, scr = SCR_MIN;
	u32 rate, max_tclk, min_tclk, best_freq = 0, best_cpsdvsr = 0, best_scr = 0, tmp, found = 0;
    	struct ctc_pl022_spi *ctc_pl022_priv = priv;
    
	rate = SPI_CLASSIC_RATE;
	/* cpsdvscr = 2 & scr 0 */
	max_tclk = spi_rate(rate, CPSDVR_MIN, SCR_MIN);
	/* cpsdvsr = 254 & scr = 255 */
	min_tclk = spi_rate(rate, CPSDVR_MAX, SCR_MAX);

	if (speed > max_tclk){
		return -EINVAL;
	}

	if (speed < min_tclk) {
		return -EINVAL;
	}

	/*
	 * best_freq will give closest possible available rate (<= requested
	 * freq) for all values of scr & cpsdvsr.
	 */
	while ((cpsdvsr <= CPSDVR_MAX) && !found) {
		while (scr <= SCR_MAX) {
			tmp = spi_rate(rate, cpsdvsr, scr);

			if (tmp > speed) {
				/* we need lower freq */
				scr++;
				continue;
			}

			/*
			 * If found exact value, mark found and break.
			 * If found more closer value, update and break.
			 */
			if (tmp > best_freq) {
				best_freq = tmp;
				best_cpsdvsr = cpsdvsr;
				best_scr = scr;

				if (tmp == speed)
					found = 1;
			}
			/*
			 * increased scr will give lower rates, which are not
			 * required
			 */
			break;
		}
		cpsdvsr += 2;
		scr = SCR_MIN;
	}

	ctc_pl022_priv->priv.clk_freq.cpsdvsr = (u8) (best_cpsdvsr & 0xFF);
	ctc_pl022_priv->priv.clk_freq.scr = (u8) (best_scr & 0xFF);
	
	return 0;
}


struct spi_slave * spi_setup_slave(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int mode)
{
	uint polarity, phase, loopback, bigend;
	u16 cr0, cr1, cpsr;
	struct ctc_pl022_spi *pl022_spi;
	int GpioMultiVal = 0;
	
	GpioMultiVal = readl(&sysctl_base->SysGpioMultiCtl);
	GpioMultiVal = (GpioMultiVal & 0xffff0000) | SPI_GpioMulti;
	writel(GpioMultiVal, &sysctl_base->SysGpioMultiCtl);

	pl022_spi = spi_alloc_slave(struct ctc_pl022_spi, bus, cs);
	if (!pl022_spi)
		return NULL;

	pl022_spi->priv.virtbase = (void __iomem *)SSP_REGS_BASE;
	pl022_spi->priv.fifodepth = 8;
	pl022_spi->slave.bus = bus;
	pl022_spi->slave.cs = cs;
	

	if (mode & SPI_CPOL){
		polarity = SSP_CLK_POL_IDLE_HIGH;
    	}   
    	else{
		polarity = SSP_CLK_POL_IDLE_LOW;
    	} 
    
	if (mode & SPI_CPHA){
        	phase = SSP_CLK_SECOND_EDGE;
    	}   
    	else{
        	phase = SSP_CLK_FIRST_EDGE;
    	}

	if (mode & SPI_LOOP){
		loopback = LOOPBACK_ENABLED;
    	}   
    	else{
        	loopback = LOOPBACK_DISABLED;
    	}
	if(mode & SPI_LSB_FIRST){
		bigend = LSB_FIRST_ENABLED;
	}
	else{
		bigend = LSB_FIRST_DISABLED;
	}

	ctc_pl022_spi_set_speed(pl022_spi, max_hz);
	
	cr0 = GEN_MASK_BITS(SSP_DATA_BITS_8, SSP_CR0_MASK_DSS, 0)	| \
	GEN_MASK_BITS(0, SSP_CR0_MASK_FRF, 4) | \
	GEN_MASK_BITS(polarity, SSP_CR0_MASK_SPO, 6) | \
	GEN_MASK_BITS(phase, SSP_CR0_MASK_SPH, 7) | \
	GEN_MASK_BITS(pl022_spi->priv.clk_freq.scr, SSP_CR0_MASK_SCR, 8);

    	cr1 = GEN_MASK_BITS(loopback, SSP_CR1_MASK_LBM, 0) | \
	GEN_MASK_BITS(SSP_ENABLED, SSP_CR1_MASK_SSE, 1) | \
	GEN_MASK_BITS(SSP_MASTER, SSP_CR1_MASK_MS, 2) | \
	GEN_MASK_BITS(bigend, SSP_CR1_MASK_BIGEND, 4);

    	cpsr = pl022_spi->priv.clk_freq.cpsdvsr;
    
    	writew(cr0, SSP_CR0(pl022_spi->priv.virtbase));
    	writew(cr1, SSP_CR1(pl022_spi->priv.virtbase));
	writew(cpsr, SSP_CPSR(pl022_spi->priv.virtbase));
	writew(DISABLE_ALL_INTERRUPTS, SSP_IMSC(pl022_spi->priv.virtbase));
	writew(CLEAR_ALL_INTERRUPTS, SSP_ICR(pl022_spi->priv.virtbase));

    	/* exit test mode */
    	writew(0, SSP_ITCR(pl022_spi->priv.virtbase));

	return &pl022_spi->slave;	
}

void spi_free_slave(struct spi_slave *slave)
{
	/*nothing to do*/
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
	struct ctc_pl022_spi *pl022_spi = (struct ctc_pl022_spi *)slave;
	return pl022_xfer(&pl022_spi->priv, slave->cs, bitlen, dout, din, flags);
}

#else
struct pl022_ssp_interface {
	enum ssp_interface pl022_iface;
};

/**
 * load_ssp_default_config - Load default configuration for SSP
 * @pl022: SSP driver private data structure
 */
static void load_ssp_default_config(u32 *addr)
{
	writew(DEFAULT_SSP_REG_CR0, SSP_CR0(addr));
	writew(DEFAULT_SSP_REG_CR1, SSP_CR1(addr));
	writew(DEFAULT_SSP_REG_DMACR, SSP_DMACR(addr));
	writew(DEFAULT_SSP_REG_CPSR, SSP_CPSR(addr));
	writew(DISABLE_ALL_INTERRUPTS, SSP_IMSC(addr));
	writew(CLEAR_ALL_INTERRUPTS, SSP_ICR(addr));
}

static int pl022_spi_claim_bus(struct udevice *dev)
{
    	uint polarity, phase, loopback, dma_rx, dma_tx;
	u16 cr0, cr1, dmacr, cpsr;
	struct pl022_spi_priv *priv;
	struct udevice *bus = dev->parent;
	struct dm_spi_slave_platdata *slave_plat = dev_get_parent_platdata(dev);
	struct pl022_ssp_interface *iface_plat;
	
	priv = dev_get_priv(bus);
	iface_plat = dev_get_parent_priv(bus);

	if (slave_plat->mode & SPI_CPOL){
		polarity = SSP_CLK_POL_IDLE_HIGH;
    	}   
    	else{
		polarity = SSP_CLK_POL_IDLE_LOW;
    	} 
    
	if (slave_plat->mode & SPI_CPHA){
        	phase = SSP_CLK_SECOND_EDGE;
    	}   
    	else{
        	phase = SSP_CLK_FIRST_EDGE;
    	}

	if (slave_plat->mode & SPI_LOOP){
		loopback = LOOPBACK_ENABLED;
    	}   
    	else{
        	loopback = LOOPBACK_DISABLED;
    	}
	
	cr0 = GEN_MASK_BITS(SSP_DATA_BITS_8, SSP_CR0_MASK_DSS, 0)	| \
	GEN_MASK_BITS(iface_plat->pl022_iface, SSP_CR0_MASK_FRF, 4) | \
	GEN_MASK_BITS(polarity, SSP_CR0_MASK_SPO, 6) | \
	GEN_MASK_BITS(phase, SSP_CR0_MASK_SPH, 7) | \
	GEN_MASK_BITS(priv->clk_freq.scr, SSP_CR0_MASK_SCR, 8);

    	cr1 = GEN_MASK_BITS(loopback, SSP_CR1_MASK_LBM, 0) | \
	GEN_MASK_BITS(SSP_ENABLED, SSP_CR1_MASK_SSE, 1) | \
	GEN_MASK_BITS(SSP_MASTER, SSP_CR1_MASK_MS, 2) | \
	GEN_MASK_BITS(DO_NOT_DRIVE_TX, SSP_CR1_MASK_SOD, 3);

    	if(priv->dma_rx){
        	dma_rx = SSP_DMA_ENABLED;
    	}
    	else{
        	dma_rx = SSP_DMA_DISABLED;
    	}

    	if(priv->dma_rx){
        	dma_tx = SSP_DMA_ENABLED;
    	}
    	else{
        	dma_tx = SSP_DMA_DISABLED;
    	}

	dmacr = GEN_MASK_BITS(dma_rx, SSP_DMACR_MASK_RXDMAE, 0) | \
	GEN_MASK_BITS(dma_tx, SSP_DMACR_MASK_TXDMAE, 1);
	
    	cpsr = priv->clk_freq.cpsdvsr;
    
    	writew(cr0, SSP_CR0(priv->virtbase));
    	writew(cr1, SSP_CR1(priv->virtbase));
	writew(dmacr, SSP_DMACR(priv->virtbase));
	writew(cpsr, SSP_CPSR(priv->virtbase));
	writew(DISABLE_ALL_INTERRUPTS, SSP_IMSC(priv->virtbase));
	writew(CLEAR_ALL_INTERRUPTS, SSP_ICR(priv->virtbase));

    	/* exit test mode */
    	writew(0, SSP_ITCR(priv->virtbase));
    
	return 0;
}

static int pl022_spi_release_bus(struct udevice *dev)
{
	struct pl022_spi_priv *priv;
	struct udevice *bus = dev->parent;

	priv = dev_get_priv(bus);

	/* nothing more to do - disable spi/ssp and power off */
	writew((readw(SSP_CR1(priv->virtbase)) &
		(~SSP_CR1_MASK_SSE)), SSP_CR1(priv->virtbase));
    
	return 0;
}

static int pl022_spi_xfer(struct udevice *dev, unsigned int bitlen,
		const void *dout, void *din, unsigned long flags)
{
	struct pl022_spi_priv *priv;
	struct udevice *bus;
	struct dm_spi_slave_platdata *slave_plat = dev_get_parent_platdata(dev);
	
	bus = dev->parent;
	priv = dev_get_priv(bus);
    
	pl022_xfer(priv, slave_plat->cs, bitlen, dout, din, flags);

	return 0;
}

static int pl022_spi_set_speed(struct udevice *bus, uint speed)
{
	/* Lets calculate the frequency parameters */
	u16 cpsdvsr = CPSDVR_MIN, scr = SCR_MIN;
	u32 rate, max_tclk, min_tclk, best_freq = 0, best_cpsdvsr = 0, best_scr = 0, tmp, found = 0;
    	struct pl022_spi_priv *priv;

    	priv = dev_get_priv(bus);
    
	rate = priv->bus_clk;
	/* cpsdvscr = 2 & scr 0 */
	max_tclk = spi_rate(rate, CPSDVR_MIN, SCR_MIN);
	/* cpsdvsr = 254 & scr = 255 */
	min_tclk = spi_rate(rate, CPSDVR_MAX, SCR_MAX);

	if (speed > max_tclk){
		return -EINVAL;
	}

	if (speed < min_tclk) {
		return -EINVAL;
	}

	/*
	 * best_freq will give closest possible available rate (<= requested
	 * freq) for all values of scr & cpsdvsr.
	 */
	while ((cpsdvsr <= CPSDVR_MAX) && !found) {
		while (scr <= SCR_MAX) {
			tmp = spi_rate(rate, cpsdvsr, scr);

			if (tmp > speed) {
				/* we need lower freq */
				scr++;
				continue;
			}

			/*
			 * If found exact value, mark found and break.
			 * If found more closer value, update and break.
			 */
			if (tmp > best_freq) {
				best_freq = tmp;
				best_cpsdvsr = cpsdvsr;
				best_scr = scr;

				if (tmp == speed)
					found = 1;
			}
			/*
			 * increased scr will give lower rates, which are not
			 * required
			 */
			break;
		}
		cpsdvsr += 2;
		scr = SCR_MIN;
	}

	priv->clk_freq.cpsdvsr = (u8) (best_cpsdvsr & 0xFF);
	priv->clk_freq.scr = (u8) (best_scr & 0xFF);
	
	return 0;
}

static int pl022_spi_set_mode(struct udevice *bus, uint mode)
{
    return 0;
}

static int pl022_spi_probe(struct udevice *bus)
{
	struct pl022_spi_platdata *plat = dev_get_platdata(bus);
	struct pl022_spi_priv *priv = dev_get_priv(bus);
	struct dm_spi_bus *dm_spi_bus;
	int GpioMultiVal = 0;

	dm_spi_bus = bus->uclass_priv;
	dm_spi_bus->max_hz = plat->speed_hz;

   	priv->dma_rx   = plat->dma_rx;
    	priv->dma_tx   = plat->dma_tx;
	priv->speed_hz = plat->speed_hz;
    	priv->virtbase = plat->virtbase;
    	priv->bus_clk  = plat->bus_clk;
	priv->num_chipselect = plat->num_chipselect;
	priv->flags = plat->flags;
    	priv->fifodepth = 8;

	GpioMultiVal = readl(&sysctl_base->SysGpioMultiCtl);
	GpioMultiVal = (GpioMultiVal & 0xffff0000) | SPI_GpioMulti;
	writel(GpioMultiVal, &sysctl_base->SysGpioMultiCtl);
	
	load_ssp_default_config(plat->virtbase);

	return 0;
}

static int pl022_spi_ofdata_to_platdata(struct udevice *bus)
{
    	fdt_addr_t addr;
	struct pl022_spi_platdata *plat = bus->platdata;
	const void *blob = gd->fdt_blob;
	int node = dev_of_offset(bus);

	addr = dev_get_addr(bus);
	if (addr == FDT_ADDR_T_NONE) {
		debug("SPI: Can't get base address or size\n");
		return -ENOMEM;
	}
	plat->virtbase = (void __iomem *)addr;
	plat->dma_rx = fdtdec_get_int(blob, node, "dma-rx", 0);
    	plat->dma_tx = fdtdec_get_int(blob, node, "dma-tx", 0);
   	plat->bus_clk = fdtdec_get_int(blob, node, "pclk", PL022_SPI_DEFAULT_SCK_FREQ);
	plat->num_chipselect = fdtdec_get_int(blob, node, "num-cs", 1);
	plat->speed_hz = fdtdec_get_int(blob, node, "spi-max-frequency", PL022_SPI_DEFAULT_SCK_FREQ);

	return 0;
}

static int pl022_child_post_bind(struct udevice *dev)
{	
	struct pl022_ssp_interface * iface_plat = dev_get_parent_priv(dev);
	
	iface_plat->pl022_iface = fdtdec_get_int(gd->fdt_blob, dev_of_offset(dev), "pl022,interface",
				    SSP_INTERFACE_MOTOROLA_SPI);
	return 0;
}

static const struct dm_spi_ops pl022_spi_ops = {
	.claim_bus	= pl022_spi_claim_bus,
	.release_bus 	= pl022_spi_release_bus,
	.xfer			= pl022_spi_xfer,
	.set_speed	= pl022_spi_set_speed,
	.set_mode	= pl022_spi_set_mode,
};

static const struct udevice_id pl022_spi_ids[] = {
	{ .compatible = "centec, pl022-spi" },
	{ }
};

U_BOOT_DRIVER(pl022_spi) = {
	.name		= "pl022_spi",
	.id	    		= UCLASS_SPI,
	.of_match 	= pl022_spi_ids,
	.ops			= &pl022_spi_ops,
	.ofdata_to_platdata = pl022_spi_ofdata_to_platdata,
	.platdata_auto_alloc_size = sizeof(struct pl022_spi_platdata),
	.priv_auto_alloc_size = sizeof(struct pl022_spi_priv),
	.probe	= pl022_spi_probe,
	.per_child_platdata_auto_alloc_size = sizeof(struct pl022_ssp_interface),
	.child_post_bind = pl022_child_post_bind,
};
#endif

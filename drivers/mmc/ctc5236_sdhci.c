/*
 * Centec MMC driver
 *
 * Author: Wangyb <wangyb@centecnetworks.com>
 *
 * Copyright 2005-2019, Centec Networks (Suzhou) Co., Ltd.
 *
 */
#include <common.h>
#include <clk.h>
#include <dm.h>
#include <malloc.h>
#include <sdhci.h>
#include <asm/arch/cpu.h>
#include <asm/arch/sysctl.h>
#include <errno.h>
#include <mmc.h>

DECLARE_GLOBAL_DATA_PTR;

#if !defined(CONFIG_SPL_BUILD) && defined(CTC5236_SDHCI_HOST_ADMA64)
#define CTC5236_UBOOT_SDHCI_HOST_ADMA64	1
#endif

#define SDHCI_ADMA_ADDRESS_HI	0x5c

void *ctc_aligned_buffer;

#ifdef CTC5236_UBOOT_SDHCI_HOST_ADMA64
#define ADMA_MAX_LEN	65532
#define ADMA_DESC_LEN	16
#define ADMA_TABLE_NO_ENTRIES (CONFIG_SYS_MMC_MAX_BLK_COUNT * \
			       MMC_MAX_BLOCK_LEN) / ADMA_MAX_LEN

#define ADMA_TABLE_SZ (ADMA_TABLE_NO_ENTRIES * ADMA_DESC_LEN)

/* Decriptor table defines */
#define ADMA_DESC_ATTR_VALID		BIT(0)
#define ADMA_DESC_ATTR_END		BIT(1)
#define ADMA_DESC_ATTR_INT		BIT(2)
#define ADMA_DESC_ATTR_ACT1		BIT(4)
#define ADMA_DESC_ATTR_ACT2		BIT(5)

#define ADMA_DESC_TRANSFER_DATA		ADMA_DESC_ATTR_ACT2
#define ADMA_DESC_LINK_DESC	(ADMA_DESC_ATTR_ACT1 | ADMA_DESC_ATTR_ACT2)

struct sdhci_adma_desc {
	u8 attr;
	u8 reserved;
	u16 len;
	u32 addr_lo;
	u32 addr_hi;
} __packed;
#endif

struct ctc_sdhci_host {
	void *ioaddr;
	struct mmc_config cfg;
	unsigned int quirks;
	unsigned int version;
	unsigned int clk_mul;
	unsigned int max_clk;

	dma_addr_t start_addr;
	int flags;
#define USE_SDMA	(0x1 << 0)
#define USE_ADMA	(0x1 << 1)
#define USE_ADMA64	(0x1 << 2)
#define USE_DMA		(USE_SDMA | USE_ADMA | USE_ADMA64)
	dma_addr_t adma_addr;
#ifdef CTC5236_UBOOT_SDHCI_HOST_ADMA64
	struct sdhci_adma_desc *adma_desc_table;
	uint desc_slot;
	uint desc_count2;
#endif
};

static inline void ctc_sdhci_writel(struct ctc_sdhci_host *host, u32 val, int reg)
{
	writel(val, host->ioaddr + reg);
}

static inline void ctc_sdhci_writew(struct ctc_sdhci_host *host, u16 val, int reg)
{
	writew(val, host->ioaddr + reg);
}

static inline void ctc_sdhci_writeb(struct ctc_sdhci_host *host, u8 val, int reg)
{
	writeb(val, host->ioaddr + reg);
}

static inline u32 ctc_sdhci_readl(struct ctc_sdhci_host *host, int reg)
{
	return readl(host->ioaddr + reg);
}

static inline u16 ctc_sdhci_readw(struct ctc_sdhci_host *host, int reg)
{
	return readw(host->ioaddr + reg);
}

static inline u8 ctc_sdhci_readb(struct ctc_sdhci_host *host, int reg)
{
	return readb(host->ioaddr + reg);
}

void ctc_sdhci_reset(struct ctc_sdhci_host *host, u8 mask)
{
	unsigned long timeout;

	/* Wait max 100 ms */
	timeout = 100;
	ctc_sdhci_writeb(host, mask, SDHCI_SOFTWARE_RESET);
	while (ctc_sdhci_readb(host, SDHCI_SOFTWARE_RESET) & mask) {
		if (timeout == 0) {
			printf("%s: Reset 0x%x never completed.\n",
			       __func__, (int)mask);
			return;
		}
		timeout--;
		udelay(1000);
	}
}
	
void ctc_sdhci_cmd_done(struct ctc_sdhci_host *host, struct mmc_cmd *cmd)
{
	int i;
	if (cmd->resp_type & MMC_RSP_136) {
		/* CRC is stripped so we need to do some shifting. */
		for (i = 0; i < 4; i++) {
			cmd->response[i] = ctc_sdhci_readl(host,
					SDHCI_RESPONSE + (3-i)*4) << 8;
			if (i != 3)
				cmd->response[i] |= ctc_sdhci_readb(host,
						SDHCI_RESPONSE + (3-i)*4-1);
		}
	} else {
		cmd->response[0] = ctc_sdhci_readl(host, SDHCI_RESPONSE);
	}
}	

void ctc_sdhci_transfer_pio(struct ctc_sdhci_host *host, struct mmc_data *data)
{
	int i;
	char *offs;
	for (i = 0; i < data->blocksize; i += 4) {
		offs = data->dest + i;
		if (data->flags == MMC_DATA_READ)
			*(u32 *)offs = ctc_sdhci_readl(host, SDHCI_BUFFER);
		else
			ctc_sdhci_writel(host, *(u32 *)offs, SDHCI_BUFFER);
	}
}

#if defined(CTC5236_UBOOT_SDHCI_HOST_ADMA64)

#define BOUNDARY_OK(addr, len) ((addr | (SZ_128M - 1)) == ((addr + len - 1) | (SZ_128M - 1)))

static void ctc_sdhci_adma_write_desc(struct ctc_sdhci_host *host, dma_addr_t addr, u16 len, bool end)
{
	struct sdhci_adma_desc *desc;
	u8 attr;

	desc = &host->adma_desc_table[host->desc_slot];

	attr = ADMA_DESC_ATTR_VALID | ADMA_DESC_TRANSFER_DATA;
	if (!end)
		host->desc_slot++;
	else
		attr |= ADMA_DESC_ATTR_END;

	desc->attr = attr;
	desc->len = len;
	desc->reserved = 0;
	desc->addr_lo = addr;
	desc->addr_hi = addr >> 32;

	host->desc_count2++;
}

static void ctc_sdhci_adma_desc(struct ctc_sdhci_host *host, dma_addr_t addr, u16 len, bool end)
{	
	int tmplen, offset;

	if (likely(!len || BOUNDARY_OK(addr, len))) {
		ctc_sdhci_adma_write_desc(host, addr, len, end);
		return;
	}

	offset = addr & (SZ_128M - 1);
	tmplen = SZ_128M - offset;
	ctc_sdhci_adma_write_desc(host, addr, tmplen, false);
	
	addr += tmplen;
	len -= tmplen;
	ctc_sdhci_adma_write_desc(host, addr, len, end);
}

static void ctc_sdhci_prepare_adma_table(struct ctc_sdhci_host *host, struct mmc_data *data)
{
	uint trans_bytes = data->blocksize * data->blocks;
	uint desc_count = DIV_ROUND_UP(trans_bytes, ADMA_MAX_LEN);
	int i = desc_count;
	dma_addr_t addr;	

	host->desc_slot = 0;
	host->desc_count2 =0;

	if (data->flags & MMC_DATA_READ)
		addr = (dma_addr_t)data->dest;
	else
		addr = (dma_addr_t)data->src;

	while (--i) {
		ctc_sdhci_adma_desc(host, addr, ADMA_MAX_LEN, false);
		addr += ADMA_MAX_LEN;
		trans_bytes -= ADMA_MAX_LEN;
	}

	ctc_sdhci_adma_desc(host, addr, trans_bytes, true);

	flush_cache((dma_addr_t)host->adma_desc_table,
		    ROUND(host->desc_count2 * sizeof(struct sdhci_adma_desc),
			  ARCH_DMA_MINALIGN));
}
#elif defined(CONFIG_MMC_SDHCI_SDMA)
static void ctc_sdhci_prepare_adma_table(struct ctc_sdhci_host *host, struct mmc_data *data)
{}
#endif

#if (defined(CONFIG_MMC_SDHCI_SDMA) || defined(CTC5236_UBOOT_SDHCI_HOST_ADMA64))
static void ctc_sdhci_prepare_dma(struct ctc_sdhci_host *host, struct mmc_data *data,
			      int *is_aligned, int trans_bytes)
{
	unsigned char ctrl;

	if (data->flags == MMC_DATA_READ)
		host->start_addr = (dma_addr_t)data->dest;
	else
		host->start_addr = (dma_addr_t)data->src;

	ctrl = ctc_sdhci_readb(host, SDHCI_HOST_CONTROL);
	ctrl &= ~SDHCI_CTRL_DMA_MASK;
	if (host->flags & USE_ADMA64)
		ctrl |= SDHCI_CTRL_ADMA64;
	else if (host->flags & USE_ADMA)
		ctrl |= SDHCI_CTRL_ADMA32;
	ctc_sdhci_writeb(host, ctrl, SDHCI_HOST_CONTROL);

	if (host->flags & USE_SDMA) {
		if ((host->quirks & SDHCI_QUIRK_32BIT_DMA_ADDR) && (host->start_addr & 0x7) != 0x0) {
			*is_aligned = 0;
			host->start_addr = (unsigned long)ctc_aligned_buffer;
			if (data->flags != MMC_DATA_READ)
				memcpy(ctc_aligned_buffer, data->src, trans_bytes);
		}

		ctc_sdhci_writel(host, host->start_addr, SDHCI_DMA_ADDRESS);

	} else if (host->flags & (USE_ADMA | USE_ADMA64)) {
		ctc_sdhci_prepare_adma_table(host, data);

		ctc_sdhci_writel(host, (u32)host->adma_addr, SDHCI_ADMA_ADDRESS);
		
		if (host->flags & USE_ADMA64)
			ctc_sdhci_writel(host, (u64)host->adma_addr >> 32, SDHCI_ADMA_ADDRESS_HI);
	}

	flush_cache(host->start_addr, ROUND(trans_bytes, ARCH_DMA_MINALIGN));
}
#else
static void ctc_sdhci_prepare_dma(struct ctc_sdhci_host *host, struct mmc_data *data,
			      int *is_aligned, int trans_bytes)
{}
#endif


int ctc_sdhci_transfer_data(struct ctc_sdhci_host *host, struct mmc_data *data)
{
	dma_addr_t start_addr = host->start_addr;
	unsigned int stat, rdy, mask, timeout, block = 0;
	bool transfer_done = false;

	timeout = 1000000;
	rdy = SDHCI_INT_SPACE_AVAIL | SDHCI_INT_DATA_AVAIL;
	mask = SDHCI_DATA_AVAILABLE | SDHCI_SPACE_AVAILABLE;
	do {
		stat = ctc_sdhci_readl(host, SDHCI_INT_STATUS);
		if (stat & SDHCI_INT_ERROR) {
			printf("%s: Error detected in status(0x%X)!\n", __func__, stat);
#ifdef CTC5236_SDHCI_HOST_DEBUG			
			if (stat & SDHCI_INT_DATA_TIMEOUT)
				printf("Err. Data timeout\n");
			if (stat & SDHCI_INT_DATA_CRC)
				printf("Err. Data crc\n");
			if (stat & SDHCI_INT_DATA_END_BIT)
				printf("Err. Data end bit\n");
#endif			
			return -EIO;
		}
		if (!transfer_done && (stat & rdy)) {
			if (!(ctc_sdhci_readl(host, SDHCI_PRESENT_STATE) & mask))
				continue;
			ctc_sdhci_writel(host, rdy, SDHCI_INT_STATUS);
			ctc_sdhci_transfer_pio(host, data);
			data->dest += data->blocksize;
			if (++block >= data->blocks) {
				/* Keep looping until the SDHCI_INT_DATA_END is
				 * cleared, even if we finished sending all the
				 * blocks.
				 */
				transfer_done = true;
				continue;
			}
		}
		if ((host->flags & USE_DMA) && !transfer_done &&
		    (stat & SDHCI_INT_DMA_END)) {
			ctc_sdhci_writel(host, SDHCI_INT_DMA_END, SDHCI_INT_STATUS);
			if (host->flags & USE_SDMA) {
				start_addr &=
				~(SDHCI_DEFAULT_BOUNDARY_SIZE - 1);
				start_addr += SDHCI_DEFAULT_BOUNDARY_SIZE;
				ctc_sdhci_writel(host, start_addr,
					     SDHCI_DMA_ADDRESS);
			}
		}
		if (timeout-- > 0)
			udelay(10);
		else {
			printf("%s: Transfer data timeout\n", __func__);
			return -ETIMEDOUT;
		}
	} while (!(stat & SDHCI_INT_DATA_END));
	return 0;
}	

#define SDHCI_READ_STATUS_TIMEOUT		1000
int ctc_sdhci_send_cmd(struct mmc *mmc, struct mmc_cmd *cmd, struct mmc_data *data)
{
	struct ctc_sdhci_host *host = (void *)mmc->priv;
	unsigned int stat = 0;
	int ret = 0;
	int trans_bytes = 0, is_aligned = 1;
	u32 mask, flags, mode;
	unsigned start = get_timer(0);

	/* Some eMMC cards, because of theirs inner firmware reason, 
	   that sometimes they return invalid ocr response. And
	   we need to adjust 2ms delay before sending CMD1. */
	if (cmd->cmdidx == MMC_CMD_SEND_OP_COND)
		udelay(2000);
	
	ctc_sdhci_writel(host, SDHCI_INT_ALL_MASK, SDHCI_INT_STATUS);
	mask = SDHCI_CMD_INHIBIT | SDHCI_DATA_INHIBIT;

	/* We shouldn't wait for data inihibit for stop commands, even
	   though they might use busy signaling */
	if (cmd->cmdidx == MMC_CMD_STOP_TRANSMISSION)
		mask &= ~SDHCI_DATA_INHIBIT;

	while (ctc_sdhci_readl(host, SDHCI_PRESENT_STATE) & mask)
		;

	mask = SDHCI_INT_RESPONSE;
	if (!(cmd->resp_type & MMC_RSP_PRESENT))
		flags = SDHCI_CMD_RESP_NONE;
	else if (cmd->resp_type & MMC_RSP_136)
		flags = SDHCI_CMD_RESP_LONG;
	else if (cmd->resp_type & MMC_RSP_BUSY) {
		flags = SDHCI_CMD_RESP_SHORT_BUSY;
		if (data)
			mask |= SDHCI_INT_DATA_END;
	} else
		flags = SDHCI_CMD_RESP_SHORT;

	if (cmd->resp_type & MMC_RSP_CRC)
		flags |= SDHCI_CMD_CRC;
	if (cmd->resp_type & MMC_RSP_OPCODE)
		flags |= SDHCI_CMD_INDEX;

	if (data)
		flags |= SDHCI_CMD_DATA;
	
	/* Set Transfer mode regarding to data flag */
	if (data) {
		ctc_sdhci_writeb(host, 0xe, SDHCI_TIMEOUT_CONTROL);
		mode = SDHCI_TRNS_BLK_CNT_EN;
		trans_bytes = data->blocks * data->blocksize;
		if (data->blocks > 1)
			mode |= SDHCI_TRNS_MULTI;

		if (data->flags == MMC_DATA_READ)
			mode |= SDHCI_TRNS_READ;

		if (host->flags & USE_DMA) {
			mode |= SDHCI_TRNS_DMA;
			ctc_sdhci_prepare_dma(host, data, &is_aligned, trans_bytes);
		}

		ctc_sdhci_writew(host, SDHCI_MAKE_BLKSZ(SDHCI_DEFAULT_BOUNDARY_ARG,
				data->blocksize),
				SDHCI_BLOCK_SIZE);
		ctc_sdhci_writew(host, data->blocks, SDHCI_BLOCK_COUNT);
		ctc_sdhci_writew(host, mode, SDHCI_TRANSFER_MODE);
	} else if (cmd->resp_type & MMC_RSP_BUSY) {
		ctc_sdhci_writeb(host, 0xe, SDHCI_TIMEOUT_CONTROL);
	}

	ctc_sdhci_writel(host, cmd->cmdarg, SDHCI_ARGUMENT);
	ctc_sdhci_writew(host, SDHCI_MAKE_CMD(cmd->cmdidx, flags), SDHCI_COMMAND);
	start = get_timer(0);
	do {
		stat = ctc_sdhci_readl(host, SDHCI_INT_STATUS);
		if (stat & SDHCI_INT_ERROR){
#ifdef CTC5236_SDHCI_HOST_DEBUG			
			if (cmd->cmdidx == MMC_CMD_SEND_OP_COND){
				printf("%s: Error detected in status(0x%X)!\n", __func__, stat);
				if (stat & SDHCI_INT_TIMEOUT)
					printf("Err. Command timeout\n");
				if (stat & SDHCI_INT_CRC)
					printf("Err. Command crc\n");
				if (stat & SDHCI_INT_END_BIT)
					printf("Err. Command end bit\n");
				if (stat & SDHCI_INT_INDEX)
					printf("Err. Command index\n");
			}
#endif			
			break;
		}
		if (get_timer(start) >= SDHCI_READ_STATUS_TIMEOUT) {
			if (host->quirks & SDHCI_QUIRK_BROKEN_R1B) {
				return 0;
			} else {
				printf("%s: Timeout for status update!\n",
				       __func__);
				return -ETIMEDOUT;
			}
		}
	} while ((stat & mask) != mask);

	if ((stat & (SDHCI_INT_ERROR | mask)) == mask) {
		ctc_sdhci_cmd_done(host, cmd);
		ctc_sdhci_writel(host, mask, SDHCI_INT_STATUS);
	} else
		ret = -1;

	if (!ret && data)
		ret = ctc_sdhci_transfer_data(host, data);

	if (host->quirks & SDHCI_QUIRK_WAIT_SEND_CMD)
		udelay(1000);

	stat = ctc_sdhci_readl(host, SDHCI_INT_STATUS);
	ctc_sdhci_writel(host, SDHCI_INT_ALL_MASK, SDHCI_INT_STATUS);
	if (!ret) {
		if ((host->quirks & SDHCI_QUIRK_32BIT_DMA_ADDR) &&
				!is_aligned && (data->flags == MMC_DATA_READ))
			memcpy(data->dest, ctc_aligned_buffer, trans_bytes);
		return 0;
	}

	ctc_sdhci_reset(host, SDHCI_RESET_CMD);
	ctc_sdhci_reset(host, SDHCI_RESET_DATA);
	if (stat & SDHCI_INT_TIMEOUT)
		return -ETIMEDOUT;
	else
		return -ECOMM;
}

static void ctc5236_sdhci_set_ios_post(struct mmc *mmc, struct ctc_sdhci_host *host)
{
	uint speed = mmc->tran_speed;
	int pwr_18v = 0;
    u16 ctrl_2 = ctc_sdhci_readw(host, SDHCI_HOST_CONTROL2);

	if ((ctc_sdhci_readb(host, SDHCI_POWER_CONTROL) & ~SDHCI_POWER_ON) == SDHCI_POWER_180)
		pwr_18v = 1;

	ctrl_2 &= ~SDHCI_CTRL_UHS_MASK;
	if (IS_SD(mmc)) {
		/* SD/SDIO */
		if (pwr_18v) {
			if (mmc->ddr_mode)
				ctrl_2 |= SDHCI_CTRL_UHS_DDR50 | SDHCI_CTRL_VDD_180;
			else if (speed <= 25000000)
				ctrl_2 |= SDHCI_CTRL_UHS_SDR25 | SDHCI_CTRL_VDD_180;
			else
				ctrl_2 |= SDHCI_CTRL_UHS_SDR50 | SDHCI_CTRL_VDD_180;
		} else {
			if (speed <= 25000000)
				ctrl_2 |= SDHCI_CTRL_UHS_SDR25;
			else
				ctrl_2 |= SDHCI_CTRL_UHS_SDR50;
		}
	} else {
		/* eMMC */
		if (mmc->ddr_mode) {
			ctrl_2 |= SDHCI_CTRL_UHS_DDR50 | SDHCI_CTRL_VDD_180;
		}
		else if (speed <= 26000000)
			ctrl_2 |= SDHCI_CTRL_UHS_SDR12 ;
		else
			ctrl_2 |= SDHCI_CTRL_UHS_SDR104;
	}
	ctc_sdhci_writew(host, ctrl_2, SDHCI_HOST_CONTROL2);

}

int ctc_sdhci_set_clock(struct mmc *mmc, unsigned int clock)
{
	struct ctc_sdhci_host *host = (void *)mmc->priv;
	unsigned int div, clk = 0, timeout;

	/* Wait max 20 ms */
	timeout = 200;
	while (ctc_sdhci_readl(host, SDHCI_PRESENT_STATE) &
			   (SDHCI_CMD_INHIBIT | SDHCI_DATA_INHIBIT)) {
		if (timeout == 0) {
			printf("%s: Timeout to wait cmd & data inhibit\n",
			       __func__);
			return -EBUSY;
		}

		timeout--;
		udelay(100);
	}

	ctc_sdhci_writew(host, 0, SDHCI_CLOCK_CONTROL);

	if (clock == 0)
		return 0;

	if (SDHCI_GET_VERSION(host) >= SDHCI_SPEC_300) {
		/*
		 * Check if the Host Controller supports Programmable Clock
		 * Mode.
		 */
		if (host->clk_mul) {
			for (div = 1; div <= 1024; div++) {
				if ((host->max_clk * host->clk_mul / div)
					<= clock)
					break;
			}

			/*
			 * Set Programmable Clock Mode in the Clock
			 * Control register.
			 */
			clk = SDHCI_PROG_CLOCK_MODE;
			div--;
		} else {
			/* Version 3.00 divisors must be a multiple of 2. */
			if (host->max_clk <= clock) {
				div = 1;
			} else {
				for (div = 2;
				     div < SDHCI_MAX_DIV_SPEC_300;
				     div += 2) {
					if ((host->max_clk / div) <= clock)
						break;
				}
			}
			div >>= 1;
		}
	} else {
		/* Version 2.00 divisors must be a power of 2. */
		for (div = 1; div < SDHCI_MAX_DIV_SPEC_200; div *= 2) {
			if ((host->max_clk / div) <= clock)
				break;
		}
		div >>= 1;
	}
#if 0
	if (host->ops && host->ops->set_clock)
		host->ops->set_clock(host, div);
#endif
	clk |= (div & SDHCI_DIV_MASK) << SDHCI_DIVIDER_SHIFT;
	clk |= ((div & SDHCI_DIV_HI_MASK) >> SDHCI_DIV_MASK_LEN)
		<< SDHCI_DIVIDER_HI_SHIFT;
	clk |= SDHCI_CLOCK_INT_EN;
	ctc_sdhci_writew(host, clk, SDHCI_CLOCK_CONTROL);

	/* Wait max 20 ms */
	timeout = 20;
	while (!((clk = ctc_sdhci_readw(host, SDHCI_CLOCK_CONTROL))
		& SDHCI_CLOCK_INT_STABLE)) {
		if (timeout == 0) {
			printf("%s: Internal clock never stabilised.\n",
			       __func__);
			return -EBUSY;
		}
		timeout--;
		udelay(1000);
	}

	clk |= SDHCI_CLOCK_CARD_EN;
	ctc_sdhci_writew(host, clk, SDHCI_CLOCK_CONTROL);
	return 0;
}	

int ctc_sdhci_set_ios(struct mmc *mmc)
{
	u32 ctrl;
	struct ctc_sdhci_host *host = (void *)mmc->priv;
	
//	if (host->ops && host->ops->set_control_reg)
	//	host->ops->set_control_reg(host);

	//if (mmc->clock != host->clock)
		ctc_sdhci_set_clock(mmc, mmc->clock);

	/* Set bus width */
	ctrl = ctc_sdhci_readb(host, SDHCI_HOST_CONTROL);
	if (mmc->bus_width == 8) {
		ctrl &= ~SDHCI_CTRL_4BITBUS;
		if ((SDHCI_GET_VERSION(host) >= SDHCI_SPEC_300) ||
				(host->quirks & SDHCI_QUIRK_USE_WIDE8))
			ctrl |= SDHCI_CTRL_8BITBUS;
	} else {
		if ((SDHCI_GET_VERSION(host) >= SDHCI_SPEC_300) ||
				(host->quirks & SDHCI_QUIRK_USE_WIDE8))
			ctrl &= ~SDHCI_CTRL_8BITBUS;
		if (mmc->bus_width == 4)
			ctrl |= SDHCI_CTRL_4BITBUS;
		else
			ctrl &= ~SDHCI_CTRL_4BITBUS;
	}

	if (mmc->clock > 26000000)
		ctrl |= SDHCI_CTRL_HISPD;
	else
		ctrl &= ~SDHCI_CTRL_HISPD;

	if (host->quirks & SDHCI_QUIRK_NO_HISPD_BIT)
		ctrl &= ~SDHCI_CTRL_HISPD;

	ctc_sdhci_writeb(host, ctrl, SDHCI_HOST_CONTROL);

#if 0
	/* If available, call the driver specific "post" set_ios() function */
	if (host->ops && host->ops->set_ios_post)
		host->ops->set_ios_post(host);
#else	
	ctc5236_sdhci_set_ios_post(mmc, host);
#endif	
	return 0;
}

void ctc_sdhci_set_power(struct ctc_sdhci_host *host, unsigned short power)
{
	u8 pwr = 0;

	if (power != (unsigned short)-1) {
		switch (1 << power) {
		case MMC_VDD_165_195:
			pwr = SDHCI_POWER_180;
			break;
		case MMC_VDD_29_30:
		case MMC_VDD_30_31:
			pwr = SDHCI_POWER_300;
			break;
		case MMC_VDD_32_33:
		case MMC_VDD_33_34:
			pwr = SDHCI_POWER_330;
			break;
		}
	}

	if (pwr == 0) {
		ctc_sdhci_writeb(host, 0, SDHCI_POWER_CONTROL);
		return;
	}

	pwr |= SDHCI_POWER_ON;

	ctc_sdhci_writeb(host, pwr, SDHCI_POWER_CONTROL);
}

int ctc_sdhci_init(struct mmc *mmc)
{
	struct ctc_sdhci_host *host = (void *)mmc->priv;

	ctc_sdhci_reset(host, SDHCI_RESET_ALL);

	if ((host->quirks & SDHCI_QUIRK_32BIT_DMA_ADDR) && !ctc_aligned_buffer) {
		ctc_aligned_buffer = memalign(8, 512*1024);
		if (!ctc_aligned_buffer) {
			printf("%s: Aligned buffer alloc failed!!!\n",
			       __func__);
			return -ENOMEM;
		}
	}

	ctc_sdhci_set_power(host, fls(mmc->cfg->voltages) - 1);
#if 0
	if (host->ops && host->ops->get_cd)
		host->ops->get_cd(host);
#endif
	/* Enable only interrupts served by the SD controller */
	ctc_sdhci_writel(host, SDHCI_INT_DATA_MASK | SDHCI_INT_CMD_MASK,
		     SDHCI_INT_ENABLE);
	/* Mask all sdhci interrupt sources */
	ctc_sdhci_writel(host, 0x0, SDHCI_SIGNAL_ENABLE);

	return 0;
}

static void ctc5236_release_emmc_post(struct ctc_sdhci_host *host)
{
	u16 clk;
    u32 timeout = 10000;
	
	clk = ctc_sdhci_readw(host, SDHCI_CLOCK_CONTROL);
	clk |= SDHCI_CLOCK_CARD_EN;
	ctc_sdhci_writew(host, clk, SDHCI_CLOCK_CONTROL);

	if(readl(&sysctl_base->SysMshCfg) & SYS_MSH_CFG_W0_CFG_RESET_BAR_MSH_SEL)
    {
        udelay(1000);
        clrbits_le32(&sysctl_base->SysMshResetCtl, SYS_MSH_RESET_CTL_W0_CFG_MSH_C_TX_RESET);
        udelay(1000);
        clrbits_le32(&sysctl_base->SysMshResetCtl, SYS_MSH_RESET_CTL_W0_CFG_MSH_C_RX_DLL_RESET);
        udelay(1000);
        clrbits_le32(&sysctl_base->SysMshResetCtl, SYS_MSH_RESET_CTL_W0_CFG_MSH_C_RX_RESET); 
    }
    else
    {
        while ((readl(&sysctl_base->SysMshStatus) 
            & SYS_MSH_STATUS_W0_MON_MSH_C_RESET_DONE) && timeout--)
        {
            udelay(1);
        }
    }
}	

static const struct mmc_ops ctc_sdhci_ops = {
	.send_cmd       = ctc_sdhci_send_cmd,
	.set_ios        = ctc_sdhci_set_ios,
	.init           = ctc_sdhci_init,
};

int ctc_sdhci_host_set(struct ctc_sdhci_host *host)
{
	u32 caps_1;	
	
	caps_1 = ctc_sdhci_readl(host, SDHCI_CAPABILITIES_1);
	host->version = ctc_sdhci_readw(host, SDHCI_HOST_VERSION);
	host->clk_mul = (caps_1 & SDHCI_CLOCK_MUL_MASK) >> SDHCI_CLOCK_MUL_SHIFT;
	host->max_clk = CONFIG_EMMC_MAX_CLK;
	host->quirks = 0;
	
#ifdef CTC5236_UBOOT_SDHCI_HOST_ADMA64
	host->adma_desc_table = (struct sdhci_adma_desc *) memalign(ARCH_DMA_MINALIGN, ADMA_TABLE_SZ * 2);

	host->adma_addr = (dma_addr_t)host->adma_desc_table;

	host->flags |= USE_ADMA64;
#else

#ifdef CONFIG_MMC_SDHCI_SDMA
	host->flags |= USE_SDMA;
#endif
#endif
	return 0;
}

static struct mmc_config ctc_sdhi_cfg = {
	.name           = "sdhci@30400000",
	.ops            = &ctc_sdhci_ops,
	.f_min          = CTC5236_SDHC_MIN_FREQ,
	.f_max          = CONFIG_EMMC_MAX_CLK / 2,
#ifdef 	MMC_ONLY_SUPPORT_1_8V
	.voltages       = MMC_VDD_165_195,
#else
	.voltages       = MMC_VDD_32_33 | MMC_VDD_33_34,
#endif
	.host_caps      = MMC_MODE_HS | MMC_MODE_HS_52MHz | CTC5236_SDHCI_HOST_CAPS,
	.b_max          = CONFIG_SYS_MMC_MAX_BLK_COUNT,
};

#ifdef CONFIG_DM_MMC
struct ctc5236_sdhci_plat {
	struct mmc_config cfg;
	struct mmc mmc;
};

static const struct sdhci_ops ctc5236_sdhci_ops = {
	.set_ios_post = ctc5236_sdhci_set_ios_post,
};

static int ctc5236_sdhci_probe(struct udevice *dev)
{
	struct mmc_uclass_priv *upriv = dev_get_uclass_priv(dev);
#ifdef CONFIG_BLK
	struct ctc5236_sdhci_plat *plat = dev_get_platdata(dev);
#endif
	struct sdhci_host *host = dev_get_priv(dev);
	u32 max_frequency;
	int ret;

	host->name = dev->name;
	host->ioaddr = (void *)dev_get_addr(dev);

	
	host->bus_width= fdtdec_get_int(gd->fdt_blob, dev_of_offset(dev),
                    "bus-width", 4);

    max_frequency = fdtdec_get_int(gd->fdt_blob, dev_of_offset(dev),
                    "max-frequency", 0);
    host->quirks = 0;
	host->max_clk = max_frequency;
    host->host_caps = MMC_MODE_8BIT | MMC_MODE_DDR_52MHz;
    host->ops = &ctc5236_sdhci_ops;
    ctc5236_release_emmc_post(host);

#ifdef CONFIG_BLK
	ret = sdhci_setup_cfg(&plat->cfg, host, 0, CTC5236_SDHC_MIN_FREQ);
	if (ret)
		return ret;

	host->mmc = &plat->mmc;
	host->mmc->dev = dev;
	host->mmc->priv = host;
#else
    ret = add_sdhci(host, 0, CTC5236_SDHC_MIN_FREQ);
    if (ret)
        return ret;
    host->mmc->dev = dev;
#endif
	upriv->mmc = host->mmc;

    ret = sdhci_probe(dev);

	return ret;
}

#ifdef CONFIG_BLK
static int ctc5236_sdhci_bind(struct udevice *dev)
{
	struct ctc5236_sdhci_plat *plat = dev_get_platdata(dev);

	return sdhci_bind(dev, &plat->mmc, &plat->cfg);
}
#endif

static const struct udevice_id ctc5236_sdhci_ids[] = {
	{ .compatible = "centec,ctc5236-sdhci" },
	{ }
};

U_BOOT_DRIVER(ctc5236_sdhci_drv) = {
	.name		= "ctc5236_sdhci",
	.id		= UCLASS_MMC,
	.of_match	= ctc5236_sdhci_ids,
	.ops		= &sdhci_ops,
#ifdef CONFIG_BLK
	.bind		= ctc5236_sdhci_bind,
#endif
	.probe		= ctc5236_sdhci_probe,
	.priv_auto_alloc_size = sizeof(struct sdhci_host),
	.platdata_auto_alloc_size = sizeof(struct ctc5236_sdhci_plat),
};
#else
int ctc5236_sdhci_init(void)
{
 	int ret = 0;
	struct mmc *mmc;
	struct ctc_sdhci_host *host = NULL;

	host = malloc(sizeof(struct ctc_sdhci_host));
	if (!host)
		return -ENOMEM;
	
	memset(host, 0, sizeof(struct ctc_sdhci_host));
	host->ioaddr = (void *)CONFIG_EMMC_BASE;

	ctc5236_release_emmc_post(host);

	ctc_sdhci_host_set(host);

	mmc = mmc_create(&ctc_sdhi_cfg, host);
	if (!mmc) {
		ret = -1;
		goto error;
	}
		
	return ret;
error:
	if (host)
		free(host);
	return ret;
}
#endif

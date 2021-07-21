#include <common.h>
#include <spi.h>
#include <spi_flash.h>
#include <malloc.h>
#include "sf_internal.h"

#define SPI_FLASH_3B_ADDR_LEN		3
#define SPI_FLASH_CMD_LEN		(1 + SPI_FLASH_3B_ADDR_LEN)

static void ctc_spi_flash_addr(u32 addr, u8 *cmd)
{
	/* cmd[0] is actual command */
	cmd[1] = addr >> 16;
	cmd[2] = addr >> 8;
	cmd[3] = addr >> 0;
}

static int ctc_spi_flash_probe_slave(struct spi_flash *flash)
{
	struct spi_slave *spi = flash->spi;
	int ret;

	/* Setup spi_slave */
	if (!spi) {
		printf("SF: Failed to set up slave\n");
		return -ENODEV;
	}

	/* Claim spi bus */
	ret = spi_claim_bus(spi);
	if (ret) {
		debug("SF: Failed to claim SPI bus: %d\n", ret);
		return ret;
	}

	ret = spi_flash_scan(flash);
	if (ret)
		goto err_read_id;

err_read_id:
	spi_release_bus(spi);
	return ret;
}

struct spi_flash *ctc_spi_flash_probe(unsigned int busnum, unsigned int cs,
				  unsigned int max_hz, unsigned int spi_mode)
{
	struct spi_slave *bus;
	struct spi_flash *flash;

	bus = spi_setup_slave(busnum, cs, max_hz, spi_mode);
	if (!bus)
		return NULL;

	/* Allocate space if needed (not used by sf-uclass */
	flash = calloc(1, sizeof(*flash));
	if (!flash) {
		debug("SF: Failed to allocate spi_flash\n");
		return NULL;
	}

	flash->spi = bus;
	if (ctc_spi_flash_probe_slave(flash)) {
		spi_free_slave(bus);
		free(flash);
		return NULL;
	}

	return flash;
}
int ctc_spi_flash_cmd_erase_ops(struct spi_flash *flash, u32 offset, size_t len)
{
	u32 erase_size, erase_addr;
	u8 cmd[SPI_FLASH_CMD_LEN];
	int ret = -1;

	erase_size = flash->erase_size;
	if (offset % erase_size || len % erase_size) {
		printf("SF: Erase offset/length not multiple of erase size 0x%x 0x%x 0x%x\n", flash->erase_size, offset, len);
		return -1;
	}

	if (flash->flash_is_locked) {
		if (flash->flash_is_locked(flash, offset, len) > 0) {
			printf("offset 0x%x is protected and cannot be erased\n",
			       offset);
			return -EINVAL;
		}
	}

	cmd[0] = flash->erase_cmd;
	while (len) {
		erase_addr = offset;

		ctc_spi_flash_addr(erase_addr, cmd);

		debug("SF: erase %2x %2x %2x %2x (%x)\n", cmd[0], cmd[1],
		      cmd[2], cmd[3], erase_addr);

		ret = spi_flash_write_common(flash, cmd, sizeof(cmd), NULL, 0);
		if (ret < 0) {
			debug("SF: erase failed\n");
			break;
		}

		offset += erase_size;
		len -= erase_size;
	}

	return ret;
}




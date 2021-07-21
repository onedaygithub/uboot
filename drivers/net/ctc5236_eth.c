/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Jay Cao <caoj@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <linux/err.h>
#include <malloc.h>
#include <miiphy.h>
#include <net.h>
#include <asm/io.h>

/* Private driver structures */
struct ctc5236_eth_dev {
	int phyaddr;
};

static int ctc5236_eth_start(struct udevice *dev)
{
	return 0;
}

static int ctc5236_eth_send(struct udevice *dev, void *packet, int length)
{

	return 0;
}

static int ctc5236_eth_recv(struct udevice *dev, int flags, uchar **packetp)
{
	return 0;
}

static void ctc5236_eth_stop(struct udevice *dev)
{
	/* Nothing to do here */
}

static int ctc5236_eth_probe(struct udevice *dev)
{
	return 0;
}

static const struct eth_ops ctc5236_eth_ops = {
	.start			= ctc5236_eth_start,
	.send			= ctc5236_eth_send,
	.recv			= ctc5236_eth_recv,
	.stop			= ctc5236_eth_stop,
};

static int ctc5236_eth_ofdata_to_platdata(struct udevice *dev)
{
	return 0;
}

static const struct udevice_id ctc5236_eth_ids[] = {
	{ .compatible = "centec,ctc5236-eth" },
	{ }
};

U_BOOT_DRIVER(ctc5236_eth) = {
	.name	= "ctc5236_eth",
	.id	= UCLASS_ETH,
	.of_match = ctc5236_eth_ids,
	.ofdata_to_platdata = ctc5236_eth_ofdata_to_platdata,
	.probe	= ctc5236_eth_probe,
	.ops	= &ctc5236_eth_ops,
	.priv_auto_alloc_size = sizeof(struct ctc5236_eth_dev),
	.platdata_auto_alloc_size = sizeof(struct eth_pdata),
};

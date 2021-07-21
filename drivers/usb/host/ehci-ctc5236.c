/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Jay Cao <caoj@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <dm.h>
#include <usb.h>
#include <asm/io.h>
#include <usb/ehci-ci.h>
#include <usb/ulpi.h>

#include <asm/arch/cpu.h>
#include <asm/arch/sysctl.h>
#include "ehci.h"

DECLARE_GLOBAL_DATA_PTR;

struct ctc5236_ehci_priv {
	struct ehci_ctrl ehcictrl;
	struct usb_ehci *ehci;
};

static int ehci_ctc5236_ofdata_to_platdata(struct udevice *dev)
{
	struct ctc5236_ehci_priv *priv = dev_get_priv(dev);

	priv->ehci = (struct usb_ehci *)dev_get_addr_ptr(dev);

	if (!priv->ehci)
		return -EINVAL;

	return 0;
}

static int ehci_ctc5236_probe(struct udevice *dev)
{
	struct usb_platdata *plat = dev_get_platdata(dev);
	struct ctc5236_ehci_priv *priv = dev_get_priv(dev);
	struct ehci_hccr *hccr;
	struct ehci_hcor *hcor;
	int ret;

	hccr = (struct ehci_hccr *)((uint32_t)&priv->ehci->caplength);
	hcor = (struct ehci_hcor *)((uint32_t) hccr +
			HC_LENGTH(ehci_readl(&hccr->cr_capbase)));


	/* USB interface reset config */
	writel(0x7f, &sysctl_base->SysUsbResetCtl);
	udelay(1);
	clrbits_le32(&sysctl_base->SysUsbResetCtl, 
        SYS_USB_RESET_CTL_W0_CFG_USB_PHY_PWR_ON_RESET);
	udelay(1);
	clrbits_le32(&sysctl_base->SysUsbResetCtl, 
        SYS_USB_RESET_CTL_W0_CFG_USB_PHY_RESET
        |SYS_USB_RESET_CTL_W0_CFG_USB_PHY_PORT_RESET 
        | SYS_USB_RESET_CTL_W0_CFG_USB_UTMI_RESET);
	udelay(1);
	clrbits_le32(&sysctl_base->SysUsbResetCtl, 
        SYS_USB_RESET_CTL_W0_CFG_USB_INTF_RESET
        |SYS_USB_RESET_CTL_W0_CFG_USB_AUX_RESET);
	udelay(1);
	clrbits_le32(&sysctl_base->SysUsbResetCtl, 
		SYS_USB_RESET_CTL_W0_CFG_USB_PHY_ATE_RESET);

	mdelay(500); 	

	return ehci_register(dev, hccr, hcor, NULL, 0, plat->init_type);
}

static const struct udevice_id ehci_ctc5236_ids[] = {
	{ .compatible = "centec,ctc5236-ehci" },
	{ }
};

U_BOOT_DRIVER(ehci_ctc5236) = {
	.name	= "ehci_ctc5236",
	.id	= UCLASS_USB,
	.of_match = ehci_ctc5236_ids,
	.ofdata_to_platdata = ehci_ctc5236_ofdata_to_platdata,
	.probe = ehci_ctc5236_probe,
	.remove = ehci_deregister,
	.ops	= &ehci_usb_ops,
	.platdata_auto_alloc_size = sizeof(struct usb_platdata),
	.priv_auto_alloc_size = sizeof(struct ctc5236_ehci_priv),
	.flags	= DM_FLAG_ALLOC_PRIV_DMA,
};


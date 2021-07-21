/*
 * (C) Copyright 2018 Liuht <liuht@centecnetworks.com>
 *
 * Centec APB GPIO driver
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <asm/arch/gpio.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <dm.h>
#include <dm/device-internal.h>
#include <dm/lists.h>
#include <dm/root.h>
#include <errno.h>

DECLARE_GLOBAL_DATA_PTR;

struct GpioSoc_regs {
   u32 GpioDataCtl;                     /* 0x00000000 */
   u32 GpioOutCtl;                      /* 0x00000004 */
   u32 GpioDebCtl;                      /* 0x00000008 */
   u32 GpioReadData;                    /* 0x0000000c */
   u32 GpioIntrEn;                      /* 0x00000010 */
   u32 GpioIntrMask;                    /* 0x00000014 */
   u32 GpioIntrLevel;                   /* 0x00000018 */
   u32 GpioIntrPolarity;                /* 0x0000001c */
   u32 GpioIntrStatus;                  /* 0x00000020 */
   u32 GpioIntrRaw;                     /* 0x00000024 */
   u32 GpioEoiCtl;                      /* 0x00000028 */
   u32 rsv11;
   u32 GpioDebCnt;                      /* 0x00000030 */
   u32 GpioVerId;                       /* 0x00000034 */
   u32 rsv14;
   u32 rsv15;
   u32 GpioHsDataCtl;                   /* 0x00000040 */
   u32 GpioHsOutCtl;                    /* 0x00000044 */
   u32 GpioHsDebCtl;                    /* 0x00000048 */
   u32 GpioHsReadData;                  /* 0x0000004c */
   u32 GpioHsIntrEn;                    /* 0x00000050 */
   u32 GpioHsIntrMask;                  /* 0x00000054 */
   u32 GpioHsIntrLevel;                 /* 0x00000058 */
   u32 GpioHsIntrPolarity;              /* 0x0000005c */
   u32 GpioHsIntrStatus;                /* 0x00000060 */
   u32 GpioHsIntrRaw;                   /* 0x00000064 */
   u32 GpioHsEoiCtl;                    /* 0x00000068 */
};

struct GpioSoc_port_regs {
   u32 GpioDataCtl;               
   u32 GpioOutCtl;                 
   u32 GpioDebCtl;                 
   u32 GpioReadData;            
   u32 GpioIntrEn;                 
   u32 GpioIntrMask;             
   u32 GpioIntrLevel;             
   u32 GpioIntrPolarity;          
   u32 GpioIntrStatus;            
   u32 GpioIntrRaw;              
   u32 GpioEoiCtl;                  
};

struct gpio_ctcapb_platdata {
	const char	*name;
	int		pins;
	struct GpioSoc_port_regs *port;
};

static int ctcapb_gpio_direction_input(struct udevice *dev, unsigned pin)
{
	struct gpio_ctcapb_platdata *plat = dev_get_platdata(dev);
     
	clrbits_le32(&plat->port->GpioOutCtl, 1 << pin);
	return 0;
}

static int ctcapb_gpio_direction_output(struct udevice *dev, unsigned pin,
				     int val)
{
	struct gpio_ctcapb_platdata *plat = dev_get_platdata(dev);

	setbits_le32(&plat->port->GpioOutCtl, 1 << pin);

	if (val)
		setbits_le32(&plat->port->GpioDataCtl, 1 << pin);
	else
		clrbits_le32(&plat->port->GpioDataCtl, 1 << pin);

	return 0;
}

static int ctcapb_gpio_get_value(struct udevice *dev, unsigned pin)
{
	struct gpio_ctcapb_platdata *plat = dev_get_platdata(dev);
	return !!(readl(&plat->port->GpioReadData) & (1 << pin));
}


static int ctcapb_gpio_set_value(struct udevice *dev, unsigned pin, int val)
{
	struct gpio_ctcapb_platdata *plat = dev_get_platdata(dev);

	if (val)
		setbits_le32(&plat->port->GpioDataCtl, 1 << pin);
	else
		clrbits_le32(&plat->port->GpioDataCtl, 1 << pin);

	return 0;
}

static int ctcapb_gpio_get_function(struct udevice *dev, unsigned pin)
{
	struct gpio_ctcapb_platdata *plat = dev_get_platdata(dev);

	if(readl(&plat->port->GpioOutCtl) & (1 << pin)){
		return GPIOF_OUTPUT;
	}
	return GPIOF_INPUT;
}

static const struct dm_gpio_ops gpio_ctcapb_ops = {
	.direction_input	= ctcapb_gpio_direction_input,
	.direction_output	= ctcapb_gpio_direction_output,
	.get_value		= ctcapb_gpio_get_value,
	.set_value		= ctcapb_gpio_set_value,
	.get_function      = ctcapb_gpio_get_function,
};

static int gpio_ctcapb_probe(struct udevice *dev)
{
	struct gpio_dev_priv *priv = dev_get_uclass_priv(dev);
	struct gpio_ctcapb_platdata *plat = dev->platdata;

	if (!plat)
		return 0;

	priv->gpio_count = plat->pins;
	priv->bank_name = plat->name;

	return 0;
}

static int gpio_ctcapb_bind(struct udevice *dev)
{
	struct gpio_ctcapb_platdata *plat = dev_get_platdata(dev);
	const void *blob = gd->fdt_blob;
	struct udevice *subdev;
	fdt_addr_t base;
	int ret, node, bank = 0;

	/* If this is a child device, there is nothing to do here */
	if (plat)
		return 0;

	base = (phys_addr_t)dev_get_addr_index(dev, 0);
	if (base == FDT_ADDR_T_NONE) {
		debug("Can't get the GPIO register base address\n");
		return -ENXIO;
	}
          
	for (node = fdt_first_subnode(blob, dev_of_offset(dev));
	     node > 0;
	     node = fdt_next_subnode(blob, node)) {
		if (!fdtdec_get_bool(blob, node, "gpio-controller"))
			continue;

		plat = NULL;
		plat = calloc(1, sizeof(*plat));
		if (!plat)
			return -ENOMEM;
		if(!bank){
			plat->port = (struct GpioSoc_port_regs *)base;
		}else{
			plat->port = (struct GpioSoc_port_regs *)(base + 0x40);
		}

		plat->pins = fdtdec_get_int(blob, node, "ctc,nr-gpios", 0);
		plat->name = fdt_stringlist_get(blob, node, "bank-name", 0,
						NULL);
		if (ret)
			goto err;

		ret = device_bind(dev, dev->driver, plat->name,
				  plat, -1, &subdev);
		if (ret)
			goto err;

		dev_set_of_offset(subdev, node);
		bank++;
	}

	return 0;

err:
	free(plat);
	return ret;
}

static const struct udevice_id gpio_ctcapb_ids[] = {
	{ .compatible = "ctc,apb-gpio" },
	{ }
};

U_BOOT_DRIVER(gpio_ctcapb) = {
	.name		= "gpio-ctcapb",
	.id		= UCLASS_GPIO,
	.of_match	= gpio_ctcapb_ids,
	.ops		= &gpio_ctcapb_ops,
	.bind		= gpio_ctcapb_bind,
	.probe		= gpio_ctcapb_probe,
};


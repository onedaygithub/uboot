/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Jay Cao <caoj@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#undef DEBUG

#include <common.h>
#include <asm/io.h>
#include <dm.h>
#include <pci.h>
#include <asm/arch/cpu.h>
#include <asm/arch/sysctl.h>

DECLARE_GLOBAL_DATA_PTR;

#define CTC5236_CTRL_PCI_IDS	0x9c

#define CTC5236_CTRL_ATR_W0	0x600
#define CTC5236_CTRL_ATR_W1	0x700
#define CTC5236_CTRL_ATR_S0	0x800
#define CTC5236_CTRL_ATR_S1	0x900
#define CTC5236_CTRL_ATR_S1_T0	0x00
#define CTC5236_CTRL_ATR_S1_T2	0x20
#define CTC5236_CTRL_ATR_S1_T3	0x40
#define CTC5236_CTRL_ATR_S1_T4	0x60
#define PCIE_ATU_BUS(x)			(((x) & 0xff) << 20)
#define PCIE_ATU_DEV(x)			(((x) & 0x1f) << 15)
#define PCIE_ATU_FUNC(x)		(((x) & 0x7) << 12)

#define LTSSM_STATE_MASK	0xff
#define LTSSM_L0		0x10

#define CTC5236_PCIE_PHY_REG_CTL		0x332800C4
#define CTC5236_PCIE_PHY_REG_DATA0		0x332800C8
#define CTC5236_PCIE_PHY_REG_STAT		0x332800CC

#define CTC5236_PCIE_PHY_REG_ADDR_LOC		16

#define CTC5236_PCIE_PHY_REG_PLL_CAL_1		0x30
#define CTC5236_PCIE_PHY_REG_PLL_CAL_4		0x3C

#define CTC5236_PCIE_PHY_REG_PLL_CAL_1_PLL_VAL	15
#define CTC5236_PCIE_PHY_REG_PLL_CAL_1_PLL_VCOSP	16
#define CTC5236_PCIE_PHY_REG_PLL_CAL_1_PLL_VCOSP_MASK 0x003F0000
#define CTC5236_PCIE_PHY_REG_PLL_CAL_4_MANU	30

struct pcie_ctc5236 {
	struct pci_controller hose;
	struct udevice *bus;
	struct fdt_resource cfg_res;
	struct fdt_resource ctrl_res;
	void __iomem *cfg;
	void __iomem *ctrl;
};

static int pcie_phy_read(int addr , int *data)
{
	u32 val;

	val = addr << CTC5236_PCIE_PHY_REG_ADDR_LOC;
	
	writel(val, CTC5236_PCIE_PHY_REG_CTL);
	

	val = readl(CTC5236_PCIE_PHY_REG_STAT);
	*data = val;

	return 0;
}

static int pcie_phy_write( int addr, int data)
{
	u32 val;

	val = data;
	
	writel(val, CTC5236_PCIE_PHY_REG_DATA0);
	

	val = (addr << CTC5236_PCIE_PHY_REG_ADDR_LOC) | 0xF3;
	
	writel(val, CTC5236_PCIE_PHY_REG_CTL);

	return 0;
}

static int pcie_link_up(struct pcie_ctc5236 *pcie)
{
	int state;
	state = readl(&sysctl_base->SysPcieStatus[0]);

	state = state &LTSSM_STATE_MASK;
	if(state < LTSSM_L0)
		return 0;

	return 1;
}

static int pcie_addr_valid(struct pcie_ctc5236 *pcie, pci_dev_t bdf)
{
	struct udevice *bus = pcie->bus;

	if (PCI_BUS(bdf) < bus->seq)
		return -EINVAL;

	if ((PCI_BUS(bdf) > bus->seq) && (!pcie_link_up(pcie)))
		return -EINVAL;

	if (PCI_BUS(bdf) <= (bus->seq + 1) && (PCI_DEV(bdf) > 0))
		return -EINVAL;

	return 0;
}

void *pcie_conf_address(struct pcie_ctc5236 *pcie, pci_dev_t bdf,
				   int offset)
{
	struct udevice *bus = pcie->bus;
	u32 busdev;

	if (PCI_BUS(bdf) == bus->seq)
		return pcie->cfg + offset;

	busdev = PCIE_ATU_BUS(PCI_BUS(bdf)) |
		 PCIE_ATU_DEV(PCI_DEV(bdf)) |
		 PCIE_ATU_FUNC(PCI_FUNC(bdf));

	if (PCI_BUS(bdf) == bus->seq + 1) {
		return pcie->cfg + busdev + offset;
	} else {
		return pcie->cfg + busdev + offset;
	}
}

static int pcie_ctc5236_read_config(struct udevice *bus, pci_dev_t bdf,
				     uint offset, ulong *valuep,
				     enum pci_size_t size)
{
	struct pcie_ctc5236 *pcie = dev_get_priv(bus);
	void *address;

	debug("PCIE CFG read: bus->seq:%d : (b,d,f)=(%2d,%2d,%2d) \n",
	      bus->seq,PCI_BUS(bdf), PCI_DEV(bdf), PCI_FUNC(bdf));
    
	if (pcie_addr_valid(pcie, bdf)) {
		*valuep = pci_get_ff(size);
        return 0;
    }

	debug("pcie_ctc5236_read_config offset:0x%x\n",offset);
    
	address = pcie_conf_address(pcie, bdf, offset);

	switch (size) {
	case PCI_SIZE_8:
		*valuep = readb(address);
		debug("pcie_ctc5236_read_config PCI_SIZE_8 read address:0x%lx:0x%08lx\n",(ulong)address,  (ulong)(*valuep));
		return 0;
	case PCI_SIZE_16:
		*valuep = readw(address);
		debug("pcie_ctc5236_read_config PCI_SIZE_16 read address:0x%lx:0x%08lx\n",(ulong)address,  (ulong)(*valuep));
		return 0;
	case PCI_SIZE_32:
		*valuep = readl(address);
		debug("pcie_ctc5236_read_config PCI_SIZE_32 read address:0x%lx:0x%08lx\n",(ulong)address,  (ulong)(*valuep));
		return 0;
	default:
		return -EINVAL;
    }
}


static int pcie_ctc5236_write_config(struct udevice *bus, pci_dev_t bdf,
				      uint offset, ulong value,
				      enum pci_size_t size)
{
	struct pcie_ctc5236 *pcie = dev_get_priv(bus);
	void *address;

	debug("PCIE CFG write: bus->seq:%d : (b,d,f)=(%2d,%2d,%2d) \n",
	      bus->seq,PCI_BUS(bdf), PCI_DEV(bdf), PCI_FUNC(bdf));

	if (pcie_addr_valid(pcie, bdf))
		return 0;

	address = pcie_conf_address(pcie, bdf, offset);

	debug("(addr,val)=(0x%04lx,0x%08lx)\n",(ulong)address,value);
    
	switch (size) {
	case PCI_SIZE_8:
		writeb(value, address);
		return 0;
	case PCI_SIZE_16:
		writew(value, address);
		return 0;
	case PCI_SIZE_32:
		writel(value, address);
		return 0;
	default:
		return -EINVAL;
	}
}


static int pcie_ctc5236_probe(struct udevice *dev)
{
	struct pcie_ctc5236 *pcie = dev_get_priv(dev);
	int temp, data;

	/* setup the correct class code for the host bridge */
	writel(PCI_CLASS_BRIDGE_PCI << 16, pcie->ctrl + CTC5236_CTRL_PCI_IDS);
    
	/* setup the correct inbound window size*/
	writel(0x00000045, pcie->ctrl + CTC5236_CTRL_ATR_W1);

	/* setup the correct PHY PLL config */
	pcie_phy_read(CTC5236_PCIE_PHY_REG_PLL_CAL_1,&data);
	debug("default pcie phy pll cal:0x%08x\n",data);
	data = data | (1<<CTC5236_PCIE_PHY_REG_PLL_CAL_1_PLL_VAL);
	pcie_phy_write(CTC5236_PCIE_PHY_REG_PLL_CAL_1,data);

	pcie_phy_read(CTC5236_PCIE_PHY_REG_PLL_CAL_1,&data);
	debug("current pcie phy pll cal:0x%08x\n",data);
	temp = data & CTC5236_PCIE_PHY_REG_PLL_CAL_1_PLL_VCOSP_MASK;
	
	temp += 2<< CTC5236_PCIE_PHY_REG_PLL_CAL_1_PLL_VCOSP;
	

	data =data & (~CTC5236_PCIE_PHY_REG_PLL_CAL_1_PLL_VCOSP_MASK);
	
	data = data | temp;
	
	pcie_phy_write(CTC5236_PCIE_PHY_REG_PLL_CAL_1,data);

	pcie_phy_read(CTC5236_PCIE_PHY_REG_PLL_CAL_1,&data);
	

	
	pcie_phy_read(CTC5236_PCIE_PHY_REG_PLL_CAL_4,&data);
	
	data = data | (1<<CTC5236_PCIE_PHY_REG_PLL_CAL_4_MANU);
	
	pcie_phy_write(CTC5236_PCIE_PHY_REG_PLL_CAL_4,data);

	udelay(3000);
	
	pcie_phy_read(CTC5236_PCIE_PHY_REG_PLL_CAL_1,&data);
	debug("config pcie phy pll cal:0x%08x\n",data);
#if 0
	/* setup the correct I/O window size*/
	writel(0x4000001f, pcie->ctrl + CTC5236_CTRL_ATR_S1 + 0x40);
	writel(0x0, pcie->ctrl + CTC5236_CTRL_ATR_S1 + 0x48);
	writel(0x20001, pcie->ctrl + CTC5236_CTRL_ATR_S1 + 0x50);
	
	writel(0x60000037, pcie->ctrl + CTC5236_CTRL_ATR_S1 + 0x40);
	writel(0x20000000, pcie->ctrl + CTC5236_CTRL_ATR_S1 + 0x48);
	writel(0x0, pcie->ctrl + CTC5236_CTRL_ATR_S1 + 0x50);
#endif
    
	return 0;
}


static int pcie_ctc5236_ofdata_to_platdata(struct udevice *dev)
{
	struct pcie_ctc5236 *pcie = dev_get_priv(dev);
	const void *fdt = gd->fdt_blob;
	int node = dev_of_offset(dev);
	int ret;

	debug("pcie_ctc5236_ofdata_to_platdata fdt:%lx\n",(ulong)fdt);

	pcie->bus = dev;
    
	ret = fdt_get_named_resource(fdt, node, "reg", "reg-names",
				     "cfg", &pcie->cfg_res);
	if (ret) {
		printf("ctc5236-pcie: resource \"pcie cfg\" not found\n");
		return ret;
	}

	pcie->cfg = map_physmem(pcie->cfg_res.start,
				fdt_resource_size(&pcie->cfg_res),
				MAP_NOCACHE);

	ret = fdt_get_named_resource(fdt, node, "reg", "reg-names",
				     "ctrl", &pcie->ctrl_res);
	if (ret) {
		printf("ctc5236-pcie: resource \"pcie ctrl\" not found\n");
		return ret;
	}

	pcie->ctrl = map_physmem(pcie->ctrl_res.start,
				fdt_resource_size(&pcie->ctrl_res),
				MAP_NOCACHE);
	debug("%s cfg:%lx ctrl:%lx\n",dev->name, (unsigned long)pcie->cfg, (unsigned long)pcie->ctrl);
    
	return 0;
}


static const struct dm_pci_ops pcie_ctc5236_ops = {
	.read_config	= pcie_ctc5236_read_config,
	.write_config	= pcie_ctc5236_write_config,
};

static const struct udevice_id pcie_ctc5236_ids[] = {
	{ .compatible = "centec,ctc5236-pcie" },
	{ }
};

U_BOOT_DRIVER(pcie_ctc5236) = {
	.name			= "pcie_ctc5236",
	.id			= UCLASS_PCI,
	.of_match		= pcie_ctc5236_ids,
	.ops			= &pcie_ctc5236_ops,
	.ofdata_to_platdata	= pcie_ctc5236_ofdata_to_platdata,
	.probe			= pcie_ctc5236_probe,
	.priv_auto_alloc_size	= sizeof(struct pcie_ctc5236),
};


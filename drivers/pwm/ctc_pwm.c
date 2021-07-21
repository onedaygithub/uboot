/*
 * Centec PWM driver
 *
 * Author: wangyb <wangyb@centecnetworks.com>
 *
 * Copyright 2005-2018, Centec Networks (Suzhou) Co., Ltd.
 *
 */
#include <common.h>
#include <dm.h>
#include <pwm.h>
#include <asm/io.h>
#include <asm/arch/cpu.h>
#include <asm/arch/sysctl.h>

struct ctc_ctlr {
	uint control;		
	uint duty_cycle;	
};

struct ctc_pwm_priv{
	struct ctc_ctlr *regs;
};

static int ctc_pwm_set_enable(struct udevice *dev, uint channel, bool enable)
{
	struct ctc_pwm_priv *priv = dev_get_priv(dev);
	struct ctc_ctlr *regs = priv->regs;
	u32 cur_value;
	
	if (channel > 3)
		return -1;
	
	cur_value = readl(&regs->control + channel * 2);
	
	if (enable){
		cur_value |= 1 << 31;
		writel(cur_value, &regs->control + channel * 2);

	}else{
		cur_value &= ~ (1 << 31);
		writel(cur_value, &regs->control + channel * 2);
	}
	return 0;
}

static int ctc_pwm_set_config(struct udevice *dev, uint channel, uint period_ns, uint duty_ns)
{
	struct ctc_pwm_priv *priv = dev_get_priv(dev);
	struct ctc_ctlr *regs = priv->regs;
	u32 cur_value;
	
	if (channel > 3)
		return -1;
	
	if (period_ns < duty_ns && duty_ns < 1000 && period_ns < 1000)
		return -1;
	
	writel((duty_ns / 1000) & 0x00ffffff , &regs->duty_cycle + channel * 2);
	
	cur_value = readl(&regs->control + channel * 2);
	
	writel((period_ns / 1000) & 0x00ffffff, &regs->control + channel * 2);

	if (cur_value & 0x80000000)
		setbits_le32(&regs->control + channel * 2, BIT(31));
	return 0;
}

static int ctc_pwm_probe(struct udevice *dev)
{
	struct ctc_pwm_priv *priv = dev_get_priv(dev);
	int GpioMultiVal = 0;
	
	priv->regs = (struct ctc_ctlr *)dev_get_addr(dev);

	GpioMultiVal = readl(&sysctl_base->SysGpioMultiCtl);
	GpioMultiVal = (GpioMultiVal & PWM_GpioMulti_MASK) | PWM_GpioMulti;
    writel(GpioMultiVal, &sysctl_base->SysGpioMultiCtl);

	return 0;
}

static const struct pwm_ops ctc_pwm_ops = {
	.set_config	= ctc_pwm_set_config,
	.set_enable	= ctc_pwm_set_enable,
};

static const struct udevice_id ctc_pwm_ids[] = {
	{ .compatible = "centec,ctc5236-pwm" },
	{ }
};

U_BOOT_DRIVER(ctc_pwm) = {
	.name	= "ctc_pwm",
	.id	= UCLASS_PWM,
	.of_match = ctc_pwm_ids,
	.ops	= &ctc_pwm_ops,
	.probe	= ctc_pwm_probe,
	.priv_auto_alloc_size	= sizeof(struct ctc_pwm_priv),
};


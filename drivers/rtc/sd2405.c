/*
 * Centec RTC driver
 *
 * Author: wangyb <wangyb@centecnetworks.com>
 *
 * Copyright 2005-2018, Centec Networks (Suzhou) Co., Ltd.
 *
 */
 
/*	#define	DEBUG	*/

#include <common.h>
#include <command.h>
#include <dm.h>
#include <i2c.h>
#include <rtc.h>

/*
 * register indices
 */
#define SD2405_REG_SC			0x0	    /* seconds      00-59 */
#define SD2405_REG_MN			0x1	    /* minutes      00-59 */
#define SD2405_REG_HR			0x2	    /* hours        00-23 */
#define SD2405_REG_DW			0x3	    /* day of week   1-7  */
#define SD2405_REG_DT			0x4	    /* day of month 00-31 */
#define SD2405_REG_MO			0x5	    /* month        01-12 */
#define SD2405_REG_YR			0x6	    /* year         00-99 */

#define SD2405_REG_CTRL1		0xf	    /* control 1 */
#define SD2405_REG_CTRL2		0x10	/* control 2 */
#define SD2405_REG_CTRL3		0x11	/* control 3 ARST*/

#define SD2405_REG_LEN			7

/*
 * register write protect
 */
#define SD2405_REG_CONTROL1_WRITE	0x80
#define SD2405_REG_CONTROL2_WRITE	0x84

static int sd2405_i2c_clear_write_protect(struct udevice *dev)
{
	int ret;
	ret = dm_i2c_reg_write(dev, SD2405_REG_CTRL2, SD2405_REG_CONTROL1_WRITE);
	ret += dm_i2c_reg_write(dev, SD2405_REG_CTRL1, SD2405_REG_CONTROL2_WRITE);
	if (ret < 0){
		return -1;
	}
		
	return 0;
}

static int sd2405_i2c_set_write_protect(struct udevice *dev)
{
	int ret;

	ret = dm_i2c_reg_write(dev, SD2405_REG_CTRL1, 0);
	ret += dm_i2c_reg_write(dev, SD2405_REG_CTRL2, 0);
	if (ret < 0){
		return -1;
	}
	
	return 0;
}

static int sd2405_rtc_set(struct udevice *dev, const struct rtc_time *tm)
{		
	int ret;
	u8 buf[SD2405_REG_LEN] = {0};

	ret = sd2405_i2c_clear_write_protect(dev);
	if (ret < 0){
		return -1;
	}
	
	buf[SD2405_REG_SC] = bin2bcd(tm->tm_sec);
	buf[SD2405_REG_MN] = bin2bcd(tm->tm_min);
	buf[SD2405_REG_HR] = bin2bcd(tm->tm_hour)|0x80;
	buf[SD2405_REG_DW] = bin2bcd(tm->tm_wday);
	buf[SD2405_REG_DT] = bin2bcd(tm->tm_mday);
	buf[SD2405_REG_MO] = bin2bcd(tm->tm_mon);
	buf[SD2405_REG_YR] = bin2bcd(tm->tm_year % 100);

	debug("sec=%d, min=%d, hour=%d, wday=%d, mday=%d, mon=%d, year=%d\n", tm->tm_sec, tm->tm_min, tm->tm_hour,
					tm->tm_wday, tm->tm_mday, tm->tm_mon, tm->tm_year);

	ret = dm_i2c_write(dev, 0x0, buf, sizeof(buf));
	if (ret < 0){
		return -1;
	}
	
	ret = sd2405_i2c_set_write_protect(dev);
	if (ret < 0){
		return -1;
	}
	
	return 0;
}

static int sd2405_rtc_get(struct udevice *dev, struct rtc_time *tm)
{	
	int ret;
	u8 buf[SD2405_REG_LEN] = {0};
	
	ret = dm_i2c_read(dev, 0, buf, sizeof(buf));
	if (ret < 0){
		return -1;
	}
	
	tm->tm_sec = bcd2bin(buf[SD2405_REG_SC]);
	tm->tm_min = bcd2bin(buf[SD2405_REG_MN]);
	tm->tm_hour = bcd2bin(buf[SD2405_REG_HR] & 0x3f);
	tm->tm_wday = bcd2bin(buf[SD2405_REG_DW]);
	tm->tm_mday = bcd2bin(buf[SD2405_REG_DT]);
	tm->tm_mon = bcd2bin(buf[SD2405_REG_MO]);
	tm->tm_year = bcd2bin(buf[SD2405_REG_YR]) + 1900;
	if (tm->tm_year < 1970)
		tm->tm_year += 100;	/* assume we are in 1970...2069 */

	
	debug("%d %d %d %d %d %d %d\n", tm->tm_sec, tm->tm_min, tm->tm_hour,
					tm->tm_wday, tm->tm_mday, tm->tm_mon, tm->tm_year);

	return 0;
}

static int sd2405_rtc_reset(struct udevice *dev)
{
	/* TODO */
	return 0;
}

static const struct rtc_ops sd2405_rtc_ops = {
	.get = sd2405_rtc_get,
	.set = sd2405_rtc_set,
	.reset = sd2405_rtc_reset,
};

static const struct udevice_id sd2405_rtc_ids[] = {
	{ .compatible = "rtc-sd2405" },
	{ }
};

U_BOOT_DRIVER(rtc_pcf2127) = {
	.name	= "rtc-sd2405",
	.id	= UCLASS_RTC,
	.of_match = sd2405_rtc_ids,
	.ops	= &sd2405_rtc_ops,
};


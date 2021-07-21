/*
 * Centec Sensor driver
 *
 * Author: wangyb <wangyb@centecnetworks.com>
 *
 * Copyright 2005-2018, Centec Networks (Suzhou) Co., Ltd.
 *
 */
 
 /*	#define	DEBUG	*/
#include <common.h>
#include <i2c.h>
#include <dtt.h>

/* The LM77 registers */
#define LM77_REG_TEMP		0x00
#define LM77_REG_CONF		0x01
#define LM77_REG_TEMP_HYST	0x02
#define LM77_REG_TEMP_CRIT	0x03
#define LM77_REG_TEMP_MIN	0x04
#define LM77_REG_TEMP_MAX	0x05

#define DTT_I2C_DEV_CODE CONFIG_SYS_I2C_DTT_ADDR

int dtt_read(int sensor, int const reg)
{
	int dlen;
	uchar data[2];
	
	/* Calculate sensor address and register */
	sensor = DTT_I2C_DEV_CODE + (sensor & 0x07);

	if (reg == LM77_REG_CONF)
		dlen = 1;
	else
		dlen = 2;

	if (i2c_read(sensor, reg, 1, data, dlen) != 0)	
		return -1;

	if (dlen == 2)
		return (short)((data[0] << 8) | data[1]);
	
	return (int)data[0];
}

int dtt_write(int sensor, int const reg, int const val)
{
	int dlen;
	uchar data[2];
	
	/* Calculate sensor address and register */
	sensor = DTT_I2C_DEV_CODE + (sensor & 0x07);

	if(reg == LM77_REG_CONF){
		dlen = 1;
		data[0] = (char)(val & 0xff);
	}else{
		dlen =2;
		data[0] = (char)((val >> 8) & 0xff);
		data[1] = (char)(val & 0xff);
	}

	if (i2c_write(sensor, reg, 1, data, dlen) != 0)
		return -1;
	
	return 0;
}

int dtt_init_one(int sensor)
{
	/* Initialize the LM77 chip - turn off shutdown mode */
	int conf;

	conf = dtt_read(sensor, LM77_REG_CONF);

	if(conf & 1)
		dtt_write(sensor, LM77_REG_CONF, conf & 0xfe);
	
	return 0;
}

static inline int LM77_TEMP_FROM_REG(s16 value)
{
	return (value / 8) * 500;
}

int dtt_get_temp(int sensor)
{	
	int ret;
	
	ret = LM77_TEMP_FROM_REG(dtt_read(sensor, LM77_REG_TEMP)) / 1000;
	
	return ret;
} 


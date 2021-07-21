/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Liuht <liuht@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <switch.h>

#define OMCMEM_BASE  0x00041000 
#define SYS_TSINGMA_MDIO_CTLR_NUM 2
#define SYS_TSINGMA_TEMP_TABLE_NUM 166
#define SYS_TSINGMA_SENSOR_TIMEOUT 1000

int
ctc5236_switch_read(u32 offset, u32 len, u32* p_value)
{
	ctc_access_t *access = (ctc_access_t *)CTC_SWITCH_BASE;
	ctc_switch_cmd_status_u_t cmd_status_u;
	u32 timeout = 0x6400;
	u32 cmd_len = 0;
	u8 index = 0;

	if(!p_value){
		printf("switch read:value buffer is NULL!\n");
		return -1;
	}

	/* switch only have 16 databuf, len must not exceed 16 */
	if ((16 < len) || (0 == len))
	{
		printf("switch read: length error! len = %d \n", len);
		return -1;
	}
	/* cmdDataLen must be power of 2 */
	if ((len & (len - 1)) == 0)
	{
		cmd_len = len;
	}
	else
	{
		cmd_len = len;
		do
		{
			cmd_len++;
		}
		while ((cmd_len <= 16) && (cmd_len & (cmd_len - 1)));
	}

	/* 1. write CmdStatusReg */
	memset(&cmd_status_u, 0, sizeof(ctc_switch_cmd_status_u_t));
	cmd_status_u.cmd_status.cmdReadType = 1;
	cmd_status_u.cmd_status.cmdEntryWords = (len==16)?0:len;   /* normal operate only support 1 entry */
	cmd_status_u.cmd_status.cmdDataLen = len;
	writel(cmd_status_u.val, &access->cmd_status);
	/* 2. write AddrReg */
	writel(offset, &access->addr);
	/* 3. polling status and check */
	cmd_status_u.val = readl(&access->cmd_status);
	while (!(cmd_status_u.cmd_status.reqProcDone) && (--timeout))
	{
		cmd_status_u.val = readl(&access->cmd_status);
	}
	/* 4. check cmd done */
	if (!(cmd_status_u.cmd_status.reqProcDone))
	{
		printf("switch read error! cmd_status = %x\n", cmd_status_u.val);
		return -1;
	}
	/* 5. check pcie read status */
	if (cmd_status_u.cmd_status.reqProcError != 0)
	{
		printf("pci read error! cmd_status = %x, offset = 0x%x\n", cmd_status_u.val, offset);
		return -1;
	}

	/* 6. read data from buffer */
	for (index = 0; index < len; index++)
	{
		p_value[index] = readl(&access->data[index]);
	}
	
	return 0;
}

int
ctc5236_switch_write(u32 offset, u32 len, u32* p_value)
{
	ctc_access_t *access = (ctc_access_t *)CTC_SWITCH_BASE;
	ctc_switch_cmd_status_u_t cmd_status_u;
	u32 timeout = 0x6400;  /* need to be confirmed */
	u32 cmd_len = 0;
	u8 index = 0;

	if(!p_value){
		printf("switch write:value buffer is NULL!\n");
		return -1;
	}

	/* switch only have 16 databuf, len must not exceed 16 */
	if ((16 < len) || (0 == len))
	{
		printf("switch write length error! len = %d \n", len);
		return -1;
	}

	/* cmdDataLen must be power of 2 */
	if ((len & (len - 1)) == 0)
	{
		cmd_len = len;
	}
	else
	{
		cmd_len = len;
		do
		{
			cmd_len++;
		}
		while ((cmd_len <= 16) && (cmd_len & (cmd_len - 1)));
	}

	/* 1. write CmdStatusReg */
	memset(&cmd_status_u, 0, sizeof(ctc_switch_cmd_status_t));
	cmd_status_u.cmd_status.cmdReadType = 0;
	cmd_status_u.cmd_status.cmdEntryWords = (len==16)?0:len;
	cmd_status_u.cmd_status.cmdDataLen = len; /* Notice: for 1 entry op, cmdDatalen eq cmdEntryWords, but for mutil entry, len = cmd_len */
	writel(cmd_status_u.val, &access->cmd_status);
	/* 2. write AddrReg */
	writel(offset, &access->addr);
	/* 3. write data into databuffer */
	for (index = 0; index < len; index++)
	{
		writel(p_value[index], &access->data[index]);
	}

	/* 4. polling status and check */
	cmd_status_u.val  = readl(&access->cmd_status);
	while (!(cmd_status_u.cmd_status.reqProcDone) && (--timeout))
	{
		cmd_status_u.val  = readl(&access->cmd_status);
	}

	/* 5. check cmd done */
	if (!(cmd_status_u.cmd_status.reqProcDone))
	{
		printf("switch write error! cmd_status = %x\n", cmd_status_u.val);
		return -1;
	}

	/* 6. check switch read status */
	if (cmd_status_u.cmd_status.reqProcError != 0)
	{
		printf("switch write error! cmd_status = %x, offset=0x%x\n", cmd_status_u.val, offset);
		return -1;
	}

	return 0;
}

static int
_sys_tsingma_peri_get_temp_with_code(u8 lchip, u32 temp_code, u32* p_temp_val)
{
    u16 temp_mapping_tbl[SYS_TSINGMA_TEMP_TABLE_NUM+1] = {
        804, 801, 798, 795, 792, 790, 787, 784, 781, 778, 775, 772, 769, 766, 763, 761, 758, 755, 752, 749, /*-40~-21*/
        746, 743, 740, 737, 734, 731, 728, 725, 722, 719, 717, 714, 711, 708, 705, 702, 699, 696, 693, 690, /*-20~-1*/
        687, 684, 681, 678, 675, 672, 669, 666, 663, 660, 658, 655, 652, 649, 646, 643, 640, 637, 634, 631, /*0~19*/
        628, 625, 622, 619, 616, 613, 610, 607, 604, 601, 599, 596, 593, 590, 587, 584, 581, 578, 575, 572, /*20~39*/
        569, 566, 563, 560, 557, 554, 551, 548, 545, 542, 540, 537, 534, 531, 528, 525, 522, 519, 516, 513, /*40~59*/
        510, 507, 504, 501, 498, 495, 492, 489, 486, 483, 481, 478, 475, 472, 469, 466, 463, 460, 457, 454, /*60~79*/
        451, 448, 445, 442, 439, 436, 433, 430, 427, 424, 421, 418, 415, 412, 409, 406, 403, 400, 397, 394, /*80~99*/
        391, 388, 385, 382, 379, 376, 373, 370, 367, 364, 361, 358, 355, 352, 349, 346, 343, 340, 337, 334, /*100~119*/
        331, 328, 325, 322, 319, 316, 0};                                                                   /*120~125*/
    u8 index = 0;

    /*if ((temp_code > temp_mapping_tbl[0]) || (temp_code < temp_mapping_tbl[SYS_TSINGMA_TEMP_TABLE_NUM-1]))
    {
        SYS_PERI_DBG_OUT(CTC_DEBUG_LEVEL_ERROR, "temp code error %d\n", temp_code);
        return CTC_E_HW_INVALID_INDEX;
    }*/

    for (index = 0; index< SYS_TSINGMA_TEMP_TABLE_NUM; index++)
    {
        if ((temp_code <= temp_mapping_tbl[index]) && (temp_code > temp_mapping_tbl[index+1]))
        {
            break;
        }
    }

    if (index < 39)
    {
        *p_temp_val = 40 - index + (1 << 31);
    }
    else
    {
        *p_temp_val = index - 40;
    }

    return 0;
}

int get_switch_temperature(void)
{	
	int temperature;
	u32 value, offset, p_value=0;
	u32 timeout = SYS_TSINGMA_SENSOR_TIMEOUT;
	
	value = 0x310c7;
	offset = 0xf *4;
	ctc5236_switch_write(OMCMEM_BASE+offset, 1, &value);
	
	/*config RTHMC_RST=1*/
	/*mask_write tbl-reg OmcMem 0x10 offset 0x0 0x00000010 0x00000010*/
	offset = 0x10 *4;
	ctc5236_switch_read(OMCMEM_BASE+offset, 1, &value);
	value |= BIT(4);
	ctc5236_switch_write(OMCMEM_BASE+offset, 1, &value);
	
	/*wait RTHMC_RST=1*/
	/*read tbl-reg OmcMem 0x10 offset 0x0*/
	timeout = SYS_TSINGMA_SENSOR_TIMEOUT;
	offset = 0x10 * 4;
	while(timeout){
		timeout--;
		ctc5236_switch_read(OMCMEM_BASE+offset, 1, &value);
		if ( 0 == (BIT(4) & value))
			break;
		mdelay(1);
	}
	if( 0 == timeout){
		return 0xffff;
	}
		
	/*config ENBIAS=1£¬ENVR=1£¬ENAD=1*/
	/*mask_write tbl-reg OmcMem 0x11 offset 0x0 0x02000007 0x03000007*/	
	offset = 0x11 * 4;
	ctc5236_switch_read(OMCMEM_BASE+offset, 1, &value);
	value |= BIT(0) | BIT(1) | BIT(2) | BIT(25);
	value &= ~BIT(24);
	ctc5236_switch_write(OMCMEM_BASE+offset, 1, &value);
	
	mdelay(1);
	
	/*set RTHM_MODE=1,RSAMPLE_DONE_INTEN=1,RDUMMY_THMRD=1,RV_SAMPLE_EN=1*/
	/*mask_write tbl-reg OmcMem 0x10 offset 0x0 0x00000203 0x00000003*/
	/*mask_write tbl-reg OmcMem 0x8 offset 0x0 0x00000004 0x00000004*/
	/*mask_write tbl-reg OmcMem 0x12 offset 0x0 0x00000001 0x00000001*/
	offset = 0x10 * 4;
	ctc5236_switch_read(OMCMEM_BASE+offset, 1, &value);
	value |= BIT(0) | BIT(1) | BIT(9);
	ctc5236_switch_write(OMCMEM_BASE+offset, 1, &value);
	offset = 0x8 * 4;
	ctc5236_switch_read(OMCMEM_BASE+offset, 1, &value);
	value |= BIT(2);
	ctc5236_switch_write(OMCMEM_BASE+offset, 1, &value);
	offset = 0x12 * 4;
	ctc5236_switch_read(OMCMEM_BASE+offset, 1, &value);
	value |= BIT(0);
	ctc5236_switch_write(OMCMEM_BASE+offset, 1, &value);

	/*mask_write tbl-reg OmcMem 0x10 offset 0x0 0x00000001 0x00000001*/	
	offset = 0x10 * 4;
	ctc5236_switch_read(OMCMEM_BASE+offset, 1, &value);
	value |= BIT(0);
	ctc5236_switch_write(OMCMEM_BASE+offset, 1, &value);
	
	mdelay(1);
	
	/*mask_write tbl-reg OmcMem 0x12 offset 0x0 0x00000001 0x00000001*/	
	offset = 0x12 * 4;
	ctc5236_switch_read(OMCMEM_BASE+offset, 1, &value);
	value |= BIT(0);
	ctc5236_switch_write(OMCMEM_BASE+offset, 1, &value);
	
	/*Wait((mask_read tbl-reg OmcMem 0xa offset 0x0 0x00000004) =1)*/
	timeout = SYS_TSINGMA_SENSOR_TIMEOUT;
	offset = 0xa * 4;
	while(timeout)
	{
	    timeout--;
	    ctc5236_switch_read(OMCMEM_BASE+offset, 1, &value);
	    if (value & BIT(2))
			break;

	    mdelay(1);
	}
	if (0 == timeout){
		return 0xffff;
	}
	
	/*mask_write tbl-reg OmcMem 0x11 offset 0x0 0x00000006 0x00000006*/
	offset = 0x11 * 4;
	ctc5236_switch_read(OMCMEM_BASE+offset, 1, &value);
	value |= BIT(1) | BIT(2);
	ctc5236_switch_write(OMCMEM_BASE+offset, 1, &value);

	mdelay(10);
	 
	/*read-reg OmcMem 0xd offset 0x0*/
	offset = 0xd * 4;
	ctc5236_switch_read(OMCMEM_BASE+offset, 1, &value);
	_sys_tsingma_peri_get_temp_with_code(0, value, &p_value);
	temperature = p_value & ~BIT(31);
	if((p_value & BIT(31)) == 0)
		return temperature;
	else
		return -temperature;

}


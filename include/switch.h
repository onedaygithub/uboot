/*
 * (C) Copyright 2004-2017 Centec Networks (suzhou) Co., LTD.
 * Liuht <liuht@centecnetworks.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

 #ifndef __CTC5236_SWITCH_H__
 #define __CTC5236_SWITCH_H__

 typedef struct ctc_switch_cmd_status_s
{
    u32 cmdReadType      : 1;
    u32 pcieReqCmdChk    : 3;
    u32 cmdEntryWords    : 4;
    u32 cmdDataLen       : 5;
    u32 reserved         : 1;
    u32 pcieReqError     : 1;
    u32 pcieDataError    : 1;
    u32 reqProcDone      : 1;
    u32 reqProcError     : 1;
    u32 reqProcTimeout   : 1;
    u32 reqProcAckError  : 1;
    u32 reqProcAckCnt    : 5;
    u32 regProcBusy        : 1;
    u32 cmdStatusWaitReq   : 1;
    u32 pciePoisoned        : 1;
    u32 regProcState        : 3;
    u32 pcieReqOverlap        : 1;
} ctc_switch_cmd_status_t;

typedef union drv_pci_cmd_status_u_e
{
    ctc_switch_cmd_status_t cmd_status;
    u32 val;
} ctc_switch_cmd_status_u_t;

#define CTC_SWITCH_BASE                   0x31100000
#define CTC_SWITCH_CMD_STATUS      0x0
#define CTC_SWITCH_ADDR           0x4
#define CTC_SWITCH_DATA_BUF   0x8

 typedef struct ctc_access_s
{
	u32 cmd_status;
	u32 addr;
	u32 data[16];
} ctc_access_t;

int ctc5236_switch_read(u32 offset, u32 len, u32* p_value);
int ctc5236_switch_write(u32 offset, u32 len, u32* p_value);
int get_switch_temperature(void);
#endif
 
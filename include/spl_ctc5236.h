/*
 * (C) Copyright 2017 Centec
 *
 * (C) Copyright 2017-2018
 * Liuht Software Engineering, liuht@centecnetworks.com.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 ********************************************************************
 ********************************************************************
 */

#ifndef __SPL_CTC5236_H__
#define __SPL_CTC5236_H__

#define NULL_RETURN(PTR) \
    if (!PTR)                   \
        return -1;     

#define ZERO_RETURN(VAL)  \
    if (!VAL)                   \
        return -1;     

#define NONZ_RETURN(VAL)  \
    if (VAL)                   \
        return -1;  

#define NEGA_RETURN(VAL)  \
    if (VAL < 0)                   \
        return -1;     

#define DIFF_RETURN(VAL1, VAL2)  \
    if (VAL1 != VAL2)                   \
        return -1;  

#define GRTR_RETURN(VAL1, VAL2)  \
    if (VAL1 > VAL2)                   \
        return -1;  

#define NONZ_GOTO_END(VAL, END) \
    if (VAL)                   \
        goto END;  

#define NOGT_GOTO_END(VAL1, VAL2, END) \
    if (VAL1 <= VAL2)                   \
        goto END; 
    
#define NEGA_GOTO_END(VAL, END)  \
    if (VAL < 0)                   \
        goto END; 

#define ZERO_GOTO_END(VAL, END)  \
    if (!VAL)                   \
        goto END; 

#define GRTR_GOTO_END(VAL1, VAL2, END)  \
    if (VAL1 > VAL2)                   \
        goto END;
    
int ctc5236_image_check_dcrc(ulong getdcrc, struct spl_image_info *spl_image);
int ctc5236_image_check_hcrc(const ctc5236_ih_t *hdr);
int ctc5236_spl_parse_ih(struct spl_image_info *spl_image, 
    const ctc5236_ih_t *header);
void ctc_memcpy_data(void *buf, u32 addr, u32 len);

#endif


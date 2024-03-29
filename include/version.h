/*
 * (C) Copyright 2000-2006
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef	__VERSION_H__
#define	__VERSION_H__

#include <timestamp.h>

#ifndef DO_DEPS_ONLY
#include "generated/version_autogenerated.h"
#endif

#if 0
#define U_BOOT_VERSION_STRING U_BOOT_VERSION " (" U_BOOT_DATE " - " \
	U_BOOT_TIME " " U_BOOT_TZ ")" CONFIG_IDENT_STRING
#endif

#define U_BOOT_VERSION_STRING "CLDK v20.05 " U_BOOT_VERSION " (" U_BOOT_DATE " - " \
	U_BOOT_TIME " " U_BOOT_TZ ")" CONFIG_IDENT_STRING

#ifndef __ASSEMBLY__
extern const char version_string[];
#endif	/* __ASSEMBLY__ */
#endif	/* __VERSION_H__ */

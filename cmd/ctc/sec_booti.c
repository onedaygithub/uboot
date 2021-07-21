#include <common.h>
#include <command.h>
#include <bootm.h>
#include <environment.h>
#include <dm.h>
#include <errno.h>
#include <asm/io.h>
#include <mapmem.h>
#include <linux/kernel.h>
#include <linux/sizes.h>
#include <asm/arch/cpu.h>
#include <asm/arch/sysctl.h>
#include <image-verify.h>
#include <ctcefuse.h>

DECLARE_GLOBAL_DATA_PTR;

static cmd_tbl_t cmd_bootm_sub[] = {
	U_BOOT_CMD_MKENT(start, 0, 1, (void *)BOOTM_STATE_START, "", ""),
	U_BOOT_CMD_MKENT(loados, 0, 1, (void *)BOOTM_STATE_LOADOS, "", ""),
#ifdef CONFIG_SYS_BOOT_RAMDISK_HIGH
	U_BOOT_CMD_MKENT(ramdisk, 0, 1, (void *)BOOTM_STATE_RAMDISK, "", ""),
#endif
#ifdef CONFIG_OF_LIBFDT
	U_BOOT_CMD_MKENT(fdt, 0, 1, (void *)BOOTM_STATE_FDT, "", ""),
#endif
	U_BOOT_CMD_MKENT(cmdline, 0, 1, (void *)BOOTM_STATE_OS_CMDLINE, "", ""),
	U_BOOT_CMD_MKENT(bdt, 0, 1, (void *)BOOTM_STATE_OS_BD_T, "", ""),
	U_BOOT_CMD_MKENT(prep, 0, 1, (void *)BOOTM_STATE_OS_PREP, "", ""),
	U_BOOT_CMD_MKENT(fake, 0, 1, (void *)BOOTM_STATE_OS_FAKE_GO, "", ""),
	U_BOOT_CMD_MKENT(go, 0, 1, (void *)BOOTM_STATE_OS_GO, "", ""),
};

int  do_validate_image(ulong image_addr)
{
    u8 nv;
    u32 image_size;
    u8 keyindex;
    ulong data;
    struct ctc_cert *cert;
    u8 hash[CER_HASH_LEN], sochash[CER_HASH_LEN];

    cert = (struct ctc_cert *)(image_addr);
    data = (image_addr+CTC5236_CERT_SIZE);

    efuse_get_nv2(&nv);
    if(nv > cert->nv){
        return -1;
    }
    if(ctc_get_pubkey_hash(cert, hash)<0){
        return -1;
    }    
    ctc_load_pkey_hash(sochash);
    if(memcmp(sochash, hash, CER_HASH_LEN)){
        return -1;
    } 
    
    if(cert->encrypt){
        u8 *aes_key;
        efuse_get_sbhk(&aes_key);
        ctc_aes_decrypt(aes_key, (u8 *)data, (u8 *)data, cert->img_size);
    }

    if(ctc_get_image_hash((u8 *)data, cert->img_size, hash)<0){
        return -1;
    }
    
    if(ctc_get_keyindex(cert, &keyindex)){
        return -1;
    }
	
    if(ctc_verify_sign(cert, hash, keyindex)){
        return -1;
    }

    image_size = cert->img_size;
    memcpy((void *)image_addr, (void *)data, image_size);

    return 0;
}
/* See Documentation/arm64/booting.txt in the Linux kernel */
struct Image_header {
	uint32_t	code0;		/* Executable code */
	uint32_t	code1;		/* Executable code */
	uint64_t	text_offset;	/* Image load offset, LE */
	uint64_t	image_size;	/* Effective Image size, LE */
	uint64_t	flags;		/* Kernel flags, LE */
	uint64_t	res2;		/* reserved */
	uint64_t	res3;		/* reserved */
	uint64_t	res4;		/* reserved */
	uint32_t	magic;		/* Magic number */
	uint32_t	res5;
};
#define LINUX_ARM64_IMAGE_MAGIC	0x644d5241

static int booti_setup(bootm_headers_t *images)
{
	struct Image_header *ih;
	uint64_t dst;
	uint64_t image_size, text_offset;

	ih = (struct Image_header *)map_sysmem(images->ep, 0);

	if (ih->magic != le32_to_cpu(LINUX_ARM64_IMAGE_MAGIC)) {
		puts("Bad Linux ARM64 Image magic!\n");
		return 1;
	}

	/*
	 * Prior to Linux commit a2c1d73b94ed, the text_offset field
	 * is of unknown endianness.  In these cases, the image_size
	 * field is zero, and we can assume a fixed value of 0x80000.
	 */
	if (ih->image_size == 0) {
		puts("Image lacks image_size field, assuming 16MiB\n");
		image_size = 16 << 20;
		text_offset = 0x80000;
	} else {
		image_size = le64_to_cpu(ih->image_size);
		text_offset = le64_to_cpu(ih->text_offset);
	}

	/*
	 * If bit 3 of the flags field is set, the 2MB aligned base of the
	 * kernel image can be anywhere in physical memory, so respect
	 * images->ep.  Otherwise, relocate the image to the base of RAM
	 * since memory below it is not accessible via the linear mapping.
	 */
	if (le64_to_cpu(ih->flags) & BIT(3))
		dst = images->ep - text_offset;
	else
		dst = gd->bd->bi_dram[0].start;

	dst = ALIGN(dst, SZ_2M) + text_offset;

	unmap_sysmem(ih);

	if (images->ep != dst) {
		void *src;

		debug("Moving Image from 0x%lx to 0x%llx\n", images->ep, dst);

		src = (void *)images->ep;
		images->ep = dst;
		memmove((void *)dst, src, image_size);
	}

	return 0;
}

/*
 * Image booting support
 */
static int booti_start(cmd_tbl_t *cmdtp, int flag, int argc,
			char * const argv[], bootm_headers_t *images)
{
	int ret;
	struct Image_header *ih;

	ret = do_bootm_states(cmdtp, flag, argc, argv, BOOTM_STATE_START,
			      images, 1);

	/* Setup Linux kernel Image entry point */
	if (!argc) {
		images->ep = load_addr;
		debug("*  kernel: default image load address = 0x%08lx\n",
				load_addr);
	} else {
		images->ep = simple_strtoul(argv[0], NULL, 16);
		debug("*  kernel: cmdline image address = 0x%08lx\n",
			images->ep);
	}

	ret = booti_setup(images);
	if (ret != 0)
		return 1;

	ih = (struct Image_header *)map_sysmem(images->ep, 0);

	lmb_reserve(&images->lmb, images->ep, le32_to_cpu(ih->image_size));

	unmap_sysmem(ih);

	/*
	 * Handle the BOOTM_STATE_FINDOTHER state ourselves as we do not
	 * have a header that provide this informaiton.
	 */
	if (bootm_find_images(flag, argc, argv))
		return 1;

	return 0;
}

int do_secure_booti(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int ret;

    if(strncmp(argv[3], "-", 1) 
        && do_validate_image(simple_strtoul(argv[3], NULL, 0))){
        return -1;
    }
    
    if(strncmp(argv[1], "-", 1) 
        && do_validate_image(simple_strtoul(argv[1], NULL, 0))){
        return -1;
    }
    if(strncmp(argv[2], "-", 1) 
        && do_validate_image(simple_strtoul(argv[2], NULL, 0))){
        return -1;
    }
    
	/* Consume 'booti' */
	argc--; argv++;

	if (booti_start(cmdtp, flag, argc, argv, &images))
		return 1;

	/*
	 * We are doing the BOOTM_STATE_LOADOS state ourselves, so must
	 * disable interrupts ourselves
	 */
	bootm_disable_interrupts();

	images.os.os = IH_OS_LINUX;
	images.os.arch = IH_ARCH_ARM64;
	ret = do_bootm_states(cmdtp, flag, argc, argv,
#ifdef CONFIG_SYS_BOOT_RAMDISK_HIGH
			      BOOTM_STATE_RAMDISK |
#endif
			      BOOTM_STATE_OS_PREP | BOOTM_STATE_OS_FAKE_GO |
			      BOOTM_STATE_OS_GO,
			      &images, 1);

	return ret;
}

static int do_secure_go(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	ulong	addr;
    ulong (*entry)(void);

	if (argc < 2)
		return CMD_RET_USAGE;

    addr = simple_strtoul(argv[1], NULL, 16);
    if(do_validate_image(addr)){
        return -1;
    }
    entry = (void *)addr;
    printf("do_secure_go 0x%x\n", (u32)entry);
    
    return entry();
}

static int do_bootm_subcommand(cmd_tbl_t *cmdtp, int flag, int argc,
			char * const argv[])
{
	int ret = 0;
	long state;
	cmd_tbl_t *c;

	c = find_cmd_tbl(argv[0], &cmd_bootm_sub[0], ARRAY_SIZE(cmd_bootm_sub));
	argc--; argv++;

	if (c) {
		state = (long)c->cmd;
		if (state == BOOTM_STATE_START)
			state |= BOOTM_STATE_FINDOS | BOOTM_STATE_FINDOTHER;
	} else {
		/* Unrecognized command */
		return CMD_RET_USAGE;
	}

	if (((state & BOOTM_STATE_START) != BOOTM_STATE_START) &&
	    images.state >= state) {
		printf("Trying to execute a command out of order\n");
		return CMD_RET_USAGE;
	}

	ret = do_bootm_states(cmdtp, flag, argc, argv, state, &images, 0);

	return ret;
}

int do_secure_bootm(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	ulong	addr;
	char *endp0;
	
#ifdef CONFIG_NEEDS_MANUAL_RELOC
	static int relocated = 0;

	if (!relocated) {
		int i;

		/* relocate names of sub-command table */
		for (i = 0; i < ARRAY_SIZE(cmd_bootm_sub); i++)
			cmd_bootm_sub[i].name += gd->reloc_off;

		relocated = 1;
	}
#endif

	 addr = simple_strtoul(argv[1], &endp0, 16);
	 if(do_validate_image(addr)){
		return -1;
	 }
	/* determine if we have a sub command */
	argc--; argv++;
	if (argc > 0) {
		char *endp;

		simple_strtoul(argv[0], &endp, 16);
		/* endp pointing to NULL means that argv[0] was just a
		 * valid number, pass it along to the normal bootm processing
		 *
		 * If endp is ':' or '#' assume a FIT identifier so pass
		 * along for normal processing.
		 *
		 * Right now we assume the first arg should never be '-'
		 */
		if ((*endp != 0) && (*endp != ':') && (*endp != '#'))
			return do_bootm_subcommand(cmdtp, flag, argc, argv);
	}

	return do_bootm_states(cmdtp, flag, argc, argv, BOOTM_STATE_START |
		BOOTM_STATE_FINDOS | BOOTM_STATE_FINDOTHER |
		BOOTM_STATE_LOADOS |
#ifdef CONFIG_SYS_BOOT_RAMDISK_HIGH
		BOOTM_STATE_RAMDISK |
#endif
#if defined(CONFIG_PPC) || defined(CONFIG_MIPS)
		BOOTM_STATE_OS_CMDLINE |
#endif
		BOOTM_STATE_OS_PREP | BOOTM_STATE_OS_FAKE_GO |
		BOOTM_STATE_OS_GO, &images, 1);
}

static char booti_help_text[] =
	"[addr [initrd[:size]] [fdt]]\n"
	"    - boot arm64 Linux Image stored in memory\n"
	"\tThe argument 'initrd' is optional and specifies the address\n"
	"\tof an initrd in memory. The optional parameter ':size' allows\n"
	"\tspecifying the size of a RAW initrd.\n"
	"";

#ifdef CONFIG_SYS_LONGHELP
static char bootm_help_text[] =
	"[addr [arg ...]]\n    - boot application image stored in memory\n"
	"\tpassing arguments 'arg ...'; when booting a Linux kernel,\n"
	"\t'arg' can be the address of an initrd image\n"
#if defined(CONFIG_OF_LIBFDT)
	"\tWhen booting a Linux kernel which requires a flat device-tree\n"
	"\ta third argument is required which is the address of the\n"
	"\tdevice-tree blob. To boot that kernel without an initrd image,\n"
	"\tuse a '-' for the second argument. If you do not pass a third\n"
	"\ta bd_info struct will be passed instead\n"
#endif
#if defined(CONFIG_FIT)
	"\t\nFor the new multi component uImage format (FIT) addresses\n"
	"\tmust be extended to include component or configuration unit name:\n"
	"\taddr:<subimg_uname> - direct component image specification\n"
	"\taddr#<conf_uname>   - configuration specification\n"
	"\tUse iminfo command to get the list of existing component\n"
	"\timages and configurations.\n"
#endif
	"\nSub-commands to do part of the bootm sequence.  The sub-commands "
	"must be\n"
	"issued in the order below (it's ok to not issue all sub-commands):\n"
	"\tstart [addr [arg ...]]\n"
	"\tloados  - load OS image\n"
#if defined(CONFIG_SYS_BOOT_RAMDISK_HIGH)
	"\tramdisk - relocate initrd, set env initrd_start/initrd_end\n"
#endif
#if defined(CONFIG_OF_LIBFDT)
	"\tfdt     - relocate flat device tree\n"
#endif
	"\tcmdline - OS specific command line processing/setup\n"
	"\tbdt     - OS specific bd_t processing\n"
	"\tprep    - OS specific prep before relocation or go\n"
#if defined(CONFIG_TRACE)
	"\tfake    - OS specific fake start without go\n"
#endif
	"\tgo      - start OS";
#endif

U_BOOT_CMD(
	secure_booti,	CONFIG_SYS_MAXARGS,	1,	do_secure_booti,
	"Validate and boot arm64 Linux Image image from memory", booti_help_text
);

U_BOOT_CMD(
    secure_go, 2, 1, do_secure_go,
    "Run efuse burn tool in on-chip ram",
    "   - Run efuse burn tool\n" 
);

U_BOOT_CMD(
	secure_bootm,	CONFIG_SYS_MAXARGS,	1,	do_secure_bootm,
	"boot application image from memory", bootm_help_text
);


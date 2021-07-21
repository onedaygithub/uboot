
#include <common.h>
#include <command.h>
#include <environment.h>
#include <dm.h>
#include <errno.h>
#include <spi.h>
#include <asm/io.h>
#include <asm/arch/cpu.h>
#include <asm/arch/sysctl.h>
#include <net.h>
#include <asm/gpio.h>
#include <phy.h>
#include <ctcmac.h>
#ifdef CONFIG_CTC_SWITCH
#include <switch.h>
#endif
#include <ctc5236_ddr.h>
#include <usb.h>
#include <ctc_i2c.h>
static struct ctcmac_status g_status[2];

int  upgrade_uboot_remote(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char cmd[1024];
    int mmc_block = 0, sector = 0, boot_size = 0;
    char erase_sector[10], erase_cmd[100];
    char sf_write[10], sf_write_cmd[100];
    char emmc_block[10], emmc_cmd[100];
    char sd_block[10], sd_cmd[100];
    int verify;

    if(argc != 3)
    {
        printf("Usage:\n%s\n", cmdtp->usage);
        return 1;
    }
    while(argc == 3){
    	if(strncmp(argv[2], "spiflash", 8) ==0 )
		break;
	else if(strncmp(argv[2], "emmc", 4) ==0)
		break;
	else if(strncmp(argv[2], "sd", 2) ==0)
		break;
	else
	{
		printf("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}
    }
    printf("Are you sure you will upgrade u-boot to %s?(y or n)", argv[1]);
    while(1){

        verify = getc();
        if(verify =='y'){
        	break;
        }
        if(verify== 'n'){
        	printf("Abort!\n");
        	return 1;
        }
        printf("\nAre you sure you will upgrade u-boot to %s?(y or n)", argv[1]);
    }
    printf("\n");

    memset(cmd, 0, 1024);
    strcpy(cmd, "tftpboot ");
    strcat(cmd, "82000000 ");
    strcat(cmd, argv[1]);
    if (run_command(cmd, flag))
        return -1;
    if(!strncmp(argv[2], "spiflash", 8))
    {
	if (run_command("sf probe 1:0 1000000 3", flag))
            return -1;
	/* SF: Detected mt25qu256aba8esy with page size 256 Bytes, erase size 4 KiB, total 32 MiB */
		
	/* sf erase */
	sector = (net_boot_file_size / 65536 + 1) * 65536;
	sprintf(erase_sector, "%x", sector);

	strcpy(erase_cmd, "sf erase 0 ");
	strcat(erase_cmd, erase_sector);

       if (run_command(erase_cmd, flag))
           return -1;

	/* sf write */
	 boot_size = net_boot_file_size;    
        sprintf(sf_write, "%x", boot_size);

	strcpy(sf_write_cmd, "sf write 82000000 0 ");
	strcat(sf_write_cmd, sf_write);

        if (run_command(sf_write_cmd, flag))
            return -1;
    }
    else if(!strncmp(argv[2], "emmc", 4))
    {
        run_command("mmc rescan", flag);
        run_command("mmc partconf 0 1 1 1", flag);

        mmc_block = net_boot_file_size / 512 + 1;
	 sprintf(emmc_block, "%x", mmc_block);

	 strcpy(emmc_cmd, "mmc write 82000000 0 ");
	 strcat(emmc_cmd, emmc_block);	
	 
        if (run_command(emmc_cmd, flag))
            return -1;
     run_command("mmc partconf 0 0 0 0", flag);
	
    }
    else if(!strncmp(argv[2], "sd", 2))
    {     
        run_command("mmc rescan", flag);
        mmc_block = net_boot_file_size / 512 + 1;
	 sprintf(sd_block, "%x", mmc_block);

	 strcpy(sd_cmd, "mmc write 82000000 8 ");
	 strcat(sd_cmd, sd_block);
	 
        if (run_command(sd_cmd, flag))
            return -1;
    }

    return 0;
}

#ifdef CONFIG_CTC_SWITCH
int  do_switch_acces(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	u32 addr, val, len;

	if((argc != 4) && (argc != 5)){
		return -1;
	}
	
	if(!strncmp(argv[1], "read", 4)){
		addr = simple_strtoul(argv[2], NULL, 0); 
		len    = simple_strtoul(argv[3], NULL, 0); 
		ctc5236_switch_read(addr, len, &val);
		printf("%08x: %08x\n", addr, val);
	}else if(!strncmp(argv[1], "write", 5)){
		addr  = simple_strtoul(argv[2], NULL, 0); 
		val    = simple_strtoul(argv[3], NULL, 0); 
		len    = simple_strtoul(argv[4], NULL, 0); 
		ctc5236_switch_write(addr, len, &val);
	}else{
		return -1;
	}
	
	return 0;
}
#endif

#ifdef CONFIG_CTC_CPUMAC_DEBUG
static void ctcmac_dump_pkt_stats(struct ctcmac_pkt_stats *stats)
{
	printf("RX bytes ucast: %lld\n",   stats->rx_good_ucast_bytes            );
	printf("RX frame ucast: %lld\n",   stats->rx_good_ucast_pkt              );
	printf("RX bytes mcast: %lld\n",   stats->rx_good_mcast_bytes            );
	printf("RX frame mcast: %lld\n",   stats->rx_good_mcast_pkt              );
	printf("RX bytes bcast: %lld\n",   stats->rx_good_bcast_bytes            );
	printf("RX frame bcast: %lld\n",   stats->rx_good_bcast_pkt              );

	printf("TX bytes ucast: %lld\n",    stats->tx_ucast_bytes               );
	printf("TX frame ucast: %lld\n",    stats->tx_ucast_pkt                 );
	printf("TX bytes mcast: %lld\n",    stats->tx_mcast_bytes               );
	printf("TX frame mcast: %lld\n",    stats->tx_mcast_pkt                 );
	printf("TX bytes bcast: %lld\n",    stats->tx_bcast_bytes               );
	printf("TX frame bcast: %lld\n",    stats->tx_bcast_pkt                 );
}
int  do_show_cpumac_status(cmd_tbl_t *cmdtp, int flag, 
									int argc, char * const argv[])
{
	struct ctcmac_status  *status;
	u32 cpumac_index;
	struct CpuMac_regs *cpumac_reg;
	struct CpuMac_mems *cpumac_mem;

	if(argc != 2){
		return -1;
	}

	cpumac_index = simple_strtoul(argv[1], NULL, 0); 
	if(cpumac_index == 0){
		cpumac_reg = (struct CpuMac_regs *)CTCMAC_0_BASE;
		cpumac_mem = (struct CpuMac_mems *)(CTCMAC_0_BASE+CPUMAC_MEM_BASE);
	}else{
		cpumac_reg = (struct CpuMac_regs *)CTCMAC_1_BASE;
		cpumac_mem = (struct CpuMac_mems *)(CTCMAC_1_BASE+CPUMAC_MEM_BASE);
	}

	status = &g_status[cpumac_index];
	ctcmac_get_mac_status(cpumac_reg, cpumac_mem, status);
	printf("link status:    ");
	if(status->link_up){
		printf("up\n");
	}else{
		printf("down\n");
	}
	printf("remote speed:   ");
	if(status->remote_speed == CTCMAC_SPEED_10M){
		printf("10M\n");
	}else if(status->remote_speed == CTCMAC_SPEED_100M){
		printf("100M\n");
	}else if(status->remote_speed == CTCMAC_SPEED_1000M){
		printf("1000M\n");
	}else if(status->remote_speed == CTCMAC_SPEED_MAX){
		printf("unknow\n");
	}
	printf("local speed:    ");
	if((status->local_rx_speed == CTCMAC_SPEED_10M) && (status->local_tx_speed == CTCMAC_SPEED_10M)){
		printf("10M\n");
	}else if((status->local_rx_speed == CTCMAC_SPEED_100M) && (status->local_tx_speed == CTCMAC_SPEED_100M)){
		printf("100M\n");
	}else if((status->local_rx_speed == CTCMAC_SPEED_1000M) && (status->local_tx_speed == CTCMAC_SPEED_1000M)){
		printf("1000M\n");
	}else if(status->remote_speed == CTCMAC_SPEED_MAX){
		printf("unknow\n");
	}

	ctcmac_dump_pkt_stats(&status->pkt_stats);

	return 0;
}

int  do_clear_cpumac_status(cmd_tbl_t *cmdtp, int flag, 
									int argc, char * const argv[])
{
	u32 cpumac_index;

	if(argc != 2){
		return -1;
	}

	cpumac_index = simple_strtoul(argv[1], NULL, 0); 
	memset(&g_status[cpumac_index], 0, sizeof(g_status[cpumac_index]));

	return 0;
}

int  do_cpumac_loopback(cmd_tbl_t *cmdtp, int flag, 
									int argc, char * const argv[])
{
	u8 enable;
	u32 serdes_id;
	struct CpuMac_regs *cpumac_reg;

	if(argc != 4){
		return -1;
	}

	serdes_id = simple_strtol(argv[1], NULL, 0);
	if((serdes_id != 0) && (serdes_id != 1)){
		return -1;
	}

	if(serdes_id == 0){
		cpumac_reg = (struct CpuMac_regs *)CTCMAC_0_BASE;
	}else{
		cpumac_reg = (struct CpuMac_regs *)CTCMAC_1_BASE;
	}
	
	if(!strncmp(argv[3], "enable", 6)){
		enable = 1;
	}else if(!strncmp(argv[3], "disable", 7)){
		enable = 0;
	}else{
		return -1;
	}

	if(!strncmp(argv[2], "internal", 8)){
		ctcmac_cfg_external_loopback_ls2(cpumac_reg, (u8)serdes_id, 0);
		ctcmac_cfg_internal_loopback_lm1(cpumac_reg, (u8)serdes_id, enable);
	}else if(!strncmp(argv[2], "external", 8)){
		ctcmac_cfg_internal_loopback_lm1(cpumac_reg, (u8)serdes_id, 0);
		ctcmac_cfg_external_loopback_ls2(cpumac_reg, (u8)serdes_id, enable);
	}else{
		return -1;
	}

	return 0;
}

int  do_cpumac_hss_access(cmd_tbl_t *cmdtp, int flag, 
									int argc, char * const argv[])
{
	u8 val=0, accid;
	u32 serdes_id, addr;

	if((argc != 5) && (argc != 4)){
		return -1;
	}
	
	serdes_id = simple_strtol(argv[2], NULL, 0);
	if((serdes_id != 0) && (serdes_id != 1) && (serdes_id != 2)){
		return -1;
	}
	
	addr = simple_strtol(argv[3], NULL, 0);
	if(addr > 0xff){
		return -1;
	}

	if(serdes_id == 0)
		accid = 0x3;
	else if(serdes_id == 1)
		accid = 0x4;
	else
		accid = 0x0;

	if(!strncmp(argv[1], "read", 4)){
		ctc_mac_hss_read((u8)addr, &val, accid);
		printf("0x%x : 0x%x\n", addr, val);
	}else if(!strncmp(argv[1], "write", 5)){
		val = simple_strtol(argv[4], NULL, 0);
		ctc_mac_hss_write((u8)addr, val, accid);
	}else{
		return -1;
	}

	return 0;
}

int  do_cpumac_ffe_access(cmd_tbl_t *cmdtp, int flag, 
									int argc, char * const argv[])
{
	u32 serdes_id;
	u16 coefficient[3];
	
	if((argc != 6) && (argc != 3)){
		return -1;
	}
	
	serdes_id = simple_strtol(argv[2], NULL, 0);
	if(serdes_id != 0 && serdes_id != 1){
		return -1;
	}
	
	if(!strncmp(argv[1], "get", 3)){
		ctcmac_get_ffe(serdes_id, coefficient);
		printf("serdes %d :c0 %d c1 %d c2 %d\n", 
			serdes_id, coefficient[0], coefficient[1], coefficient[2]);
	}else if(!strncmp(argv[1], "set", 3)){
		coefficient[0] = simple_strtol(argv[3], NULL, 0);
		coefficient[1] = simple_strtol(argv[4], NULL, 0);
		coefficient[2] = simple_strtol(argv[5], NULL, 0);
		ctcmac_set_ffe(serdes_id, coefficient);
	}else{
		return -1;
	}

	return 0;
}

int  do_cpumac_sendpkt(cmd_tbl_t *cmdtp, int flag, 
									int argc, char * const argv[])
{
	u32 cpumac_index;
	long pkt_cnt;
	struct CpuMac_regs *cpumac_reg;
	struct CpuMac_mems *cpumac_mem;

	if(argc != 3){
		return -1;
	}

	cpumac_index = simple_strtoul(argv[1], NULL, 0); 
	pkt_cnt = simple_strtol(argv[2], NULL, 0); 
	if(cpumac_index == 0){
		cpumac_reg = (struct CpuMac_regs *)CTCMAC_0_BASE;
		cpumac_mem = (struct CpuMac_mems *)(CTCMAC_0_BASE+CPUMAC_MEM_BASE);
	}else{
		cpumac_reg = (struct CpuMac_regs *)CTCMAC_1_BASE;
		cpumac_mem = (struct CpuMac_mems *)(CTCMAC_1_BASE+CPUMAC_MEM_BASE);
	}

	while(pkt_cnt > 0){
		ctcmac_sendpkt(cpumac_reg, cpumac_mem);
		mdelay(1);
		pkt_cnt--;
	}

	return 0;
}
#endif


int  do_ddr_parameter_display(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int * ddr_cfg_base = (unsigned int *)CONFIG_SYS_CTC5236_DDRCFG_BASE;
	unsigned int ddr_type;
	char *cmd;

	/* get ddr interface type */
	ddr_type = (readl(ddr_cfg_base)&0xf);
	
	if(argc > 2)
	{
		printf("Usage:\n%s\n", cmdtp->usage);
		return CMD_RET_USAGE;
	}

	if(argc == 1)
		cmd = "all";
	else
		cmd = argv[1];

	if(!strncmp(cmd, "all", 3))
	{
		if(ddr_type)
			printf("DDR TYPE : DDR4\n");
		else
			printf("DDR TYPE : DDR3\n");

		printf("\nDDR timing parameter: \n");
	}
	
	if(!strncmp(cmd, "tck", 3) || !strncmp(cmd, "all", 3))
		ctc_print_ddr_cycle_time();
	
	if(!strncmp(cmd, "tcl", 3) || !strncmp(cmd, "all", 3))
		ctc_print_cas_latency_time(ddr_type);

	if(!strncmp(cmd, "twr", 3) || !strncmp(cmd, "all", 3))
		ctc_print_twr(ddr_type);

	if(!strncmp(cmd, "trcd", 4) || !strncmp(cmd, "all", 3))
		ctc_print_trcd();	

	if(!strncmp(cmd, "trp", 3) || !strncmp(cmd, "all", 3))
		ctc_print_trp();

	if(!strncmp(cmd, "trfc", 4) || !strncmp(cmd, "all", 3))
		ctc_print_trfc();	
	
	if(!strncmp(cmd, "tras", 4) || !strncmp(cmd, "all", 3))
		ctc_print_tras();

	if(!strncmp(cmd, "tfaw", 4) || !strncmp(cmd, "all", 3))
		ctc_print_tfaw();

	if(!strncmp(cmd, "tccd", 4) || !strncmp(cmd, "all", 3))
		ctc_print_tccd(ddr_type);

	if(!strncmp(cmd, "trrd", 4) || !strncmp(cmd, "all", 3))
		ctc_print_trrd(ddr_type);

	if(!strncmp(cmd, "twtr", 4) || !strncmp(cmd, "all", 3))
		ctc_print_twtr(ddr_type);

	if(!strncmp(cmd, "odt", 3) || !strncmp(cmd, "all", 3))
		ctc_dispaly_odt(ddr_type);

	if(!strncmp(cmd, "mr", 2) || !strncmp(cmd, "all", 3))
		ctc_dispaly_mr(ddr_type);

	if(!strncmp(cmd, "addr", 4) || !strncmp(cmd, "all", 3))
		ctc_print_ddr_addr_info();	

	return CMD_RET_SUCCESS;
}

static char * env_grep(cmd_tbl_t *cmdtp, int flag,
		       int argc, char * const argv[])
{
	char *res = NULL;
	int grep_how, grep_what;

	grep_how  = H_MATCH_SUBSTR;	/* default: substring search	*/
	grep_what = H_MATCH_BOTH;	/* default: grep names and values */

	hexport_r(&env_htab, '\n', flag | grep_what | grep_how,
			&res, 0, argc, argv);

	return res;		
}

int do_restore_environment(cmd_tbl_t *cmdtp, int flag, int argc, char* const argv[])
{
	DECLARE_GLOBAL_DATA_PTR;
	char cmd[1024];
	char *mac_info = NULL;
	char *sn_info = NULL;
	char *mac_name[] = {"ethaddr"};
	char *sn_name[] = {"serialno"};
	char mac[20];
	char sn[20];

	/* grep cpumac address ethaddr=...  */
	mac_info = env_grep(cmdtp, flag, 1, (char ** const )(&mac_name));
	/* grep SN serialno=...  */
	sn_info = env_grep(cmdtp, flag, 1, (char ** const )(&sn_name));

	/* remove ethaddr= and just keep data  */
	strncpy(mac, mac_info+8, 18);
	mac[18] = '\0';

	/* remove serialno= and just keep data  */
	strncpy(sn, sn_info+9, 13);
	sn[13] = '\0';

    	set_default_env(NULL);
    	gd->env_valid = 1;
    	printf("## Resetting to default environment\n");
    	
	sprintf(cmd, "setenv ethaddr %s", mac);
	run_command(cmd, flag);
	
	sprintf(cmd, "setenv serialno %s", sn);
	run_command(cmd, flag);
	
    	run_command("saveenv", flag);
    	return 0;
}

/*
 * With driver model this isn't right since we can have multiple controllers
 * and the device numbering starts at 1 on each bus.
 * TODO(sjg@chromium.org): Add a way to specify the controller/bus.
 */
static struct usb_device *ctc_usb_find_device(int devnum)
{
#ifdef CONFIG_DM_USB
	struct usb_device *udev;
	struct udevice *hub;
	struct uclass *uc;
	int ret;

	/* Device addresses start at 1 */
	devnum++;
	ret = uclass_get(UCLASS_USB_HUB, &uc);
	if (ret)
		return NULL;

	uclass_foreach_dev(hub, uc) {
		struct udevice *dev;

		if (!device_active(hub))
			continue;
		udev = dev_get_parent_priv(hub);
		if (udev->devnum == devnum)
			return udev;

		for (device_find_first_child(hub, &dev);
		     dev;
		     device_find_next_child(&dev)) {
			if (!device_active(hub))
				continue;

			udev = dev_get_parent_priv(dev);
			if (udev->devnum == devnum)
				return udev;
		}
	}
#else
	struct usb_device *udev;
	int d;

	for (d = 0; d < USB_MAX_DEVICE; d++) {
		udev = usb_get_dev_index(d);
		if (udev == NULL)
			return NULL;
		if (udev->devnum == devnum)
			return udev;
	}
#endif

	return NULL;
}

static int ctc_usb_get_descriptor(struct usb_device *dev, unsigned char type,
			unsigned char index, void *buf, int size)
{
	return usb_control_msg(dev, usb_rcvctrlpipe(dev, 0),
			       USB_REQ_GET_DESCRIPTOR, USB_DIR_IN,
			       (type << 8) + index, 0, buf, size,
			       USB_CNTL_TIMEOUT);
}

int  do_get_usb_device_desc(cmd_tbl_t *cmdtp, int flag, 
									int argc, char * const argv[])
{
	int err = 0;
	u32 device_id, port_id, count;
	struct usb_device *udev = NULL;
	struct usb_device_descriptor desc;
	
	device_id = simple_strtol(argv[1], NULL, 0);
	port_id = simple_strtol(argv[2], NULL, 0);
	count = simple_strtol(argv[3], NULL, 0);
	
	udev = ctc_usb_find_device(device_id);
	if (udev == NULL) {
		printf("Device %d does not exist.\n", device_id);
		return 1;
	}

	while(count --){
		err = ctc_usb_get_descriptor(udev, USB_DT_DEVICE, port_id, &desc, 8);
		if (err < 8) {
			if (err < 0) {
				printf("unable to get device descriptor (error=%d)\n",
					err);
				return err;
			} else {
				printf("USB device descriptor short read (expected 8, got %i)\n", err);
				return -EIO;
			}
		}
	}

	return 0;
}

static bool i2c_slave_dev = false;
int  do_i2c_slave(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int dev, val;
	
	if((argc != 3) && (argc != 2)) {
		return -1;
	}

	if(!strncmp(argv[1], "dev", 3)) { 
		dev = simple_strtoul(argv[2], NULL, 0); 
		i2c_ctc_reg_slave(dev);
		i2c_slave_dev = true;
	}else if(!strncmp(argv[1], "send", 4)) {
		if (i2c_slave_dev == false)
			return -1;
		val = simple_strtoul(argv[2], NULL, 0);
		i2c_ctc_send_data(val);

	}else if(!strncmp(argv[1], "recv", 4)) {
		if (i2c_slave_dev == false)
			return -1;
		i2c_ctc_receive_data();
	}
	return 0;
}

int  do_get_switch_tmpr(cmd_tbl_t *cmdtp, int flag, 
									int argc, char * const argv[])
{
	printf("temperature : %d\n", get_switch_temperature());
	
	return 0;
}

U_BOOT_CMD(
    upgrade_uboot, 3, 1, upgrade_uboot_remote,
    "upgrade_uboot u-boot.bin spiflash|emmc|sd",
    "upgrade_uboot u-boot.bin spiflash|emmc|sd"
    "   - tftp u-boot from net server and upgrade it\n" 
);

#ifdef CONFIG_CTC_SWITCH
U_BOOT_CMD(
    switch, 5, 1, do_switch_acces,
    "switch access",
    "read addr len\n"
    "switch write addr val len\n" 
);
#endif

#ifdef  CONFIG_CTC_CPUMAC_DEBUG
U_BOOT_CMD(
    cpumac_show, 2, 1, do_show_cpumac_status,
    "show cpumac mac/pcs status",
    "cpumac_show 0|1\n"
);

U_BOOT_CMD(
    cpumac_clear, 2, 1, do_clear_cpumac_status,
    "clear cpumac mac/pcs status",
    "cpumac clear port 0|1 status\n"
);

U_BOOT_CMD(
    cpumac_loopback, 4, 1, do_cpumac_loopback,
    "cpumac loopback enable cfg",
    "cpumac_loopback serdesID internal|external enable|disable\n"
);

U_BOOT_CMD(
    cpumac_hss, 5, 1, do_cpumac_hss_access,
    "cpumac hss access",
    "cpumac_hss read|write serdesID register [val]\n"
);

U_BOOT_CMD(
    cpumac_sendpkt, 3, 1, do_cpumac_sendpkt,
    "cpumac send packet",
    "cpumac_sendpkt 0|1 count\n"
);
#endif

U_BOOT_CMD(
    cpumac_ffe, 6, 1, do_cpumac_ffe_access,
    "cpumac set ffe",
    "cpumac_ffe set|get serdesID C0 C1 C2\n"
);

U_BOOT_CMD(
    ddr_parameter_display, 2, 1, do_ddr_parameter_display,
    "display some ddr parameter",
    "- display all of the following\n"
    "ddr_parameter_display odt - display odt time\n"
    "ddr_parameter_display tck - display tck time\n"
    "ddr_parameter_display tcl - display tcl time\n"
    "ddr_parameter_display trcd - display trcd time\n"
    "ddr_parameter_display trp - display trp time\n"
    "ddr_parameter_display tras - display tras time\n"
    "ddr_parameter_display trfc - display trfc time\n"
    "ddr_parameter_display tfaw - display tfaw time\n"
    "ddr_parameter_display tccd - display tccd time\n"
    "ddr_parameter_display twr - display twr time\n"
    "ddr_parameter_display twtr - display twtr time\n"
    "ddr_parameter_display trrd - display trrd_s time for ddr4 or trrd time for ddr3\n"
    "ddr_parameter_display mr - display mode register\n"
    "ddr_parameter_display addr - display ddr address info\n"
);

U_BOOT_CMD(
   reenv,1,1,do_restore_environment,
   "reset environment with default value",
   "get_reenv_environment_length\n"
    "   - restore enviroment arguments to default value\n" 
);

U_BOOT_CMD(
    usb_get_desc, 4, 1, do_get_usb_device_desc,
    "usb_get_desc",
    "usb_get_desc device port\n"
);

U_BOOT_CMD(
    i2c_slave, 3, 1, do_i2c_slave,
    "i2c slave mode ",
    "dev 0|1\n"
    "i2c_slave send val\n"
    "i2c_slave recv\n" 
);

U_BOOT_CMD(
    switch_tmpr, 1, 1, do_get_switch_tmpr,
    "get switch core temperature",
    "switch_tmpr\n"
);

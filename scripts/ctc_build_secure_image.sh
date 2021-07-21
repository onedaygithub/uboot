#!/bin/sh

TOP_DIR=`pwd`
PARA_NUM=$#

function usage(){
cat << EOF
*********************************************************************************************
Usage: $0 [-h] [-nv: <nv count> -keycnt <key count> -keydir <key directory> -board <board type> -boot <boot mode> [-aeskey <aes key>]]
Most used options:
 -nv     : nv count,0,1,3,7 is valid
 -keycnt : key count 1,4 is valid
 -keydir : key directory
 -board  : board type, 24x2c,48t4x2q,48m4y2d,48x is valid
 -boot   : boot mode, qspi,mmc is valid
 -aeskey :aes key number, if not set, aes function is disabled
Most used examples:

 $0 -nv 7 -keycnt 4 -keydir ~/workspace/software/develop/workspace_test/ -aeskey 1234567890123456 -board 24x2c  -boot qspi
EOF
}

function parse_cli(){
	if [ "$1" == "-h" ];then
		usage
		return 1
	fi
	
	for ((i=0; i<$PARA_NUM; i=i+2))
	do
		if [ "${PARA[$i]}" == "-nv" ];then
			NV2=${PARA[$i+1]}
		fi
                if [ "${PARA[$i]}" == "-keycnt" ];then
                        KEYCNT=${PARA[$i+1]}
                fi
                if [ "${PARA[$i]}" == "-keydir" ];then
                        KEYDIR=${PARA[$i+1]}
                fi
                if [ "${PARA[$i]}" == "-aeskey" ];then
                        AESKEY=${PARA[$i+1]}
                fi
                if [ "${PARA[$i]}" == "-board" ];then
                        BOARD=${PARA[$i+1]}
                fi
                if [ "${PARA[$i]}" == "-boot" ];then
                        BOOTMODE=${PARA[$i+1]}
                fi
	done
	echo "nv count  : $NV2"
        echo "key count : $KEYCNT"
        echo "key dir   : $KEYDIR"
        echo "aes key   : $AESKEY"
        echo "board     : $BOARD"
        echo "boot mode : $BOOTMODE"
	
	if [ "$NV2" != "0" -a "$NV2" != "1" -a "$NV2" != "3" -a "$NV2" != "7" ];then
		echo "Invalid nv count"
		usage
		return 1
	fi
	if [ "$KEYCNT" != "1" -a "$KEYCNT" != "4" ];then
		echo "Invalid key count"
		usage
		return 1
	fi
	if [ -z "$KEYDIR" ];then
		echo "Invalid key dir"
		usage
		return 1
	fi
	if [ "$BOARD" != "24x2c" -a "$BOARD" != "48t4x2q" -a "$BOARD" != "48m4y2d" -a "$BOARD" != "48x" ];then
		echo "Unsupported board type"
		usage
		return 1
	fi
	if [ "$BOOTMODE" != "qspi" -a "$BOOTMODE" != "mmc" ];then
		echo "Unsupported boot mode"
		usage
		return 1
	fi
	 
	return 0
}

if [ "$PARA_NUM" == "12" ];then
	PARA=($1 $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} ${12})
elif [ "$PARA_NUM" == "10" ];then
	PARA=($1 $2 $3 $4 $5 $6 $7 $8 $9 ${10})
else
	usage
	exit
fi

parse_cli
if [ "$?" == "1" ];then
	exit
fi

cd ..
make CROSS_COMPILE=aarch64-linux-gnu- ARCH=arm e530_${BOARD}_${BOOTMODE}_secure_defconfig 
make CROSS_COMPILE=aarch64-linux-gnu- ARCH=arm DEVICE_TREE=e530-${BOARD} ctc5236-full.bin -j16
cd scripts
$TOP_DIR/../tools/ctcsign -C 2 -n $KEYCNT -c $NV2 -s $TOP_DIR/../spl/ctc5236-spl.bin  -d $KEYDIR
mv $TOP_DIR/cert2.txt $TOP_DIR/../spl.cert
	
objcopy -I binary -O binary --pad-to=0xa00 --gap-fill=0xff $TOP_DIR/../key.cert
objcopy -I binary -O binary --pad-to=0xa00 --gap-fill=0xff $TOP_DIR/../spl.cert

if [ ! -z $AESKEY ]; then
	$TOP_DIR/../tools/ctcsign -E -s $TOP_DIR/../u-boot.img -k $AESKEY
	$TOP_DIR/../tools/ctcsign -D -s $TOP_DIR/../u-boot.img -k $AESKEY
	$TOP_DIR/../tools/ctcsign -C 2 -n $KEYCNT -c $NV2 -s $TOP_DIR/../u-boot.img_pad -e 1 -d $KEYDIR
	mv $TOP_DIR/cert2.txt $TOP_DIR/../uboot.cert
	UBOOT_NAME="u-boot.img_enc"
else
	$TOP_DIR/../tools/ctcsign -C 2 -n $KEYCNT -c $NV2 -s $TOP_DIR/../u-boot.img -d $KEYDIR
	mv $TOP_DIR/cert2.txt $TOP_DIR/../uboot.cert
	UBOOT_NAME="u-boot.img"
fi

objcopy -I binary -O binary --pad-to=0xa00 --gap-fill=0xff $TOP_DIR/../uboot.cert

if [ ! -z $AESKEY ]; then
	cat $TOP_DIR/../key.cert $TOP_DIR/../spl.cert $TOP_DIR/../spl/ctc5236-spl.bin $TOP_DIR/../uboot.cert $TOP_DIR/../$UBOOT_NAME > $TOP_DIR/../e530-secure-${BOARD}-${BOOTMODE}-enc.bin
	echo "Install secure image to $TOP_DIR/../e530-secure-${BOARD}-${BOOTMODE}-enc.bin"
else
	cat $TOP_DIR/../key.cert $TOP_DIR/../spl.cert $TOP_DIR/../spl/ctc5236-spl.bin $TOP_DIR/../uboot.cert $TOP_DIR/../$UBOOT_NAME > $TOP_DIR/../e530-secure-${BOARD}-${BOOTMODE}.bin
	echo "Install secure image to $TOP_DIR/../e530-secure-${BOARD}-${BOOTMODE}.bin"
fi

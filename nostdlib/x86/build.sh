#!/bin/sh
STEM=qemu-wrapper-arm
CC="gcc -Os -Wall -pedantic -Wextra -Wstrict-aliasing=1 \
	-fno-builtin -nostdlib -static -fno-pie"
$CC -S $STEM.c -o /dev/stdout | \
	sed 's/.rodata/.text/g' >$STEM.s
sh elfheader.sh 0 0 > elfheader.s
ASM="$CC elfheader.s $STEM.s -o $STEM.out"
$ASM
TEXT=$(readelf -l $STEM.out | grep 'LOAD .* R E')
ph_cut(){
        cut -f$1 -dx | cut -f1 -d\ 
}
TEXT_ADDR=$(echo $TEXT | ph_cut 3)
TEXT_SIZE=$(echo $TEXT | ph_cut 6)
sh elfheader.sh $TEXT_ADDR $TEXT_SIZE > elfheader.s
$ASM
LENGTH=$(echo $TEXT_SIZE | tr 'abcdef' 'ABCDEF')
SIZE=$(echo "ibase=16 ; $LENGTH" | bc )
dd if=$STEM.out of=$STEM.out.trunc bs=4096 skip=1
truncate --size=$SIZE $STEM.out.trunc
chmod +x $STEM.out.trunc

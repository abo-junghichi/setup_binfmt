#!/bin/sh
CC="gcc -Os -Wall -pedantic -Wextra -Wstrict-aliasing=1 \
	-fno-builtin -nostdlib -static -fno-pie"
$CC -S qemu-wrapper-arm.c -o /dev/stdout | \
	sed 's/.rodata/.text/g' >qemu-wrapper-arm.s
sh elfheader.sh 0 0 > elfheader.s
ASM="$CC elfheader.s qemu-wrapper-arm.s -o qemu-wrapper-arm.out"
$ASM
TEXT=$(readelf -l qemu-wrapper-arm.out | grep 'LOAD .* R E')
ph_cut(){
        cut -f$1 -dx | cut -f1 -d\ 
}
TEXT_ADDR=$(echo $TEXT | ph_cut 3)
TEXT_SIZE=$(echo $TEXT | ph_cut 6)
sh elfheader.sh $TEXT_ADDR $TEXT_SIZE > elfheader.s
$ASM
LENGTH=$(echo $TEXT_SIZE | tr 'abcdef' 'ABCDEF')
SIZE=$(echo "ibase=16 ; $LENGTH" | bc )
dd if=qemu-wrapper-arm.out of=qemu-wrapper-arm.out.trunc bs=4096 skip=1
truncate --size=$SIZE qemu-wrapper-arm.out.trunc
chmod +x qemu-wrapper-arm.out.trunc

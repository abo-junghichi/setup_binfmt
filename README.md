# setup\_binfmt
A batch of setup scripts for binfmt\_misc of Linux.

## How to use
Generate the scripts.
<pre>
$ ./build.sh
</pre>
This command generates "setup\_bifmt\_\*.sh".

Before using these generated scripts, you should prepare user-mode qemu and binfmt service.
See
[Embedded Handbook/General/Compiling with qemu user chroot - Gentoo Wiki](https://wiki.gentoo.org/wiki/Embedded_Handbook/General/Compiling_with_qemu_user_chroot).

## Tested environments
These scripts are tested using
[stage3 file-trees from Gentoo Linux](https://www.gentoo.org/downloads/)
and
[the RISC-V RV32 file-tree from diskimage of TinyEMU by Fabrice Bellard](https://bellard.org/tinyemu/)
on Gentoo Linux.

Amoung ARM instruction set family, only ARMv7a is tested.
It appears this ISA needs additionanl argument "-B 0x1000" to qemu.

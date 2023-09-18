
if [ "little" = $ENDIAN ]
then
	ECODE=\\x01
	ETYPE=\\x02\\x00
	EMASK=\\xfe\\xff
elif [ "big" = $ENDIAN ]
then
	ECODE=\\x02
	ETYPE=\\x00\\x02
	EMASK=\\xff\\xfe
else
	exit 1
fi

MAGIC=\\x7fELF
 MASK='\xff\xff\xff\xff'

MAGIC=$MAGIC$BIT
 MASK=$MASK\\xff

MAGIC=$MAGIC$ECODE\\x01\\x00
 MASK=$MASK\\xff\\xff\\x00

MAGIC=$MAGIC\\x00
 MASK=$MASK$ABI

MAGIC=$MAGIC\\x00\\x00\\x00\\x00\\x00\\x00\\x00$ETYPE
 MASK=$MASK\\xff\\xff\\xff\\xff\\xff\\xff\\xff$EMASK

MAGIC=$MAGIC$ISA
 MASK=$MASK\\xff\\xff

echo ":$ARCH:M::$MAGIC:$MASK:$INTERPRETER:$FLAGS" > /proc/sys/fs/binfmt_misc/register

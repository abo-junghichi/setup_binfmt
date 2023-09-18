#!/bin/sh

for CONF in *.conf
do
	ARCH=`basename --suffix=.conf $CONF`
	DIST=setup_binfmt_$ARCH.sh
	echo '#!/bin/sh' > $DIST
	cat $CONF stem.sh >> $DIST
done

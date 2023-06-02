#!/bin/bash

MOUNT_POINT=./test3

path=$PWD
echo umounting $MOUNT_POINT
umount $MOUNT_POINT

mkdir -p $MOUNT_POINT

> /tmp/lufs-pkgfs.lst

export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
echo mounting $MOUNT_POINT
lufsmount pkgfs:// $MOUNT_POINT

echo execing pathexec
pathexec=`which pathexec.sh`
if [ ! $pathexec ]; then
	echo pathexec not found
	exit -1
fi

echo chroot $MOUNT_POINT $pathexec $path $*
#chroot $MOUNT_POINT $pathexec $path $*

umount $MOUNT_POINT

cat /tmp/lufs-pkgfs.lst

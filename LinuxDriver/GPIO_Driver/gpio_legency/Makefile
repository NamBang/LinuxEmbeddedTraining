# character device driver

EXTRA_CFLAGS = -Wall
obj-m += led_ex.o

CROSS := /home/yb/BeagleBone-Black/gcc-11.3.0-nolibc/arm-linux-gnueabi/bin/arm-linux-gnueabi-
KERNEL := /home/yb/BeagleBone-Black/kernelbuildscripts/KERNEL

all:
	make ARCH=arm CROSS_COMPILE=$(CROSS) -C $(KERNEL) M=$(shell pwd) modules
clean:
	make -C $(KERNEL) M=$(shell pwd) clean

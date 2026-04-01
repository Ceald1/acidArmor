obj-m += acidarmor.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
#CFLAGS_gumdrop.o := -fcf-protection=none -fno-jump-tables


# bear -- make -C /lib/modules/$(uname -r)/build M=$PWD modules
all:
	bear -- make -C $(KDIR) M=$(PWD) modules
	# $(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

install:
	sudo insmod acidarmor.ko

uninstall:
	sudo rmmod acidarmor

reload: uninstall install

dmesg:
	dmesg | tail -20

.PHONY: all clean install uninstall reload dmesg

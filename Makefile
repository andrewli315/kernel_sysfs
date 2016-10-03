
ifneq ($(KERNELRELEASE),)
	obj-m := sysfs.o
	module-objs :=sysfs.o
else
		KERNELDIR ?= /lib/modules/$(shell uname -r)/build
		PWD := $(shell pwd)
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod.c
endif



ifneq ((KERNELRELEASE),)
	obj-m := sysfs.o
else
		KERNELDIR ?= /lib/mdoule/$(shell uname -r)/build
		PWD := $(shell pwd)
all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod.c
endif

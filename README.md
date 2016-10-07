# kernel_sysfs

**Compile**

$sudo make

**install module**

$sudo insmod sysfs.ko

$echo "String" >> /sys/kernel/hw1/swap_string

$cat /sys/kernel/hw1/swap_string

**remove kernel module**

$sudo rmmod sysfs

$dmesg | tail 


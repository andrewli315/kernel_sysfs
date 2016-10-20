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

*First Module*
	
	string swap
	
	read one integer and a string and then shifting the string index according to the integer

*Second Module*
	
	simple infix calculator
	
	using two stack to record operand and operator

*Third Module*
	
	a sum tree
	
	using a stack to store the root number. When accounting the leaf, compute the sum of the linking with leaf.





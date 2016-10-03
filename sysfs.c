#include <linux/init.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/string.h>

MODULE_LICENSE("Dual BSD/GPL");



static struct kobject *hw_kobject;
static struct ssize_t q1_show(struct kobject *kobject,
			struct kobj_attribute *attr, char *buf)
{

}
static struct ssize_t q1_store(struct kobject *kobject,
			struct kobj_attribute *attr, char *buf, size_t count)
{

}
static int __init hw_init(void)
{
	hw_kobject = kobject_create_and_add("hw1",kernel_kobj);
	struct kobj_attribute hw_attr = 
		__ATTR(q1,0660,q1_show,q1_store);
	sysfs_create_file(hw_kobject, &hw_attr.attr);
}
static void __exit hw_exit(void)
{
	printk(KERN_ALERT "clean up hw kobject\n");
}
module_init(hw_init);
module_exit(hw_exit);

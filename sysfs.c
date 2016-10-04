#include <linux/init.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/string.h>

MODULE_LICENSE("Dual BSD/GPL");

static int q1;
static struct kobject *hw_kobject;
static ssize_t q1_show(struct kobject *kobject,
			struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf,"q1 show %s:%d\n",attr->attr.name,q1);
}
static ssize_t q1_store(struct kobject *kobject,
			struct kobj_attribute *attr, char *buf, size_t count)
{
	sprintf(buf,"%du\n",q1);
	return count;
}
static struct kobj_attribute hw_attr = __ATTR(q1,0660,q1_show,q1_store);
static int __init hw_init(void)
{
	hw_kobject = kobject_create_and_add("hw1",kernel_kobj);
	sysfs_create_file(hw_kobject, &hw_attr.attr);
}
static void __exit hw_exit(void)
{
	printk(KERN_ALERT "clean up hw kobject\n");
}
module_init(hw_init);
module_exit(hw_exit);


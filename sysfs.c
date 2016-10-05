#include <linux/init.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>

MODULE_LICENSE("Dual BSD/GPL");

static char* str;
static char s[100];
static struct kobject *hw_kobject;
void swap_char(char *a,char *b);
int atoi(char *str);
static ssize_t s_show(struct kobject*,
			struct kobj_attribute*,char*);
static ssize_t s_store(struct kobject*,
			struct kobj_attribute*, char*,size_t);

static ssize_t s_show(struct kobject *kobject,
			struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf,"%s\n",s);
}
static ssize_t s_store(struct kobject *kobject,
			struct kobj_attribute *attr, char *buf, size_t count)
{
	sscanf(buf,"%s\n",&str);
	int i,t;
    int len = strlen(str);
    char s2[100];
    char temp[2];
    strncpy(temp,str,2);
    t = atoi(temp);
    memcpy(s2,str+2,len-2);
    for(i=0;i<(len/2 - 1);i++)
    {
	    swap_char(&s2[i],&s2[t]);
    }
	strcpy(s,s2);
	return count;
}
static struct kobj_attribute hw_attr =
		 __ATTR(swap_string,0660,s_show,s_store);
static int __init hw_init(void)
{
	int retval;
	hw_kobject = kobject_create_and_add("hw1",kernel_kobj);
	if(!hw_kobject)
		return -ENOMEM;
	retval = sysfs_create_file(hw_kobject, &hw_attr.attr);
	return retval;
}
static void __exit hw_exit(void)
{
	printk(KERN_ALERT "CLEAN UP MODULE\n");
}
void swap_char(char *a,char *b)
{
	char temp;
	temp = *a;
	*a = *b;
	*b = temp;
}
int atoi(char *str)
{
	int i;
	int sum =0;
	for(i=0;str[i]!='\0';i++)
	{
		sum = sum*10+str[i]-'0';
	}
	return sum;
}
module_init(hw_init);
module_exit(hw_exit);


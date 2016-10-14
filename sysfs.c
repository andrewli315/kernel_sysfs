#include <linux/init.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>

MODULE_LICENSE("Dual BSD/GPL");

static int s;
char test[40];
static struct kobject *hw_kobject;
void swap_char(char *a,char *b);
int atoi(char *str);
char* concat(char*,char*);

static ssize_t s_show(struct kobject*,
			struct kobj_attribute*,char*);
static ssize_t s_store(struct kobject*,
			struct kobj_attribute*, const char*,size_t);

static ssize_t c_show(struct kobject*,
			struct kobj_attribute*,char*);
static ssize_t c_store(struct kobject*,
			struct kobj_attribute*, const char*,size_t);

static ssize_t t_show(struct kobject*,
			struct kobj_attribute*,char*);
static ssize_t t_store(struct kobject*,
			struct kobj_attribute*, const char*,size_t);



static ssize_t s_show(struct kobject *kobject,
			struct kobj_attribute *attr, char *buf)
{
	int i;
	int len=0;
	char str[100];
	char arr[100];
	concat(str,test);

	len = (int)strlen(str)-1;
	for(i=0;i<len;i++)
	{
		if((i+s) < len)
			arr[i+s] = *(str+i);
		else
			arr[(i+s)%len] = *(str+i);

	}
	arr[len]='\0';
	return sprintf(buf,"String: %s\n",arr);
}
static ssize_t s_store(struct kobject *kobject,
			struct kobj_attribute *attr,const char *buf, size_t count)
{
	sscanf(buf,"%du",&s);
	memcpy(test,buf,strlen(buf));
	return count;
}


static ssize_t s_show(struct kobject *kobject,
			struct kobj_attribute *attr, char *buf)
{
	
	return sprintf(buf,"String");
}
static ssize_t s_store(struct kobject *kobject,
			struct kobj_attribute *attr,const char *buf, size_t count)
{
	return count;
}
static ssize_t s_show(struct kobject *kobject,
			struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf,"String");
}
static ssize_t s_store(struct kobject *kobject,
			struct kobj_attribute *attr,const char *buf, size_t count)
{
	return count;
}


static struct kobj_attribute hw_attr1 =
		 __ATTR(swap_string,0660,s_show,s_store);

static struct kobj_attribute hw_attr2 =
		 __ATTR(calc,0660,c_show,c_store);

static struct kobj_attribute hw_attr3 =
		 __ATTR(tree,0660,t_show,t_store);

static struct attribute *attrs[] = {
       &hw_attr1.attr,
       &hw_attr2.attr,
       &hw_attr3.attr,
       NULL,   /* need to NULL terminate the list of attributes */
};	 
static struct attribute_group attr_group = {
       .attrs = attrs,
};
static int __init hw_init(void)
{
	int retval;
	hw_kobject = kobject_create_and_add("hw1",kernel_kobj);
	
	retval = sysfs_create_file(hw_kobject, &attr_group);
	return retval;
}
static void __exit hw_exit(void)
{
		printk(KERN_ALERT "CLEAN UP MODULE");
		kobject_put(hw_kobject);
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
char* concat(char*a, char*b)
{
	int len = strlen(b);
	int i = 0;
	int j = 0;
	for(i=0;i<len;i++)
	{
		if(*(b+i) == ' ')
    		break;
	}
	for(j=0;j<len-i;j++)
	{
		*(a+j) = *(b+i+j+1);
	}
	*(a+j+1) = '\0';
	return a;
}

module_init(hw_init);
module_exit(hw_exit);


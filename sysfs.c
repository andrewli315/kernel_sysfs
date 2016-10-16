#include <linux/init.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>

#define N 100
#define bool int
#define true 1
#define false 0

MODULE_LICENSE("Dual BSD/GPL");

#define N 100

int num_stack[N];
static int result;
int op_stack[N];
int n_top = 0;
int op_top = -1;
static char input[N];
static int s;
char test[40];

static char* s_name;
static char* c_name;
static char* t_name;

static char* ss_name;
static char* cc_name;
static char* tt_name;

static int mask = 111;

static struct kobject *hw_kobject;

//declare needy function
//fucntion for string swap
void swap_char(char *a,char *b);
int atoi(char *str);
char* concat(char*,char*);
//function for infix calculator
void n_push(int);
int n_pop(void);
char op_pop(void);
void op_push(char);
char top(void);
int prior(char);
int isOp(char);

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


module_param(ss_name, charp, S_IRUGO); 

module_param(cc_name, charp, S_IRUGO);

module_param(tt_name, charp, S_IRUGO);

module_param(mask,int , S_IRUGO);

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


static ssize_t c_show(struct kobject *kobject,
			struct kobj_attribute *attr, char *buf)
{
    int i=0;
	char integer[N];
	char op;
	int j=0;
	int operand1,operand2;
	op_push('#');
	while(input[i]!='\0')
	{
		if(input[i] >= '0' && input[i] <= '9' )
		{
			integer[j]=input[i];
			j++;
		}
		else if(isOp(input[i]) == 1 || input[i]=='=')
		{
			integer[j] = '\0';
			n_push(atoi(integer));
			j=0;
			if(prior(input[i]) > prior(top()))
			{
				op_push(input[i]);
			
			}
			else
			{
				while(prior(input[i]) <= prior(top()))
				{
					operand2 = n_pop();
					operand1 = n_pop();
					op = op_pop();
					switch(op){
						case '+':result = operand1+operand2;
							break;
						case '-':result = operand1-operand2;
							break;
						case '*':result = operand1*operand2;
							break;
						case '/':result = operand1/operand2;
							break;
						case '%':result = operand1%operand2;
							break;
					}
					n_push(result);
				}
				if(input[i]!='=')
						op_push(input[i]);
			}
		}
		i++;
	}
	return sprintf(buf,"answer is %d\n",result);
}
static ssize_t c_store(struct kobject *kobject,
			struct kobj_attribute *attr,const char *buf, size_t count)
{
	memcpy(input,buf,strlen(buf));
	return count;
}
static ssize_t t_show(struct kobject *kobject,
			struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf,"String");
}
static ssize_t t_store(struct kobject *kobject,
			struct kobj_attribute *attr,const char *buf, size_t count)
{
	memcpy(test,buf,strlen(buf));
	return count;
}


static struct kobj_attribute hw_attr1 =
		 __ATTR(string_swap,0660,s_show,s_store);

static struct kobj_attribute hw_attr2 =
		 __ATTR(calc,0660,c_show,c_store);

static struct kobj_attribute hw_attr3 =
		 __ATTR(sum_tree,0660,t_show,t_store);

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
	if(mask <100)
	{
		mask = (mask/8)*10 + (mask%8);
	}
	if(mask /100 == 1)
		s_name = ss_name;
	else
		s_name = "string_swap";
	if((mask/10)%10 == 1)
		c_name = cc_name;
	else
		c_name = "calc";
	if(mask%100 == 1)
		t_name = tt_name;
	else
		t_name = "sum_tree";

	hw_kobject = kobject_create_and_add("hw1",kernel_kobj);
	retval = sysfs_create_group(hw_kobject, &attr_group);
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

void n_push(int num)
{
	if(n_top >= N-1)
	{
		printk(KERN_ALERT "number stack is full\n");
	}
	else
		num_stack[++n_top] = num;
}
int n_pop(void)
{
	if(n_top<0)
	{
		printk(KERN_ALERT "index is out of range\n");
		return 0;
	}
	else
		return num_stack[n_top--];
}
void op_push(char op)
{
	if(op_top>=N-1)
	{
		printk(KERN_ALERT "operator stack is full\n");
	}
	op_stack[++op_top] = op;
}

char op_pop(void)
{
	if(op_top<0)
	{
		printk(KERN_ALERT "opstack index is out of range\n");
		return '\0';
	}
	else
		return op_stack[op_top--];
}

char top(void)
{
	return op_stack[op_top];
}
int prior(char op)
{
	switch(op){
		case '+':return 2;
			break;
		case '-':return 2;
			break;
		case '*':return 4;
			break;
		case '/':return 4;
			break;
		case '%':return 4;
			break;
		case '#':return 0;
			break;
		case '=':return 1;
			break;
		default:return 0;
			break;
	}
}
int isOp(char c)
{
	switch(c){
		case '+':return 1;
			break;
		case '-':return 1;
			break;
		case '*':return 1;
			break;
		case '/':return 1;
			break;
		case '%':return 1;
			break;
		default:return 0;
			break;
	}
}


module_init(hw_init);
module_exit(hw_exit);


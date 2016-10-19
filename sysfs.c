#include <linux/init.h>
#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#define N 100

MODULE_LICENSE("Dual BSD/GPL");

#define N 100

//variable for calculator
int num_stack[N];
static int result;
int op_stack[N];
int n_top = 0;
int op_top = -1;
static char input[N];

//variable for string 
static int s;
char test[40];

//variable for sum tree
static char str[N];
int stack[N];
int cp_stk[N];
int s_top = -1;
int cp_top = -1;
typedef struct NODE{
	int num;
	struct NODE *parent;
}node;

static char* name1="";
static char* name2="";
static char* name3="";

//initialize the 
static int mask = 0;

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
//function for sum tree
void copy(void);
int cp_pop(void);
int pop(void);
int isEmpty(void);
void push(int);
node* new_node(int x);

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


module_param(name1, charp, S_IRUGO); 

module_param(name2, charp, S_IRUGO);

module_param(name3, charp, S_IRUGO);

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
	int length = strlen(input);
	int j=0;
	int operand1,operand2;
	op_push('#');
	input[length] = '=';
	input[length+1] = '\0';
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
	char res_str[N];
	node **p = (node**)vmalloc(sizeof(node*));
	node *tmp;
	int length = strlen(str);
	int i,k=0,j=0;
	int temp = 0;
	int sum=0;
	char num[N];
	str[length] = '\0';
	for(i=0;i<30;i++)
		p[i] = (node*)vmalloc(sizeof(node));
	i=0;
	while(str[i] != '\0')
	{
		if(str[i] == '(')
		{
			num[j] = '\0';
			push(atoi(num));
			j=0;
		}
		else if(str[i] >= '0' && str[i] <= '9')
		{
			num[j++] = str[i];
		}
		else if(str[i] == ')')
		{
			if(str[i-1] >= '0' && str[i-1] <= '9')
			{
				num[j] = '\0';
				p[k] = new_node(atoi(num));
				tmp = p[k];
				while(isEmpty() != 0)
				{
					tmp->parent = new_node(pop());
					tmp = tmp->parent;
				}
				j=0;
				k++;
				copy();
				pop();
				cp_pop();
			}
		}
		else if(str[i] == ' ')
		{
			num[j] = '\0';
			p[k] = new_node(atoi(num));
			tmp = p[k];
			while(isEmpty()!=0)
			{
				tmp->parent = new_node(pop());
				tmp = tmp->parent;
			}
			j=0;
			copy();
			k++;
		}
		i++;
	}
	printk(KERN_ALERT "k = %d\n",k);
	for(i=0;i<k;i++)
	{
		tmp = p[i];
		while(tmp != NULL)
		{
			sum += tmp->num;
			tmp = tmp->parent;
		}
		temp += sprintf(res_str+temp,"%d, ",sum);
		sum = 0;
	}
	return sprintf(buf,"%s\n",res_str);
}
static ssize_t t_store(struct kobject *kobject,
			struct kobj_attribute *attr,const char *buf, size_t count)
{
	memset(str,'\0',sizeof(str));
	memcpy(str,buf,strlen(buf));
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
	{
		hw_attr1.attr.name = name1;
	}
	if((mask/10)%10 == 1)
		hw_attr2.attr.name = name2;
	if(mask%100 == 1)
		hw_attr3.attr.name = name3;
		
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

node* new_node(int x)
{
	node *new;
	new = (node*)vmalloc(sizeof(node));
	new->num = x;
	return new;
}
int isEmpty(void)
{
	if(s_top == -1)
		return 0;
	else 
		return 1;
}
int pop(void)
{
	if(s_top < 0)
	{
		printk(KERN_ALERT "stack is empty");
		return 0;
	}
	return stack[s_top--];
}
int cp_pop(void)
{
	if(cp_top < 0)
	{
		printk(KERN_ALERT "stack is empty");
		return 0;
	}
	return cp_stk[cp_top--];
}
void push(int x)
{
		
	stack[++s_top] = x;
	cp_stk[++cp_top] = x;
}
void copy(void)
{
	int i;
	for(i=0;i<N;i++)
	{
		stack[i] = cp_stk[i];
	}
	s_top = cp_top;
}
module_init(hw_init);
module_exit(hw_exit);

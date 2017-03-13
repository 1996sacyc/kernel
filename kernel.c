#include<linux/init.h>
#include<linux/fs.h>
#include<linux/kobject.h>
#include<linux/module.h>
#include<linux/moduleparam.h>
#include<linux/string.h>
#include<linux/printk.h>
#include<linux/sysfs.h>

MODULE_LICENSE("Dual BSD/GPL");

static struct kobject *hw1_kobject;
static char input[100];
static char output[100];

static int stack[50];
static int top;
static int len_glo;
static int output_int=0;
static int tem=0;

static unsigned int mask = 111;
static char *name1="swap_string";
static char *name2="calc";
static char *name3="sum_tree";

void clean(void);
void calculate(int parent,int level);
int lookback(int level);
void sort(int i,int j, int arr_num[]);
void sort2(int i,int j, char arr_calc[]);

module_param(mask, uint, S_IRUGO);
module_param(name1, charp, S_IRUGO);
module_param(name2, charp, S_IRUGO);
module_param(name3, charp, S_IRUGO);
//4 parameter


static ssize_t swap_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){    
    return sprintf(buf, "%s\n", output);
}

static ssize_t swap_store(struct kobject *kobj, struct kobj_attribute *attr, char *buf, size_t count){
    	
    char index1[20];    //index tem

    sscanf(buf, "%s%s", index1,input);


    int i=0;
    int len=strlen(input);  //len of string
	int index=index1[0]-'0';    //index
	int count_index=0;  //output index control
  
	for(i=index; i<len; i++){
		output[count_index]=input[i];
		count_index++;
	}
	
    for(i=0; i<index; i++){
		output[count_index]=input[i];
		count_index++;
	}

    return count;

}

static ssize_t calc_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){    
    return sprintf(buf, "%d\n",output_int);
}

static ssize_t calc_store(struct kobject *kobj, struct kobj_attribute *attr, char *buf, size_t count){
    
    
    int len=0;  //without space
    int i,j;
    int index_num=0;
    int index_calc=0;
    int control=1;
    int x=0;
    int y=0;

    int arr_num[100];
    char arr_calc[50];

    int eat=0;
    int eat_tem=0;	
    int neg=0;
    int pos=0;
    int counter=0;

    for(i=0; i<strlen(buf)-1; i++){
        if(buf[i]!=' '){
            if(control){
                if(eat){
                    if(i==strlen(buf)-2){
                        if(neg){
                            eat_tem=eat_tem*10+buf[i]-'0';
                            arr_num[x]=-eat_tem;
                        }else if(pos){
                            eat_tem=eat_tem*10+buf[i]-'0';
                            arr_num[x]=eat_tem;
                        }else{
                            eat_tem=eat_tem*10+buf[i]-'0';
                            arr_num[x]=eat_tem;
                        }

                        counter++;
                        x++;
                        len++;
                        index_num++;
                        control=0;

                    }else{
                        eat_tem=eat_tem*10+buf[i]-'0';
                    }
                }else if(buf[i]=='-'){
                    neg=1;   
                }else if(buf[i]=='+'){
                    pos=1;   
                }else if(i!=strlen(buf)-2){
                    eat=1;
                    eat_tem=buf[i]-'0';   
                }else if(i==strlen(buf)-2){
                    if(neg){
                        arr_num[x]=-(buf[i]-'0');
                        neg=0;
                    }else if(pos){
                        arr_num[x]=(buf[i]-'0');
                        pos=0;
                    }else{
                        arr_num[x]=buf[i]-'0';
                    }

                    counter++;
                    x++;
                    len++;
                    index_num++;
                    control=0;
                }//last                        

            }else{
                arr_calc[y]=buf[i];
                y++;
                len++;
                index_calc++;
            }
    
        }else{
            if(control){
                if(neg){
                    arr_num[x]=-eat_tem;
                    neg=0;
                }else if(pos){
                    arr_num[x]=eat_tem;
                    pos=0;
                }else{
                    arr_num[x]=eat_tem;
                }
                
                eat_tem=0;
                eat=0;
                x++;
                len++;
                index_num++;
                control=0;
            }else{
                control=1;
            }
        }

    }

    for(j=0; j<len/2; j++){
	  for(i=0;i<index_calc; i++){
	  	if(arr_calc[i]=='/'){
			arr_num[i]=arr_num[i]/arr_num[i+1];
            index_num--;
            index_calc--;
            sort2(i,index_calc,arr_calc);
			sort(i+1,index_num,arr_num);
			break;
		}else if(arr_calc[i]=='*'){
			arr_num[i]=arr_num[i]*arr_num[i+1];
			index_num--;
			index_calc--;
			sort2(i,index_calc,arr_calc);
			sort(i+1,index_num,arr_num);
			break;
		}else if(arr_calc[i]=='%'){
			arr_num[i]=arr_num[i]%arr_num[i+1];
			index_num--;
			index_calc--;
			sort2(i,index_calc,arr_calc);
			sort(i+1,index_num,arr_num);
			break;
		}
	  }
	}

    while (index_calc!=0){
	  for(i=0;i<index_calc; i++){
		switch(arr_calc[i]){
            case '+': arr_num[i]=arr_num[i]+arr_num[i+1];
                      index_num--;
                      index_calc--;
					  sort2(i,index_calc,arr_calc);
					  sort(i+1,index_num,arr_num);
					  break;
					  
            case '-': arr_num[i]=arr_num[i]-arr_num[i+1];
					  index_num--;
					  index_calc--;
					  sort2(i,index_calc,arr_calc);
					  sort(i+1,index_num,arr_num);
					  break;
					  
            default: break;
		}
	  }	
	}
    
    output_int=arr_num[0];

    return count;

}

void sort(int i, int j, int arr_num[]){
	int z;

	for(z=i; z<j; z++){
		arr_num[z]=arr_num[z+1];
	}
}

void sort2(int i, int j, char arr_calc[]){
	int z;

	for(z=i; z<j; z++){
		arr_calc[z]=arr_calc[z+1];
	}
}

static ssize_t sum_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){    
    return sprintf(buf, "%s\n", output);
}


struct node{
	int label;
	int value;
}tree[100];

static ssize_t sum_store(struct kobject *kobj, struct kobj_attribute *attr, char *buf, size_t count){
        
    int len=strlen(buf); //totel len include ()

	int output1[50];
	len_glo=len;
	int i,j;
	int tree_index=0;
	int tree_level=0;
	int level_tem=0;
	int counter=0;
	int tem=0;

    int eat=0;
    int eat_tem=0;	
    int neg=0;

	for(i=0; i<len; i++){
		tree[i].label=0;
		tree[i].value=0;
	}//init

    for(i=0; i<len; i++){
		
		if(tree[i].label==3){
			tree[i].value=0;
		}
        else if(buf[i]=='-'){
            neg=1;
            tree[i].label=4;
        }//negative node
		else if(buf[i]=='('){
			tree_level++;
			tree[i].label=0;
			tree[i].value=tree_level;
			stack[top]=tree_level;
			top++;
			counter++;

            if(eat){
                if(neg){
                    tree[i-1].value=-eat_tem;
                    tree[i-1].label=1;
                    neg=0;
                }else{
                    tree[i-1].value=eat_tem;
                    tree[i-1].label=1;
                }
            }
            eat=0;

		}else if(buf[i]==')'){
			tree[i].label=0;
			tree[i].value=stack[top-1];  //same level
			top--;

            if(eat){
                if(neg){
                    tree[i-1].value=-eat_tem;
                    tree[i-1].label=1;
                    neg=0;
                }else{
                    tree[i-1].value=eat_tem;
                    tree[i-1].label=1;
                }
            }
            eat=0;
		}
		else if(buf[i]==' '){
			tree[i].label=2;

            if(eat){
                if(neg){
                    tree[i-1].value=-eat_tem;
                    tree[i-1].label=1;
                    neg=0;
                }else{
                    tree[i-1].value=eat_tem;
                    tree[i-1].label=1;
                }
            }
            eat=0;
		}
		else if(eat){
            eat_tem=eat_tem*10+buf[i]-'0';
			tree[i].label=1;
			tree[i-1].label=3;
			
		}//up to 2 digit
		else{
			eat_tem=buf[i]-'0';
            tree[i].label=1;
			eat=1;
		}
	}
	
	clean();
	top=0;

	for(i=0; i<counter-1; i++){
		
		for(j=0; j<len; j++){
			if(tree[j].label==0){
				if(tree[j].value!=level_tem){
					level_tem=tree[j].value;
					clean();
				}else{
					tree[j].label=4;
					tree[j].value=0;
					tem=lookback(level_tem);
					calculate(tem,level_tem);
					clean();
					break;
				}
			}else if(tree[j].label==1){
				stack[top]=tree[j].value;
				top++;
			}
		}
	
	}
	
	counter=0;
	
	for(i=0; i<len; i++){
		if(tree[i].label==1&&tree[i].value!=0){
			output1[counter]=tree[i].value;
			counter++;
		}
	}
	j=0;  

    for(i=0; i<counter; ++i){
        j+=sprintf(output+j,"%d",output1[i]);
        if(i!=counter-1){
            j+=sprintf(output+j,",");
        }

    }

    return count;
}

void calculate(int parent, int level){
	int i;
	int level_index=0;
	
	for(i=0; i<100; i++){
		if((tree[i].label==0)&&(tree[i].value==level)){
			level_index=i;
			tree[i].label=4;
			tree[i].value=0;
			
            if(i>2&&tree[i-1].label==3){
                tree[i-2].label=4;
			    tree[i-2].value=0; 
            }else{
                tree[i-1].label=4;
			    tree[i-1].value=0; 
            }

			break;
		}
	}//find the start index
	
	for(i=0; i<top; i++){
		if(tree[level_index].label!=1){
			level_index++;
			i--;
		}//label=2 or 3  case
		else{
			stack[i]=stack[i]+parent;
			tree[level_index].value=stack[i];
			level_index++;
		}
		
	}
	
}

int lookback(int level){
	int i;
	int tem_parent=0;
	
	for(i=0; i<len_glo; i++){
		if(tree[i].label==0){
			if(tree[i].value==level){
				return tem_parent;
			}
		}else if(tree[i].label==1){
			tem_parent=tree[i].value;
		}
	}
}//find parent

void clean(void){
	int i;
	
	for(i=0; i<50; i++){
		stack[i]=0;
	}
	
	top=0;
}//clean stack

static struct kobj_attribute swap_attribute = __ATTR(name1, 0660, swap_show, swap_store);
static struct kobj_attribute calc_attribute = __ATTR(name2, 0660, calc_show, calc_store);
static struct kobj_attribute sum_attribute = __ATTR(name3, 0660, sum_show, sum_store);

static int __init create_hw1(void){

    int error1= 0;
    int error2= 0;
    int error3= 0;

    pr_debug("Module initialized successfully \n");

    hw1_kobject = kobject_create_and_add("hw1", kernel_kobj);

    if(!hw1_kobject)
        return -ENOMEM;

    swap_attribute.attr.name = name1;
    calc_attribute.attr.name = name2;
    sum_attribute.attr.name = name3;

    if(mask==111||mask==110||mask==101||mask==100){
        error1 = sysfs_create_file(hw1_kobject, &swap_attribute.attr);
        sysfs_create_file(hw1_kobject, &swap_attribute.attr);
    }
    if(mask==010||mask==111||mask==011||mask==110){
        error2 = sysfs_create_file(hw1_kobject, &calc_attribute.attr);
        sysfs_create_file(hw1_kobject, &calc_attribute.attr);
    }
    if(mask==111||mask==101||mask==001||mask==011){
        error3 = sysfs_create_file(hw1_kobject, &sum_attribute.attr);
        sysfs_create_file(hw1_kobject, &sum_attribute.attr);
    }


    if(error1){
        pr_debug("fail to create the file \n");    
    }

   return error1;
}

static void __exit cleanup_hw1(void){
    pr_debug("Module initialized successfully \n");
    kobject_put(hw1_kobject);
}

module_init(create_hw1);
module_exit(cleanup_hw1);

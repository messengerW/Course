#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>

// 声明一个int型变量，默认值为1
static pid_t pid=1;

// 等待用户加载模块时键入pid
module_param(pid,int,0644);

static int hello_init(void)
{
    struct task_struct *p;
    struct list_head *pp;
    struct task_struct *psibling;
    struct task_struct *pchildren;

    p = pid_task(find_vpid(pid), PIDTYPE_PID);

    // 父进程
    if(p->parent == NULL) {
        printk("<No Parent>");
    } else {
	printk("Parent:%s(%d)",p->parent->comm,p->parent->pid);
    }
    
    // 当前进程
    printk("\t\tMe     ——:%s(%d)",p->comm,p->pid);


    // 兄弟进程
    list_for_each(pp, &p->parent->children)
    {
        psibling = list_entry(pp, struct task_struct, sibling);
	if(psibling != p)
            printk("\t\tSibling————————:%s(%d)",psibling->comm,psibling->pid);
    }

    // 子进程
    list_for_each(pp, &p->children)
    {
       	pchildren = list_entry(pp, struct task_struct, sibling);
       	printk("\t\t\t\t\t\t——————————————————Children:%s(%d)", pchildren->comm, pchildren->pid);
    }
    return 0;
}

static void hello_exit(void)
{
    printk(KERN_ALERT"Bye~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~i am module2\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");

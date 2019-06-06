#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define READY 0x01		//	就绪态，用 1 标识
#define RUNNING 0x02	//	运行态，用 2 标识
#define TIMESLICE 0x02	//	时间片，设定为 2

typedef struct _PCB
{   
	int pid;			//	进程号
	int state;  		//	进程状态
	int time;  			//	仍需运行的时间
    struct _PCB *next; 		//	指向下一个进程
} PCB, *pPCB;

pPCB Create(int n)
{
	//	创建链表的函数，返回值为链表头节点
	int i;
	pPCB head, tail, pnew;
	
	//	头结点不存放实际数据，初始化全存 -1，方便对链表进行排序
	head = (struct _PCB *)malloc(sizeof(struct _PCB));
	head->pid = -1;
	head->state = -1;
	head->time = -1;
	
	pnew = (struct _PCB *)malloc(sizeof(struct _PCB));
	pnew->pid = 0;
	pnew->state = READY;
	scanf("%d",&pnew->time);
	head->next = tail = pnew;
	
	//	从链表的第二个结点开始储存进程信息
	for(i = 1; i < n; i++)
	{
		pnew = (struct _PCB *)malloc(sizeof(struct _PCB));
		pnew->pid = i;
		pnew->state = READY;
		scanf("%d", &pnew->time);
		tail->next = pnew;
		tail = pnew;
	}
	tail->next = NULL;
	return head;
}

pPCB Append(pPCB Queue, pPCB pNew)
{
	//	增添函数，功能是在链表末尾增加一个结点
	while(Queue->next)		//	这个循环的作用是找到链表的最后一个结点
	{
		printf("***** Append : pid = %d\n",Queue->pid);
		Queue = Queue->next;
	}
	pNew = (struct _PCB *)malloc(sizeof(struct _PCB));
	pNew->next = NULL;
	Queue->next = pNew;		//	把 pnew 放至队尾
	Queue->next->state = READY;		//	状态切换成 ready
}

void Delete(pPCB Queue, int pid)
{
	//	删除函数，根据进程号删除指定进程(删除就绪队列队首结点)，flag用来标记操作是否成功
	int flag = 0;
	while(Queue)
	{
		if( Queue->pid == pid )
		{
			printf("***** Delete : pid = %d\n",Queue->pid);
			Queue->next = Queue->next->next;
			flag = 1;
		}
		Queue = Queue->next;
	}
	if(!flag)
		printf("Delete Error! Check the pid ( %d ).\n",pid);
}

void Sort(pPCB Queue)
{
	//	排序函数，采用的是冒泡排序
    struct _PCB *head, *tail, *before, *current, *next, *temp;	//	temp 用作辅助交换
    head = Queue;
    tail = NULL;
    // 从链表头开始将较大值往后沉，最终得到一列关键字从小到大的序列
    while(head->next != tail)
    {
        for(before = head, current = before->next, next = current->next;
			next != tail;
			before = before->next, current = current->next, next = next->next)
        {
            // 相邻的结点比较关键字 time
            if(current->time > next->time)
            {
                current->next = next->next;
                before->next = next;
                next->next = current;
                temp = next;
                next = current;
                current = temp;
            }
        }
        tail = current;		// 尾结点向前移动一位，然后执行下一个while循环
    }
}

void Run(pPCB Queue, int pid)
{
	//	进程每执行一次，time - 2，flag 用作标记操作是否成功
	int flag = 0;
	printf("===> Process %d start running.\n",Queue->pid);
	while(Queue->next)
	{
		printf("***** Run : pid = %d\t flag = %d \n",Queue->pid,flag);
		if( Queue->pid == pid )				//	找到这个进程
		{
			printf("***** Found pid = %d\n",Queue->pid);
			Queue->state = RUNNING;			//	状态切换为 running
			Queue->time = Queue->time - 2;	//	time - 2
			flag = 1;
		}
		Queue = Queue->next;
	}
	printf("===> Process %d ended.\n",Queue->pid);
	if(!flag)
		printf("Run Error ! check the pid ( %d ).\n",pid);
}

pPCB  Scheduler_FCFS(pPCB Queue)
{
	pPCB p;
	//	先来先服务调度
	while(Queue && Queue->next)		//	首先判断就绪队列不为空
	{
		printf("===> Before scheduler , Queue : pid = %d \t time = %d \n",Queue->pid,Queue->time);
		if( Queue->time <= 0 )					//	删除 time <= 0 的进程
			Delete(Queue,Queue->pid);
		else {
			Delete(Queue,Queue->pid);			//	先从就绪队列队首删除这个进程
			printf("===> After Delete , Queue : pid = %d \t time = %d \n",Queue->pid,Queue->time);
			p = (struct _PCB *)malloc(sizeof(struct _PCB));
			//p = head;
			Run(Queue,Queue->pid);		//	再执行这个的进程 （state = RUNNING）
			printf("===> After Run , Queue : pid = %d \t time = %d \n",Queue->pid,Queue->time);
			p->pid = Queue->pid;
			p->state = Queue->state;
			p->time = Queue->time;
			Append(Queue->next,p);				//	执行完后，插入到就绪队列队尾 （state = READY）
			printf("===> After Append , Queue : pid = %d \t time = %d \n",Queue->pid,Queue->time);
			printf("\n");
		}					
		Queue = Queue->next;
	}
}

pPCB  Scheduler_SPF(pPCB Queue)
{
	
}

void Print(pPCB Queue)
{
	//	打印函数 只打印学号
	while(Queue)
	{
		if( Queue->pid >= 0 )
		{
			if( Queue )
			{
				printf("进程号：%d\t",Queue->pid);
				printf("仍需运行时间：%d\n",Queue->time);
			}
		}
		Queue = Queue->next;
	}
}

int main()
{
	int n;
	char str[20], cmd[3][20];		//	str字符串用来存键入的调度方式（ FCFS / SPF）
	pPCB ReadyQueue, RunningQueue;
	
	strcpy(cmd[0],"q");			//	q 为退出命令
	strcpy(cmd[1],"fcfs");		//	先来先服务	
	strcpy(cmd[2],"SPF");		//	短进程优先
	strcpy(str,"0");
	
	while( strcmp(str,cmd[0]) != 0 )
	{
		if( scanf("%s", str) != EOF )		//	调度方式
		{
			scanf("%d",&n);					//	进程个数
			ReadyQueue = Create(n);			//	创建就绪队列
			printf("Created succussfully ! pid = %d\n",ReadyQueue->pid);
			if ( strcmp(str,cmd[1]) == 0 ) {		//	选用先来先服务调度
				Print(ReadyQueue);
				printf("\n===Begin===\n");
				Scheduler_FCFS(ReadyQueue->next);
				printf("===End===\n");
			}
			else if (strcmp(str,cmd[2]) == 0) {		//	选用短进程，需要先排序
				Sort(ReadyQueue);
				Print(ReadyQueue);
			} else {
				strcpy(str,"q");
			}
		} else 
			strcpy(str,"q");
	}
	
	return 0;
	
	
}
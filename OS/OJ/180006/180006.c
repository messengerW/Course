#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define READY 0x01		//	就绪态，用 1 标识
#define RUNNING 0x02	//	运行态，用 2 标识
#define TIMESLICE 0x02	//	时间片，设定为 2

typedef struct _PCB
{   
	int pid;				//	进程号
	int state;  			//	进程状态
	int time;  				//	仍需运行的时间
    struct _PCB *next; 		//	指向下一个进程
} PCB, *pPCB;

pPCB readyQueue = NULL;
pPCB runningQueue = NULL;

void AppendReadyQueue(int pid,int time)
{
	pPCB temp;
	pPCB proc = (pPCB)malloc(sizeof(PCB));
	proc->pid = pid;
	proc->time = time;
	proc->state = READY;
	proc->next = NULL;

	if (!proc){
		exit(0);
	}else if (readyQueue->next == NULL){
		readyQueue->next = proc;
	}else{
		temp = readyQueue->next;
		while(temp->next!=NULL){
			temp=temp->next;
		}
		temp->next = proc;
	}
}

void DeleteReadyQueue()
{
	pPCB temp;
	if (readyQueue->next == NULL)
		exit(0);
	else
	{
		temp = readyQueue->next;
	    readyQueue->next = temp->next;
	    free(temp);
	}
}

void AppendRunningQueue(int pid,int time)
{
	pPCB temp;
	pPCB proc = (pPCB)malloc(sizeof(PCB));
	proc->pid = pid;
	proc->time = time;
	proc->state = RUNNING;
	proc->next = NULL;

	if (!proc){
		exit(0);
	}else if (runningQueue->next == NULL){
		runningQueue->next = proc;
	}else{
		temp = runningQueue->next;
		while(temp->next!=NULL){
			temp=temp->next;
		}
		temp->next = proc;
	}
}

void DeleteRunningQueue()
{
	pPCB temp;
	if (runningQueue->next == NULL)
		exit(0);
	else
	{
		temp = runningQueue->next;
		runningQueue->next = temp->next;
		free(temp);
	}
}

void sort()
{
	//	如果是短进程优先，则需要按照长短排序
	pPCB p = readyQueue->next;
	pPCB temp = (pPCB)malloc(sizeof(pPCB));
	pPCB q;
	pPCB r;
	while(p!=NULL)
	{
		q = p;
		r = p;
		while(q!=NULL)
		{
			if (q->time < r->time)
				r = q;
			q = q->next;
		}
		
		temp->pid = r->pid;
		temp->time = r->time;
		r->pid = p->pid;
		r->time = p->time;
		p->pid = temp->pid;
		p->time = temp->time;
		
		p = p->next;
	}
}

void modifyPCBState(pPCB p)
{
	if (readyQueue->next == p)
	{
		printf(" P0%d",p->pid);
		AppendRunningQueue(p->pid,p->time);
		DeleteReadyQueue();
	}
	if (runningQueue->next == p)
	{
		p->time = p->time-TIMESLICE;
		
		if (p->time <= 0)
			DeleteRunningQueue();
		else
		{
			AppendReadyQueue(p->pid,p->time);
			DeleteRunningQueue();
		}
		
	}
}

int main(){
	
	int i, n;
	int time[100];
	char str[20], cmd[5][20];		//	str字符串用来存键入的调度方式（ FCFS / SPF）
	
	readyQueue = (pPCB)malloc(sizeof(PCB));
	readyQueue->next = NULL;
	runningQueue = (pPCB)malloc(sizeof(PCB));
	runningQueue->next = NULL;
	
	strcpy(cmd[0],"q");			//	q 为退出命令
	strcpy(cmd[1],"fcfs");		//	先来先服务	
	strcpy(cmd[2],"FCFS");
	strcpy(cmd[3],"spf");		//	短进程优先
	strcpy(cmd[4],"SPF");
	strcpy(str,"0");
	
	while( strcmp(str,cmd[0]) != 0 )
	{
		if( scanf("%s", str) != EOF )		//	调度方式
		{
			scanf("%d",&n);					//	进程个数
			for (i = 0; i < n; i++)
			{
				scanf("%d",&time[i]);
				AppendReadyQueue(i,time[i]);
			}
			if ( strcmp(str,cmd[1]) == 0 || strcmp(str,cmd[2]) == 0 ) {		//	选用先来先服务调度
				printf("fcfs order:");
				while(readyQueue->next != NULL || runningQueue->next != NULL){
					modifyPCBState(readyQueue->next);
					modifyPCBState(runningQueue->next);
				}
			} else if ( strcmp(str,cmd[3]) == 0 || strcmp(str,cmd[4]) == 0 ) {		//	选用短进程，需要先排序
				printf("spf order:");
				sort();
				while(readyQueue->next != NULL || runningQueue->next != NULL){
					modifyPCBState(readyQueue->next);
					sort();
					modifyPCBState(runningQueue->next);
				}
			} else 
				strcpy(str,"q");
		}
	}
	return 0;
}
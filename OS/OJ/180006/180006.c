#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define READY 0x01		//	����̬���� 1 ��ʶ
#define RUNNING 0x02	//	����̬���� 2 ��ʶ
#define TIMESLICE 0x02	//	ʱ��Ƭ���趨Ϊ 2

typedef struct _PCB
{   
	int pid;				//	���̺�
	int state;  			//	����״̬
	int time;  				//	�������е�ʱ��
    struct _PCB *next; 		//	ָ����һ������
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
	//	����Ƕ̽������ȣ�����Ҫ���ճ�������
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
	char str[20], cmd[5][20];		//	str�ַ������������ĵ��ȷ�ʽ�� FCFS / SPF��
	
	readyQueue = (pPCB)malloc(sizeof(PCB));
	readyQueue->next = NULL;
	runningQueue = (pPCB)malloc(sizeof(PCB));
	runningQueue->next = NULL;
	
	strcpy(cmd[0],"q");			//	q Ϊ�˳�����
	strcpy(cmd[1],"fcfs");		//	�����ȷ���	
	strcpy(cmd[2],"FCFS");
	strcpy(cmd[3],"spf");		//	�̽�������
	strcpy(cmd[4],"SPF");
	strcpy(str,"0");
	
	while( strcmp(str,cmd[0]) != 0 )
	{
		if( scanf("%s", str) != EOF )		//	���ȷ�ʽ
		{
			scanf("%d",&n);					//	���̸���
			for (i = 0; i < n; i++)
			{
				scanf("%d",&time[i]);
				AppendReadyQueue(i,time[i]);
			}
			if ( strcmp(str,cmd[1]) == 0 || strcmp(str,cmd[2]) == 0 ) {		//	ѡ�������ȷ������
				printf("fcfs order:");
				while(readyQueue->next != NULL || runningQueue->next != NULL){
					modifyPCBState(readyQueue->next);
					modifyPCBState(runningQueue->next);
				}
			} else if ( strcmp(str,cmd[3]) == 0 || strcmp(str,cmd[4]) == 0 ) {		//	ѡ�ö̽��̣���Ҫ������
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
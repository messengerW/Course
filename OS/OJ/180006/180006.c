#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define READY 0x01		//	����̬���� 1 ��ʶ
#define RUNNING 0x02	//	����̬���� 2 ��ʶ
#define TIMESLICE 0x02	//	ʱ��Ƭ���趨Ϊ 2

typedef struct _PCB
{   
	int pid;			//	���̺�
	int state;  		//	����״̬
	int time;  			//	�������е�ʱ��
    struct _PCB *next; 		//	ָ����һ������
} PCB, *pPCB;

pPCB Create(int n)
{
	//	��������ĺ���������ֵΪ����ͷ�ڵ�
	int i;
	pPCB head, tail, pnew;
	
	//	ͷ��㲻���ʵ�����ݣ���ʼ��ȫ�� -1������������������
	head = (struct _PCB *)malloc(sizeof(struct _PCB));
	head->pid = -1;
	head->state = -1;
	head->time = -1;
	
	pnew = (struct _PCB *)malloc(sizeof(struct _PCB));
	pnew->pid = 0;
	pnew->state = READY;
	scanf("%d",&pnew->time);
	head->next = tail = pnew;
	
	//	������ĵڶ�����㿪ʼ���������Ϣ
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
	//	��������������������ĩβ����һ�����
	while(Queue->next)		//	���ѭ�����������ҵ���������һ�����
	{
		printf("***** Append : pid = %d\n",Queue->pid);
		Queue = Queue->next;
	}
	pNew = (struct _PCB *)malloc(sizeof(struct _PCB));
	pNew->next = NULL;
	Queue->next = pNew;		//	�� pnew ������β
	Queue->next->state = READY;		//	״̬�л��� ready
}

void Delete(pPCB Queue, int pid)
{
	//	ɾ�����������ݽ��̺�ɾ��ָ������(ɾ���������ж��׽��)��flag������ǲ����Ƿ�ɹ�
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
	//	�����������õ���ð������
    struct _PCB *head, *tail, *before, *current, *next, *temp;	//	temp ������������
    head = Queue;
    tail = NULL;
    // ������ͷ��ʼ���ϴ�ֵ����������յõ�һ�йؼ��ִ�С���������
    while(head->next != tail)
    {
        for(before = head, current = before->next, next = current->next;
			next != tail;
			before = before->next, current = current->next, next = next->next)
        {
            // ���ڵĽ��ȽϹؼ��� time
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
        tail = current;		// β�����ǰ�ƶ�һλ��Ȼ��ִ����һ��whileѭ��
    }
}

void Run(pPCB Queue, int pid)
{
	//	����ÿִ��һ�Σ�time - 2��flag ������ǲ����Ƿ�ɹ�
	int flag = 0;
	printf("===> Process %d start running.\n",Queue->pid);
	while(Queue->next)
	{
		printf("***** Run : pid = %d\t flag = %d \n",Queue->pid,flag);
		if( Queue->pid == pid )				//	�ҵ��������
		{
			printf("***** Found pid = %d\n",Queue->pid);
			Queue->state = RUNNING;			//	״̬�л�Ϊ running
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
	//	�����ȷ������
	while(Queue && Queue->next)		//	�����жϾ������в�Ϊ��
	{
		printf("===> Before scheduler , Queue : pid = %d \t time = %d \n",Queue->pid,Queue->time);
		if( Queue->time <= 0 )					//	ɾ�� time <= 0 �Ľ���
			Delete(Queue,Queue->pid);
		else {
			Delete(Queue,Queue->pid);			//	�ȴӾ������ж���ɾ���������
			printf("===> After Delete , Queue : pid = %d \t time = %d \n",Queue->pid,Queue->time);
			p = (struct _PCB *)malloc(sizeof(struct _PCB));
			//p = head;
			Run(Queue,Queue->pid);		//	��ִ������Ľ��� ��state = RUNNING��
			printf("===> After Run , Queue : pid = %d \t time = %d \n",Queue->pid,Queue->time);
			p->pid = Queue->pid;
			p->state = Queue->state;
			p->time = Queue->time;
			Append(Queue->next,p);				//	ִ����󣬲��뵽�������ж�β ��state = READY��
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
	//	��ӡ���� ֻ��ӡѧ��
	while(Queue)
	{
		if( Queue->pid >= 0 )
		{
			if( Queue )
			{
				printf("���̺ţ�%d\t",Queue->pid);
				printf("��������ʱ�䣺%d\n",Queue->time);
			}
		}
		Queue = Queue->next;
	}
}

int main()
{
	int n;
	char str[20], cmd[3][20];		//	str�ַ������������ĵ��ȷ�ʽ�� FCFS / SPF��
	pPCB ReadyQueue, RunningQueue;
	
	strcpy(cmd[0],"q");			//	q Ϊ�˳�����
	strcpy(cmd[1],"fcfs");		//	�����ȷ���	
	strcpy(cmd[2],"SPF");		//	�̽�������
	strcpy(str,"0");
	
	while( strcmp(str,cmd[0]) != 0 )
	{
		if( scanf("%s", str) != EOF )		//	���ȷ�ʽ
		{
			scanf("%d",&n);					//	���̸���
			ReadyQueue = Create(n);			//	������������
			printf("Created succussfully ! pid = %d\n",ReadyQueue->pid);
			if ( strcmp(str,cmd[1]) == 0 ) {		//	ѡ�������ȷ������
				Print(ReadyQueue);
				printf("\n===Begin===\n");
				Scheduler_FCFS(ReadyQueue->next);
				printf("===End===\n");
			}
			else if (strcmp(str,cmd[2]) == 0) {		//	ѡ�ö̽��̣���Ҫ������
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
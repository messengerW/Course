#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#define TRUE 1

#define BUF_SIZE 255 //缓冲大小
#define PERM S_IRUSR|S_IWUSR
#define KEY_NUM 1000
int f = 0;
int id = 0;
typedef struct msgbuf msgbuf;

struct msgbuf
{
    long mtype; //消息类型
    char mtext[BUF_SIZE + 1]; //256，消息正文
};


sem_t full;
sem_t empty;
sem_t mutex;

pthread_t write_pid; 
pthread_t read_pid;

key_t key;

int msgid;

struct msgbuf msg;

void Init()
{
    sem_init(&full,0,0);
    sem_init(&empty,0,1);
    sem_init(&mutex,0,1);

    key = KEY_NUM;//给键值赋值
    if((msgid = msgget(key,PERM|IPC_CREAT)) == -1)
    {
        fprintf(stderr, "Create Message Queue Error %s\n",strerror(errno) );
        exit(EXIT_FAILURE);
    }   
}

void * ReadProcess(void *arg)
{
    msgbuf msg;
    msg.mtype = 1;
    int flag=0;
    while(TRUE)
    {
        sem_wait(&full);
        sem_wait(&mutex);

        msgrcv(msgid,&msg,sizeof(msgbuf),1,0);//接收类型为1的消息（即mtype=1）

        if(strcmp(msg.mtext,"end1") == 0 && id == 1)
        {
            msg.mtype = 2;
            strncpy(msg.mtext,"over1",BUF_SIZE);
            msgsnd(msgid,&msg,sizeof(msgbuf),0);
            sem_post(&empty);
            sem_post(&mutex);
            flag++;
	    msg.mtype = 1;
        }
	    if(strcmp(msg.mtext,"end2") == 0 && id == 2)
        {
            msg.mtype = 2;
            strncpy(msg.mtext,"over2",BUF_SIZE);
            msgsnd(msgid,&msg,sizeof(msgbuf),0);
            sem_post(&empty);
            sem_post(&mutex);
            flag++;
	    msg.mtype = 1;
        }
        printf("Receive:  %s\n\n",msg.mtext);
        if(flag == 2)
	{
		break;
	}
        sem_post(&empty);
        sem_post(&mutex);
    }
    exit(EXIT_SUCCESS);
}

void * WriteProcess1(void *arg)
{   
    char input[50];
    msgbuf msg;
    msg.mtype = 1;
    while (TRUE)
    {
        sem_wait(&empty);
        sem_wait(&mutex);
        sleep(0.1);
        printf("Sender1: Please Input the message you want to send.\n");
        scanf("%s",input);
        if(strcmp(input,"end1") == 0 )
        {
	    id = 1;
            strncpy(msg.mtext,"end1",BUF_SIZE);
            msgsnd(msgid,&msg,sizeof(msgbuf),0);
            sem_post(&full);
            sem_post(&mutex);
            f++;
	    break; 
        }
	
        strncpy(msg.mtext,input,BUF_SIZE);
        msgsnd(msgid,&msg,sizeof(msgbuf),0);
        printf("Sender1: %s\n",msg.mtext);
        sem_post(&full);
        sem_post(&mutex);
    }

    if(f != 2)
    {
	    memset(&msg,'\0',sizeof(msgbuf));
            msgrcv(msgid,&msg,sizeof(msgbuf),2,0);
    }
    printf("Sender1:%s\n",msg.mtext );
    return 0;
}

void * WriteProcess2(void *arg)
{
    char input[50];
    msgbuf msg;
    msg.mtype = 1;
    while (TRUE)
    {
        sem_wait(&empty);
        sem_wait(&mutex);
        sleep(0.1);
        printf("Sender2: Please Input the message you want to send.\n");
        scanf("%s",input);
        if(strcmp(input,"end2") == 0)
        {
            id = 2;
            strncpy(msg.mtext,"end2",BUF_SIZE);
            msgsnd(msgid,&msg,sizeof(msgbuf),0);
            sem_post(&full);
            sem_post(&mutex);
	    f++;
            break; //输出exit后，转化为end，然后跳出while循环
        }
        strncpy(msg.mtext,input,BUF_SIZE);
        msgsnd(msgid,&msg,sizeof(msgbuf),0);
        printf("Sender2: %s\n",msg.mtext);
        sem_post(&full);
        sem_post(&mutex);
    }


    if(f != 2)
    {      
	    memset(&msg,'\0',sizeof(msgbuf));
            msgrcv(msgid,&msg,sizeof(msgbuf),2,0);
    }
    printf("Sender2:%s\n",msg.mtext );
   return 0;
}

int main()
{
    Init();
    pthread_create(&write_pid,NULL,WriteProcess1,NULL);
    pthread_create(&write_pid,NULL,WriteProcess2,NULL);
    pthread_create(&read_pid,NULL,ReadProcess,NULL);
    pthread_join(write_pid,NULL);
    pthread_join(read_pid,NULL);
    printf("Main Function End...\n");
    return 0;
}

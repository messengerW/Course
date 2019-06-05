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

//message buffer struct,experiment instruction book
typedef struct msgbuf
{
    long mtype; //unsigned long type,>0 
    char mtext[128]; //128,message content
}msgbuf;

//sem_t:actually long int
sem_t full;
sem_t empty;
sem_t mutex;

//messageid
int msgid;

//buf
msgbuf buf;

void *receiver(void *a)
{
    //init msg
    int flag1 = 0;
    int flag2 = 0;
    while(1)
    {
        //sem_wait = wait
        //may cause deadlock if changing these two wait
        sem_wait(&full);//wait for sender
        sem_wait(&mutex);//mutex for message queue
	if ( flag1 && flag2 ) {
            sem_destroy(&full);
	    sem_destroy(&empty);
            sem_destroy(&mutex);
            break;
        }
        //receive from message queue
        msgrcv(msgid,&buf,sizeof(buf.mtext),1,0);//receive mtype=1
        //print message
        printf("Receiver thread: %s\n\n",buf.mtext);
        //if received "end1",send over1 to receiver
        if ( strcmp(buf.mtext,"end1") == 0 )
	{
            flag1 = 1;
            buf.mtype = 2;//change mtype to 2 
            strcpy(buf.mtext,"over1");
            msgsnd(msgid,&buf,sizeof(buf.mtext),0);
	}
	//if received "end2",send over2 to receiver
	if ( strcmp(buf.mtext,"end2") == 0 )
        {
            flag2 = 1;
            buf.mtype = 2;//change mtype to 3 
            strcpy(buf.mtext,"over2");
            msgsnd(msgid,&buf,sizeof(buf.mtext),0);
        }
        //sem_post = signal
        sem_post(&mutex);
	sem_post(&empty);
    }
    exit(1);
}


void *sender1(void *a)
{
    char s[100];
    buf.mtype = 1;
    int flag = 0;
    while(1)
    {
        sem_wait(&empty);//wait for receive
        sem_wait(&mutex);//mutex for message queue
        if(flag)
	{
            // for type2
            int num = msgrcv(msgid,&buf,sizeof(buf.mtext),2,0);
	    printf("bytes : %d\n",num);
            printf("Response content: ");
            printf("%s\n",buf.mtext );//print the response 
            printf("Thanks for use! Good Bye~\n");
            sem_post(&full);
            sem_post(&mutex);
            break;
	}
        printf("Input the message1:\n");
        scanf("%s",s);
        if(strcmp(s,"exit1") == 0)//if user input "exit" to quit
	{
	    flag = 1;
            strcpy(s,"end1");
	}
        strcpy(buf.mtext,s);
        msgsnd(msgid,&buf,sizeof(buf.mtext),0);	// 注意，这里发送的消息类型1
        printf("Sender1 thread: %s\n",buf.mtext);
//	printf("flag = %d\n",flag);
	sem_post(&full);
        sem_post(&mutex);
    }
    //IPC_RMID:remove message queue
    msgctl(msgid,IPC_RMID,NULL);
    exit(1);
}

void *sender2(void *a)
{
    char s[100];
    buf.mtype = 1;
    int flag = 0;
    while(1)
    {
        sem_wait(&empty);//wait for receive
        sem_wait(&mutex);//mutex for message queue
        if(flag)
        {
            // for type3
	    msgrcv(msgid,&buf,sizeof(buf.mtext),3,0);
            printf("Response content: ");
            printf("%s\n",buf.mtext );//print the response 
            printf("Thanks for use! Gooooood Bye~\n");
            sem_post(&full);
            sem_post(&mutex);
            break;
        }
        printf("Input the message2:\n");
        scanf("%s",s);
        if(strcmp(s,"exit2") == 0)//if user input "exit" to quit
        {
            flag = 1;
            strcpy(s,"end2");
        }
        strcpy(buf.mtext,s);
        msgsnd(msgid,&buf,sizeof(buf.mtext),0);
        printf("Sender2 thread: %s\n",buf.mtext );
        sem_post(&full);
        sem_post(&mutex);
    }
    //IPC_RMID:remove message queue
    msgctl(msgid,IPC_RMID,NULL);
    exit(1);
}


int main()
{
    //pthread_t:thread description symbol
    pthread_t sender1ID;
    pthread_t sender2ID; 
    pthread_t receiveID;
    key_t key = 0;//key=0 means create a new message queue
    sem_init(&full,0,0);
    sem_init(&empty,0,1);
    sem_init(&mutex,0,1);
    //S_IRUSR|S_IWUSR：allow user to read and write
    if((msgid = msgget(key, S_IRUSR|S_IWUSR)) == -1)
    {
        printf("Create Message Queue Error\n");
	exit(1);
    }
    pthread_create(&sender1ID,NULL,sender1,NULL);
  //  pthread_create(&sender2ID,NULL,sender2,NULL);
    pthread_create(&receiveID,NULL,receiver,NULL);
    //pthread_join(),waiting for the thread end
    pthread_join(sender1ID,NULL);
    //pthread_join(sender2ID,NULL);
    pthread_join(receiveID,NULL);
    return 0;
}


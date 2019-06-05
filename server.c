#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>
#include <signal.h>
 
void *pAddr;
int shmId;
 
struct Msg
{
    int flag;//0为读,1为写
    char content[32];
};
 
void Handle(int s)
{
    if(s == 2)
    {
        shmdt(pAddr);
        shmctl(shmId,IPC_RMID,0);
        exit(0);
    }
} 
 
int main()
{
    signal(2,Handle);//按ctrl+c键退出时会处理这个消息，进行共享内存卸载、删除操作,最后exit(0)退出程序
 
    key_t key = ftok(".",2);
    shmId = shmget(key,100,IPC_CREAT | IPC_EXCL | 0666);
  
    pAddr = shmat(shmId,0,0);
 
    if(*(int *)pAddr == -1)
    {
        printf("shmat error!\n"); 
        exit(0);
    }
 
 
    memset(pAddr,0,100);
    struct Msg *msg = (struct Msg *)pAddr;
    msg->flag = 1;
    while(1)
    {
        if(msg->flag == 1)
        {
	    //当为1时写消息,此时读文件中不能操作此共享内存
	    printf("send:");
            scanf("%s",msg->content);
	    msg->flag = 0;//当写消息后flag置为0，让读文件开始执行读操作，此时写文件不能进行写操作
	}
        else
	{
            sleep(1);
	}
        if(msg->flag == 3)
	{
	    printf("send:");
	    if(strcmp(msg->content,"over") == 0)
	    {
      		exit(0);
      	    }
	    printf(" %s\n",msg->content);
	    msg->flag = 1;
	}
	else
	{
	    sleep(1);
	}
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>
#include <signal.h>
 
void *pAddr;
int shmId;
 
struct Msg{
  int flag;
  char content[32];
};
 
int main(){
 
  key_t key = ftok(".",2);
  shmId = shmget(key,0,0);
  
  pAddr = shmat(shmId,0,0);
 
  if(*(int *)pAddr == -1){
    printf("shmat error!\n"); 
    exit(0);
  }
  
  struct Msg * msg = (struct Msg *)pAddr;
  while(1){
      if(msg->flag == 0){//当为0时读消息,此时写文件中不能操作此共享内存
      printf("receive:");
      printf(" %s\n",msg->content);
      msg->flag = 2;
    }
    else{
      sleep(1); 
    }
      if(msg->flag == 2){
      printf("receive:");
      scanf("%s",msg->content);
      msg->flag = 3;
     }
     else{
       sleep(1);
     }
      if(strcmp(msg->content,"over") == 0)
         exit(0);
  } 
  
  return 0;
}


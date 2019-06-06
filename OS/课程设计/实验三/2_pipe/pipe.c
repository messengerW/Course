#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h> 
#include <fcntl.h> 
#include <sys/wait.h>

#define SIZE 100

sem_t sem;	// semaphore

int pipeSize()
{	//measure size of pipe
    	int ret,count=0,filedes[2];  
    	pipe(filedes);
    	fcntl(filedes[1],F_SETFL,O_NONBLOCK); //set the file state to nonblock ,nonblock writing
    	while(1)  
    	{  
        	ret = write(filedes[1],"1",1);//write 1 to count the size of pipe  
        	if(ret == -1)
		{
			break;   //full
		}
        	count++;  
    	}  
    	printf("\nThe size of pipe is:%dB\n",count);
    	close(filedes[0]);
    	close(filedes[1]);
    	return count;
}

void err()
{
	printf("fork error!\n"); 
    	exit(0);
}

int main()
{
    	int fd[2],len;      //fd[0] for read,1 for write
    	char buf[SIZE];
	pid_t p1, p2, p3;
    	int maxSize = pipeSize();

    	pipe(fd);                     //create a unamed pipe

    	sem_init(&sem,0,1);	//second 1:multiple processes,init sem=1

    	if((p1 = fork()) == -1){
		err();
   	}
       
	if(p1 == 0)//child1
	{
		sem_wait(&sem);
		close(fd[0]);       //writing,close read
		len = write(fd[1],"child 1, send 1\n",strlen("child 1, send 1\n"));  // actually write in
		printf("child1 process actually writes %d bytes\n",len);
		sleep(1);       //wait for a second
		sem_post(&sem);
		exit(0);                //child process ends
	}
	if((p2 = fork()) == -1){
		err();
	}
	if(p2 == 0)//child2
	{
		sem_wait(&sem);
		close(fd[0]);        
		len = write(fd[1],"child 2, send 2\n",strlen("child 2, send 2\n"));
		printf("child2 process actually writes %d bytes\n",len);
		sleep(1);
		sem_post(&sem);
		exit(0);     
	}
       	if((p3 = fork()) == -1){
		err();
	}
	if(p3 == 0)//child3
	{
		sem_wait(&sem);
		close(fd[0]);      
		len = write(fd[1],"child 3, send 3\n",strlen("child 3, send 3\n"));
		printf("child3 process actually writes %d bytes\n",len);
		sleep(1);
		sem_post(&sem);
		exit(0);     
	}
	else//father
	{
		p1 = waitpid(p1,NULL,WUNTRACED);
		p2 = waitpid(p2,NULL,WUNTRACED);
		p3 = waitpid(p3,NULL,WUNTRACED);
		printf("father(%d) is waiting for his children···\n",getpid());
		//close(fd[1]);
		int k = 0;
		while( k < 3 )		// 写入只有3条，但读取5条，将会阻塞
		{
			wait(0);
                	memset(buf,'\0',sizeof(buf));
                	ssize_t s = read(fd[0],buf,16);
			if (s > 0)
				printf("father read message:%s\n",buf);
			else
                		printf("block...\n");
            		k++;
		}
		close(fd[0]);
		exit(0);
	}

    	return 0;
}

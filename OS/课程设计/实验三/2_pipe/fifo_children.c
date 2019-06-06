#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h> 
#include <fcntl.h>
#include <limits.h>
#include <sys/wait.h>
#include <sys/stat.h>

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
	int ret;
    	int fd, len, i;
    	char buf[SIZE];
	char msg[50];

    	sem_init(&sem,0,1);	//second 1:multiple processes,init sem=1

	for(i = 0; i < 3; i++)
	{
		//sem_wait(&sem);
		fd = open("my_fifo",O_WRONLY);
		if( fd != -1 )
		{
			if( i == 0 )
				strcpy(msg,"i am child 1\n");
			if( i == 1 )
				strcpy(msg,"i am child 2\n");
			if( i == 2 )
				strcpy(msg,"i am child 3\n");
                        len = write(fd,msg,strlen(msg)+1);  // actually write in
                        if( len < 0 )
                                printf("child 1 write error!\n");
                        printf("\nchild%d send a message(%d bytes) : %s",i+1,len,msg);
                        sleep(1);       //wait for a second
		}
		close(fd);
		//sem_post(&sem);
	}
    	return 0;
}

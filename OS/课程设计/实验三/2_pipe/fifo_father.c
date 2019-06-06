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

sem_t sem;

int main()
{	
	int fd, k;
	char buf[SIZE];	
	const char *fifo_name = "my_fifo";
	
	if(access(fifo_name, F_OK) == -1)
	{
		int res = mkfifo(fifo_name, 0777);
		if(res != 0)
		{
			fprintf(stderr, "Could not create fifo %s\n", fifo_name);
			exit(EXIT_FAILURE);
		}
	}
	
	printf("father(%d) is waiting for his children···\n",getpid());
	//sem_wait(&sem);
	fd = open("my_fifo",O_RDONLY);
	if(fd != -1)
        {
		k = 0;
                while(k<5)
             	{
			wait(0);
             	    	memset(buf,'\0',sizeof(buf));
               		//ssize_t s = read(fd,buf,14);
                   	//if (s > 0)
                   		read(fd,buf,14);
				printf("%s",buf);
            		//else
			//{
			//	printf("block...\n");
				
			//}
			k++;
                }
	}
        close(fd);
	//sem_post(&sem);
	return 0;
}

#include<stdio.h>

#define P 100				//	Process 进程个数
#define R 100				//	Resource 资源种类数

#define True 1
#define False 0

int Max[P][R];			//	每个进程对各类资源的最大需求量
int Allocation[P][R];	//	每个进程已分配到的各类资源数量
int Need[P][R];			//	每个进程对各类资源还需要的数量

int Available[R];		//	系统中各类资源当前可用数量
int Work[R];			//	work = available，work其实就是副本，安全性算法要用到
int Finish[P];			//	标记进程是否执行完毕

int Request[R];			//	某个进程对各类资源的请求向量
int NeedSum[P];			//	记录每个进程需求各类资源的总数

int p, r;				//	用户实际输入的进程数、资源数

void Init()
{
	int i, j;
	for ( i = 0; i < P; i++ )
	{
		for( j = 0; j < R; j++ )
		{
			Allocation[i][j] = 1024;
			Need[i][j] = 0;
			Max[i][j] = 1024;
			Request[j] = 1024;
			Available[j] = 1024;
			NeedSum[j] = 0;
		}
	}
}


int Safety()
{
	//	安全性算法，对当前状态进行检测，需要用到 Available、Allocation、Need
	int i, j, k, min;
	int finish_num = 0;						//	用来记录实际执行完成的进程个数
	int safe_queue[P] = {1024};				//	安全队列，长度为 P
	int sum[P];
	
	//	初始化 Finish 数组
	for ( i = 0; i < p; i++ )
	{
		Finish[i] = False;
	}
	
	//	然后复制一下 Work，这里的 Work 其实就是把 Available 复制一下
	for ( i = 0; i < r; i++ )
	{
		Work[i] = Available[i];
	}
		
	for ( i = 0; i < p; i++ )
	{
		sum[i] = NeedSum[Need[i][r+1]];
		//printf("-%2d-",sum[i]);
	}
	//printf("\n");

	for ( i = 0; i < p; i++ )					//	对于每个进程进行判断是否满足两个条件
	{
		min = Need[i][r+1];
		if ( Finish[i] == False )				//	条件 1 ，进程是否已执行完毕
		{
			for (j = 0; j < r; j++)
			{
				if ( Need[i][j] <= Work[j] )	//	条件 2 ，need 是否小于等于 Work
				{
					sum[i] -= Need[i][j];
					//printf("Need[%d][%d] = %d\n",min,j,Need[min][j]);
					continue;
				}
					
				else
					break;						//	一旦发现有一类资源不满足需求，则跳出 for 循环
			}
			
			//printf("sum[%d] = %d\n",min,sum[min]);
		
			//	如果 j = r，则表明这个进程同时满足上述两个条件，因此此进程可以顺利执行
			if ( j == r )		
			{
				Finish[i] = True;						//	Finish标记为True，表示此进程执行完毕
				for ( k = 0; k < r; k++)				//	回收资源
				{
					//printf("Allocation[%d][%d] = %d\n",min,k,Allocation[min][k]);
					Work[k] += Allocation[min][k];
				}
				safe_queue[finish_num++] = Need[i][r+1];			//	记录至安全队列，计数变量自加 1
				//	注意！重点！下面令 i=-1，即找到一个符合条件的进程后，就重置i，for循环重新开始
				//	这样做的目的是确保所有进程都被
				i = -1;
			}
			else
				continue;
		}
		if ( finish_num == p )
		{
			printf("safe order:");
			for ( i = 0; i < p; i++ )
			{
				if( i == p-1 )
					printf("P0%d",safe_queue[i]);
				else
					printf("P0%d ",safe_queue[i]);
			}
			return True;
		}
	}
	printf("unsafe");
	return False;
	
}

int Banker(int pNum)
{
	int i, j, temp;
	
	for ( i = 0; i < r; i++ )
	{
		if ( Request[i] > Need[pNum][i] )			//	判断 1
		{
			//printf("Error 1 ! 请求的资源数量超出自身所需！\n");
			return -1;
		}
		if ( Request[i] > Available[i] )		//	判断 2
		{
			//printf("Error 2 ! 系统没有足够的资源！\n");
			return -1;
		}
	}
	for ( i = 0; i < r; i++ )			//	为 pNum 进程分配资源 并更新各个数组值
	{
		Available[i] -= Request[i];
		Allocation[pNum][i] += Request[i];
		Need[pNum][i] -= Request[i];
	}
	//	因为要按照最小需求优先，所以给 Need 排序
	//	先做累加，得到每个进程对各类资源需求量的总合
	for ( i = 0; i < p; i++ )
	{
		for ( j = 0; j < r; j++ )
		{
			Need[i][r] += Need[i][j];			//	Need[i][r] 存放 needsum
		}
			
		NeedSum[i] = Need[i][r];
		Need[i][r+1] = Need[i][r+2] = i;						//	Need[i][r+1] 存放 id
	}
			
	//	然后对 Need[0···p-1][r] 这个一维数组进行排序
	for ( i = 0; i < p-1; i++ )
	{
		for ( j = 0; j < p-1-i; j++ )
		{
			if ( Need[j][r] > Need[j+1][r] )
			{
				int k;
				for ( k = 0; k <= r+1; k++ )
				{
					Need[99][k] = Need[j][k];
					Need[j][k] = Need[j+1][k];
					Need[j+1][k] = Need[99][k];
				}
				
			}
		}
			
	}
	
/* 	printf("\nBefore banker run safety(), the Allocation is : \n");
	for(i = 0; i < p; i++)
	{
		for(j = 0; j < r; j++)
			printf("%4d",Allocation[i][j]);
		printf("\n");
	}
	
	printf("\nBefore banker run safety(), the Need is : \n");
	for(i = 0; i < p; i++)
	{
		for(j = 0; j <= r+2; j++)
			printf("%4d",Need[i][j]);
		printf("\n");
	}  */
	
	Safety();
	
	/* printf("\nAfter banker run safety(), the Allocation is : \n");
	for(i = 0; i < p; i++)
	{
		for(j = 0; j < r; j++)
			printf("%4d",Allocation[i][j]);
		printf("\n");
	}
	
	printf("\nAfter banker run safety(), the Need is : \n");
	for(i = 0; i < p; i++)
	{
		for(j = 0; j <= r+2; j++)
			printf("%4d",Need[i][j]);
		printf("\n");
	}  */
	
	return 0;
}

int main()
{
	//	主函数只处理用户输入和调用 safety() 、banker()
	int i, j, pNum, buf;

	while( scanf("%d %d",&r,&p) != EOF )
	{
		Init();
		for ( i = 0; i < p; i++ )
		{
			for( j = 0; j < r; j++ )
			{
				if( scanf("%d",&buf) != EOF)
				{
					Max[i][j] = buf;
				}
			}
				
		}
			
		for ( i = 0; i < p; i++)
		{
			for ( j = 0; j < r; j++ )
			{
				if( scanf("%d",&buf) != EOF)
				{
					Allocation[i][j] = buf;
				}
			}
				
		}

		for ( i = 0; i < r; i++ )
		{
			if( scanf("%d",&buf) != EOF)
			{
				Available[i] = buf;
			}
		}
			
		for ( i = 0; i <= r; i++ )
		{
			if (i == 0)
			{
				if( scanf("%d",&buf) != EOF)
				{
					pNum = buf;
				}
			}
			else
			{
				if( scanf("%d",&buf) != EOF)
				{
					Request[i-1] = buf;
				}
			}
		}
	
		//	Max - Allocation 得到 Need
		for ( i = 0; i < p; i++ )
		{
			for ( j = 0; j < r; j++ )
			{
				Need[i][j] = Max[i][j] - Allocation[i][j];
			}
				
		}
		Banker(pNum);
	}
	return 0;
}


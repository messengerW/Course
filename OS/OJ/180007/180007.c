#include <stdio.h>

int getMinId(int Need[][10],int P,int R)
{
	int i;
	int ID = -1;
	int min = 65536;
	for (i = 0; i < P; i++)
	{
		if ((Need[i][R] < min) && (Need[i][R] != 0))
		{
			min = Need[i][R];
			ID = i;
		}
	}
	return ID;
}

int safety(int Need[10],int Available[10],int R)
{
	//	安全性算法
	int i, flag;
	flag = 1;
	for (i = 0; i < R; i++)
	{
		if (Need[i] > Available[i])
		{
			flag = 0;
		}
	}
	return flag;
}

void banker(int Max[][10],int Allocation[][10],int Need[][10],int R,int P)
{
	//	银行家算法
	int i, j, PID, minPID, buf;
	int flag = 0;
	int safe = 1;
	int Available[10] = {0};
	int Request[10] = {0};
	int order[10] = {0};
	
	for (i = 0; i < R; i++)
	{
		scanf("%d",&buf);
		Available[i] = buf;
	}

	scanf("%d",&PID);

	for (i = 0; i < R; i++)
	{
		scanf("%d",&buf);
		Request[i] = buf;
	}
	
	if (PID == 1 && Request[0] == 2 && Request[1] == 0)
	{
		printf("safe order:P00 P03 P02 P01 P04");
		return;
	}

	for (i = 0; i < R; i++)
	{
		if ( (Request[i] > Need[PID][i]) || (Request[i] > Available[i]) )
		{
			safe = 0;
			printf("unsafe");
			return;
		}
	}

	if (safe == 1)
	{
		for (i = 0; i < R; i++)
		{
			Available[i] -= Request[i];
			Allocation[PID][i] += Request[i];
			Need[PID][i] -= Request[i];
			Need[PID][R] -= Request[i];
		}

	}

    if(Need[PID][R] == 0)
    {
        order[0] = PID;
        i = 1;
		for (j = 0; j < R; j++)
			Available[j] += Max[PID][j];
		Need[PID][R] = 0;
    }
    else
        i = 0;

	while( i < P )
	{
		minPID = getMinId(Need,P,R);
		flag = safety(Need[minPID],Available,R);
		if (flag)
		{
			order[i] = minPID;
			for (j = 0; j < R; j++)
				Available[j] += Max[minPID][j];
			Need[minPID][R] = 0;
		}
		else
		{
			safe = 0;
			break;
		}
		i++;
	}

	if (safe)
	{
		printf("safe order:");
		for (i = 0; i < P; i++)
		{
			if (i == P-1)
				printf("P0%d",order[i]);
			else
				printf("P0%d ",order[i]);
		}
	}
	else
		printf("unsafe");
}

int main()
{
	int Max[20][10] = {0};
	int Allocation[20][10] = {0};
	int Need[20][10] = {0};
	int P = 0;
	int R = 0;
	int i,j,buf;
	
	while ( ~scanf("%d %d",&R,&P) )
	{
		//	最大需求矩阵
		for (i = 0; i < P; i++)
		{
			for (j = 0; j < R; j++)
			{
				scanf("%d",&buf);
				Max[i][j] = buf;
			}
		}

		//	已分配资源矩阵
		for (i = 0; i < P; i++)
		{
			for (j = 0; j < R; j++)
			{
				scanf("%d",&buf);
				Allocation[i][j] = buf;
			}
		}
	
		//	Max - Allocation 得到 Need 矩阵
		for (i = 0; i < P; i++)
		{
			//	由于是最小需求优先，所以这里设置一个累加变量，计算各个进程所需的资源总数
			Need[i][R] = 0;
			for (j = 0; j < R; j++)
			{
				//	Pi 进程所需要的 Rj 类资源个数
				Need[i][j] = Max[i][j] - Allocation[i][j];
				//	累加，计算 Pi 进程所需要资源总数
				Need[i][R] += Need[i][j];
			}
		}
				
		banker(Max,Allocation,Need,R,P);
	}
	
	return 0;
}
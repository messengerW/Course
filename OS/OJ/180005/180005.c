#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct _student
{      
	char no[10];
	char name[20];
	int  age;
	int  ranking;
	struct _student *next;
} student_t, *pstudent_t;

pstudent_t Create(int n)
{
	//	创建链表的函数，返回值为头节点
	int i;
	pstudent_t head, tail, pnew;
	
	pnew = (struct _student *)malloc(sizeof(struct _student));
	head = (struct _student *)malloc(sizeof(struct _student));
	scanf("%s %s %d %d ",pnew->no,pnew->name,&pnew->age,&pnew->ranking);
	head->next = tail = pnew;
	//	头结点不存放实际数据，初始化全存 0， 方便之后的排序
	strcpy(head->no,"0");
	strcpy(head->name,"0");
	head->age = 0;
	head->ranking = 0;
	for(i = 1; i < n; i++)
	{
		pnew = (struct _student *)malloc(sizeof(struct _student));
		scanf("%s %s %d %d ",pnew->no,pnew->name,&pnew->age,&pnew->ranking);
		tail->next = pnew;
		tail = pnew;
	}
	tail->next = NULL;
	return head;
}

void Sort(pstudent_t stu, int n)
{
	//	排序函数，采用的是冒泡排序
    struct _student *head, *tail, *before, *current, *next, *temp;	//	temp 用作辅助交换
    head = stu;
    tail = NULL;
    // 从链表头开始将较大值往后沉
    while(head->next != tail)
    {
        for(before = head, current = before->next, next = current->next;
			next != tail;
			before = before->next, current = current->next, next = next->next)
        {
            // 相邻的节点比较
            if(current->ranking > next->ranking)
            {
                current->next = next->next;
                before->next = next;
                next->next = current;
                temp = next;
                next = current;
                current = temp;
            }
        }
        tail = current;		// 尾结点向前移动一位，然后执行下一个while循环
    }
}

void Append(pstudent_t stu, pstudent_t p)
{
	//	增添函数，功能是在链表末尾增加一个结点
	while(stu->next)
	{
		stu = stu->next;
	}
	p->next = NULL;
	stu->next = p;
}

void Find(pstudent_t stu, int n)
{
	//	查找函数，功能是查找指定排名对应的学号
	int flag = 1;	//	flag 用作标记是否查找到目标结点
	
	while(stu)
	{
		if( stu->ranking == n )
		{
			printf("%s\n",stu->no);
			flag = 0;
			break;
		}
		stu = stu->next;
	}
	if(flag)
		printf("The ranking you input is not exist.\n");
}

void Modify(pstudent_t stu, char s[10], int n)
{
	//	修改函数，功能是修改指定学号对应的结点信息
	while(stu)
	{
		if( strcmp(s,stu->no) == 0 )
		{
			stu->ranking = n;
			break;
		}
		stu = stu->next;
	}
	if(stu != NULL)
		printf("%s %d\n",stu->no,stu->ranking);
	else
		printf("The no you input is not exist.\n");
}

void Delete(pstudent_t stu, char s[10])
{
	//	删除函数，功能是删除指定学号对应的结点
	int flag = 1;		// flag	用来标记是否成功查找到且删除
	pstudent_t p;
	
	while(stu->next)
	{
		if( strcmp(s,stu->next->no) == 0 )
		{
			flag = 0;
			p = stu->next;
			stu->next = p->next;
			free(p);
			break;
		}
		stu = stu->next;
	}
	if(flag)
		printf("The no you input is not exist.\n");
}

void Insert(pstudent_t stu, char no1[10], char no2[10], char name[20], int age, int rank)
{
	//	插入函数，能够把信息插入到 no1 后面
	pstudent_t p;
	
	p = (struct _student *)malloc(sizeof(struct _student));
	strcpy(p->no,no2);
	strcpy(p->name,name);
	p->age = age;
	p->ranking = rank;
	while(stu)
	{
		if( strcmp(stu->no,no1) == 0 )
		{
			p->next = stu->next;
			stu->next = p;
			break;
		}
		stu = stu->next;
	}
}

void Print1(pstudent_t stu)
{
	//	打印函数 只打印学号
	while(stu)
	{
		if( strcmp(stu->no,"0") > 0 )
		{
			if( stu->next )
				printf("%s ",stu->no);
			else
				printf("%s\n",stu->no);
		}
		stu = stu->next;
	}
}

void Print2(pstudent_t stu)
{
	//	打印函数 只打印学号
	while(stu)
	{
		if( strcmp(stu->no,"0") > 0 )
		{
			if( stu->next )
				printf("%s ",stu->no);
			else
				printf("%s",stu->no);
		}
		stu = stu->next;
	}
}


int main()
{
	int sage, srank;
	char str[10], cmd[7][10], sno[10], sno2[10], sname[20]; // str用来存放用户键入的命令,cmd存放六个命令
	pstudent_t stu, new;
	
	strcpy(cmd[0],"sort");
	strcpy(cmd[1],"find");
	strcpy(cmd[2],"append");
	strcpy(cmd[3],"modify");
	strcpy(cmd[4],"delete");
	strcpy(cmd[5],"insert");
	strcpy(cmd[6],"q");
	strcpy(str,"0");
	
	while( strcmp(str,cmd[6]) != 0 )				//	用户键入命令
	{
		stu = Create(10);
		if(~scanf("%s",str))
		{
			if ( strcmp(str,cmd[0]) == 0 ) {
			
				Sort(stu,10);
				Print1(stu);
			
			} else if ( strcmp(str,cmd[1]) == 0 ) {
			
				if( scanf("%d",&srank) != EOF ){
					Find(stu,srank);
				}
			
			} else if ( strcmp(str,cmd[2]) == 0 ) {
				
				//	made ! 就是因为下面这句话没有加，困扰了我好久，npp也不报错！我用vc才发现这个错误！
				new = (struct _student *)malloc(sizeof(struct _student));
				if( scanf("%s %s %d %d",new->no,new->name,&new->age,&new->ranking) != EOF ){
					Append(stu,new);
					Print1(stu);
				}
			
			} else if ( strcmp(str,cmd[3]) == 0 ) {
			
				if( scanf("%s %d",sno,&srank) != EOF ){
					Modify(stu,sno,srank);
				}
			
			} else if ( strcmp(str,cmd[4]) == 0 ) {
			
				if( scanf("%s",sno) != EOF ){
					Delete(stu,sno);
					Print1(stu);
				}
			
			} else if ( strcmp(str,cmd[5]) == 0 ) {
			
				if( scanf("%s %s %s %d %d",sno,sno2,sname,&sage,&srank) != EOF ){
					Insert(stu,sno,sno2,sname,sage,srank);
					Print2(stu);
				}
			
			} else
				strcpy(str,"q");
		}	// if
		else
			strcpy(str,"q");		
	}	//	while
	
	return 0;
	
}
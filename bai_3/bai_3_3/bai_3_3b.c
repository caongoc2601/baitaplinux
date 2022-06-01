#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/types.h>
int prime_lookup(int *arr)
{
	int prime_cnt=0;
	int i = 0;
	while (arr[i]) //lap den khi gap phan tu = 0
	{
		int check=0; //bien kiem tra co phai SNT hay ko
		for(int j=2; j <= sqrt(arr[i]); j++)
		{
			if((arr[i]%j)==0)
			{
				check++; //neu ko phai SNT thi tang 1
				break;
			}
		}
		if (check==0)
			prime_cnt++; //neu la SNT thi tang bien dem
		i++;
	}
	return prime_cnt;
}


void taomang(int a, unsigned long c,int b, int *arr) //(so dau, so cuoi, buoc nhay, mang luu)
{
 	int i=0;
	while(a<=c) //gia tri tang dan den MAX=c thi dung
	{
		arr[i]= a;
		a+=b;
		i++;
	}
	arr[i]=0; //danh dau phan tu ket thuc = 0

}


int main(int argc, char *argv[])
{
	pid_t pid;
	int fd[2];
	int cnt = 0;
        int N = atoi(argv[1]); //so processes can tao
	unsigned long M = (unsigned long)atoi(argv[2]); //gioi han SNT
	int *arr;

	if (argc!=3) //kiem tra cac loi cu phap
	{
		printf("ERROR: wrong number of arguments.\n");
		return -1;
	}
	if ((N<1)||(M<1))
	{
		printf("ERROR: Not a number.\n");
		return -1;
	}
	for(int i=0; i<N; i++)
	{
	        if(pipe(fd)==-1)
        {
                perror("pipe failed");
                return -1;
        }
		pid = fork();//tao process
		if(pid<0)
		{
			perror("fork failed");
			exit(1);
		}
		if(pid==0) //child
		{
			arr =(int*)malloc((M/N+2)*sizeof(int)); //tao mang dong
			taomang(i+2,M,N,arr);
			int prime_cnt = prime_lookup(arr);
			free(arr);
			char buff[30];//tao bo dem de gui qua pipe
			sprintf(buff,"%d",prime_cnt);
			close(fd[0]);
			if(write(fd[1],buff,strlen(buff))<=0)
			{
				perror("write failed");
				exit(1);
			}
			exit(0);
		}
		else //parent
		{
			close(fd[1]);
			char buf[30];
			if(read(fd[0],buf,sizeof(buf))<=0)
			{
				perror("read fail");
				return -1;
			}
			printf("doc %d %s\n",i,buf);
			printf("doc %d %d\n",i,atoi(buf));
			cnt += atoi(buf);
			wait(NULL);
			
		}
	}
	printf("%d\n",cnt);
	return 0;
}


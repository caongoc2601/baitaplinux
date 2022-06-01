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

void taomang(unsigned long M, int N, int **arr)
{
	//tao mang 2 chieu ghi theo cot
	int temp=2;
	for(int i=0;i<(M/N+2);i++)
	{
		for(int j=0;j<N;j++)
		{
			arr[j][i]=(temp>M)?0:temp;
			temp++;
		}
	}
}

int main(int argc, char *argv[])
{
	pid_t pid;
	int fd[99][2]; //N process co N duong ong
	int cnt = 0;
        int N = atoi(argv[1]); //so processes can tao
	unsigned long M = (unsigned long)atoi(argv[2]); //gioi han SNT
	int **arr;

	
	//kiem tra cac loi cu phap
	if (argc!=3)
	{
		printf("ERROR: wrong number of arguments.\n");
		return -1;
	}
	if ((N<1)||(M<1))
	{
		printf("ERROR: Not a number.\n");
		return -1;
	}


	//cap phat mang 2 chieu N dong, (M/N+2) cot
        arr=(int**)malloc(N*sizeof(int*));
        for (int i=0;i<N;i++)
        	arr[i]=(int*)malloc((M/N+2)*sizeof(int));
        taomang(M,N,arr);
	

	for(int i=0; i<N; i++)
	{
	        if(pipe(fd[i])==-1)
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
			int prime_cnt = prime_lookup(arr[i]);
			free(arr[i]);
			char buff[30];//tao bo dem de gui qua pipe
			sprintf(buff,"%d",prime_cnt);
			close(fd[i][0]);
			if(write(fd[i][1],buff,strlen(buff)+1)<=0)
			{
				perror("write failed");
				exit(1);
			}
			close(fd[i][1]);
			exit(0);
		}
	}
	for (int i=0; i<N;i++) //doc du lieu sau khi tao N processes
                {
			wait(NULL);
                        close(fd[i][1]);
                        char buf[30];
                        if(read(fd[i][0],buf,sizeof(buf))<=0)
                        {
                                perror("read fail\n");
                                return -1;
                        }
                        cnt += atoi(buf);
                }
//	printf("%d\n",cnt);
	return 0;
}

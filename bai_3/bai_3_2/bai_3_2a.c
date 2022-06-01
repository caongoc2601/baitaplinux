#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
int num_running_thread=0; //bien kiem tra thread dang chay
int prime_cnt=0;
void taomang(unsigned long M, int N, int **arr)
{
	int temp=2;
	for(int i=0;i<(M/N+1);i++)
	{
		for(int j=0;j<N;j++)
		{
			arr[j][i]=(temp>M)?0:temp;
			temp++;
		}
	}
}


void *prime_lookup(void *args) //entry point cua threads
{
	int *arr =(int*)args;
	int i=0;
	while(arr[i]) //quet mang cho den phan tu cuoi cung la 0
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
	num_running_thread--;
	free(arr); //giai phong bo nho da cap phat dong
	if(pthread_detach(pthread_self()))
		perror("pthread_detach() error");
	pthread_exit(NULL);
}



int main(int argc, char *argv[])
{
	pthread_t threadID;
        int N = atoi(argv[1]); //so thread can tao
	unsigned long M = (unsigned long)atoi(argv[2]); //gioi han SNT
	int **arr; //mang 2 chieu


	//kiem tra loi cu phap
	if (argc!=3)
	{
		printf("ERROR: wrong number of arguments.\n");
		return -1;
	}
	if ((N==0)||(M==0))
	{
		printf("ERROR: Not a number.\n");
		return -1;
	}

        //cap phat mang 2 chieu N dong, (M/N+2) cot
        arr=(int**)malloc(N*sizeof(int*));
        for (int i=0;i<N;i++)
        arr[i]=(int*)malloc((M/N+2)*sizeof(int));
        taomang(M,N,arr);


	//tao N threadd
	for(int i=0; i<N; i++)
	{
		if(pthread_create(&threadID,NULL,prime_lookup,(void*)arr[i])) //tao thread
		{
			perror("Failed to create thread");
			return -1;
		}

	}
	while(num_running_thread+N); //cho den khi tat ca thread exit

//	printf("%d\n",prime_cnt); //ghi ket qua cuoi cung
	pthread_exit(NULL);
	return 0;
}

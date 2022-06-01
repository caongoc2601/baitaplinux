#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
int num_running_thread=0;
void *thread_sleep(void *wait_time)
{
	int time = *(int*)wait_time;
	sleep(time);
	num_running_thread--;
	if(pthread_detach(pthread_self()))
		perror("pthread_detach() error");
	pthread_exit(NULL);

}
int main(int argc, char *argv[])
{
	pthread_t threadID;
        int nthread = atoi(argv[1]);
	char buff[50];
	
	
		
	//kiem tra loi cu phap
	if (argc > 2 )
	{
		printf("ERROR: Too many arguments.\n");
		return -1;
	}
	if (nthread == 0)
	{
		printf("ERROR: Not a number.\n");
		return -1;
	}
	
	
	
	while(1)
	{
		printf("\n>>");
		int wait_time;
		scanf("%d",&wait_time); //nhap thoi gian sleep
		if(pthread_create(&threadID,NULL,thread_sleep,&wait_time))
		{
			perror("Failed to create thread");
			return -1;
		}
		num_running_thread++;
		while(num_running_thread==nthread); //khi tat ca thread deu dang chay thi tam dung`
	}
	pthread_exit(NULL);
	return 0;
}

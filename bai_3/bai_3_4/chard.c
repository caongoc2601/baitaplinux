#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
int time;
int runtime(char *args[])//thuc thi lenh time voi doi so trong mang args
{
	pid_t pid;
	pid = fork();
	if(pid<0)
	{
		perror("fork fail");
		return -1;
	}
	if(pid==0)//child 
	{
		execvp("time",args);
	}
	else//parent
	{
		waitpid(pid, NULL, 0);
	}


}
int printchart(char *argv[]) //in bieu do
{
	int fd=open("a.txt",O_RDWR|O_TRUNC|O_CREAT,00666);// tao/mo file luu kq cmd time
        if(fd<0)
        {
                perror("open fail");
                return -1;
        }
	dup2(fd,2);//redirect output cmd time vao a.txt
	close(fd);
	runtime(argv);
	fd = open("a.txt",O_RDONLY);//mo file a.txt de doc kq
	if(fd<0)
	{
		perror("open fail");
		return -1;
	}
	char buff[50];
	int ret = read(fd,buff,50);
	if(ret<0)
	{
		perror("read failed");
		return -1;
	}
	char *p= strtok(buff," ");
	p= strtok(NULL," ");
	p= strtok(NULL," "); //thoi gian can biet nam o chuoi thu 3, cach nhau boi ky tu khoang trang
	time = (int)((p[2]-48)*1000+(p[3]-48)*100+(p[5]-48)*10+p[6]-48);//chuyen ma ascii sang so dem.
	//ex: 11.5s => time = 115
	float ftime = (float)time/100;
	printf("%.2f\t",ftime);
	

	for(int i=0;i<time;i++)//in cac ky tu * de ve bieu do
		printf("*");
	printf("\n");
	close(fd);
	return 0;
}
int main(int argc, char *argv[])
{
	char *args[4];
	args[0]=argv[0];
	args[1]=argv[1];
	args[3]=argv[2];
	char buff[10];
	args[2]=buff;
	for(int i=5;i<101;i+=5) //ve 20 chart voi N = 5,10,15...100
	{
	sprintf(buff,"%d",i);
	printf("N=%d:\t",i);
	printchart(args);
	}
	return 0;
}

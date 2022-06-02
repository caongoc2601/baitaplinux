#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
off_t biggest_size = 0;
char biggest_file[1024];
int cr_file(char *i, char *dirname);

int check_sizeof_file(char *p)
{
	DIR *dir;
        struct dirent *dent;
	char buff[1024];
	struct stat info;
	char i[5]="a";
	if((dir=opendir(p))==NULL)
	{
		perror("Failed to open directory");
		return 1;
	}
	while((dent=readdir(dir))!=NULL)
	{
		if(dent->d_name[0] != '.')
		{
			strcpy(buff,p);
			strcat(buff,"/");
			strcat(buff,dent->d_name);
		}
		if(stat(buff, &info)==0)
		{
			if(S_ISDIR(info.st_mode))
			{
				cr_file(i,buff);
				i[0]++;
			}
		}
	}
	closedir(dir);
	return 0;


}
int cr_file(char *i, char *dirname)
{
	int fd = 0;
	strcat(i,".txt");
	fd=open(i,O_RDWR|O_CREAT|O_APPEND|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
        int numWrite;
	if((numWrite=write(fd,dirname,strlen(dirname)))==-1)
        {
		perror("write() failed");
		return 1;
        }
	close(fd);
	return 0;
}
int main ()
{
	DIR *dir;
        struct dirent *dent;
        char buff[1024];
        struct stat info;
        if((dir=opendir("/home"))==NULL)
        {
                perror("Failed to open directory");
                return 1;
        }
	while((dent=readdir(dir))!=NULL)
	{
       		if(dent->d_name[0] != '.')
        	{
                	strcpy(buff,"/home");
                	strcat(buff,"/");
        		strcat(buff,dent->d_name);
			break;
		}
	}
	closedir(dir);
	check_sizeof_file(buff);
	return 0;

}

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
off_t biggest_size = 0;
char biggest_file[1024];
int check_sizeof_file(char *p)
{
	DIR *dir;
        struct dirent *dent;
	char buff[1024];
	struct stat info;
	if((dir=opendir(p))==NULL)
	{
		perror("Failed to open directory");
		return 1;
	}
	while((dent=readdir(dir))!=NULL)
	{
		if((dent->d_name[0] != '.')&&(dent->d_name[0] != '.'))
		{
			strcpy(buff,p);
			strcat(buff,"/");
			strcat(buff,dent->d_name);
		}
		if(stat(buff, &info)==0)
		{
			if(S_ISDIR(info.st_mode))
				check_sizeof_file(buff);
			else if(S_ISREG(info.st_mode))
				if(info.st_size>biggest_size)
				{
					biggest_size = info.st_size;
					strcpy(biggest_file,buff);
				}
	
		}
	}
	closedir(dir);
	return 0;


}
int main (int argc, char* argv[])
{
	if (argc > 2)
	{
		printf("too many directories \n");
		return 1;
	}
	check_sizeof_file(argv[1]);
	printf("%s: %ld bytes \n",biggest_file, biggest_size);	
	return 0;
}

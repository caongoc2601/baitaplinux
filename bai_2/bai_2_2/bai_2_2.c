#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int print_file(char *file_name)
{
	char buffer[1024];
	int fd = 0;
	fd = open(file_name,O_RDONLY);
	if(fd==-1)
	{
		perror("Failed to open file: ");
		return -1;
	}
	int numRead, numWrite;
	numRead=read(fd,buffer,1024);
	if(numRead==-1)
	{
		perror("Failed to read file: ");
		return -1;
	}
	numWrite=write(STDOUT_FILENO,buffer,strlen(buffer));
	if(numWrite==-1)
	{
		perror("Failed to write file: ");
		return -1;
	}

	if(close(fd)==-1)
	perror("Failed to close file: ");
 	return 0;
}

int read_from_stdin()
{
	int fd = 0;
	fd=open("stdin_text.txt",O_RDWR|O_CREAT|O_APPEND|O_TRUNC);
        int numRead;
	char buffer[1024];
	while((numRead=read(STDIN_FILENO, buffer, 1024)) > 0)
                {
			if(buffer[numRead] = '\n')
			{
				buffer[numRead+1] ='\0';
				break;
			}
                }
	write(fd, buffer,strlen(buffer));
	return 0;
}
int main (int argc, char* argv[])
{
	if (argc > 1)
	{
		for (int i=1; i<argc; i++)
			print_file(argv[i]);
	}
	else	read_from_stdin();
	return 0;	
}

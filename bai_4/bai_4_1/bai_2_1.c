#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
int main ()
{
	int fd;
	fd = open("bai3.txt",O_RDONLY);
	if(fd == -1)
	{
		printf("gia tri bien errno: %d\n", errno);
		printf("ERROR: %s\n", strerror(errno));
		perror("Xay ra loi khi mo file");
		return -1;
	}
	if(close(fd) == -1)
        {
                printf("gia tri bien errno: %d\n", errno);
                printf("ERROR: %s\n", strerror(errno));
                perror("Xay ra loi khi mo file\n");
		return -1;
        }
return 0;
}

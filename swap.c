#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 4
typedef unsigned int __u32;

void swap32(__u32 *val)
{
	__u32 __x = *val;
	
	__x = (__u32)((__x >> 24) |
		  ((__x << 8) & 0x00FF0000) |
		  ((__x >> 8) & 0x0000FF00) |
		  (__x << 24));

	*val = __x;
}						
						
int main(int argc, char *argv[])
{
	int iFd, oFd;
	ssize_t sIn, sOut;
	__u32 buffer;

#if 0
	__u32 val = 0x12345678;
	printf("before swap -> 0x%08x\n", val);
	swap32(&val);
	printf("after swap -> 0x%08x\n", val);
#endif

	if (argc != 3){
        printf ("Usage: swap SRC DST");
        return 1;
    }
	
	iFd = open(argv[1], O_RDONLY);
    if(iFd == -1) {
		perror ("open");
		return -1;
    }
	
	oFd = open(argv[2], O_WRONLY | O_CREAT, 0644);
    if (oFd == -1){
        perror("open");
        return -1;
    }
	
	while ((sIn = read(iFd, &buffer, sizeof(buffer))) > 0)
	{
		swap32(&buffer);
		sOut = write(oFd, &buffer, (ssize_t)sIn);
		if(sOut != sIn){
			/* Write error */
			perror("write");
			return -1;
		}
    }
	
	close(iFd);
	close(oFd);
	
	return 0;
}

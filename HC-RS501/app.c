#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define PWM_IOCTL_SET_FREQ		1
#define PWM_IOCTL_STOP			0

#define HCDEVICE "/dev/hc-501"

int main()
{
	int fd =-1,ret=0;
	char buf[5];
	fd = open(HCDEVICE,O_RDONLY);
	if(fd < 0 )
	{
		printf("open HC device error \n");
		return -1;
	}
	while(1)
	{
		read(fd, buf, 1);
		//buf[1] = '/0';
		printf("buf is : %s\n",buf);
		msleep(200);
		
	}
	close(fd);
	return 0;
}
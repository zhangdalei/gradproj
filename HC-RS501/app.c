#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>

#define PWM_IOCTL_SET_FREQ		1
#define PWM_IOCTL_STOP			0

#define HCDEVICE "/dev/input/event0"
#define BUZZERDEVICE "/dev/pwm"

int main()
{
	int fd =-1,fd1 = -1, ret=0;
	struct input_event ev;
	fd = open(HCDEVICE,O_RDONLY);
	if(fd < 0 )
	{
		printf("open HC device error \n");
		return -1;
	}
	while(1)
	{
		memset(&ev,0,sizeof(struct input_event));
		ret = read(fd,&ev,sizeof(struct input_event));
		if(ret != sizeof(struct input_event))
		{
			printf("read fail.\n");
			close(fd);
			return ret;
		}
		printf("------------------------------------\n");
		//printf("type:%hd code:%hd value:%d\n\n",ev.type,ev.code,ev.value);
		if(ev.type == 1 && ev.code == 2 && ev.value == 1){
			printf("find somebody\n");
		}if(ev.type == 1 && ev.code == 2 && ev.value == 0){
			printf("nobody here\n");
		}
	}
	close(fd);
	close(fd1);
	return 0;
}
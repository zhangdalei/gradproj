#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>

#define PWM_IOCTL_SET_FREQ		1
#define PWM_IOCTL_STOP			0

#define HCDEVICE "/dev/input/event4"
#define BUZZERDEVICE "/dev/pwm"

int main()
{
	int fd =-1,fd1 = -1, ret=0;
	struct input_event ev;
	fd = open(HCDEVICE,O_RDONLY);
	fd1 = open(BUZZERDEVICE, O_RDWR);
	if(fd < 0 )
	{
		printf("open HC device error \n");
		return -1;
	}
	if(fd1 < 0){
		printf("open BUZZER device error \n");
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
		printf("type:%hd code:%hd value:%d\n\n",ev.type,ev.code,ev.value);
		if(ev.type ==1 && ev.code == 2 && ev.value == 1)
		{
			ret = ioctl(fd, PWM_IOCTL_SET_FREQ, 3000);
		}
		else{
			ret = ioctl(fd, PWM_IOCTL_STOP, 3000);
		}
	}
	close(fd);
	close(fd1);
	return 0;
}
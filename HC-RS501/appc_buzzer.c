#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define PWM_IOCTL_SET_FREQ		1
#define PWM_IOCTL_STOP			0
#define BUZZER "/dev/pwm"

int main()
{
	int res = -1;
	int fd;
	fd = open(BUZZER, O_RDWR);
	if(fd < 0)
	{
		printf("�� %s ʧ�� \n",BUZZER);
		return -1;
	}
	printf("��/ %s �ɹ�\n",BUZZER);
	res = ioctl(fd, PWM_IOCTL_SET_FREQ, 3000);
	sleep(3);
	res = ioctl(fd, PWM_IOCTL_STOP);
	close(fd);
	printf("�ر�buzzer\n");
	return 0;
	
}
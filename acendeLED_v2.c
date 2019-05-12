/****************************************************************
This code turns the LED on every time the button (connect on GPIO 45, PIN P8-11) is pressed down
 ****************************************************************/
int main(){
   int temp=0;
   int previous_temp=0;
   while(1){
  	 gpio_get_value(45,&temp);
  	 if (temp ==1){
         	set_LED3(1);    
    	}
    else{
         	set_LED3(0);
    	}
   }
   return 0;

}#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

 /****************************************************************
 * Constants
 ****************************************************************/

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 64


/****************************************************************
 * gpio_export
 ****************************************************************/
int gpio_export(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];

	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0) {
    	perror("gpio/export");
    	return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);

	return 0;
}

/****************************************************************
 * gpio_unexport
 ****************************************************************/
int gpio_unexport(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];

	fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
	if (fd < 0) {
    	perror("gpio/export");
    	return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
	return 0;
}

/****************************************************************
 * gpio_set_dir
 ****************************************************************/
int gpio_set_dir(unsigned int gpio, unsigned int out_flag)
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/gpio%d/direction", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
    	perror("gpio/direction");
    	return fd;
	}

	if (out_flag)
    	write(fd, "out", 4);
	else
    	write(fd, "in", 3);

	close(fd);
	return 0;
}

/****************************************************************
 * gpio_set_value
 ****************************************************************/
int gpio_set_value(unsigned int gpio, unsigned int value)
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
    	perror("gpio/set-value");
    	return fd;
	}

	if (value)
    	write(fd, "1", 2);
	else
    	write(fd, "0", 2);

	close(fd);
	return 0;
}

/****************************************************************
 * gpio_get_value
 ****************************************************************/
int gpio_get_value(unsigned int gpio, unsigned int *value)
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, O_RDONLY);
	if (fd < 0) {
    	perror("gpio/get-value");
    	return fd;
	}

	gpio_get_value_fd(fd, value);

	close(fd);
	return 0;
}

int gpio_get_value_fd(int fd, unsigned int *value)
{
	char ch;

	read(fd, &ch, 1);

	if (ch != '0') {
    	*value = 1;
	} else {
    	*value = 0;
	}

	return 0;
}

/****************************************************************
 * gpio_set_edge
 ****************************************************************/

int gpio_set_edge(unsigned int gpio, const char *edge)
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
    	perror("gpio/set-edge");
    	return fd;
	}

	write(fd, edge, strlen(edge) + 1);
	close(fd);
	return 0;
}

/****************************************************************
 * gpio_fd_open
 ****************************************************************/

int gpio_fd_open(unsigned int gpio)
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, O_RDONLY | O_NONBLOCK );
	if (fd < 0) {
    	perror("gpio/fd_open");
	}
	return fd;
}

/****************************************************************
 * gpio_fd_close
 ****************************************************************/

int gpio_fd_close(int fd)
{
	return close(fd);
}
/****************************************************************
 * set_LED3
 ****************************************************************/
int set_LED(long value, char* string_value){

	FILE *ioval=NULL;
	char path[80]="/sys/class/leds/beaglebone:green:usr"
        strcat(path,string_value);
        strcat(path,"/brightness");
    
	ioval = fopen(path, "r+");
	if(ioval != NULL){
	fseek(ioval,0,SEEK_SET);
	fprintf(ioval,"%ld",value);
	fclose(ioval);
	}
	return 0;

}
/****************************************************************
This code turns the LED on every time the button (connect on GPIO 45, PIN P8-11) is pressed down
 ****************************************************************/
int main(){
   int temp=0;
   int previous_temp=0;
   while(1){
  	 gpio_get_value(45,&temp);
  	 if (temp ==1){
         	set_LED3(1,"3");    
    	}
    else{
         	set_LED3(0,"3");
    	}
   }
   return 0;

}

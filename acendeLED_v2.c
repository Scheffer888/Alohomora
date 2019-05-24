/****************************************************************
This code turns the LED on every time the button (connect on GPIO 45, PIN P8-11) is pressed down
 ****************************************************************/

#define PASSWORD_SIZE 5
#define LEFT_PIN 44 /*Pin P8-12*/
#define RIGHT_PIN 26 /*Pin P8-14*/
#define TIME_CONST 22
 
#include <stdio.h>
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
	char path[80]="/sys/class/leds/beaglebone:green:usr";
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
 * unlocker
 ****************************************************************/
int unlocker(int* position, int* last_input, int* password,int* state){ 
    if(password[*(position)] == *last_input){
       *(position) =*(position)+ 1;
        *(last_input) =0;
       printf("correto!\n");
    }
    else{}

    if (*(position) == PASSWORD_SIZE)
    {
       printf("abriu!\n");
       set_LED(1, "3");
       *position = 0; 
       *last_input =0;
       *state =1;
    }
    else{} 
    return 0;
}

/****************************************************************
 * Locker
 ****************************************************************/
int locker(int* state){ 
      if(*state ==1){
        printf("fechou!\n");
        set_LED(0,"3");
        *state =0;
      }

    
    return 0;
}
/****************************************************************
 * Decoder
 ****************************************************************/

 int decoder(int list[2]){
     if(list[0]==1 && list[1]==0){return -1;}
     else if(list[0]==0 && list[1]==1){return 1;}
     else{return 0;}


}
/****************************************************************
 * Debouncing Code
 ****************************************************************/
void DebounceSwitch(int *Key_changed,int *status, int pin, int *Count)
{
    int RawState=0;
    *Key_changed = 0;
    gpio_get_value(pin,&RawState);
    if (RawState == *status) {
        // Set the timer which will allow a change from the current state.
        if (*status==0) *Count = 10;
        else                 *Count = 2;
    } else {
        // Key has changed - wait for new state to become stable.
        if (--*Count == 0) {
            // Timer expired - accept the change.
            *status = RawState;
            *Key_changed=1;
            // And reset the timer.
            if (*status==0) *Count = 10;
            else                 *Count = 2;
        }
    }
}

/****************************************************************

 ****************************************************************/
int pins[2] = {LEFT_PIN,RIGHT_PIN};

int main(){
      
   int i=0;
   
   int state =0; /*0 Close,1 Open*/
   int button_status[2] = {0,0};  
   int button_pressed[2] = {0,0};
   int button_changed[2] = {0,0};                
   int count[2] = {10,10}; 
   
   int time =0;

   int correct_password[PASSWORD_SIZE]={3,-3,2,-1,1};
   int password_position =0;
   int sum = 0;


   while(1){
       
       DebounceSwitch(&button_changed[0],&button_status[0],pins[0],&count[0]);
       DebounceSwitch(&button_changed[1],&button_status[1],pins[1],&count[1]);
       if((button_changed[0]==1 && button_status[0]==1) || (button_changed[1] ==1 && button_status[1]==1)){
       		sum =sum+decoder(button_status);
                locker(&state);
       		printf("Status 1: %d, Status 2: %d\n", button_status[0],button_status[1]);
       		printf("Sum: %d \n", sum);
      		unlocker(&password_position, &sum,correct_password,&state);
       		printf("Position: %d ", password_position);
       		printf("State: %d \n", state);
                button_changed[0]=0;
                button_changed[1] =0; 
       }


   }
   return 0;

}

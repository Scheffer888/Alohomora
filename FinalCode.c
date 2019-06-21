/****************************************************************
This code turns the LED on every time the button (connect on GPIO 45, PIN P8-11) is pressed down
 ****************************************************************/

#define PASSWORD_SIZE 5
#define PIN_1 44 /*Pin P8-12*/
#define PIN_2 26 /*Pin P8-14*/
#define PIN_3 46 /*Pin P8-16*/
#define PIN_4 65 /*Pin P8-18*/
#define PIN_LED 48 /*Pin P9-15*/
#define TIME_CONST 22
#define MAX_COMBINATIONS 12
#define ENCODER_SIZE 6
 
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
 * Decoder
 ****************************************************************/

 int position_decoder(int list[4]){
	if(list[0] ==0 && list[1] ==0 && list[2] ==0 && list[3] ==0){return 3;}
        else if(list[0] ==0 && list[1] ==0 && list[2] ==0 && list[3] ==1){return 4;}
        else if(list[0] ==1 && list[1] ==0 && list[2] ==0 && list[3] ==1){return 5;}
        else if(list[0] ==1 && list[1] ==1 && list[2] ==0 && list[3] ==1){return 0;}
        else if(list[0] ==0 && list[1] ==1 && list[2] ==0 && list[3] ==1){return 1;}
        else if(list[0] ==0 && list[1] ==1 && list[2] ==1 && list[3] ==1){return 2;}
	else{return -1;}


}


/****************************************************************
 * Debouncing Code
 ****************************************************************/
void DebounceSwitch(int *Key_changed,int *status,int *previous_status, int pin, int *Count)
{
    int RawState=0;
    *Key_changed = 0;
    gpio_get_value(pin,&RawState);
    if (RawState == *status) {
        // Set the timer which will allow a change from the current state.
        if (*status==0) *Count = 5;
        else                 *Count = 2;
    } else {
        // Key has changed - wait for new state to become stable.
        if (--*Count == 0) {
            // Timer expired - accept the change.
            *previous_status = *status;
            *status = RawState;
            *Key_changed=1;
            // And reset the timer.
            if (*status==0) *Count = 5;
            else                 *Count = 2;
        }
    }
}

/****************************************************************

 ****************************************************************/
int pins[4] = {PIN_1,PIN_2,PIN_3,PIN_4};
int led_pin = PIN_LED;

int main(){
      
   int i=0;
   int difference =0;
   int correct_position = 0;
	
   int state =0; /*0 Close,1 Open*/
   int encoder_status[4] = {0,0,0,0};
   int encoder_previous_status[4] = {0,0,0,0}; 
   int encoder_changed[4] = {0,0,0,0};                
   int count[4] = {5,5,5,5}; 
   

   int time =0;
   int position =-2;
   int previous_position = 0;

   int correct_password[PASSWORD_SIZE]={-1,2,-1,1,-1};
   int password_position =0;
   int sum = 0;
   
   for(i=0;i<4;i++){
	gpio_get_value(pins[i],&encoder_status[i]);
   }
     
   previous_position = position_decoder(encoder_status);
   
   printf("Insira a senha no encoder. \n");

   while(1){
       for(i=0;i<4;i++){
       		DebounceSwitch(&encoder_changed[i],&encoder_status[i],&encoder_previous_status[i],pins[i],&count[i]);
       }
     
       if(encoder_changed[0] == 1 || encoder_changed[1] == 1 || encoder_changed[2] == 1 || encoder_changed[3] == 1){
       		//printf("Encoder 0: %d, Encoder 1: %d, Encoder 2: %d, Encoder 3: %d  \n",encoder_status[0],encoder_status[1],encoder_status[2],encoder_status[3]);
		position=position_decoder(encoder_status);
		if(position >= 0){

			if(state == 1){
				        printf("Fechou!\n");
        				set_LED(0,"3");
					state=0;
        				gpio_set_value(led_pin,0);
					password_position =0;
 	 				sum =0;
		
			}
			else{
				difference =position-previous_position;
                		if(difference >1){difference =-ENCODER_SIZE%(position-previous_position);}
				else if(difference <-1){difference =ENCODER_SIZE%(position-previous_position);}
				else{}
				sum =sum+difference;
                        	if(sum >= MAX_COMBINATIONS || sum <= -MAX_COMBINATIONS){
					password_position =0;
 	 				sum =0;
					printf("Insira a senha no encoder. \n");
				}
				else if(correct_position == 1 && difference*correct_password[password_position+1] > 0){
					password_position =password_position+1;
					sum =difference;
					if(sum == correct_password[password_position]){
						correct_position =1;
						if(password_position == PASSWORD_SIZE -1){
							printf("Abriu!\n");
       							set_LED(1, "3");
       				                	gpio_set_value(led_pin,1);
       							state =1;
						
						}
						else{}						
						
					}
					else{correct_position =0;}
                                       
				}
				else{
					if(sum == correct_password[password_position]){
						correct_position =1;
						if(password_position == PASSWORD_SIZE -1){
							printf("Abriu!\n");
       							set_LED(1, "3");
       				                	gpio_set_value(led_pin,1);
       							state =1;
						
						}
						else{}						
						
					}
					else{correct_position =0;}
			
				}
			}
			
			//printf("Position: %d, Previous Position: %d\n", position,previous_position );
			//printf("Difference: %d \n", difference);
                        //printf("Correct position: %d \n", correct_position);
                        //printf("Sum: %d \n", sum);
                        //printf("Password_position: %d \n", password_position);
			printf("Posição: %d\n", sum%MAX_COMBINATIONS);
			printf("\n");
                        previous_position = position;
			

		}
		else{printf("Failure to read encoder, error code: %d%d%d%d \n", encoder_status[0],encoder_status[1],encoder_status[2],encoder_status[3]);
		}
                

  		for(i=0;i<4;i++){
			encoder_changed[i]=0;
   		}
           
       }


   }
   return 0;

}

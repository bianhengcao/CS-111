
/*NAME:Bianheng Cao
EMAIL:bianhengcao@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <poll.h>
#include <time.h>
#include <signal.h>
#include <mraa.h>
#include <mraa/aio.h>
#include <mraa/gpio.h>
#include <math.h>
#include <pthread.h>

int period=1;
sig_atomic_t volatile stop=0;
char scale='F';
char* l_name;
FILE *log_;
static int l_flag;
mraa_aio_context rotary;
mraa_gpio_context button;

void *temp(void* arg)
{
	while(1)
	{
		time_t t;
		time(&t);
		struct tm* tz=localtime(&t);
		char buf[9];
		strftime(buf, 9, "%2H:%2M:%2S", tz);

		int info=mraa_aio_read(rotary);
		float temp;
		float R=(1023.0/(float)info - 1)*100000.0;
		temp=1.0/(log(R/100000.0)/4275+1/298.15)-273.15;
		if(scale=='C')
			;
		else
			temp=temp*1.8 + 32.0;

		if(!stop)
		{
			fprintf(stdout, "%s %.1f\n", buf, temp);
			if(l_flag)
				fprintf(log_, "%s %.1f\n", buf, temp);
		}
		sleep(period);
	}
	pthread_exit(NULL);
}

void *proccess(void* arg)
{
	while(1)
	{
		if(mraa_gpio_read(button)!=0)
		{
			time_t t;
			time(&t);
			struct tm* tz=localtime(&t);
			char ex[9];
			strftime(ex, 9, "%2H:%2M:%2S", tz);
			if(l_flag)
				fprintf(log_, "%s SHUTDOWN\n", ex);
			fprintf(stdout, "%s SHUTDOWN\n", ex);
			exit(0);
		}
		char buf[128];
		int n=0;
		int x=0;
		while(n=read(0, buf+x, 1) > 0 && buf[x] != '\n')
			x++;
		if(n < 0)
		{
			fprintf(stderr, "Stdin read failed\n");
	  		exit(1);
		}

		if(x > 0)
		{
			buf[x]='\0';
			if(strcmp(buf, "SCALE=F")==0)
			{
				if(l_flag)
					fprintf(log_, "SCALE=F\n");
				scale='F';
			}
			else if(strcmp(buf, "SCALE=C")==0)
			{
				if(l_flag)
					fprintf(log_, "SCALE=C\n");
				scale='C';
			}
			else if(strncmp(buf, "PERIOD=", 7)==0)
			{
				int x = atoi(buf+7);
				period=x;
				if(l_flag)
					fprintf(log_, "PERIOD=%d\n", x);
			}
			else if(strcmp(buf, "STOP")==0)
			{
				if(l_flag)
					fprintf(log_, "STOP\n");
				stop=1;
			}
			else if(strcmp(buf, "START")==0)
			{
				if(l_flag)
					fprintf(log_, "START\n");
				stop=0;
			}
			else if(strncmp(buf, "LOG", 3)==0)
			{
				if(l_flag)
					fprintf(log_, "%s\n", buf);
			}
			else if(strcmp(buf, "OFF")==0)
			{
				time_t t;
				time(&t);
				struct tm* tz=localtime(&t);
				char buf2[9];
				strftime(buf2, 9, "%2H:%2M:%2S", tz);
				fprintf(stdout, "%s SHUTDOWN\n", buf2);
				if(l_flag)
				{
					fprintf(log_, "%s SHUTDOWN\n", buf2);
					fclose(log_);
				}
				mraa_aio_close(rotary);
				mraa_gpio_close(button);
				exit(0);
			}
			else
			{
				fprintf(stderr, "Invalid command, try again\n");
				exit(1);
			}
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char** argv)
{
  while(1)
    {
    static struct option long_options[]=
      {
        {"period", required_argument, NULL, 'p'},
		{"scale", required_argument, NULL, 's'},
		{"log", required_argument, NULL, 'l'},
		{0,0,0,0}
      };
    int option_index;
    int in;
    int c=getopt_long(argc, argv, "p:s:l:", long_options, &option_index);
    if(c==-1)
      break;
    switch(c)
      {
      case 'p':
		period=atoi(optarg);
		if(period<0)
	 	 {
	  	  fprintf(stderr, "Enter a valid argument for period\n");
	  	  exit(1);
	 	 }
		break;
      case 's':
		if(optarg[0]=='F')
			scale='F';
		else if(optarg[0]=='C')
			scale='C';
		else
	    {
	  	  fprintf(stderr, "Enter a valid argument for scale: C or F\n");
	  	  exit(1);
	  	}
		break;
      case 'l':
		l_name=optarg;
		l_flag=1;
		break;
      default:
		fprintf(stderr, "Please enter a valid option with the correct argument(s)\n");
		exit(1);
       }
    }
    if(l_flag)
    {
    	log_=fopen(l_name, "w");
    	if(log_==NULL)
		{
		  fprintf(stderr, "Enter a valid argument for log\n");
	  	  exit(1);
		}
    }
    button=mraa_gpio_init(62);//GPIO_51
    rotary=mraa_aio_init(1);
    if(rotary==NULL)
    {
    	fprintf(stderr, "Couldn't initialize rotary:%s\n", strerror(errno));
    	exit(1);
    }
    if(button==NULL)
    {
    	fprintf(stderr, "Couldn't initialize button:%s\n", strerror(errno));
    	exit(1);
    }
    pthread_t threads[2];
    if(pthread_create(&threads[0], NULL, temp, NULL) != 0)
    {
       	fprintf(stderr, "Failed to create thread:%s\n", strerror(errno));
      	exit(1);
    }
    if(pthread_create(&threads[1], NULL, proccess, NULL) != 0)
    {
     	fprintf(stderr, "Failed to create thread:%s\n", strerror(errno));
      	exit(1);
    }
    int i;
    for(i=0;i<2;i++)
    {
    	if(pthread_join(threads[i], NULL) != 0 )
    	{
    		fprintf(stderr, "Failed to join threads:%s\n", strerror(errno));
    		exit(1);
   	    }
    }
    free(threads);
    if(l_flag)//close file
    	fclose(log_);
    mraa_aio_close(button);
    mraa_gpio_close(rotary);
}

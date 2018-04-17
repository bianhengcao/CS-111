
/*NAME:Bianheng Cao
EMAIL:bianhengcao@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <poll.h>
#include <time.h>
#include <mraa.h>
#include <mraa/aio.h>
#include <math.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>

int port=0;
char* host = "";
char* l_file = "";
FILE* lg;
int id=-1;
int period=1;
sig_atomic_t volatile stop=0;
char scale='F';
mraa_aio_context rotary;

int main(int argc, char** argv)
{
  //take arguements
  while(1)
    {
    static struct option long_options[]=
      {
        {"period", required_argument, NULL, 'p'},
		{"scale", required_argument, NULL, 's'},
		{"log", required_argument, NULL, 'l'},
		{"id", required_argument, NULL, 'i'},
		{"host", required_argument, NULL, 'h'},
		{0,0,0,0}
      };
    int option_index;
    int in;
    char ch;
    int c=getopt_long(argc, argv, "p:s:l:i:h:", long_options, &option_index);
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
      	ch=optarg[0];
		if(ch=='F')
			scale='F';
		else if(ch=='C')
			scale='C';
		else
	    {
	  	  fprintf(stderr, "Enter a valid argument for scale: C or F\n");
	  	  exit(1);
	  	}
		break;
      case 'l':
      	if(strlen(optarg)==0)
      	{
      		fprintf(stderr, "Please enter a valid file name\n");
	    	exit(1);
      	}
		l_file=optarg; 
		break;
	  case 'i':
	    if(strlen(optarg)!=9)
	    {
	    	fprintf(stderr, "Not enough digits for an ID number\n");
	    	exit(1);
	    }
	    id=atoi(optarg);
	    break;
	  case 'h':
	  	if(strlen(optarg)==0)
      	{
      		fprintf(stderr, "Please enter a valid host name\n");
	    	exit(1);
      	}
	  	host=optarg;
	    break;
      default:
		fprintf(stderr, "Please enter a valid option with the correct argument(s)\n");
		exit(1);
       }
    }
    int flag = 0;
	int i;
  	for (i = optind; i<argc; i++) 
  	{
    	if (isdigit(argv[i][0]))
    	{
      		flag = 1;
      		port= atoi(argv[i]);
    	}
	}
    if(flag==0)
    {
    	fprintf(stderr, "Please enter a port number\n");
    	exit(1);
    }
    if(id==-1)
    {
    	fprintf(stderr, "Please enter an ID number\n");
    	exit(1);
    }
    if(strcmp(l_file, "") == 0)
    	{
		  fprintf(stderr, "Please enter a log file\n");
	  	  exit(1);
		}
    lg=fopen(l_file, "w");
    if(lg==NULL)
		{
		  fprintf(stderr, "Failed to open log file:%s\n", strerror(errno));
	  	  exit(1);
		}
	if(strcmp(host, "") == 0)
		{
			fprintf(stderr, "Please enter a host name\n");
			exit(1);
		}
		
    rotary=mraa_aio_init(1);
    if(rotary==NULL)
    {
    	fprintf(stderr, "Couldn't initialize rotary:%s\n", strerror(errno));
    	exit(1);
    }
    int sfd=socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0)
    {
    	fprintf(stderr, "Error opening socket\n");
    	exit(2);
    }
    struct hostent* svr = gethostbyname(host);
    if(svr==NULL)
    {
    	fprintf(stderr, "Could not find host\n");
    	exit(1);
    }
    struct sockaddr_in adr;
    adr.sin_port=htons(port);
    adr.sin_family=AF_INET;
    memcpy((char*)&adr.sin_addr.s_addr,(char*)svr->h_addr,svr->h_length);
  	if(connect(sfd,(struct sockaddr*) &adr,sizeof(adr)) < 0)
  	{
    	fprintf(stderr, "Could not connect to server\n");
    	exit(2);
  	}
  	dprintf(sfd, "ID=%d\n", id);
    fprintf(lg, "ID=%d\n", id);
    fflush(lg);
  	struct pollfd polls[1];
  	polls[0].fd = sfd;
 	polls[0].events = POLLIN;

 	time_t clock=0;
	char buf[128];
	while(1)
	{
		int n=poll(polls, 1, 0);
		if(n==-1)
		{
			fprintf(stderr, "Error happened while polling\n");
			exit(2);
		}
		if(polls[0].revents & POLLIN)
		{
			int n=0;
			int x=0;
			while(n=read(sfd, buf+x, 1) > 0 && buf[x] != '\n')
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
					fprintf(lg, "SCALE=F\n");
					fflush(lg);
					scale='F';
				}
				else if(strcmp(buf, "SCALE=C")==0)
				{
					fprintf(lg, "SCALE=C\n");
					fflush(lg);
					scale='C';
				}
				else if(strncmp(buf, "PERIOD=", 7)==0)
				{
					int x = atoi(buf+7);
					period=x;
					fprintf(lg, "PERIOD=%d\n", x);
					fflush(lg);
				}
				else if(strcmp(buf, "STOP")==0)
				{
					fprintf(lg, "STOP\n");
					fflush(lg);
					stop=1;
				}
				else if(strcmp(buf, "START")==0)
				{
					fprintf(lg, "START\n");
					fflush(lg);
					stop=0;
				}
				else if(strncmp(buf, "LOG", 3)==0)
				{
					fprintf(lg, "%s\n", buf+4);
					fflush(lg);
				}
				else if(strcmp(buf, "OFF")==0)
				{
					time_t t;
					time(&t);
					struct tm* tz=localtime(&t);
					char buf2[9];
					strftime(buf2, 9, "%2H:%2M:%2S", tz);
					dprintf(sfd, "%s SHUTDOWN\n", buf2);
					fprintf(lg, "OFF\n");
					fprintf(lg, "%s SHUTDOWN\n", buf2);
					fflush(lg);
					fclose(lg);
					mraa_aio_close(rotary);
					exit(0);
				}
				else
				{
					dprintf(sfd, "Invalid command, try again\n");
					exit(1);
				}
			}
		}
		time_t now=time(0);
		if(now-clock>=period && !stop)
		{
			int info=mraa_aio_read(rotary);
			float temp;
			float R=(1023.0/(float)info - 1)*100000.0;
			temp=1.0/(log(R/100000.0)/4275+1/298.15)-273.15;*/
			if(scale=='C')
				;
			else
				temp=temp*1.8 + 32.0;
			clock=time(0);
			time_t t;
			time(&t);
			struct tm* tz=localtime(&t);
			char buf[9];
			strftime(buf, 9, "%2H:%2M:%2S", tz);
			dprintf(sfd, "%s %.1f\n", buf, temp);
			fprintf(lg, "%s %.1f\n", buf, temp);
			fflush(lg);
		}
	} 
   	if(fclose(lg)!=0)
   	{
   		fprintf(stderr, "Failed to close log:%s\n", strerror(errno));
   		exit(1);
    }
    if(close(sfd)<0)
    {
    	fprintf(stderr, "Failed to close socket\n");
    	exit(2);
    }
    mraa_aio_close(rotary);
}

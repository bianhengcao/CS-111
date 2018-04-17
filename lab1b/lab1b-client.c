/*NAME:Bianheng Cao
EMAIL:bianhengcao@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "zlib.h"

struct termios ini_state;

void error(char* err)
{
  tcsetattr(0, TCSANOW, &ini_state);
  fprintf(stderr, "%s\n%s", err, strerror(errno));
  exit(1);
}

int main(int argc, char** argv)
{
  static int port_flag=0;
  static int port_num=0;
  static int log_flag=0;
  static int compress_flag=0;
  char* log_name;
  int lfd;//log fd
  while(1)
    {
    static struct option long_options[]=
      {
        {"port", required_argument, NULL, 'p'},
	{"log", required_argument, NULL, 'l'},
	{"compress", no_argument , NULL, 'c'},
        {0,0,0,0}
      };
    int option_index;
    int c=getopt_long(argc, argv, "p:l:c", long_options, &option_index);
    if(c==-1)
      break;
    switch(c)
      {
      case 'p':
	port_flag=1;
	port_num=atoi(optarg);
	break;
      case'l':
	log_flag=1;
	log_name=optarg;
	break;
      case 'c':
	compress_flag=1;
	break;
      default:
	fprintf(stderr, "Please enter a valid option with the correct argument(s)\n");
	exit(1);
      }
    }
  if(log_flag)
    {
      lfd=open(log_name,O_WRONLY|O_CREAT|O_TRUNC, 0666);
      if(lfd<0)
	{
	  fprintf(stderr, "%s\n", strerror(errno));
	  exit(1);
	}
    }
  tcgetattr(0, &ini_state);
  struct termios copy_state=ini_state;
  copy_state.c_iflag=ISTRIP;
  copy_state.c_oflag=0;
  copy_state.c_lflag=0;
  tcsetattr(0, TCSANOW, &copy_state);
  if(port_flag)
    {
      struct sockaddr_in svr_addr;
      int sfd = socket(AF_INET, SOCK_STREAM, 0);
      if(sfd<0)
	error("Failed to create socket");
      struct hostent *server = gethostbyname("localhost");
      if(server==NULL)
	error("Cannot find server");
      bzero((char*) &svr_addr, sizeof(svr_addr));
      svr_addr.sin_family = AF_INET;
      bcopy((char*) server->h_addr, (char*) &svr_addr.sin_addr.s_addr,server->h_length);
      svr_addr.sin_port = htons(port_num);
      if(connect(sfd,(struct sockaddr*) &svr_addr,sizeof(svr_addr))<0)
	error("Failed to connect to socket");
      struct pollfd fds[2];
      fds[0].fd = 0;
      fds[0].events = POLLIN | POLLHUP | POLLERR;
      fds[1].fd = sfd;
      fds[1].events = POLLIN | POLLHUP | POLLERR;
      while(1)
	{
	  int state=poll(fds, 2, 0);
	  if(state<0)
	    error("Client polling failed");
	  if(state==0)
	    continue;
	  if(fds[0].revents && POLLIN)
	    {
	      if(compress_flag)
		{
		  char* buff= malloc(256*sizeof(char));
                  int n=read(0, buff, 256);
                  if(n<0)
                    error("Read call from keyboard failed");
		  int i;
                  for(i=0;i<n;i++)
                    {
                      if(buff[i]=='\r' || buff[i]=='\n')
                        {
                          char* buff2= malloc(2*sizeof(char));
                          buff2[0]='\r';
                          buff2[1]='\n';
                          if(write(0,buff2,2)<0)
                            error("Write call to stdin failed");
                          free(buff2);
                        }
                      else
                        {
                          if(write(0,buff+i,1)<0)
                            error("Write call to stdin");
			}
                    }
		  unsigned char zip[5000];
		  z_stream def;
		  int flush;
		  def.zalloc = Z_NULL;
		  def.zfree = Z_NULL;
		  def.opaque = Z_NULL;
		  if(deflateInit(&def, Z_DEFAULT_COMPRESSION) != Z_OK)
		    error("Failed to initialize deflate");
		  def.avail_in=n;//number of bytes to read
		  def.next_in=buff;//pointer to input buffer
		  def.avail_out=5000;//maximum size of buffer
		  def.next_out=zip;//pointer to output buffer
		  if(log_flag)
                    {
                      if(dprintf(lfd, "SENT %d bytes: ", n)<0)
                        error("Failed to write to log");
		    }
		  while(def.avail_in>0)
		    {
		      deflate(&def, flush);
		      if(write(fds[1].fd,zip,5000)<0)
			error("Write call to server failed");
		      if(log_flag)
			{
			  if(write(lfd, zip, 5000)<0)
			    error("Failed to write to log");
			  if(dprintf(lfd, "\n")<0)
			    error("Failed to write to log");
			}
		    }
		  (void)deflateEnd(&def);
                  free(buff);
		}
	      else
		{
		  char* buff= malloc(256*sizeof(char));
                  int n=read(0, buff, 256);
		  int i;
		  for(i=0;i<n;i++)
		    {
		      if(buff[i]=='\r' || buff[i]=='\n')
			{
			  char* buff2= malloc(2*sizeof(char));
			  buff2[0]='\r';
			  buff2[1]='\n';
			  if(write(0,buff2,2)<0)
			    error("Write call to stdin failed");
			  if(write(fds[1].fd,buff2+1,1)<0)
			    error("Write call to server failed");
			  free(buff2);
			}
		      else
			{
			  if(write(0,buff+i,1)<0)
			    error("Write call to stdin");
			  if(write(fds[1].fd,buff+i,1)<0)
			    error("Write call to server failed");
			}
		    }
		  if(log_flag)
		    {
		      if(dprintf(lfd, "SENT %d bytes: ", n)<0)
			error("Failed to write to log");
		      if(write(lfd, buff, n)<0)
			error("Failed to write to log");
		      if(dprintf(lfd, "\n")<0)
			error("Failed to write to log");
		    }
		  free(buff);
		}
	    }
	  if(fds[1].revents && POLLIN)
	    {
	      if(compress_flag)
		{
		  char* buff= malloc(256*sizeof(char));
                  unsigned char zip[10000];
		  int n=0;
		  int i=read(fds[1].fd, zip, 10000);
                  if(i<0)
                    error("Read call from keyboard failed");
                  if(i==0)//exit program
                    {
                      if(close(sfd)<=0)
                        error("Failed to close socket");
                      if(log_flag)
                        {
                          if(close(lfd)<0)
                            error("Error closing log file");
                        }
                      tcsetattr(0, TCSANOW, &ini_state);
                      exit(0);
                    }
		  z_stream inf;
		  inf.zalloc=Z_NULL;
		  inf.zfree=Z_NULL;
		  inf.opaque=Z_NULL;
		  inf.avail_in=0;
		  inf.next_in=Z_NULL;
		  if(inflateInit(&inf) != Z_OK)
			exit("Failed to decompress data");
		  inf.avail_out=256;
		  inf.next_out=buff;
		  inflate(&inf, Z_NO_FLUSH);
		  n=inf.avail_in;
		  (void)inflateEnd(&inf);
		  for(i=0;i<n;i++)
                    {
                      if(buff[i]=='\r' || buff[i]=='\n')
                        {
                          char* buff2= malloc(2*sizeof(char));
                          buff2[0]='\r';
                          buff2[1]='\n';
                          if(write(0,buff2,2)<0)
                            error("Write call to stdin failed");
                          free(buff2);
                        }
                      else
			{
			  if(write(0,buff+i,1)<0)
			    error("Write call to stdin");
			}
		    }
		  if(log_flag)
		    {
		      if(dprintf(lfd, "RECEIVED %d bytes: ", n)<0)
			error("Failed to write to log");
		      if(write(lfd, buff,n)<0)
			error("Failed to write to log");
		      if(dprintf(lfd,"\n")<0)
			error("Failed to write to log");
		    }
		  free(buff);
		}
	      else
		{
		  char* buff= malloc(256*sizeof(char));
		  int n=read(fds[1].fd, buff, 256);
		  if(n<0)
		    error("Read call from keyboard failed");
		  if(n==0)//exit program
		    {
		      if(close(sfd)<0)
			error("Failed to close socket");
		      if(log_flag)
			{
			  if(close(lfd)<0)
			    error("Error closing log file");
			}
		      tcsetattr(0, TCSANOW, &ini_state);
		      exit(0);
		    }
		  int i;
		  for(i=0;i<n;i++)
		    {
		      if(buff[i]=='\r' || buff[i]=='\n')
			{
			  char* buff2= malloc(2*sizeof(char));
			  buff2[0]='\r';
			  buff2[1]='\n';
			  if(write(0,buff2,2)<0)
			    error("Write call to stdin failed");
			  free(buff2);
			}
		      else
			{
			  if(write(0,buff+i,1)<0)
			    error("Write call to stdin");
			}
		    }
		  if(log_flag)
		    {
		      if(dprintf(lfd, "RECEIVED %d bytes: ", n)<0)
			error("Failed to write to log");
		      if(write(lfd, buff,n)<0)
			error("Failed to write to log");
		      if(dprintf(lfd,"\n")<0)
			error("Failed to write to log");
		    }
		  free(buff);
		}
	    }
	}
    } 
  else
    {
      error("Must use --port!");
    }
}

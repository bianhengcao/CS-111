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
pid_t id;
int sfd;
int new_sfd;

void error(char* err)
{
  fprintf(stderr, "%s\n", err);
  close(sfd);
  close(new_sfd);
  exit(1);
}

void exit_status()
{
  int status;
  if(waitpid(id, &status, 0)<0)
    error("Failed to wait for child proccess to finish");
  int exit2 = (status & 0x007f);
  int exit3 = (status & 0xff00); 
  fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", exit2, exit3);
  exit(0);
}

void handler(int sig)
{
  if(sig == SIGINT)
    {
      exit(0);
    }
  if(sig == SIGPIPE)
    {
      exit(0);
    }
  if (sig == SIGTERM)
    {
      exit(0);
    }
}

int main(int argc, char** argv)
{
  static int port_flag=0;
  static int port_num=0;
  static int compress_flag=0;
  while(1)
    {
    static struct option long_options[]=
      {
        {"port", required_argument, NULL, 'p'},
	{"compress", no_argument, NULL, 'c'},
        {0,0,0,0}
      };
    int option_index;
    int c=getopt_long(argc, argv, "p:c:", long_options, &option_index);
    if(c==-1)
      break;
    switch(c)
      {
      case 'p':
	port_flag=1;
	port_num=atoi(optarg);
	break;
      case 'c':
	compress_flag=1;
	break;
      default:
	error("Please enter a valid arguement");
      }
    }
  if(compress_flag)
    {

    }
  if(port_flag)
    {
      if(signal(SIGINT, handler) == SIG_ERR)
	error("Setting up signal handler failed");
      if(signal(SIGPIPE, handler) == SIG_ERR)
        error("Setting up signal handler failed");
      if(signal(SIGTERM, handler) == SIG_ERR)
	error("Setting up signal handler failed");
      struct sockaddr_in svr_addr;
      struct sockaddr_in clt_addr;
      sfd = socket(AF_INET, SOCK_STREAM, 0);
      if(sfd<0)
	error("Failed to create socket");
      bzero((char*) &svr_addr, sizeof(svr_addr));//zero out bits
      svr_addr.sin_family = AF_INET;
      svr_addr.sin_addr.s_addr = INADDR_ANY;
      svr_addr.sin_port = htons(port_num);
      if(bind(sfd, (struct sockaddr*) &svr_addr, sizeof(svr_addr)) < 0) 
	error("Error binding server to socket");
      listen(sfd, 5);
      int clt_len = sizeof(clt_addr);
      new_sfd = accept(sfd, (struct sockaddr*) &clt_addr, &clt_len);
      if(new_sfd<0) 
	error("Error accepting connection");
      int to_shell[2];
      int from_shell[2];
      if(pipe(to_shell)<0) 
	error("Error creating pipes");
      if(pipe(from_shell)<0) 
	error("Error creating pipes");
      id=fork();
      if(id<0)
	error("Failed to create child proccess");      
      if(id==0)//child proccess
	{
	  if(close(to_shell[1])<0) 
	    error("Failed to close pipe end");
	  if(close(from_shell[0])<0) 
	    error("Failed to close pipe end");
	  if(dup2(to_shell[0],0)<0) 
	    error("Failed to close pipe end");
	  if(dup2(from_shell[1],1)<0) 
	    error("Failed to dup file descriptors failed");
	  if(dup2(from_shell[1],2)<0) 
	    error("Failed to dup file descriptors failed");
	  if(close(to_shell[0])<0)
	    error("Failed to close pipe end");
	  if(close(from_shell[1])<0) 
	    error("Failed to close pipe end");
	  char* args[2]= {"/bin/bash", NULL};
	  if(execvp("/bin/bash", args)<0)
	    error("Failed to open bash shell");
	}
      else//parent proccess
	{
	  if(close(to_shell[0])<0)
	    error("Failed to close pipe end");
	  if(close(from_shell[1])<0)
	     error("Failed to close pipe end");
	  struct pollfd fds[2];
	  fds[0].fd = new_sfd;
	  fds[0].events = POLLIN | POLLHUP | POLLERR;
	  fds[1].fd = from_shell[0];
	  fds[1].events = POLLIN | POLLHUP | POLLERR;
	  while(1)
	    {
	      if(poll(fds, 2, 0)<0)
		error("Server polling failed");
	      if(fds[0].revents && POLLIN)
		{
		  char* buff= malloc(256*sizeof(char));
		  int n;
		  if(compress_flag)
		    {
		      unsigned char zip[10000];
		      int i=read(fds[1].fd, zip, 10000);
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
		    }
		  else
		    n=read(new_sfd, buff, 256);
		  if(n<=0)
		    {
		      if(kill(id, SIGTERM)<0)
			error("Failed to kill child proccess");
		      exit_status();
		      break;
		    }
		  int i;
		  for(i=0;i<n;i++)
		    {
		      if(buff[i]==0x03)
			{
			  if(kill(id,SIGINT)<0)
			    error("Failed to kill child proccess");
			  exit_status();
			}
		      if(buff[i]==0x04)
			{
			  if(close(to_shell[1])<0)
			    error("Failed to close server to shell pipe");
			}
		      else
			{
			  if(write(to_shell[1],buff+i,1)<0)
			    error("Write call to shell failed");
			}
		    }
		  free(buff);
		}
	      if(fds[1].revents && POLLIN)
		{
		  char* buff= malloc(256*sizeof(char));
		  int n=read(from_shell[0], buff, 256);
		  if(n<0)
		    error("Read call from shell failed");
		  if(n==0)//exit program
		    {
		      close(sfd);
		      close(new_sfd);
		      exit_status();
		      break;
		    }
		  if(compress_flag)
		    {
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
		      while(def.avail_in>0)
			{
			  deflate(&def, flush);
			}
		      (void)deflateEnd(&def);
		      if(write(new_sfd,zip,5000)<0)
			error("Write call to server failed");

		    }
		  else
		    {
		      int i;
		      for(i=0;i<n;i++)
			{
			  if(write(new_sfd,buff+i,1)<0)
			    error("Write call to server failed");
			}
		    }
		  free(buff);
		}
	      if(fds[1].revents && (POLLHUP || POLLERR) ) 
		{
		  if(close(fds[1].fd)<0)
		    error("Failed to close shell");
		  exit_status();
		  exit(0);
		}
	    }
	}
    }
  else
    {
      error("Must use --port!");
      exit(1);
    }
}

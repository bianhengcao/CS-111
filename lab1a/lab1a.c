 /*NAME:Bianheng Cao
EMAIL:bianhengcao@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <poll.h>
#include <sys/ioctl.h>

void handler(int signum)
{
  if(signum == SIGINT)
      exit(0);
  if(signum == SIGPIPE)
      exit(0);
}

int main(int argc, char** argv)
{
  struct termios ini_state;
  tcgetattr(0, &ini_state);
  struct termios copy_state=ini_state;
  copy_state.c_iflag=ISTRIP;
  copy_state.c_oflag=0;
  copy_state.c_lflag=0;
  tcsetattr(0, TCSANOW, &copy_state);
  static int shell_flag=0;
  while(1)
    {
    static struct option long_options[]=
      {
        {"shell", no_argument, NULL, 's'},
        {0,0,0,0}
      };
    int option_index;
    int c=getopt_long(argc, argv, "s", long_options, &option_index);
    if(c==-1)
      break;
    switch(c)
      {
      case 's':
	shell_flag=1;
	break;
      default:
	fprintf(stderr, "Please enter a valid arguement\n");
	tcsetattr(0, TCSANOW, &ini_state);
	exit(1);
      }
    }
  pid_t id;
  int to_shell[2];
  int from_shell[2];
  if(shell_flag)
    {
      signal(SIGPIPE, handler);
      if(pipe(to_shell)<0)
	{
	  fprintf(stderr, "Setting up pipes failed\n", strerror(errno));
	  tcsetattr(0, TCSANOW, &ini_state);
          exit(1);
	}
      if(pipe(from_shell)<0)
        {
          fprintf(stderr, "Setting up pipes failed\n", strerror(errno));
	  tcsetattr(0, TCSANOW, &ini_state);
          exit(1);
        }
      id=fork();
      if(id<0)
	{
	  fprintf(stderr, "Creating child proccess failed\n", strerror(errno));
	  tcsetattr(0, TCSANOW, &ini_state);
	  exit(1);
	}
      if(id==0)//child
	{
	  if(close(to_shell[1])<0)
	    {
	      fprintf(stderr, "Child proccess failed to close pipe end\n");
              tcsetattr(0, TCSANOW, &ini_state);
              exit(1);

	    }
	  if(close(from_shell[0])<0)
            {
              fprintf(stderr, "Child proccess failed to close pipe end\n");
              tcsetattr(0, TCSANOW, &ini_state);
              exit(1);
            }
	  if(dup2(to_shell[0], 0) < 0)
	    {
	      fprintf(stderr, "Child proccess failed to duplicate file descriptors:\n", strerror(errno));
              tcsetattr(0, TCSANOW, &ini_state);
              exit(1);
	    }
	  if(dup2(from_shell[1], 1) < 0)
	    {
              fprintf(stderr, "Child proccess failed to duplicate file descriptors: %s\n", strerror(errno));
              tcsetattr(0, TCSANOW, &ini_state);
              exit(1);
	    }
	  if(dup2(from_shell[1], 2) < 0)
	    {
	      fprintf(stderr, "Child proccess failed to duplicate file descriptors: %s\n", strerror(errno));
	      tcsetattr(0, TCSANOW, &ini_state);
	      exit(1);
	    }
	  if(close(to_shell[0])<0)
            {
              fprintf(stderr, "Child proccess failed to close pipe end\n");
              tcsetattr(0, TCSANOW, &ini_state);
              exit(1);
            }
          if(close(from_shell[1])<0)
            {
              fprintf(stderr, "Child proccess failed to close pipe end\n");
              tcsetattr(0, TCSANOW, &ini_state);
              exit(1);
            }
	  if(execvp("/bin/bash", NULL) < 0)
            {
              fprintf(stderr, "Child proccess failed to execute shell:\n", strerror(errno));
              tcsetattr(0, TCSANOW, &ini_state);
              exit(1);
            }
	}
      else//parent
	{
	  if(close(to_shell[0])<0)
	    {
	      fprintf(stderr, "Parent proccess failed to close pipe end\n");
              tcsetattr(0, TCSANOW, &ini_state);
              exit(1);
	    }
	  if(close(from_shell[1])<0)
	    {
	      fprintf(stderr, "Parent process failed to close pipe end\n");
              tcsetattr(0, TCSANOW, &ini_state);
              exit(1);
	    }
	  struct pollfd fds[2];
	  fds[0].fd=0;
	  fds[0].events=POLLIN|POLLHUP|POLLERR;
	  fds[1].fd=from_shell[0];
	  fds[1].events=POLLIN|POLLHUP|POLLERR;
	  while(1)
	    {
	      if(poll(fds, 2, 0)<0)
		{
		  fprintf(stderr, "Parent process failed to poll correctly\n", strerror(errno));
		  tcsetattr(0, TCSANOW, &ini_state);
		  exit(1);
		}
	      if(fds[0].revents&&POLLIN)//keyboard
		{
		  char* buff= malloc(256*sizeof(char));
		  int n=read(0, buff, 256);
		  if(n<0)
		    {
		      fprintf(stderr, "Parent proccess read call failed\n", strerror(errno));
		      tcsetattr(0, TCSANOW, &ini_state);
		      exit(1);
		    }
		  int i;
		  for(i=0;i<n;i++)
		    {
		      if(buff[i]=='\r' || buff[i]=='\n')
			{
			  char* buff2= malloc(2*sizeof(char));
			  buff2[0]='\r';
			  buff2[1]='\n';
			  if(write(to_shell[1],buff2+1,1)<0)//forward only \n
                            {
                              fprintf(stderr, "Failed to write carriage return and newl\
ine to shell\n");
                              tcsetattr(0, TCSANOW, &ini_state);
                              exit(1);
                            }
			  if(write(1,buff2,2)<0)
			    {
			      fprintf(stderr, "Failed to write carriage return and newline to terminal\n");
			      tcsetattr(0, TCSANOW, &ini_state);
			      exit(1);
			    }
			  free(buff2);
			}
		      else if(buff[i]==0x04)
			{
			  if(close(to_shell[1])<0)
			    {
			      fprintf(stderr, "Parent proccess failed to close pipe\n");
                              tcsetattr(0, TCSANOW, &ini_state);
                              exit(1);
			    }  			      
			}
		      else if(buff[i]==0x03)
			{
			  kill(id, SIGINT);
			}
		      else
			{
			  if(write(1,buff+i,1)<0)
			    {
			      fprintf(stderr, "Failed to write to terminal\n", strerror(errno));
			      tcsetattr(0, TCSANOW, &ini_state);
			      exit(1);
			    }
			  if(write(to_shell[1],buff+i,1)<0)
                            {
                              fprintf(stderr, "Failed to write to shell\n", strerror(errno));
                              tcsetattr(0, TCSANOW, &ini_state);
                              exit(1);
                            }
			}
		    }
		  free(buff);
		}
	      if(fds[1].revents&&POLLIN)//shell
		{
		  char* buff=malloc(256*sizeof(char));
		  int n=read(from_shell[0], buff, 256);
		  int i;
		  for(i=0;i<n;i++)
		    {
		      if(buff[i]==0x0D || buff[i]==0x0A)
			{
			  char* buff2=malloc(2*sizeof(char));
			  buff2[0]='\r';
			  buff2[1]='\n';
			  if(write(1, buff2, 2)<0)
			    {
			      fprintf(stderr, "Failed to write to terminal\n", strerror(errno));
                              tcsetattr(0, TCSANOW, &ini_state);
                              exit(1);
			    }
			}
		      else
			{
			  if(write(1, buff+i, 1)<0)
                            {
                              fprintf(stderr, "Failed to write to terminal\n", strerror(errno));
                              tcsetattr(0, TCSANOW, &ini_state);
                              exit(1);
                            }  
			}
		    }
		}
	    }
	  if(fds[1].revents&&(POLLHUP||POLLERR))
	    {
	      if(close(to_shell[1])<0)
		{
		  fprintf(stderr, "Parent process failed to close pipe end\n");
		  tcsetattr(0, TCSANOW, &ini_state);
		  exit(1);
		}
	      tcsetattr(0, TCSANOW, &ini_state);
	      exit(0);
	    }
	}
    }
 else
   {
     char* buff= malloc(100*sizeof(char));
     int n=read(0, buff, 100);
     if(n<0)
       {
	 fprintf(stderr, "Default option read call failed\n", strerror(errno));
	 tcsetattr(0, TCSANOW, &ini_state);
	 exit(1);
       }
     int run_flag=0;
     while(n>0)
       {
	 if(n<0)
	   {
	     fprintf(stderr, "Default option read call failed\n", strerror(errno));
	     tcsetattr(0, TCSANOW, &ini_state);
	     exit(1);
	   }
	 int i;
	 for(i=0;i<n;i++)
	   {
	     if(buff[i]=='\r' || buff[i]=='\n')
	       {
		 char* buff2= malloc(2*sizeof(char));
		 buff2[0]='\r';
		 buff2[1]='\n';
		 if(write(1,buff2,2)<0)
		   {
		      fprintf(stderr, "Default option failed to write carriage return and newline\n");
		      tcsetattr(0, TCSANOW, &ini_state);
		      exit(1);
		   }
		  free(buff2);
		}
	      else if(buff[i]==0x04)
		run_flag=1;
	      else
		{
		  if(write(1,buff+i,1)<0)
		    {
		      fprintf(stderr, "Default option failed to write\n", strerror(errno));
		      tcsetattr(0, TCSANOW, &ini_state);
		      exit(1);
		    }
		}
	    }
	 if(run_flag)
	   break;
	  n=read(0, buff, 100);
       }
     free(buff);
   } 
  tcsetattr(0, TCSANOW, &ini_state);
 exit(0);
}

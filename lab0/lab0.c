/*NAME:Bianheng Cao
EMAIL:bianhengcao@gmail.com
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

void seg_handler(int signum)
  {
    fprintf(stderr, "--catch caught a segmentation fault caused by --segfault!\n");
    exit(4);
  } 

int main(int argc, char **argv)
{
  static int segfault=0;
  static int catch=0;
  int ifd;
  int ofd;
  int c;
  while(1)
  {
    static struct option long_options[]=
      {
	{"input", required_argument, NULL, 'i'},
	{"output", required_argument, NULL, 'o'},
	{"segfault", no_argument, &segfault, 1},
	{"catch", no_argument, &catch, 1},
	{0,0,0,0}
      };
    int option_index;
    c=getopt_long(argc, argv, "i:o:", long_options, &option_index);
    if(c==-1)
      break;
    switch(c)
      {
      case 0:
	break;
      case 'i':
        ifd=open(optarg, O_RDONLY);
      	if(ifd<0)
	  {
	    fprintf(stderr, "the option --input with argument %s failed: %s\n", optarg, strerror(errno));
	    exit(2);
	  }
	close(0);
	dup(ifd);
	close(ifd);
	break;
      case 'o':
	ofd = open(optarg, O_RDONLY);
	if(ofd > 0)
	  {
	    fprintf(stderr, "the option --output with argument %s failed: file already exists\n", optarg);
	    exit(3);
	  }
	ofd = creat(optarg, 0666);
	if(ofd<0)
	  {
	    fprintf(stderr, "the option --output with argument %s failed: %s\n", optarg, strerror(errno));
	    exit(3);
	  }
	close(1);
	dup(ofd);
	close(ofd);
	break;
      default:
	fprintf(stderr, "Please enter valid arguement\n");
	exit(1);
      }
  }
  if(catch)
    {
      signal(SIGSEGV, seg_handler);
    }
  if(segfault)
    {
      char* ptr=NULL;
      *ptr="Segmentation fault";
    }
  char* arr = malloc(sizeof(char));
  while(read(0, arr, sizeof(char) != 0))
	write(1, arr, sizeof(char));
  free(arr);
  close(0);
  close(1);
  exit(0);
}

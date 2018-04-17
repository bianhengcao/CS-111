/*NAME:Bianheng Cao
EMAIL:bianhengcao@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sched.h>

static int opt_yield=0;
static int mtx=0;
static int spin=0;
static int cas=0;
pthread_mutex_t lock;
int spin_lock=0;

struct args
{
  long long* ptr;
  int itr;
};

void add(long long *pointer, long long value) 
{
  long long sum = *pointer + value;
  if(opt_yield)
    sched_yield();
  *pointer = sum;
}

void add2(struct args* arg)
{
  int i;
  for(i=0;i<arg->itr;i++)
    {
      if(mtx)
	{
	  pthread_mutex_lock(&lock);
	  add(arg->ptr,1);
	  add(arg->ptr,-1);
	  pthread_mutex_unlock(&lock);
	}
      else if(spin)
	{
	  while(__sync_lock_test_and_set(&spin_lock,1)==1)
	    ;
	  add(arg->ptr,1);
          add(arg->ptr,-1);
	  __sync_lock_release(&spin_lock);
	}
      else if(cas)
	{
	  while(__sync_val_compare_and_swap(&spin_lock,0,1)==1)
	    ;
	  add(arg->ptr,1);
          add(arg->ptr,-1);
	  __sync_lock_release(&spin_lock);
	}
      else
	{
	  add(arg->ptr,1);
          add(arg->ptr,-1);
	}
    }
  pthread_exit(NULL);
}

int main(int argc, char** argv)
{
  static int t_num=1;
  static int itr=1;
  while(1)
    {
    static struct option long_options[]=
      {
        {"threads", required_argument, NULL, 't'},
	{"iterations", required_argument, NULL, 'i'},
	{"yield", no_argument, NULL, 'y'},
	{"sync", required_argument, NULL, 's'},
	{0,0,0,0}
      };
    int option_index;
    int c=getopt_long(argc, argv, "t:i:ymsc", long_options, &option_index);
    if(c==-1)
      break;
    switch(c)
      {
      case 't':
	t_num=atoi(optarg);
	if(t_num<1)
	  {
	    fprintf(stderr, "Enter a valid argument for threads\n");
	    exit(1);
	  }
	break;
      case 'i':
	itr=atoi(optarg);
	if(itr<1)
	  {
	    fprintf(stderr, "Enter a valid argument for iterations\n");
	    exit(1);
	  }
	break;
      case 'y':
        opt_yield=1;
        break;
      case 's':
	if(optarg[0]=='m')
	  {
	    mtx=1;
	    pthread_mutex_init(&lock, NULL);
	  }
	else if(optarg[0]== 's')
	  spin=1;
	else if(optarg[0]=='c')
	  cas=1;
	break;
      default:
	fprintf(stderr, "Please enter a valid option with the correct argument(s)\n");
	exit(1);
      }
    }
  long long count=0;
  clockid_t id=CLOCK_REALTIME;
  struct timespec start;
  if(clock_gettime(id, &start)<0)
    {
      fprintf(stderr, "Failed to get clock start:%s\n", strerror(errno));
      exit(1);
    }
  pthread_t ids[t_num];
  struct args arg;
  arg.ptr=&count;
  arg.itr=itr;
  int i;
  for(i=0;i<t_num;i++)
    {      
      if(pthread_create(&ids[i], NULL, &add2, &arg) != 0)
	{
	  fprintf(stderr, "Failed to create thread:%s\n", strerror(errno));
	  exit(1);
	}
    }
  for(i=0;i<t_num;i++)
    {
      if(pthread_join(ids[i], NULL) != 0)
        {
          fprintf(stderr, "Failed to join thread:%s\n", strerror(errno));
          exit(1);
        }
    }
  pthread_mutex_destroy(&lock);
  struct timespec end;
  if(clock_gettime(id, &end)<0)
    {
      fprintf(stderr, "Failed to get clock resolution:%s\n", strerror(errno));
      exit(1);
    }
  int ops=2*t_num*itr;
  long ttl_time=1000000000*(end.tv_sec-start.tv_sec) + end.tv_nsec-start.tv_nsec;
  long avg_time=ttl_time/ops;
  fprintf(stdout, "add");
  if(opt_yield)
    fprintf(stdout, "-yield");
  if(mtx)
    fprintf(stdout, "-m");
  else if(spin)
    fprintf(stdout, "-s");
  else if(cas)
    fprintf(stdout, "-c");
  else
    fprintf(stdout, "-none");
  fprintf(stdout, ",%d,%d,%d,%ld,%ld,%lld\n", t_num, itr, ops, ttl_time, avg_time, count);
  exit(0);
}

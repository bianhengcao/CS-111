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
#include <signal.h>
#include "SortedList.h"

static int t_num=1;
static int itr=1;
int opt_yield=0;
static int mtx=0;
static int spin=0;
pthread_mutex_t lock;
int spin_lock=0;
SortedList_t* list;
SortedListElement_t* arr;

struct args
{
  SortedListElement_t* element;
  int itr;
};

void handler(int num)
{
  if(num==SIGSEGV)
    {
      fprintf(stderr, "Caught a sgmentation fault\n");
      exit(2);
    }
}

void add(SortedListElement_t* arg)
{
  SortedListElement_t* start=arg;
  if(mtx)
    {
      int i;
      for(i=0;i<itr;i++)
	{
	  pthread_mutex_lock(&lock);
	  SortedList_insert(list, start+i);
	  pthread_mutex_unlock(&lock);
	}
      pthread_mutex_lock(&lock);
      int len=SortedList_length(list);
      pthread_mutex_unlock(&lock);
      if(len<0)
        {
          fprintf(stderr, "SortedList_length failed\n");
          exit(2);
        }
      for(i=0;i<itr;i++)
        {
	  pthread_mutex_lock(&lock);
          SortedListElement_t* ptr  = SortedList_lookup(list, start[i].key);
          if(ptr==NULL)
            {
              fprintf(stderr, "SortedList_lookup failed\n");
              exit(2);
            }
          if(SortedList_delete(ptr)!=0)
            {
              fprintf(stderr, "SortedList_delete failed\n");
              exit(2);
            }
	  pthread_mutex_unlock(&lock);
        }
    }
  else if(spin)
    {
      int i;
      for(i=0;i<itr;i++)
	{
	  while(__sync_lock_test_and_set(&spin_lock, 1) ==1)
		;
	  SortedList_insert(list, start+i);
	  __sync_lock_release(&spin_lock);
	}
      while(__sync_lock_test_and_set(&spin_lock, 1) == 1)
	;
      if(SortedList_length(list)<0)
	{
	  fprintf(stderr, "SortedList_length failed\n");
	  exit(2);
	}
      __sync_lock_release(&spin_lock);
      for(i = 0;i<itr;i++){
	while(__sync_lock_test_and_set(&spin_lock, 1) == 1)
	  ;
	SortedListElement_t* ptr = SortedList_lookup(list, start[i].key);
	if(ptr==NULL)
	  {
	    fprintf(stderr,"SortedList_lookup failed\n");
	    exit(2);
	  }
	if(SortedList_delete(ptr)!= 0){
	  fprintf(stderr, "SortedList_delete failed\n");
	  exit(2);
	}
	__sync_lock_release(&spin_lock);
      }
    }
  else
    {
      int i;
      for(i=0;i<itr;i++)
	SortedList_insert(list, start+i);
      int len=SortedList_length(list);
      if(len<0)
	{
	  fprintf(stderr, "SortedList_length failed\n");
	  exit(2);
	}
      for(i=0;i<itr;i++)
	{
	  SortedListElement_t* ptr  = SortedList_lookup(list, start[i].key);
	  if(ptr==NULL)
	    {
	      fprintf(stderr, "SortedList_lookup failed\n");
	      exit(2);
	    }
	  if(SortedList_delete(ptr)!=0)
	    {
              fprintf(stderr, "SortedList_delete failed\n");
              exit(2);
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
        {"threads", required_argument, NULL, 't'},
	{"iterations", required_argument, NULL, 'i'},
	{"yield", required_argument, NULL, 'y'},
	{"sync", required_argument, NULL, 's'},
	{0,0,0,0}
      };
    int option_index;
    int in;
    int c=getopt_long(argc, argv, "t:i:y:s:", long_options, &option_index);
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
        for(in=0;in<strlen(optarg);in++)
	  {
	    if(optarg[in]=='i')
	      opt_yield = opt_yield|INSERT_YIELD;
	    else if(optarg[in]=='d')
	      opt_yield = opt_yield|DELETE_YIELD;
	    else if(optarg[in]=='l')
	      opt_yield = opt_yield|LOOKUP_YIELD;
	  }
        break;
      case 's':
	if(optarg[0]=='m')
	  {
	    mtx=1;
	    pthread_mutex_init(&lock, NULL);
	  }
	else if(optarg[0]== 's')
	  spin=1;
	break;
      default:
	fprintf(stderr, "Please enter a valid option with the correct argument(s)\n");
	exit(1);
      }
    }
  if(signal(SIGSEGV, handler)<0)
    {
      fprintf(stderr, "Signal system call failed%s\n", strerror(errno));
      exit(1);
    }
  list=malloc(sizeof(SortedList_t));
  list->key=NULL;
  list->next=list->prev=list;
  int num=itr*t_num;
  SortedListElement_t* arr = malloc(num*sizeof(SortedListElement_t));
  int j;
  for(j=0;j<num;j++){
    char* key = malloc(1024*sizeof(char));//size of one key
    int rand_int=rand()%26;
    int z;
    for(z=0;z<1023;z++){
      key[z]='a'+rand_int;//random string
      rand_int=rand()%26;
    }
    key[1023]='\0';
    arr[j].key=(const char*)key;
  }
  clockid_t id=CLOCK_REALTIME;
  struct timespec start;
  if(clock_gettime(id, &start)<0)
    {
      fprintf(stderr, "Failed to get clock start:%s\n", strerror(errno));
      exit(1);
    }
  pthread_t ids[t_num];
  int i;
  for(i=0;i<t_num;i++)
    {
      if(pthread_create(&ids[i], NULL, &add, arr+i*itr) != 0)
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
  struct timespec end;
  if(clock_gettime(id, &end)<0)
    {
      fprintf(stderr, "Failed to get clock resolution:%s\n", strerror(errno));
      exit(1);
    }
  free(list);
  free(arr);
  pthread_mutex_destroy(&lock);
  int ops=3*t_num*itr;
  long ttl_time=1000000000*(end.tv_sec-start.tv_sec) + end.tv_nsec-start.tv_nsec;
  long avg_time=ttl_time/ops;
  fprintf(stdout, "list-");
  if(opt_yield & INSERT_YIELD)
    fprintf(stdout, "i");
  if(opt_yield & DELETE_YIELD)
    fprintf(stdout, "d");
  if(opt_yield & LOOKUP_YIELD)
    fprintf(stdout, "l");
  if(opt_yield ==0)
    fprintf(stdout, "none");
  if(mtx)
    fprintf(stdout, "-m");
  else if(spin)
    fprintf(stdout, "-s");
  else
    fprintf(stdout, "-none");
  fprintf(stdout, ",%d,%d,%d,%d,%ld,%ld\n", t_num, itr, 1, ops, ttl_time, avg_time);
  exit(0);
}

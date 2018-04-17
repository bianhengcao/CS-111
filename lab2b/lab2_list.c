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
static int l_num=1;
int opt_yield=0;
static int mtx=0;
static int spin=0;
pthread_mutex_t* locks;
int* spin_locks;
SortedList_t** list;
SortedListElement_t* arr;

unsigned long hash(const char* key)//CRC hash function from https://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200101/homework10/hashfuncs.html
{
  unsigned long hash=0;
  int i;
  int len=strlen(key);
  int j;
  for(i=0;i<len;i++)
    {
      j=hash&0xf8000000;
      hash=hash<<5;
      hash=hash^(j>>27);
      hash=hash^(key[i]);
    }
  return hash;
}

void handler(int num)
{
  if(num==SIGSEGV)
    {
      fprintf(stderr, "Caught a segmentation fault\n");
      exit(2);
    }
}

void add(SortedListElement_t* arg)
{
  SortedListElement_t* start=arg;
  clockid_t id=CLOCK_REALTIME;
  struct timespec start_t;
  struct timespec end;
  long long ret=0;
  if(mtx)
    {
      int i;
      for(i=0;i<itr;i++)//insert
	{
	  int j=hash((start+i)->key)%l_num;
	  if(clock_gettime(id, &start_t)<0)
	    {
	      fprintf(stderr, "Failed to get clock start:%s\n", strerror(errno));
	      exit(1);
	    }
	  pthread_mutex_lock(locks+j);
	  if(clock_gettime(id, &end)<0)
	    {
	      fprintf(stderr, "Failed to get clock resolution:%s\n", strerror(errno));
	      exit(1);
	    }
	  ret+=1000000000*(end.tv_sec-start_t.tv_sec) + end.tv_nsec-start_t.tv_nsec;
	  SortedList_insert(list[j], start+i);
	  pthread_mutex_unlock(locks+j);
	}

      int len=0;
      for(i=0;i<l_num;i++)//enumerate through every list for count
	{
	  if(clock_gettime(id, &start_t)<0)
	    {
	      fprintf(stderr, "Failed to get clock start:%s\n", strerror(errno));
	      exit(1);
	    }
	  pthread_mutex_lock(locks+i);
	  if(clock_gettime(id, &end)<0)
	    {
	      fprintf(stderr, "Failed to get clock resolution:%s\n", strerror(errno));
	      exit(1);
	    }
	  ret+=1000000000*(end.tv_sec-start_t.tv_sec) + end.tv_nsec-start_t.tv_nsec;
	  int l=SortedList_length(list[i]);
	  if(l<0)
            {
              fprintf(stderr, "SortedList_length failed\n");
              exit(2);
            }
	  len+=l;
	  pthread_mutex_unlock(locks+i);
	}

      for(i=0;i<itr;i++)//lookup and delete
        {
	  int j=hash((start+i)->key)%l_num;
	  if(clock_gettime(id, &start_t)<0)
	    {
	      fprintf(stderr, "Failed to get clock start:%s\n", strerror(errno));
	      exit(1);
	    }
	  pthread_mutex_lock(locks+j);
	  if(clock_gettime(id, &end)<0)
	    {
	      fprintf(stderr, "Failed to get clock resolution:%s\n", strerror(errno));
	      exit(1);
	    }
	  ret+=1000000000*(end.tv_sec-start_t.tv_sec) + end.tv_nsec-start_t.tv_nsec;
          SortedListElement_t* ptr  = SortedList_lookup(list[j], start[i].key);
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
	  pthread_mutex_unlock(locks+j);
        }
    }
  else if(spin)
    {
      int i;
      for(i=0;i<itr;i++)//insert
	{
	  int j=hash((start+i)->key)%l_num;
	  if(clock_gettime(id, &start_t)<0)
	    {
	      fprintf(stderr, "Failed to get clock start:%s\n", strerror(errno));
	      exit(1);
	    }
	  while(__sync_lock_test_and_set(spin_locks+j,1)==1)
            ;
	  if(clock_gettime(id, &end)<0)
	    {
	      fprintf(stderr, "Failed to get clock resolution:%s\n", strerror(errno));
	      exit(1);
	    }
	  ret+=1000000000*(end.tv_sec-start_t.tv_sec)+end.tv_nsec-start_t.tv_nsec;
	  SortedList_insert(list[j], start+i);
	  __sync_lock_release(spin_locks+j);
	}

      int len=0;
      for(i=0;i<l_num;i++)//enumerate through every list for count
	{
	  if(clock_gettime(id, &start_t)<0)
	    {
	      fprintf(stderr, "Failed to get clock start:%s\n", strerror(errno));
	      exit(1);
	    }
	  while(__sync_lock_test_and_set(spin_locks+i, 1) ==1)
	    ;
	  if(clock_gettime(id, &end)<0)
	    {
	      fprintf(stderr, "Failed to get clock resolution:%s\n", strerror(errno));
	      exit(1);
	    }
	  ret+=1000000000*(end.tv_sec-start_t.tv_sec) + end.tv_nsec-start_t.tv_nsec;
	  int l=SortedList_length(list[i]);
          if(l<0)
            {
              fprintf(stderr, "SortedList_length failed\n");
              exit(2);
            }
          len+=l;
	  __sync_lock_release(spin_locks+i);
	}
      
      for(i = 0;i<itr;i++)//lookup and delete
	{
	  int j=hash((start+i)->key)%l_num;
	  if(clock_gettime(id, &start_t)<0)
	    {
	      fprintf(stderr, "Failed to get clock start:%s\n", strerror(errno));
	      exit(1);
	    }
	  while(__sync_lock_test_and_set(spin_locks+j, 1) ==1)
            ;
	  if(clock_gettime(id, &end)<0)
	    {
	      fprintf(stderr, "Failed to get clock resolution:%s\n", strerror(errno));
	      exit(1);
	    }
	  ret+=1000000000*(end.tv_sec-start_t.tv_sec) + end.tv_nsec-start_t.tv_nsec;
	  SortedListElement_t* ptr = SortedList_lookup(list[j], start[i].key);
	  if(ptr==NULL)
	    {
	      fprintf(stderr,"SortedList_lookup failed\n");
	      exit(2);
	    }
	  if(SortedList_delete(ptr)!= 0)
	    {
	      fprintf(stderr, "SortedList_delete failed\n");
	      exit(2);
	    }
	  __sync_lock_release(spin_locks+j);
	}
    }
  else
    {
      int i;
      for(i=0;i<itr;i++)//insert
	{
	  int j=hash((start+i)->key)%l_num;
	  SortedList_insert(list[j], start+i);
	}
      
      int len=0;
      for(i=0;i<l_num;i++)//enumerate through every list for count
	{
	  int l=SortedList_length(list[i]);
	  if(l<0)
	    {
	      fprintf(stderr, "SortedList_length failed\n");
	      exit(2);
	    }
	  len+=l;
	}

      for(i=0;i<itr;i++)//lookup and delete
	{
	  int j=hash((start+i)->key)%l_num;
	  SortedListElement_t* ptr  = SortedList_lookup(list[j], start[i].key);
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
  pthread_exit(ret);
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
	{"lists", required_argument, NULL, 'l'},
	{0,0,0,0}
      };
    int option_index;
    int in;
    int c=getopt_long(argc, argv, "t:i:y:s:l:", long_options, &option_index);
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
	  mtx=1;
	else if(optarg[0]== 's')
	  spin=1;
	break;
      case 'l':
	l_num=atoi(optarg);
	if(l_num<1)
	  {
            fprintf(stderr, "Enter a valid argument for list number\n");
            exit(1);
          }
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
  list=malloc(l_num*sizeof(SortedList_t*));
  locks=malloc(l_num*sizeof(pthread_mutex_t));
  spin_locks=malloc(l_num*sizeof(int));
  int z;
  for(z=0;z<l_num;z++)
    {
      list[z]=malloc(sizeof(SortedList_t));
      (list[z])->key=NULL;
      (list[z])->next=list[z];
      (list[z])->prev=list[z];
      pthread_mutex_init(locks+z, NULL); 
      spin_locks[z]=0;
    }
  int num=itr*t_num;
  SortedListElement_t* arr = malloc(num*sizeof(SortedListElement_t));
  int j;
  for(j=0;j<num;j++)
    {
      char* key = malloc(128*sizeof(char));//size of one key
      int rand_int=rand()%26;
      int z;
      for(z=0;z<127;z++){
	key[z]='a'+rand_int;//random string
	rand_int=rand()%26;
      }
      key[127]='\0';
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
  long long times=0;
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
      void* time;
      if(pthread_join(ids[i], &time) != 0)
        {
          fprintf(stderr, "Failed to join thread:%s\n", strerror(errno));
          exit(1);
        }
      times+=time;
    }
  if(t_num==1)
    times=0;
  struct timespec end;
  if(clock_gettime(id, &end)<0)
    {
      fprintf(stderr, "Failed to get clock resolution:%s\n", strerror(errno));
      exit(1);
    } 
  for(z=0;z<l_num;z++)
    free(list[z]);
  for(j=0;j<num;j++)
      free(arr[j].key);
  free(list);
  free(arr);
  for(z=0;z<l_num;z++)
      pthread_mutex_destroy(locks+z);
  free(locks);
  free(spin_locks);
  int ops=3*t_num*itr;
  long ttl_time=1000000000*(end.tv_sec-start.tv_sec) + end.tv_nsec-start.tv_nsec;
  long avg_time=ttl_time/ops;
  long lock_avg=times/(t_num*(2*itr+1));
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
  fprintf(stdout, ",%d,%d,%d,%d,%ld,%ld,%lld\n", t_num, itr, l_num, ops, ttl_time, avg_time, lock_avg);
  exit(0);
}

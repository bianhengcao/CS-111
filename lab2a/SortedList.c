#include "SortedList.h"
#include <stdio.h>
#include <string.h>
#include <sched.h>

void SortedList_insert(SortedList_t *list, SortedListElement_t *element)
{
  SortedListElement_t* ptr=list->next;
  while( ptr->key!=NULL && (strcmp(ptr->key, element->key)<0)) 
    ptr=ptr->next;
  ptr->prev->next=element;
  element->prev=ptr->prev;
  if(opt_yield & INSERT_YIELD)
    sched_yield();
  element->next=ptr;
  ptr->prev=element;
}

int SortedList_delete(SortedListElement_t *element)
{
  if(element==NULL)
    return 1;
  if(element->next->prev != element->prev->next)
    return 1;
  if(opt_yield & DELETE_YIELD)
    sched_yield();
  element->next->prev=element->prev;
  element->prev->next=element->next;
  return 0;
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key)
{
  SortedListElement_t* ptr=list->next;
  while(ptr!=list)
    {
      if(strcmp(ptr->key, key)==0)
	return ptr;
      if(opt_yield & LOOKUP_YIELD)
	sched_yield();
      ptr=ptr->next;
    }
  return NULL;
}

int SortedList_length(SortedList_t *list)
{
  int count=0;
  SortedListElement_t* ptr=list->next;
  while(ptr!=list)
    {
      if(ptr->prev->next!=ptr||ptr->next->prev!=ptr)
	return -1;
      count++;
      if(opt_yield & LOOKUP_YIELD)
        sched_yield();
      ptr=ptr->next;
    }
  return count;
}

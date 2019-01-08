#include "alloc.h"

#ifndef _SYS/TYPES_H_
#define _SYS/TYPES_H_
#include <sys/types.h>
#endif

#ifndef _STDIO_H_
#define _STDIO_H_
#include <stdio.h>
#endif

void *base=0;
void *end=0;
meta *find_location(size_t size);
extern int type=FIRST;

void *m_malloc(size_t size)
{
  meta *target;

  if(base==0){
    base=sbrk(0);
    end=base;
  }
  if(size%4!=0){
    size=(size/4+1)*4;
  }
  size_t len=size+METASIZE;
  if(end==base)
    target=-1;
  else{
    target=find_location(size);
  }
  if(target==-1 || (!target->next && (!target->free || target->free && target->size<size))){
    meta *new=end;
    end+=len;
    if(sbrk(len)==-1){
      return 0;
    }
    new->free=0;
    new->next=0;
    new->prev=target;
    new->size=size;
    if(target!=-1){
      target->next=new;
    }
    target=new;
  }else{
    m_realloc(target->data,size);
  }
  return target->data;
}

void m_free(void *point)
{
  meta *current=point-METASIZE;
  current->free=1;
  if(current->next && current->next->free==1){
    current->size+=current->next->size + METASIZE;
    current->next=current->next->next;
  }
  if(current->prev!=-1){
    if(current->prev->free){
      current=current->prev;
      current->size+=current->next->size+METASIZE;
      current->next=current->next->next;
    }
    if(!current->next){
      end-=current->size+METASIZE;
      current->prev->next=0;
    }
  }else if(!current->next && !current->prev)
    end=brk(base);
  point=0;
}

meta *find_location(size_t size)
{
  meta *index=base,*location=-1;
  int max=0;
  int min=2e9;
  switch(type){
    case FIRST:
      for(;;){
        if(index->free && index->size>=size){
          location=index;
          break;
        }
        if(index->next==NULL){
          location=index;
          break;
        }
        index=index->next;
        if(index==NULL)
          break;
      }
    break;
    case BEST:
      for(;;){
        if(index->free == 1 && index->size >= size && index->size<min){
     	  location=index;
     	  min=index->size;
   	}
        if(location==-1 && !index->next)
          location=index;
        index = index->next;
        if(index)
          break;
      }
    break;
    case WORST:
      for(;;){
        if((index->size)>=size && index->free){
          if(index->size>max)
            location=index;
        }
        if(location==-1 && !index->next)
          location=index;
        index=index->next;
	if(index)
	  break;
      }
    break;
    }
  return location;
}


void *m_realloc(void* pointer, size_t size){
  meta *current = pointer-METASIZE;

  if(size%4!=0){
    size=(size/4+1)*4;
  }

  if(current->size == size)
    return pointer;

  else if(current->size < size){
    if(current->next && current->next->free && current->size + current->next->size + METASIZE>=size){
      current->size += current->next->size + METASIZE;
      current->next = current->next->next;
      current->next->prev = current;
      if(((current->size)-size) < METASIZE){
        return pointer;
      }else{
        meta *new = (int)current + size + METASIZE;
        new->prev = current;
        new->next = current->next;
        new->size = current->size - size - METASIZE;
        current->next = new;
        current->size = size;
        current->free = 0;
        m_free(new->data);
        return current->data;
      }
    }else {
      m_free(current->data);
      void * new = m_malloc(size);
      strcpy(new, pointer);
      return new;
    }
  }
  else if(((current->size)-size) < METASIZE){
    return pointer;
  }
  else{
    meta *new = (int)current + size + METASIZE;
    new->prev = current;
    new->next = current->next;
    new->size = current->size - size - METASIZE;
    current->next = new;
    current->size = size;
    current->free = 0;
    m_free(new->data);
    return current->data;
  }
}

void print_list()
{
  meta *current=base;

  while(current)
  {
    printf("%d %d ",current->free,current->size);
    if(!current->free)
      printf("%s\n",current->data);
    else
      printf("\n");
    current=current->next;
  }

}

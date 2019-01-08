#ifndef _STDINT_H_
#define _STDINT_H_
#include <stdint.h>
#endif

#ifndef _SYS/TYPES_H_
#define _SYS/TYPES_H_
#include <sys/types.h>
#endif

#ifndef _ALLOC_H_
#define _ALLOC_H_

#define FIRST 1
#define BEST 2
#define WORST 4
#define METASIZE 16

extern int type;

typedef struct meta_struct {
uint32_t size;
uint32_t free;
struct meta_struct *next;
struct meta_struct *prev;

char data[1];
} meta;

void *m_malloc(size_t size);
void m_free(void *point);
void *m_realloc(void* point,size_t size);

#endif


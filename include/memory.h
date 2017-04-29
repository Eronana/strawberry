#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <cstdlib>
#define NEW(T) ((T*)alloc(sizeof(T)))
void *alloc(size_t size);

#endif

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <cstdlib>
#define NEW(T) new(alloc(sizeof(T))) T
void *alloc(size_t size);

#endif

#include "memory.h"
#include <cstdlib>

void *alloc(size_t size)
{
    return malloc(size);
}
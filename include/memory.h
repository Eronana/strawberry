#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <cstdlib>
#include "stack.hpp"
#include "vm_data.h"

const size_t MIN_GC_SIZE=1024;
extern bool gc_enable;
STRING_TYPE newString();
ARRAY_TYPE newArray();
OBJECT_TYPE newObject();
void gc();
void setRoot(Stack<V_VALUE> *v_stack);
void addExtraRoot(V_VALUE *root);
#endif

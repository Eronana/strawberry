#include "memory.h"
#include <cstdlib>
#include <map>
#include <algorithm>
using namespace std;
struct MemoryUnit
{
    bool live;
    size_t size;
    DATATYPE type;
};
map<void*,MemoryUnit> memory_table;
size_t total_size;
size_t base_size=MIN_GC_SIZE;
Stack<V_VALUE> *root;

#define DEF_NEW_METHOD(A,B) \
B##_TYPE new##A()\
{\
    if(total_size>base_size*2)gc();\
    B##_TYPE ret=new B##_OTYPE;\
    total_size+=sizeof(B##_OTYPE);\
    memory_table[ret]={false,sizeof(B##_OTYPE),T_##B};\
    return ret;\
}

DEF_NEW_METHOD(String,STRING)
DEF_NEW_METHOD(Array,ARRAY)
DEF_NEW_METHOD(Object,OBJECT)


void setRoot(Stack<V_VALUE> *v_stack)
{
    root=v_stack;
}
bool markLive(void *p,DATATYPE type)
{
    if(!memory_table.count(p))return false;
    if(memory_table[p].live)return false;
    memory_table[p].live=true;
    return true;
}
void markDie()
{
    for(auto &x:memory_table)x.second.live=false;
}
void sweep(V_VALUE &val)
{
    switch(val.type)
    {
        case T_STRING:
            markLive(val.v_string,T_STRING);
            break;
        case T_ARRAY:
            if(markLive(val.v_array,T_ARRAY))for(auto &x:*val.v_array)sweep(x); 
            break;
        case T_OBJECT:
            if(markLive(val.v_object,T_OBJECT))for(auto &x:*val.v_object)sweep(x.second);
            break;
    }
}

void gc()
{
    markDie();
    for(int i=1;i<root->size();i++)sweep((*root)[i]);
    total_size=0;
    for(decltype(memory_table)::iterator next=memory_table.begin(),iter;next!=memory_table.end();)
    {
        iter=next++;
        if(iter->second.live)
        {
            total_size+=iter->second.size;
            continue;
        }
        switch(iter->second.type)
        {
            case T_STRING:
                delete reinterpret_cast<STRING_TYPE>(iter->first);
                break;
            case T_ARRAY:
                delete reinterpret_cast<ARRAY_TYPE>(iter->first);
                break;
            case T_OBJECT:
                delete reinterpret_cast<OBJECT_TYPE>(iter->first);
                break;
        }
        memory_table.erase(iter);
    }
    base_size=max(MIN_GC_SIZE,total_size);
}

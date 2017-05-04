#include "memory.h"
#include <cstdlib>
#include <map>
#include <algorithm>
#include <vector>
using namespace std;
struct MemoryUnit
{
    bool alive;
    size_t size;
    DATATYPE type;
};
map<void*,MemoryUnit> memory_table;
size_t total_size;
size_t base_size=MIN_GC_SIZE;
Stack<V_VALUE> *root;
vector<V_VALUE*> extraRoot;
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

void addExtraRoot(V_VALUE *root)
{
    extraRoot.push_back(root);
}
void setRoot(Stack<V_VALUE> *v_stack)
{
    root=v_stack;
}
bool mark_alive(void *p,DATATYPE type)
{
    if(!memory_table.count(p))return false;
    if(memory_table[p].alive)return false;
    memory_table[p].alive=true;
    return true;
}

void mark_value(V_VALUE &val)
{
    switch(val.type)
    {
        case T_STRING:
            mark_alive(val.v_string,T_STRING);
            break;
        case T_ARRAY:
            if(mark_alive(val.v_array,T_ARRAY))for(auto &x:*val.v_array)mark_value(x); 
            break;
        case T_OBJECT:
            if(mark_alive(val.v_object,T_OBJECT))for(auto &x:*val.v_object)mark_value(x.second);
            break;
    }
}

void mark()
{
    // mark all as dead
    for(auto &x:memory_table)x.second.alive=false;

    // mark alive
    for(int i=1;i<root->size();i++)mark_value((*root)[i]);
    for(auto x:extraRoot)mark_value(*x);
}

void sweep()
{
    // re-calculate total size
    total_size=0;
    for(decltype(memory_table)::iterator next=memory_table.begin(),iter;next!=memory_table.end();)
    {
        iter=next++;
        if(iter->second.alive)
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
}

void gc()
{
    mark();
    sweep();
    base_size=max(MIN_GC_SIZE,total_size);
}

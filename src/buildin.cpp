#include "buildin.h"
#include "memory.h"
#include "vm.h"

BUILD_FUNC_SIGN(print)
{
    if(argc)fputs(GET_ARGV(0).toString().c_str(),stdout);
    for(int i=1;i<argc;i++)fprintf(stdout," %s",GET_ARGV(i).toString().c_str());
}

BUILD_FUNC_SIGN(println)
{
    GET_BUILDIN_FUNC_NAME(print)(argc,vm);
    putchar('\n');
}

BUILD_FUNC_SIGN(scani)
{
    vm.reg_ret.type=T_INT;
    scanf("%d",&vm.reg_ret.v_int);
}
BUILD_FUNC_SIGN(scanf)
{
    vm.reg_ret.type=T_FLOAT;
    scanf("%lf",&vm.reg_ret.v_float);
}

BUILD_FUNC_SIGN(scans)
{
    char s[1000];
    vm.reg_ret.type=T_STRING;
    scanf("%s",s);
    vm.reg_ret.v_string=newString();
    *vm.reg_ret.v_string=s;
}

BUILD_FUNC_SIGN(exit)
{
    vm.next_ip=0x7fffffff;
}

BUILD_FUNC_SIGN(len)
{
    vm.reg_ret.type=T_INT;
    switch(GET_ARGV(0).type)
    {
        case T_ARRAY:
            vm.reg_ret.v_int=GET_ARGV(0).v_array->size();
            break;
        case T_OBJECT:
            vm.reg_ret.v_int=GET_ARGV(0).v_object->size();
            break;
        default:
            vm.reg_ret.v_int=1;
    }
}

BUILD_FUNC_SIGN(push)
{
    if(GET_ARGV(0).type!=T_ARRAY)return;
    for(int i=1;i<argc;i++)GET_ARGV(0).v_array->push_back(GET_ARGV(i));
    vm.reg_ret=GET_ARGV(0);
}

BUILD_FUNC_SIGN(clear)
{
    if(GET_ARGV(0).type==T_ARRAY)GET_ARGV(0).v_array->clear();
    else if(GET_ARGV(0).type==T_OBJECT)GET_ARGV(0).v_object->clear();
    else return;
    vm.reg_ret=GET_ARGV(0);
}

BUILD_FUNC_SIGN(resize)
{
    if(GET_ARGV(0).type!=T_ARRAY)return;
    GET_ARGV(0).v_array->resize(GET_ARGV(2));
    vm.reg_ret=GET_ARGV(0);
}

BUILD_FUNC_SIGN(get_keys)
{
    vm.reg_ret.type=T_ARRAY;
    vm.reg_ret.v_array=newArray();
    if(GET_ARGV(0).type==T_OBJECT)
    {
        int i=0;
        vm.reg_ret.v_array->resize(GET_ARGV(0).v_object->size());
        for(auto &x:*GET_ARGV(0).v_object)
        {
            (*vm.reg_ret.v_array)[i].type=T_STRING;
            (*vm.reg_ret.v_array)[i].v_string=newString();
            *(*vm.reg_ret.v_array)[i].v_string=x.first;
            i++;
        }
    }
    else if(GET_ARGV(0).type==T_ARRAY)
    {
        int size=GET_ARGV(0).v_array->size();
        vm.reg_ret.v_array->resize(size);
        for(int i=0;i<size;i++)
        {
            (*vm.reg_ret.v_array)[i].type=T_INT;
            (*vm.reg_ret.v_array)[i].v_int=i;
        }
    }
}


BUILD_FUNC_SIGN(gc)
{
    gc();
}

BUILD_FUNC_SIGN(each)
{
    auto &v=vm.v_stack.top();
    auto &f=vm.v_stack.top(1);
    if(v.type==T_ARRAY)
    {
        for(int i=0;i<v.v_array->size();i++)
        {
            vm.push(f);
            vm.push((*v.v_array)[i]);
            vm.push(i);
            vm.callReturn(2);
        }
    }
    else if(v.type==T_OBJECT)
    {
        for(auto &x:*v.v_object)
        {
            vm.push(f);
            vm.push(x.first);
            vm.push(x.second);
            vm.callReturn(2);
        }
    }
}

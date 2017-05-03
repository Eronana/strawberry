#include "buildin.h"
#include "memory.h"

BUILD_FUNC_SIGN(print)
{
    for(int i=0;i<argc;i++)fputs(GET_ARGV(i).toString().c_str(),stdout);
}

BUILD_FUNC_SIGN(println)
{
    GET_BUILDIN_FUNC_NAME(print)(argc,v_stack,ret);
    putchar('\n');
}

BUILD_FUNC_SIGN(scani)
{
    ret.type=T_INT;
    scanf("%d",&ret.v_int);
}
BUILD_FUNC_SIGN(scanf)
{
    ret.type=T_FLOAT;
    scanf("%lf",&ret.v_float);
}

BUILD_FUNC_SIGN(scans)
{
    char s[1000];
    ret.type=T_STRING;
    scanf("%s",s);
    ret.v_string=NEW(STRING_OTYPE);
    *ret.v_string=s;
}

BUILD_FUNC_SIGN(exit)
{
    int v=0;
    if(argc)v=GET_ARGV(0).toInt();
    exit(v);
}

BUILD_FUNC_SIGN(len)
{
    ret.type=T_INT;
    switch(GET_ARGV(0).type)
    {
        case T_ARRAY:
            ret.v_int=GET_ARGV(0).v_array->size();
            break;
        case T_OBJECT:
            ret.v_int=GET_ARGV(0).v_object->size();
            break;
        default:
            ret.v_int=1;
    }
}

BUILD_FUNC_SIGN(push)
{
    if(GET_ARGV(0).type!=T_ARRAY)return;
    for(int i=1;i<argc;i++)GET_ARGV(0).v_array->push_back(GET_ARGV(i));
    ret=GET_ARGV(0);
}

BUILD_FUNC_SIGN(clear)
{
    if(GET_ARGV(0).type==T_ARRAY)GET_ARGV(0).v_array->clear();
    else if(GET_ARGV(0).type==T_OBJECT)GET_ARGV(0).v_object->clear();
    else return;
    ret=GET_ARGV(0);
}

BUILD_FUNC_SIGN(resize)
{
    if(GET_ARGV(0).type!=T_ARRAY)return;
    GET_ARGV(0).v_array->resize(GET_ARGV(2));
    ret=GET_ARGV(0);
}

BUILD_FUNC_SIGN(get_keys)
{
    ret.type=T_ARRAY;
    ret.v_array=NEW(ARRAY_OTYPE);
    if(GET_ARGV(0).type==T_OBJECT)
    {
        int i=0;
        ret.v_array->resize(GET_ARGV(0).v_object->size());
        for(auto &x:*GET_ARGV(0).v_object)
        {
            (*ret.v_array)[i].type=T_STRING;
            (*ret.v_array)[i].v_string=NEW(STRING_OTYPE);
            *(*ret.v_array)[i].v_string=x.first;
            i++;
        }
    }
    else if(GET_ARGV(0).type==T_ARRAY)
    {
        int size=GET_ARGV(0).v_array->size();
        ret.v_array->resize(size);
        for(int i=0;i<size;i++)
        {
            (*ret.v_array)[i].type=T_INT;
            (*ret.v_array)[i].v_int=i;
        }
    }
}

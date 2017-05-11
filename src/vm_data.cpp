#include "vm_data.h"
#include <cstdlib>

V_VALUE::V_VALUE(bool b):type(T_BOOL),v_bool(b){}
V_VALUE::V_VALUE(INT_TYPE i):type(T_INT),v_int(i){}
V_VALUE::V_VALUE(FLOAT_TYPE f):type(T_FLOAT),v_float(f){}
V_VALUE::V_VALUE(STRING_TYPE s):type(T_STRING),v_string(s){}
V_VALUE::V_VALUE():type(T_NULL),v_int(0){}

V_VALUE &V_VALUE::operator+=(const V_VALUE &rhs)
{
    switch(type)
    {
        case T_INT:
            v_int+=rhs.toInt();
            break;
        case T_FLOAT:
            v_float+=rhs.toFloat();
            break;
        case T_STRING:
            *v_string+=rhs.toString();
            break;
        case T_ARRAY:
            if(rhs.type==T_ARRAY)
                v_array->insert(v_array->end(),rhs.v_array->begin(),rhs.v_array->end());
            else
                v_array->push_back(rhs);
        default:
            setNull();
    }
    return *this;
}


V_VALUE &V_VALUE::operator-=(const V_VALUE &rhs)
{
    switch(type)
    {
        case T_INT:
            v_int-=rhs.toInt();
            break;
        case T_FLOAT:
            v_float-=rhs.toFloat();
            break;
        default:
            setNull();
    }
    return *this;
}


V_VALUE &V_VALUE::operator*=(const V_VALUE &rhs)
{
    switch(type)
    {
        case T_INT:
            v_int*=rhs.toInt();
            break;
        case T_FLOAT:
            v_float*=rhs.toFloat();
            break;
        case T_STRING:
            if(int t=rhs.toInt())
            {
                string s=*v_string;
                while(--t)(*v_string)+=s;
            }
            else setNull();
            break;
        default:
            setNull();
    }
    return *this;
}

V_VALUE &V_VALUE::operator/=(const V_VALUE &rhs)
{
    switch(type)
    {
        case T_INT:
            v_int/=rhs.toInt();
            break;
        case T_FLOAT:
            v_float/=rhs.toFloat();
            break;
        default:
            setNull();
    }
    return *this;
}

V_VALUE &V_VALUE::operator%=(const V_VALUE &rhs)
{
    if(type==T_INT) v_int%=rhs.toInt();
    else setNull();
    return *this;
}

V_VALUE &V_VALUE::operator&=(const V_VALUE &rhs)
{
    if(type==T_INT) v_int&=rhs.toInt();
    else setNull();
    return *this;
}

V_VALUE &V_VALUE::operator|=(const V_VALUE &rhs)
{
    if(type==T_INT) v_int|=rhs.toInt();
    else setNull();
    return *this;
}

V_VALUE &V_VALUE::operator^=(const V_VALUE &rhs)
{
    if(type==T_INT) v_int^=rhs.toInt();
    else setNull();
    return *this;
}

V_VALUE &V_VALUE::operator<<=(const V_VALUE &rhs)
{
    if(type==T_INT) v_int<<=rhs.toInt();
    else setNull();
    return *this;
}

V_VALUE &V_VALUE::operator>>=(const V_VALUE &rhs)
{
    if(type==T_INT) v_int>>=rhs.toInt();
    else setNull();
    return *this;
}

V_VALUE &V_VALUE::operator++()
{
    if(type==T_INT)v_int++;
    else if(type==T_FLOAT)v_float++;
    else setNull();
    return *this;
}

V_VALUE &V_VALUE::operator--()
{
    if(type==T_INT)v_int--;
    else if(type==T_FLOAT)v_float--;
    else setNull();
    return *this;
}


V_VALUE V_VALUE::operator||(const V_VALUE &rhs) const
{
    V_VALUE ret(*this);
    ret.lor(rhs);
    return ret;
}

V_VALUE V_VALUE::operator&&(const V_VALUE &rhs) const
{
    V_VALUE ret(*this);
    ret.land(rhs);
    return ret;
}

V_VALUE V_VALUE::operator!() const
{
    V_VALUE ret(*this);
    ret.lnot();
    return ret;
}

V_VALUE V_VALUE::operator~() const
{
    V_VALUE ret(*this);
    ret.bnot();
    return ret;
}

V_VALUE V_VALUE::operator-() const
{
    V_VALUE ret(*this);
    ret.negative();
    return ret;
}

V_VALUE V_VALUE::operator+() const
{
    V_VALUE ret(*this);
    ret.positive();
    return ret;
}


bool V_VALUE::operator<(const V_VALUE &rhs) const
{
    switch(type)
    {
        case T_INT:
            return v_int<rhs.toInt();
        case T_FLOAT:
            return v_float<rhs.toFloat();
        case T_STRING:
            return *v_string<rhs.toString();
        default:
            return false;
    }
}

bool V_VALUE::operator>(const V_VALUE &rhs) const
{
    return rhs<*this;
}

bool V_VALUE::operator<=(const V_VALUE &rhs) const
{
    return !(*this>rhs);
}

bool V_VALUE::operator>=(const V_VALUE &rhs) const
{
    return !(*this<rhs);
}

bool V_VALUE::operator==(const V_VALUE &rhs) const
{
    if(type!=rhs.type)return false;
    switch(type)
    {
        case T_NULL:
            return true;
        case T_INT:
            return v_int==rhs.v_int;
        case T_FLOAT:
            return v_float==rhs.v_float;
        case T_BOOL:
            return v_bool==rhs.v_bool;
        case T_STRING:
            return *v_string==*rhs.v_string;
        case T_ARRAY:
            return *v_array==*rhs.v_array;
        case T_OBJECT:
            return *v_object==*rhs.v_object;
        case T_NATIVE_FUNCTION:
            return v_native_function==rhs.v_native_function;
        case T_FUNCTION:
            return v_function.func==rhs.v_function.func&&
                v_function.external==rhs.v_function.external;
    }
}
bool V_VALUE::operator!=(const V_VALUE &rhs) const
{
    return !(*this==rhs);
}

V_VALUE::operator bool()
{
    return toBool();
}

void V_VALUE::negative()
{
    if(type==T_INT)v_int=-v_int;
    else if(type==T_FLOAT)v_float=-v_float;
    else setNull();
}

void V_VALUE::positive()
{
    if(type!=T_INT&&type!=T_FLOAT)
    {
        type=T_FLOAT;
        v_float=toFloat();
    }
}

void V_VALUE::lnot()
{
    type=T_BOOL;
    v_bool=!toBool();
}
void V_VALUE::land(const V_VALUE &rhs)
{
    type=T_BOOL;
    v_bool=v_bool&&rhs.toBool();
}
void V_VALUE::lor(const V_VALUE &rhs)
{
    type=T_BOOL;
    v_bool=v_bool||rhs.toBool();
}

void V_VALUE::bnot()
{
    if(type==T_INT)v_int=~v_int;
    else setNull();
}

void V_VALUE::bor(const V_VALUE &rhs)
{
    if(type==T_INT&&rhs.type==T_INT)v_int|=rhs.v_int;
    else setNull();
}

void V_VALUE::band(const V_VALUE &rhs)
{
    if(type==T_INT&&rhs.type==T_INT)v_int&=rhs.v_int;
    else setNull();
}

void V_VALUE::setNull()
{
    type=T_NULL;
    v_int=0;
}

void V_VALUE::setBool(bool v)
{
    type=T_BOOL;
    v_bool=v;
}


int V_VALUE::toInt() const
{
    switch(type)
    {
        case T_INT:
            return v_int;
        case T_FLOAT:
            return v_float;
        case T_STRING:
            return atoi(v_string->c_str());
        case T_BOOL:
            return v_bool;
        default:
            return 0;
    }
}

bool V_VALUE::toBool() const
{
    switch(type)
    {
        case T_INT:
            return v_int;
        case T_FLOAT:
            return v_float;
        case T_NULL:
            return false;
        case T_BOOL:
            return v_bool;
        default:
            return true;
    }
}

float V_VALUE::toFloat() const
{
    switch(type)
    {
        case T_INT:
            return v_int;
        case T_FLOAT:
            return v_float;
        case T_STRING:
            return atof(v_string->c_str());
        case T_BOOL:
            return v_bool;
        default:
            return 0;
    }
}

string V_VALUE::toString() const
{
    char s[30];
    switch(type)
    {
        case T_INT:
            sprintf(s,"%d",v_int);
            return s;
        case T_FLOAT:
            sprintf(s,"%f",v_float);
            return s;
        case T_STRING:
            return *v_string;
        case T_BOOL:
            return v_bool?"true":"false";
        case T_NATIVE_FUNCTION:
            return "[Native Function]";
        case T_FUNCTION:
            return "[Finction]";
        case T_ARRAY:
            return "[Array]";
        case T_OBJECT:
            return "[OBJECT]";
        case T_NULL:
            return "null";
    }
}

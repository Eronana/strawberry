#ifndef _VM_OPCODE_H_
#define _VM_OPCODE_H_

#include <vector>
#include <string>
#include <map>
#include "stack.hpp"
using namespace std;

#define NATIVE_FUNCTION_SIGN(NAME) void (NAME)(int argc, Stack<V_VALUE> &v_stack, V_VALUE &ret)
#define GET_ARGV(N) v_stack.top(N)
struct V_VALUE;
typedef NATIVE_FUNCTION_SIGN(*NATIVE_FUNCTION_TYPE);
typedef int FUNCTION_TYPE;
typedef int NULL_TYPE;
typedef int BOOL_TYPE;
typedef int INT_TYPE;
typedef double FLOAT_TYPE;
typedef string STRING_OTYPE;
typedef vector<V_VALUE> ARRAY_OTYPE;
typedef map<string,V_VALUE> OBJECT_OTYPE;

typedef STRING_OTYPE *STRING_TYPE;
typedef ARRAY_OTYPE*ARRAY_TYPE;
typedef OBJECT_OTYPE *OBJECT_TYPE;

typedef unsigned char BYTE;
typedef BYTE CODE;
enum DATATYPE
{
    // value type
    T_NULL,
    T_BOOL,
    T_INT,
    T_FLOAT,
    T_STRING,
    T_ARRAY,
    T_OBJECT,
    T_NATIVE_FUNCTION,
    T_FUNCTION,
};

struct V_VALUE
{
    DATATYPE type;
    union
    {
        NULL_TYPE v_null;
        BOOL_TYPE v_bool;
        INT_TYPE v_int;
        FLOAT_TYPE v_float;
        STRING_TYPE v_string;
        ARRAY_TYPE v_array;
        OBJECT_TYPE v_object;
        NATIVE_FUNCTION_TYPE v_native_function;
        FUNCTION_TYPE v_function;
    };

    V_VALUE(bool b);
    V_VALUE(INT_TYPE i);
    V_VALUE(FLOAT_TYPE f);
    V_VALUE(STRING_TYPE s);
    V_VALUE();

    V_VALUE &operator+=(const V_VALUE &rhs);
    V_VALUE &operator-=(const V_VALUE &rhs);
    V_VALUE &operator*=(const V_VALUE &rhs);
    V_VALUE &operator/=(const V_VALUE &rhs);
    V_VALUE &operator%=(const V_VALUE &rhs);
    V_VALUE &operator&=(const V_VALUE &rhs);
    V_VALUE &operator|=(const V_VALUE &rhs);
    V_VALUE &operator^=(const V_VALUE &rhs);
    V_VALUE &operator<<=(const V_VALUE &rhs);
    V_VALUE &operator>>=(const V_VALUE &rhs);

    V_VALUE &operator++();
    V_VALUE &operator--();

    V_VALUE operator||(const V_VALUE &rhs) const;
    V_VALUE operator&&(const V_VALUE &rhs) const;
    V_VALUE operator!() const;
    V_VALUE operator~() const;
    V_VALUE operator-() const;
    V_VALUE operator+() const;

    bool operator<(const V_VALUE &rhs) const;
    bool operator>(const V_VALUE &rhs) const;
    bool operator<=(const V_VALUE &rhs) const;
    bool operator>=(const V_VALUE &rhs) const;
    bool operator==(const V_VALUE &rhs) const;
    bool operator!=(const V_VALUE &rhs) const;

    operator bool();

    void negative();
    void positive();
    void lnot();
    void land(const V_VALUE &rhs);
    void lor(const V_VALUE &rhs);
    void bnot();
    void bor(const V_VALUE &rhs);
    void band(const V_VALUE &rhs);
    void setNull();
    void setBool(bool v);
    int toInt() const;
    bool toBool() const;
    float toFloat() const;
    string toString() const;
};

#endif

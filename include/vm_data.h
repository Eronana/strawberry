#ifndef _VM_OPCODE_H_
#define _VM_OPCODE_H_

#include <vector>
#include <string>
#include <map>
using namespace std;

struct V_VALUE;
typedef void (*NATIVE_FUNCTION_TYPE)(int argc, V_VALUE *argv, V_VALUE *ret);
typedef int NULL_TYPE;
typedef int BOOL_TYPE;
typedef int INT_TYPE;
typedef double FLOAT_TYPE;
typedef string *STRING_TYPE;
typedef vector<V_VALUE> *ARRAY_TYPE;
typedef map<string,V_VALUE> *OBJECT_TYPE;
typedef uint32_t CODE;
typedef unsigned char BYTE;
enum DATATYPE
{
    // value type
    T_NULL,
    T_BOOL,
    T_INT,
    T_FLOAT,
    T_NATIVE_FUNCTION,
    // function type
    T_FUNCTION,
    // reference type
    T_STRING,
    T_ARRAY,
    T_OBJECT
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
        BYTE *gc_object;
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
    void negation();
    void positive();
    void bnot();
    void bor(const V_VALUE &rhs);
    void band(const V_VALUE &rhs);
    void setNull();
    int toInt() const;
    bool toBool() const;
    float toFloat() const;
    string toString() const;
};

#endif

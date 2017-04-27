#include "vm_data.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
    V_VALUE i(123);
    V_VALUE f(345.778);
    string wtf("999");
    V_VALUE s(&wtf);
    cout<<(~i).toString()<<endl;
    cout<<(-i).toString()<<endl;
    V_VALUE x(i);
    x^=i;
    cout<<x.toString()<<endl;
    i+=f;
    cout<<i.toString()<<endl;
    i+=s;
    cout<<i.toString()<<endl;
    ++i;
    ++i;
    --i;
    s+=i;
    cout<<s.toString()<<endl;
    cout<<i.toString()<<endl;
    cout<<f.toString()<<endl;
    cout<<bool(i==s)<<endl;
    return 0;
}
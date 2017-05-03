#ifndef _STACK_H_
#define _STACK_H_

#include <vector>
template<typename T>
class Stack
{
private:
    std::vector<T> data;
public:
    T &top(int n=0)
    {
        return *(data.rbegin()+n);
    }

    void add(int n=1)
    {
        data.resize(data.size()+n);
    }

    void sub(int n=1)
    {
        data.resize(data.size()-n);
    }

    T pop()
    {
        T ret=top();
        sub(1);
        return ret;
    }

    void push(const T &a)
    {
        data.push_back(a);
    }

    void clear()
    {
        data.clear();
    }

    size_t size()
    {
        return data.size();
    }
    
    void resize(size_t size)
    {
        data.resize(size);
    }

    T &operator[](int idx)
    {
        return data[idx];
    }
};
#endif
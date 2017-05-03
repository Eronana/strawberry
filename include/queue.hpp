#ifndef _QUEUE_H_
#define _QUEUE_H_
#include <vector>
template <typename T>
class Queue
{
private:
  std::vector<T> data;
  int base;

public:
  Queue() : base(0) {}
  size_t size() const
  {
    return data.size() - base;
  }
  void push(const T &a)
  {
    data.push_back(a);
  }
  void pop(int n)
  {
    if ((base += n) == data.size())
    {
      data.clear();
      base = 0;
    }
  }
  const T &operator[](int index) const
  {
    return data[index + base];
  }
  const T *begin() const
  {
    return &data[base];
  }
};
#endif

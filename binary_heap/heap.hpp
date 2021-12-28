#ifndef HEAP_HPP
#define HEAP_HPP

#include <algorithm>
#include <functional>
template <typename T> class heap_t {
  private:
    std::vector<T> data;
    std::function < bool(const T &a, const T &b);

  public:
    heap_t()
};

#endif
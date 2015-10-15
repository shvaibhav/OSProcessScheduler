#pragma once
#include <system_error>
#include <type_traits>
#include <new>
#include <algorithm>
#include <unordered_set>

namespace scheduler
{

//store trivial objects in a vector that may be efficiently expanded using realloc
template<typename T> class trivial_vector
{
private:
  static const size_t GROW_DIVISOR = 4;
public:
  trivial_vector(size_t capacity)
  :m_array{nullptr},
  m_size{0},
  m_capacity{capacity}
  {
    //not supported by GCC
    //static_assert(std::is_trivially_copyable<T>::value,"Type must be trivially copyable.");
    static_assert(std::is_trivially_destructible<T>::value,"Type must be trivially destructable.");
    if(capacity < GROW_DIVISOR)
      throw std::system_error(std::make_error_code(std::errc::invalid_argument),"Initial capacity too small.");
    m_array = static_cast<T*>(std::realloc(nullptr,sizeof(T)*capacity));
    if(m_array == nullptr)
      throw std::system_error(std::make_error_code(std::errc::not_enough_memory));
  }
  ~trivial_vector()
  {
    free(m_array);
  }
  bool empty() const
  {
    return m_size == 0;
  }
  size_t size() const
  {
    return m_size;
  }
  size_t capacity() const
  {
    return m_capacity;
  }
  void clear()
  {
    m_size = 0;
  }
  const T& operator[](size_t i) const
  {
    return m_array[i];
  }
  T& operator[](size_t i)
  {
    return m_array[i];
  }
  const T* data() const
  {
    return m_array;
  }
  T* data()
  {
    return m_array;
  }
  const T* begin() const
  {
    return m_array;
  }
  T* begin()
  {
    return m_array;
  }
  const T* end() const
  {
    return m_array+m_size;
  }
  T* end()
  {
    return m_array+m_size;
  }
  void push_back(const T& t)
  {
    if(m_size == m_capacity)
      grow();
    new(m_array+(m_size++))T(t);
  }
  template<class...Args> void emplace_back(Args&&...args)
  {
    if(m_size == m_capacity)
      grow();
    new(m_array+(m_size++))T(args...);
  }
  void unique()
  {
    //remove non-unique objects
    if(m_size < 2)
      return;
    if(unique_is_sorted())
      unique_sorted();
    else
      unique_unsorted();
  }
  void sort()
  {
    std::sort(begin(),end());
  }
private:
  bool unique_is_sorted()
  {
    //test if vector is already sorted by unique property
    for(size_t i=0; i<m_size-1; ++i)
    {
      if(m_array[i+1].unique() < m_array[i].unique())
        return false;
    }
    return true;
  }
  void unique_sorted()
  {
    //remove non-unique objects if list is sorted by unique property
    size_t j=0;
    for(size_t i=1; i<m_size; ++i)
    {
      if(m_array[j].unique() == m_array[i].unique())
        continue;
      ++j;
      if(i != j)
        m_array[j] = m_array[i];
    }
    m_size = j+1;
  }
  void unique_unsorted()
  {
    //remove non-unique objects if list is not sorted by unique property
    std::unordered_set<decltype(std::declval<T>().unique())> set(m_size);
    size_t j=0;
    for(size_t i=0; i<m_size; ++i)
    {
      if(set.find(m_array[i].unique()) != set.end())
        continue;
      set.insert(m_array[i].unique());
      if(i != j)
        m_array[j] = m_array[i];
      ++j;
    }
    m_size = j;
  }
  void grow()
  {
    size_t capacity = m_capacity+m_capacity/GROW_DIVISOR;
    T* array = static_cast<T*>(std::realloc(m_array,sizeof(T)*capacity));
    if(array == nullptr)
      throw std::system_error(std::make_error_code(std::errc::not_enough_memory));
    m_array = array;
    m_capacity = capacity;
  }
private:
  T* m_array;
  size_t m_size;
  size_t m_capacity;
};

}

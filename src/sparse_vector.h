#pragma once
#include <new>
#include <type_traits>
#include <system_error>

namespace scheduler
{

//efficient allocator for objects of a trivial type
//objects may be deleted randomly in O(1) time
//objects may be added randomly in O(1) time by using a free list
//when the capacity increases a new array is allocated so pointers to existing objects are preserved
template<typename T> class sparse_vector
{
private:
  static const size_t GROW_DIVISOR = 4;
public:
  sparse_vector(size_t capacity)
  :m_free{nullptr},
  m_array{nullptr},
  m_size{0},
  m_capacity{capacity},
  m_narray{0}
  {
    //not supported by GCC
    //static_assert(std::is_trivially_copyable<T>::value,"Type must be trivially copyable.");
    static_assert(std::is_trivially_destructible<T>::value,"Type must be trivially destructable.");

    if(capacity < GROW_DIVISOR)
      throw std::system_error(std::make_error_code(std::errc::invalid_argument),"Capacity too small.");

    //free list
    m_free = static_cast<T**>(std::realloc(nullptr,sizeof(T*)*capacity));
    if(m_free == nullptr)
      throw std::system_error(std::make_error_code(std::errc::not_enough_memory));

    //array of arrays
    m_array = static_cast<T**>(std::realloc(nullptr,sizeof(T*)));
    if(m_array == nullptr)
      throw std::system_error(std::make_error_code(std::errc::not_enough_memory));

    //first array
    m_array[0] = static_cast<T*>(std::malloc(sizeof(T)*capacity));
    if(m_array[0] == nullptr)
      throw std::system_error(std::make_error_code(std::errc::not_enough_memory));
    m_narray = 1;

    //add first array pointers to free list
    for(size_t i=0; i<capacity; ++i)
      m_free[i] = m_array[0]+i;
  }
  ~sparse_vector()
  {
    std::free(m_free);
    if(m_array != nullptr)
    {
      for(size_t i=0; i<m_narray; ++i)
        std::free(m_array[i]);
    }
    std::free(m_array);
  }
  bool empty() const
  {
    return m_size == 0;
  }
  size_t size() const
  {
    return m_size;
  }
  template<class...Args> T* alloc(Args&&...args)
  {
    if(m_size == m_capacity)
      grow();
    T* t = m_free[m_size++];
    new(t)T(args...);
    return t;
  }
  void free(T* t)
  {
    m_free[--m_size] = t;
  }
private:
  void grow()
  {
    size_t add = m_capacity/GROW_DIVISOR;

    T** array = static_cast<T**>(std::realloc(m_array,sizeof(T*)*(m_narray+1)));
    if(array == nullptr)
      throw std::system_error(std::make_error_code(std::errc::not_enough_memory));
    m_array = array;

    m_array[m_narray] = static_cast<T*>(std::malloc(sizeof(T)*add));
    if(m_array[m_narray] == nullptr)
      throw std::system_error(std::make_error_code(std::errc::not_enough_memory));
    ++m_narray;

    T** free = static_cast<T**>(std::realloc(m_free,sizeof(T*)*(m_capacity+add)));
    if(free == nullptr)
      throw std::system_error(std::make_error_code(std::errc::not_enough_memory));
    m_free = free;

    for(size_t i=0; i<add; ++i)
      m_free[m_capacity+i] = m_array[m_narray-1]+i;

    m_capacity += add;
  }
private:
  T** m_free;
  T** m_array;
  size_t m_size;
  size_t m_capacity;
  size_t m_narray;
};

}

#pragma once
#include "slist.h"
#include <iostream>

namespace scheduler
{

//doubly linked list
template<typename T,typename N> class list
{
public:
  class iterator
  {
  public:
    iterator()
    :m_t{nullptr}
    {}
    iterator(T* t)
    :m_t{t}
    {}
    const T* operator->() const
    {
      return m_t;
    }
    T* operator->()
    {
      return m_t;
    }
    const T* operator*() const
    {
      return m_t;
    }
    T* operator*()
    {
      return m_t;
    }
    iterator& operator++()
    {
      m_t = m_t->N::prev;
      return *this;
    }
    iterator operator++(int)
    {
      iterator it(m_t);
      m_t = m_t->N::prev;
      return it;
    }
    bool operator==(const iterator& it) const
    {
      return m_t == it.m_t;
    }
    bool operator!=(const iterator& it) const
    {
      return m_t != it.m_t;
    }
  private:
    T* m_t;
  };
public:
  list(const list& list) = delete;
  list& operator=(const list& list) = delete;
  list()
  :m_front{nullptr},
  m_back{nullptr}
  {}
  bool empty() const
  {
    return m_front == nullptr;
  }
  void clear()
  {
    m_front = nullptr;
    m_back = nullptr;
  }
  void push_front(T* t)
  {
    t->N::prev = m_front;
    t->N::next = nullptr;
    if(m_front != nullptr)
      m_front->N::next = t;
    else
      m_back = t;
    m_front = t;
  }
  void push_back(T* t)
  {
    t->N::prev = nullptr;
    t->N::next = m_back;
    if(m_back != nullptr)
      m_back->N::prev = t;
    else
      m_front = t;
    m_back = t;
  }
  T* pop_front()
  {
    if(m_front == nullptr)
      return nullptr;
    T* t = m_front;
    m_front = m_front->N::prev;
    if(m_front != nullptr)
      m_front->N::next = nullptr;
    else
      m_back = nullptr;
    return t;
  }
  T* pop_back()
  {
    if(m_back == nullptr)
      return nullptr;
    T* t = m_back;
    m_back = m_back->N::next;
    if(m_back != nullptr)
      m_back->N::prev = nullptr;
    else
      m_front = nullptr;
    return t;
  }
  void remove(T* t)
  {
    if(m_front != t)
      t->N::next->N::prev = t->N::prev;
    else
      m_front = t->N::prev;
    if(m_back != t)
      t->N::prev->N::next = t->N::next;
    else
      m_back = t->N::next;
  }
  const T* front() const
  {
    return m_front;
  }
  T* front()
  {
    return m_front;
  }
  const T* back() const
  {
    return m_back;
  }
  T* back()
  {
    return m_back;
  }
  iterator begin()
  {
    return iterator(m_front);
  }
  iterator end()
  {
    return iterator();
  }
private:
  T* m_front;
  T* m_back;
};

}

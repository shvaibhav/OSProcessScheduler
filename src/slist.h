#pragma once

namespace scheduler
{

//singly linked list
template<typename T,typename N> class slist
{
  template<typename T2,typename N2> friend class list;
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
  slist(const slist& slist) = delete;
  slist& operator=(const slist& slite) = delete;
  slist()
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
    if(m_front == nullptr)
      m_back = t;
    m_front = t;
  }
  void push_back(T* t)
  {
    t->N::prev = nullptr;
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
    if(m_front == nullptr)
      m_back = nullptr;
    return t;
  }
  const T* front() const
  {
    return m_front;
  }
  T* front()
  {
    return m_front;
  }
  iterator begin()
  {
    return iterator(m_front);
  }
  iterator end()
  {
    return iterator();
  }
  void insert_sorted(T* t)
  {
    if(m_front == nullptr)
    {
      t->N::prev = nullptr;
      m_front = t;
      m_back = t;
      return;
    }

    T* cur = m_front;
    if(*t < *cur)
    {
      t->N::prev = m_front;
      m_front = t;
      return;
    }

    T* next = cur->N::prev;
    while(true)
    {
      if(next == nullptr)
        m_back = t;
      if(next == nullptr || *t < *next)
      {
        cur->N::prev = t;
        t->N::prev = next;
        return;
      }
      cur = next;
      next = cur->N::prev;
    }
  }
  void merge(slist& list)
  {
    if(list.empty())
      return;

    if(empty())
    {
      m_front = list.m_front;
      m_back = list.m_back;
      list.m_front = nullptr;
      list.m_back = nullptr;
      return;
    }

    T* back1 = m_back;
    T* back2 = list.m_back;
    T* front1 = m_front;
    T* front2 = list.m_front;
    list.m_front = nullptr;
    list.m_back = nullptr;

    if(*front2 < *front1)
    {
      m_front = front2;
      m_back = front2;
      front2 = front2->N::prev;
      if(front2 == nullptr)
      {
        m_back->N::prev = front1;
        m_back = back1;
        return;
      }
    }
    else
    {
      m_front = front1;
      m_back = front1;
      front1 = front1->N::prev;
      if(front1 == nullptr)
      {
        m_back->N::prev = front2;
        m_back = back2;
        return;
      }
    }

    while(true)
    {
      if(*front2 < *front1)
      {
        m_back->N::prev = front2;
        m_back = front2;
        front2 = front2->N::prev;
        if(front2 == nullptr)
        {
          m_back->N::prev = front1;
          m_back = back1;
          return;
        }
      }
      else
      {
        m_back->N::prev = front1;
        m_back = front1;
        front1 = front1->N::prev;
        if(front1 == nullptr)
        {
          m_back->N::prev = front2;
          m_back = back2;
          return;
        }
      }
    }
  }
private:
  T* m_front;
  T* m_back;
};

}

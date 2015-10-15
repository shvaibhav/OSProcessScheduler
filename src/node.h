#pragma once

namespace scheduler
{

//linked list node
//delare your class a subclass so that it may be inserted into a list or slist
//your class may belong to multiple link lists by picking different TAG incomplete types
template<typename T,typename TAG> class node
{
  template<typename A,typename B> friend class list;
  template<typename A,typename B> friend class slist;
private:
  T* prev;
  T* next;
};

}

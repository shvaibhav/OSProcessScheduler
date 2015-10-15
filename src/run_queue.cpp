#include "run_queue.h"
#include <iostream>

namespace scheduler
{

run_queue::run_queue()
:m_max{99},
m_is_max{false}
{}

void run_queue::insert(boost_slist& list)
{
  for(process* proc : list)
  {
    priority_t priority = proc->priority();
    run_list& queue = m_queue[priority];
    queue.push_back(proc);

    if(priority >= m_max)
    {
      m_max = priority;
      m_is_max = true;
    }
  }
}

void run_queue::remove(boost_slist& list)
{
  for(process* proc : list)
  {
    priority_t priority = proc->priority();
    run_list& queue = m_queue[priority];
    queue.remove(proc);
  }

  if(m_is_max && m_queue[m_max].empty())
    m_is_max = false;
}

process* run_queue::top()
{
  //search for highest nonempty queue if not cached
  if(!m_is_max)
  {
    do
    {
      if(m_max == 0)
        return nullptr;
      --m_max;
    }
    while(m_queue[m_max].empty());
    m_is_max = true;
  }

  priority_t priority = m_max;
  run_list& queue = m_queue[priority];
  process* proc = queue.pop_front();

  if(queue.empty())
    m_is_max = false;

  return proc;
}

}

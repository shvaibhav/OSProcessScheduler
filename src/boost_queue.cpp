#include "boost_queue.h"

namespace scheduler
{

const priority_t boost_queue::BOOST_TABLE[49] = {10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,49,49,49,49,49,49,49,49,49};

bool boost_queue::get(time_run_t time,boost_slist& list)
{
  if(m_queue.empty())
    return false;
  if(m_queue.front()->boost() != time)
    return false;
  while(true)
  {
    process* proc = m_queue.pop_front();
    list.push_back(proc);
    if(m_queue.empty())
      return true;
    if(m_queue.front()->boost() != time)
      return true;
  }
}

void boost_queue::insert(time_run_t time,boost_slist& list)
{
  time_run_t boost = time + BOOST_TIME;
  for(auto it=list.begin(); it!=list.end(); )
  {
    auto cur = it++;
    if(cur->priority() < MAX_BOOST)
    {
      cur->boost(boost);
      m_queue.push_back(*cur);
    }
    else
      cur->boost(BOOST_NONE);
  }
  list.clear();
}

void boost_queue::remove(process* proc)
{
  if(proc->boost() == BOOST_NONE)
    return;
  m_queue.remove(proc);
}

void boost_queue::boost(boost_slist& list)
{
  for(process* p : list)
    p->priority(BOOST_TABLE[p->priority()]);
}

}

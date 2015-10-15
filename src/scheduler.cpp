#include "scheduler.h"

namespace scheduler
{

scheduler::scheduler(time_run_t quantum,bool output,safe_file_handle&& arrivals)
:m_process(PROCESS_INIT_STORAGE),
m_arrival(std::move(arrivals),m_process),
m_running(quantum,output,m_process)
{}

void scheduler::run()
{
  time_run_t time = 0;
  boost_slist list;
  boost_slist arrive;
  bool arrivals = true;
  while(!m_process.empty() || arrivals)
  {
    //get processes that are boosted this tick
    if(m_boost.get(time,list))
    {
      //remove them from run queues and boost them
      m_run.remove(list);
      m_boost.boost(list);
    }

    //get currently running process if quantum exprired
    m_running.get(time,list);

    //get new arrivals
    if(arrivals)
    {
      if(!m_arrival.get(time,arrive))
        arrivals = false;
      //merge linked lists to ensure pid order
      list.merge(arrive);
    }

    if(!list.empty())
    {
      //insert processes to run and boost queues
      m_run.insert(list);
      m_boost.insert(time,list);
    }

    if(m_running.empty())
    {
      //if no process running grab the top one from the run queue
      process* proc = m_run.top();
      if(proc != nullptr)
      {
        m_boost.remove(proc);
        m_running.run(time,proc);
      }
    }

    ++time;
  }
}

}

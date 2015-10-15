#include "running.h"

namespace scheduler
{

running::running(time_run_t quantum,bool output,sparse_vector<process>& process)
:m_out_event{output},
m_out_process{output},
m_process(process),
m_quantum{quantum},
m_running{nullptr}
{}

bool running::empty() const
{
  return m_running == nullptr;
}

void running::get(time_run_t time,boost_slist& list)
{
  if(m_running == nullptr)
    return;
  if(time != m_stop)
    return;
  if(m_running->remaining() != 0)
    list.insert_sorted(m_running);
  else
  {
    m_out_process.event(m_running->arrival(),m_stop,m_running->burst(),m_running->pid(),m_running->initial_priority());
    m_process.free(m_running);
  }
  m_running = nullptr;
}

void running::run(time_run_t time,process* proc)
{
  m_running = proc;
  time_run_t remaining = proc->remaining();
  time_run_t run = std::min(m_quantum,remaining);
  remaining -= run;
  m_stop = time + run;
  proc->remaining(remaining);

  m_out_event.event(time,m_stop,proc->pid(),proc->priority());

  //demote process priority

  priority_t priority = proc->priority();
  //fixed kernel
  //user >= 0
  if(priority < PRIORITY_KERNEL)
  {
    if(priority >= run)
      priority -= run;
    else
      priority = 0;
    proc->priority(priority);
  }
  /*
  //fixed kernel
  //user >= inital
  if(priority < PRIORITY_KERNEL)
  {
    priority_t pri_diff = priority-proc->initial_priority();
    if(pri_diff > run)
      priority -= run;
    else
      priority = proc->initial_priority();
    proc->priority(priority);
  }
  //dynamic kernel
  //user >= 0
  if(priority < PRIORITY_KERNEL)
  {
    if(priority >= run)
      priority -= run;
    else
      priority = 0;
    proc->priority(priority);
  }
  else
  {
    priority_t pri_diff = priority-PRIORITY_KERNEL;
    if(pri_diff > run)
      priority -= run;
    else
      priority = PRIORITY_KERNEL;
    proc->priority(priority);
  }
  */
}

}

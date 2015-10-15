#include "process.h"

namespace scheduler
{

process::process(time_input_t arrival,time_input_t burst,pid_t pid,priority_t priority)
:m_arrival{arrival},
m_burst{burst},
m_remain{burst},
m_pid{pid},
m_initial_priority{priority},
m_priority{priority}
{}

time_run_t process::boost() const
{
  return m_boost;
}

void process::boost(time_run_t b)
{
  m_boost = b;
}

time_run_t process::arrival() const
{
  return m_arrival;
}

time_run_t process::burst() const
{
  return m_burst;
}

time_run_t process::remaining() const
{
  return m_remain;
}

void process::remaining(time_run_t remain)
{
  m_remain = remain;
}

pid_t process::pid() const
{
  return m_pid;
}

priority_t process::initial_priority() const
{
  return m_initial_priority;
}

priority_t process::priority() const
{
  return m_priority;
}

void process::priority(priority_t pri)
{
  m_priority = pri;
}

}

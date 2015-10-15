#include "process_input.h"

namespace scheduler
{

process_input::process_input(time_input_t arrival,time_input_t burst,pid_t pid,priority_t priority)
:m_arrival{arrival},
m_burst{burst},
m_pid{pid},
m_priority{priority},
m_zero{}
{}

time_input_t process_input::arrival() const
{
  return m_arrival;
}

time_input_t process_input::burst() const
{
  return m_burst;
}

pid_t process_input::pid() const
{
  return m_pid;
}

priority_t process_input::priority() const
{
  return m_priority;
}

}

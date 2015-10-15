#pragma once
#include "include.h"
#include <cstddef>

namespace scheduler
{

//stores information about processes that have not yet arrived
class process_input
{
public:
  process_input(time_input_t arrival,time_input_t burst,pid_t pid,priority_t priority);
  time_input_t arrival() const;
  time_input_t burst() const;
  pid_t pid() const;
  priority_t priority() const;
  pid_t unique() const
  {
    return m_pid;
  }
  bool operator<(const process_input& p) const
  {
    if(m_arrival < p.m_arrival)
      return true;
    if(m_arrival == p.m_arrival)
      return m_pid < p.m_pid;
    return false;
  }
private:
  time_input_t m_arrival;
  time_input_t m_burst;
  pid_t m_pid;
  priority_t m_priority;
  uint8_t m_zero[3];
};

}

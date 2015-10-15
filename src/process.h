#pragma once
#include "include.h"
#include "node.h"
#include "list.h"

namespace scheduler
{

struct run_tag;
struct boost_tag;
class process;
typedef node<process,run_tag> run_node;
typedef node<process,boost_tag> boost_node;

//class that stores information about all processes that have arrived but not yet terminated
class process : public run_node, public boost_node
{
public:
  process(time_input_t arrival,time_input_t burst,pid_t pid,priority_t priority);
  time_run_t boost() const;
  void boost(time_run_t b);
  time_run_t arrival() const;
  time_run_t burst() const;
  time_run_t remaining() const;
  void remaining(time_run_t remain);
  pid_t pid() const;
  priority_t initial_priority() const;
  priority_t priority() const;
  void priority(priority_t pri);
  bool operator<(const process& proc) const
  {
    return m_pid < proc.m_pid;
  }
private:
  time_run_t m_boost;
  time_run_t m_arrival;
  time_run_t m_burst;
  time_run_t m_remain;
  pid_t m_pid;
  priority_t m_initial_priority;
  priority_t m_priority;
};

typedef slist<process,run_node> run_slist;
typedef slist<process,boost_node> boost_slist;
typedef list<process,run_node> run_list;
typedef list<process,boost_node> boost_list;

}

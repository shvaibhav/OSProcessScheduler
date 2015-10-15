#pragma once
#include "process.h"
#include "sparse_vector.h"
#include "output_event.h"
#include "output_process.h"

namespace scheduler
{

//logic for process that is currently running
class running
{
private:
  //min priority of a kernel process
  static const priority_t PRIORITY_KERNEL = 50;
public:
  running(time_run_t quantum,bool output,sparse_vector<process>& process);
  //return false if no processes is currently running
  bool empty() const;
  //add process to list if it is done running at specified time
  void get(time_run_t time,boost_slist& list);
  //run specified process
  void run(time_run_t time,process* proc);
private:
  output_event m_out_event;
  output_process m_out_process;
  sparse_vector<process>& m_process;
  time_run_t m_quantum;
  process* m_running;
  time_run_t m_stop;
};

}

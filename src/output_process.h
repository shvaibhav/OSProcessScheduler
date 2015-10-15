#pragma once
#include "include.h"
#include "safe_file_handle.h"

namespace scheduler
{

//output information about process when a process terminates
class output_process
{
private:
  static const char* FILE_NAME;
public:
  output_process(bool output);
  ~output_process();
  void event(time_run_t arrival,time_run_t terminate,time_run_t burst,pid_t pid,priority_t priority);
private:
  safe_file_handle m_file;
  size_t m_nproc;
  time_run_t m_turn;
  time_run_t m_wait;
};

}

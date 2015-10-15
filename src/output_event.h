#include "process.h"
#include "safe_file_handle.h"

namespace scheduler
{

//write to file every time a new process is scheduled
class output_event
{
  static const char* FILE_NAME;
public:
  output_event(bool output);
  ~output_event();
  void event(time_run_t start,time_run_t stop,pid_t pid,priority_t priority);
private:
  safe_file_handle m_file;
};

}

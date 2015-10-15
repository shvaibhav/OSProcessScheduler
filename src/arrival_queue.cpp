#include "arrival_queue.h"

namespace scheduler
{

arrival_queue::arrival_queue(safe_file_handle&& file,sparse_vector<process>& procs)
:m_file(std::move(file)),
m_process(procs)
{}

bool arrival_queue::get(time_run_t time,boost_slist& list)
{ 
  try
  {
    //loop until head of buffer is not a current arrival
    while(true)
    {
      const process_input& in = m_file.get();
      if(in.arrival() != time)
        return true;
      m_file.next();
      process* proc = m_process.alloc(in.arrival(),in.burst(),in.pid(),in.priority());
      list.push_back(proc);
    }
  }
  catch(std::system_error& e)
  {
    if(e.code() != std::errc::io_error)
      throw;
    return false;
  }
}

}

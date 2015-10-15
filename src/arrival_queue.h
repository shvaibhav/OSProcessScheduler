#pragma once
#include "file_read.h"
#include "process_input.h"
#include "process.h"
#include "sparse_vector.h"

namespace scheduler
{

//read arriving processes from file and pass them to the scheduler
class arrival_queue
{
private:
  //number of processes to read into buffer for every file read
  static const size_t INPUT_BUFFER = 10000;
public:
  arrival_queue(safe_file_handle&& file,sparse_vector<process>& procs);
  //find all processes that arrive at the speciified time
  //allocate the new processes in the sparse_vector
  //add the new processes to the specified list
  //return false if end of file is reached
  bool get(time_run_t time,boost_slist& list);
private:
  file_read<process_input,INPUT_BUFFER> m_file;
  sparse_vector<process>& m_process;
};

}

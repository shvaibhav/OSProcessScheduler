#include "output_event.h"
#include <cinttypes>
#include <system_error>

namespace scheduler
{

const char* output_event::FILE_NAME = "output_event.js";

output_event::output_event(bool output)
{
  if(!output)
    return;

  m_file = std::fopen(FILE_NAME,"w");
  if(!m_file)
    throw std::system_error(std::make_error_code(std::errc::permission_denied),FILE_NAME);

  std::fprintf(m_file,"var g_event=[");
}

output_event::~output_event()
{
  if(m_file)
    std::fprintf(m_file,"];");
}

void output_event::event(time_run_t start,time_run_t stop,pid_t pid,priority_t priority)
{
  if(!m_file)
    return;
  std::fprintf(m_file,"[%" PRIu64 ",%" PRIu64 ",%u,%u],",start,stop,pid,priority);
}

}

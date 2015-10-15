#include "output_process.h"
#include <cinttypes>
#include <system_error>
#include <iostream>
#include <iomanip>

namespace scheduler
{

const char* output_process::FILE_NAME = "output_process.js";

output_process::output_process(bool output)
:m_nproc{0},
m_turn{0},
m_wait{0}
{
  if(!output)
    return;

  m_file = std::fopen(FILE_NAME,"w");
  if(!m_file)
    throw std::system_error(std::make_error_code(std::errc::permission_denied),FILE_NAME);

  std::fprintf(m_file,"var g_process={process:{");
}

output_process::~output_process()
{
  double turn;
  double wait;
  if(m_nproc != 0)
  {
    turn = static_cast<double>(m_turn)/static_cast<double>(m_nproc);
    wait = static_cast<double>(m_wait)/static_cast<double>(m_nproc);
  }
  else
  {
    turn = 0.0;
    wait = 0.0;
  }

  std::cout << "Total Processes:         " << m_nproc << std::endl;
  std::cout << std::fixed << std::setprecision(2) << "Average Turnaround Time: " << turn << std::endl;
  std::cout << std::fixed << std::setprecision(2) << "Average Waiting Time:    " << wait << std::endl;
  if(m_file)
    std::cout << "See output.html for Gantt chart." << std::endl;

  if(!m_file)
    return;
  std::fprintf(m_file,"},stats:{total:%" PRIu64 ",turnaround:%.2f,wait:%.2f}};",static_cast<uint64_t>(m_nproc),turn,wait);
}

void output_process::event(time_run_t arrival,time_run_t terminate,time_run_t burst,pid_t pid,priority_t priority)
{
  time_run_t turn = terminate-arrival;
  time_run_t wait = turn-burst;

  m_turn += turn;
  m_wait += wait;
  ++m_nproc;

  if(!m_file)
    return;

  std::fprintf(m_file,"%u:[%" PRIu64 ",%" PRIu64 ",%" PRIu64 ",%u,%" PRIu64 ",%" PRIu64 "],",
    pid,arrival,terminate,burst,priority,turn,wait);
}

}

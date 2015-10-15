#include "input_parser.h"
#include "scheduler.h"
#include <iostream>

void print_help();
bool parse_arguments(int narg,const char** arg,uint64_t& quantum,const char*& input_file,bool& output);
bool iequals(const char* s1,const char* s2);
bool string_to_uint64(const char* string,uint64_t& value);
namespace scheduler
{
  bool schedule(uint64_t quantum,const char* input_file,bool output);
  void get_input(const char* input_file,safe_file_handle& out);
  void run_scheduler(uint64_t quantum,bool output,safe_file_handle&& file);
}

int main(int argc,const char** argv)
{
  uint64_t quantum;
  const char* input_file;
  bool output;

  if(!parse_arguments(argc,argv,quantum,input_file,output))
  {
    print_help();
    return -1;
  }
  return scheduler::schedule(quantum,input_file,output) ? 0 : -1;
}

namespace scheduler
{

bool schedule(uint64_t quantum,const char* input_file,bool output)
{
  try
  {
    safe_file_handle arrivals;
    //parse
    get_input(input_file,arrivals);
    //schedule
    run_scheduler(quantum,output,std::move(arrivals));
  }
  catch(std::system_error& e)
  {
    std::cerr << std::endl << e.what() << std::endl;
    return false;
  }
  return true;
}

void run_scheduler(uint64_t quantum,bool output,safe_file_handle&& file)
{
  scheduler s(quantum,output,std::move(file));
  s.run();
}

void get_input(const char* input_file,safe_file_handle& out)
{
  //parse input file, remove non-unique pids, sort by arrival, and write to temporary file
  trivial_vector<process_input> processes(256);
  {
    input_parser input(input_file);
    input.parse(processes);
  }
  processes.unique();
  processes.sort();
  out = std::tmpfile();
  if(!out)
    throw std::system_error(std::make_error_code(std::errc::permission_denied),"Unable to create temporary file");
  out.no_buffer();
  if(std::fwrite(processes.data(),sizeof(process_input),processes.size(),out) != processes.size())
    throw std::system_error(std::make_error_code(std::errc::no_space_on_device));
}

}

bool string_to_uint64(const char* string,uint64_t& value)
{
  const uint8_t* str = reinterpret_cast<const uint8_t*>(string);
  uint64_t ch = *(str++);
  if(ch == 0)
    return false;
  value = ch-'0';
  if(value > 9)
    return false;
  while((ch = *(str++)) != 0)
  {
    ch -= '0';
    if(ch > 9)
      return false;
    value = 10*value + ch;
  }
  return true;
}

bool iequals(const char* s1,const char* s2)
{
  //case insensitive string comparrison
  char c1 = *(s1++);
  char c2 = *(s2++);
  while(c1 != 0 && c2 != 0)
  {
    if(std::tolower(c1) != std::tolower(c2))
      return false;
    c1 = *(s1++);
    c2 = *(s2++);
  }
  return c1 == c2;
}

void print_help()
{
  std::cout << "scheduler -q [quantum] -i [process file] -n|-o" << std::endl
    << "  -h, --help                        print help" << std::endl
    << "  -q, --quantum                     time quantum" << std::endl
    << "  -i, --input                       input file" << std::endl
    << "  -o, --out                         output to files (default)" << std::endl
    << "  -n, --noout                       do not output to files" << std::endl;
}

bool parse_arguments(int narg,const char** arg,uint64_t& quantum,const char*& input_file,bool& output)
{
  if(narg == 1)
    return false;

  bool is_quantum = false;
  bool is_input = false;
  bool is_noout = false;
  bool is_out = false;

  output = true;
  for(int i=1; i<narg; ++i)
  {
    if(iequals("-h",arg[i]) || iequals("--h",arg[i]) ||
       iequals("-help",arg[i]) || iequals("--help",arg[i]) ||
       iequals("help",arg[i]))
    {
      return false;
    }
    else if(iequals("-q",arg[i]) || iequals("--quantum",arg[i]))
    {
      if(is_quantum)
      {
        std::cerr << "Error: Quantum specified multiple times." << std::endl;
        return false;
      }
      if(++i >= narg)
      {
        std::cerr << "Error: No quantum specified." << std::endl;
        return false;
      }
      if(!string_to_uint64(arg[i],quantum))
      {
        std::cerr << "Error: Quantum is not a positive integer." << std::endl;
        return false;
      }
      if(quantum == 0)
      {
        std::cerr << "Error: Quantum is zero." << std::endl;
        return false;
      }
      is_quantum = true;
    }
    else if(iequals("-i",arg[i]) || iequals("--input",arg[i]))
    {
      if(is_input)
      {
        std::cerr << "Error: Input file specified multiple times." << std::endl;
        return false;
      }
      if(++i >= narg)
      {
        std::cerr << "Error: Input file not specified." << std::endl;
        return false;
      }
      input_file = arg[i];
      is_input = true;
    }
    else if(iequals("-o",arg[i]) || iequals("--out",arg[i]))
    {
      if(is_out)
      {
        std::cerr << "Error: Output to file specified multiple times." << std::endl;
        return false;
      }
      if(is_noout)
      {
        std::cerr << "Error: Output to file conflicts with no output to file." << std::endl;
        return false;
      }
      is_out = true;
    }
    else if(iequals("-n",arg[i]) || iequals("--noout",arg[i]))
    {
      if(is_noout)
      {
        std::cerr << "Error: No output to file specified multiple times." << std::endl;
        return false;
      }
      if(is_out)
      {
        std::cerr << "Error: No output to file conflicts with output to file." << std::endl;
        return false;
      }
      output = false;
      is_noout = true;
    }
    else
    {
      std::cerr << "Error: Unrecognized Option: " << arg[i] << std::endl;
      return false;
    }
  }
  if(!is_quantum)
  {
    std::cerr << "Error: No quantum specified." << std::endl;
    return false;
  }
  if(!is_input)
  {
    std::cerr << "Error: No input file specified." << std::endl;
    return false;
  }
  return true;
}

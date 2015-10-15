#pragma once
#include "process_input.h"
#include "trivial_vector.h"
#include "file_read.h"

namespace scheduler
{

//parse tab delimited process file and store in process_input structures
class input_parser
{
private:
  //max characters to store in buffer
  static const size_t BUFFER_SIZE = 1024*1024;
  //max characters in a text column
  static const uint32_t COL_CHAR_MAX = 12;
  //max valid priority
  static const priority_t MAX_PRIORITY = 99;
public:
  input_parser(const char* file_name);
  void parse(trivial_vector<process_input>& procs);
private:
  bool string_to_uint32(const uint8_t* str,uint32_t len,uint32_t& value);
  void read_to_newline();
private:
  file_read<uint8_t,BUFFER_SIZE> m_file;
};

}

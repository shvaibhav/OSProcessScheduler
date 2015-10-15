#include "input_parser.h"
#include <stdexcept>

namespace scheduler
{

input_parser::input_parser(const char* file_name)
:m_file(file_name)
{}

inline bool input_parser::string_to_uint32(const uint8_t* str,uint32_t len,uint32_t& value)
{
  //convert str to uint32
  //return false if not an unsigned interger
  if(len == 0)
    return false;
  value = str[0]-'0';
  if(value > 9)
    return false;
  for(uint32_t i=1; i<len; ++i)
  {
    uint32_t ch = str[i]-'0';
    if(ch > 9)
      return false;
    value = 10*value+ch;
  }
  return true;
}

inline void input_parser::read_to_newline()
{
  //read until newline is reached
  while(true)
  {
    uint8_t ch = m_file.get();
    m_file.next();
    switch(ch)
    {
    case '\r':
    case '\n':
      break;
    default:
      continue;
    }
    break;
  }
}

void input_parser::parse(trivial_vector<process_input>& procs)
{
  uint32_t pid;
  uint32_t burst;
  uint32_t arrival;
  uint32_t priority;

  uint8_t str[4][COL_CHAR_MAX];
  uint32_t nstr[4];
  uint32_t col;
  bool eof = false;
  uint8_t* cur;
  uint8_t* end;
  while(!eof)
  {
    try
    {
      col = 0;
      cur = str[0];
      end = cur+COL_CHAR_MAX;
      while(true)
      {
        uint8_t ch = m_file.get();
        m_file.next();
        switch(ch)
        {
        case '\r':
        case '\n':
          break;
        case '\t':
          nstr[col] = COL_CHAR_MAX-(end-cur);
          ++col;
          if(col < 4)
          {
            cur = str[col];
            end = cur+COL_CHAR_MAX;
            continue;
          }
          read_to_newline();
          break;
        default:
          if(cur < end)
            *(cur++) = ch;
          continue;
        }
        break;
      }
    }
    catch(std::system_error& e)
    {
      if(e.code() != std::errc::io_error)
        throw;
      eof = true;
    }
    if(col < 3)
      continue;
    else if(col == 3)
      nstr[col] = COL_CHAR_MAX-(end-cur);
    if(!string_to_uint32(str[0],nstr[0],pid))
      continue;
    if(!string_to_uint32(str[1],nstr[1],burst))
      continue;
    if(!string_to_uint32(str[2],nstr[2],arrival))
      continue;
    if(!string_to_uint32(str[3],nstr[3],priority))
      continue;
    if(burst == 0)
      continue;
    if(priority > MAX_PRIORITY)
      continue;
    procs.emplace_back(static_cast<time_input_t>(arrival),
      static_cast<time_input_t>(burst),
      static_cast<pid_t>(pid),
      static_cast<priority_t>(priority));
  }
}

}

#include "safe_file_handle.h"

namespace scheduler
{

safe_file_handle::safe_file_handle()
:m_file{nullptr}
{}

safe_file_handle::safe_file_handle(safe_file_handle&& file)
:m_file{file.m_file}
{
  file.m_file = nullptr;
}

safe_file_handle::safe_file_handle(std::FILE* file)
:m_file{file}
{}

safe_file_handle::~safe_file_handle()
{
  close();
}

safe_file_handle& safe_file_handle::operator=(safe_file_handle&& file)
{
  close();
  m_file = file.m_file;
  file.m_file = nullptr;
  return *this;
}

safe_file_handle& safe_file_handle::operator=(std::FILE* file)
{
  close();
  m_file = file;
  return *this;
}

safe_file_handle::operator bool() const
{
  return m_file != nullptr;
}

safe_file_handle::operator std::FILE*()
{
  return m_file;
}

void safe_file_handle::no_buffer()
{
  if(m_file != nullptr)
    std::setvbuf(m_file,nullptr,_IONBF,0);
}

void safe_file_handle::rewind()
{
  if(m_file != nullptr)
    std::rewind(m_file);
}

void safe_file_handle::close()
{
  if(m_file != nullptr)
    std::fclose(m_file);
  m_file = nullptr;
}

}


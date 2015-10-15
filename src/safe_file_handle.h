#pragma once
#include <cstdio>

namespace scheduler
{

//safely store std::FILE pointers to make sure they are closed properly
class safe_file_handle
{
public:
  safe_file_handle();
  safe_file_handle(safe_file_handle&& file);
  safe_file_handle(std::FILE* file);
  ~safe_file_handle();
  safe_file_handle& operator=(safe_file_handle&& file);
  safe_file_handle& operator=(std::FILE* file);
  operator std::FILE*();
  operator bool() const;
  void close();
  void no_buffer();
  void rewind();
private:
  std::FILE* m_file;
};

}

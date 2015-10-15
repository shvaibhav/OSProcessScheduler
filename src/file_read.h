#pragma once
#include "safe_file_handle.h"
#include <cstdio>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace scheduler
{

//read data as blocks of T into a buffer
//reads C blocks of T into buffer
//the buffer is filled by a second thread when the buffer is empty
template<typename T,size_t C> class file_read
{
public:
  file_read(safe_file_handle&& file)
  :m_buf1{nullptr},
  m_buf2{nullptr},
  m_buf1_n{0},
  m_buf2_n{0},
  m_buf_i{0},
  m_file(std::move(file)),
  m_read{true},
  m_swap{false},
  m_run{true}
  {
    if(!m_file)
      throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory));

    init();
  }
  file_read(const char* file_name)
  :m_buf1{nullptr},
  m_buf2{nullptr},
  m_buf1_n{0},
  m_buf2_n{0},
  m_buf_i{0},
  m_read{true},
  m_swap{false},
  m_run{true}
  {
    m_file = std::fopen(file_name,"rb");
    if(!m_file)
      throw std::system_error(std::make_error_code(std::errc::no_such_file_or_directory),file_name);

    init();
  }
  ~file_read()
  {
    //release reading thread
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_run = false;
    }
    m_condition.notify_one();
    if(m_thread.joinable())
      m_thread.join();

    std::free(m_buf1);
    std::free(m_buf2);
  }
  typename std::conditional<sizeof(T)<=sizeof(size_t),T,const T&>::type get()
  {
    if(m_buf_i < m_buf1_n)
      return m_buf1[m_buf_i];
    //eof
    if(m_buf1_n != C)
      throw std::system_error(std::make_error_code(std::errc::io_error));

    //swap buffers
    std::unique_lock<std::mutex> lock(m_mutex);
    while(!m_swap)
      m_condition.wait(lock);
    m_swap = false;
    swap();
    m_read = true;
    lock.unlock();
    m_condition.notify_one();

    return get();
  }
  void next()
  {
    ++m_buf_i;
  }
private:
  void init()
  {
    //not supported by GCC
    //static_assert(std::is_trivially_copyable<T>::value,"Type must be trivially copyable.");
    static_assert(std::is_trivially_destructible<T>::value,"Type must be trivially destructable.");

    m_buf1 = static_cast<T*>(std::malloc(sizeof(T)*C));
    if(m_buf1 == nullptr)
      throw std::system_error(std::make_error_code(std::errc::not_enough_memory));
    m_buf2 = static_cast<T*>(std::malloc(sizeof(T)*C));
    if(m_buf2 == nullptr)
      throw std::system_error(std::make_error_code(std::errc::not_enough_memory));

    m_file.no_buffer();
    m_file.rewind();

    bool ret = read();
    swap();
    if(ret)
      m_thread = std::move(std::thread(&file_read::thread,this));
  }
  void thread()
  {
    while(true)
    {
      //read next block
      std::unique_lock<std::mutex> lock(m_mutex);
      while(!m_read && m_run)
        m_condition.wait(lock);
      if(!m_run)
        return;
      m_read = false;
      bool ret = read();
      m_swap = true;
      lock.unlock();
      m_condition.notify_one();
      if(!ret)
        return;
    }
  }
  bool read()
  {
    m_buf2_n = std::fread(m_buf2,sizeof(T),C,m_file);
    return m_buf2_n == C;
  }
  void swap()
  {
    T* temp = m_buf1;
    m_buf1 = m_buf2;
    m_buf2 = temp;
    m_buf1_n = m_buf2_n;
    m_buf2_n = 0;
    m_buf_i = 0;
  }
private:
  T* m_buf1;
  T* m_buf2;
  size_t m_buf1_n;
  size_t m_buf2_n;
  size_t m_buf_i;
  safe_file_handle m_file;
  std::thread m_thread;
  std::mutex m_mutex;
  std::condition_variable m_condition;
  bool m_read;
  bool m_swap;
  bool m_run;
};

}

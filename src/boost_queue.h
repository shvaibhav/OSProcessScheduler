#pragma once
#include "process.h"

namespace scheduler
{

class boost_queue
{
private:
  //how long a process must be in the queue before it is boosted
  static const time_run_t BOOST_TIME = 100;
  //only boost processes with a priority below this
  static const priority_t MAX_BOOST = 49;
  //flag to specify that process is not eligable to be boosted
  static const time_run_t BOOST_NONE = ~static_cast<time_run_t>(0);
  //lookup table to find the boosted priority from the current priority
  static const priority_t BOOST_TABLE[49];
public:
  //add all processes that should be boosted at the specified time to the specified list
  //return false if no processes are boosted at this time
  bool get(time_run_t time,boost_slist& list);
  //insert all processes in specified list to boost queue
  void insert(time_run_t time,boost_slist& list);
  //remove process from queue
  void remove(process* proc);
  //boost all processes in list
  void boost(boost_slist& list);
private:
  boost_list m_queue;
};

}

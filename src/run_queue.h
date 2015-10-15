#pragma once
#include "process.h"

namespace scheduler
{

//100 different run queues
class run_queue
{
private:
  //number of queues
  static const priority_t NQUEUE = 100;
public:
  run_queue();
  //insert list of processes into respective run queues
  void insert(boost_slist& list);
  //remove list of processes from respective run queues
  void remove(boost_slist& list);
  //remove front process from highest nonempty queue
  process* top();
private:
  run_list m_queue[NQUEUE];
  priority_t m_max;
  bool m_is_max;
};

}

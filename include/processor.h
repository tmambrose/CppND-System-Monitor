#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
 // DONE: Declare any necessary private members
 private:
   vector<string> cpu_stats_ {};
   float user_ {};  // Time (Hz) spent in user mode.
   float nice_ {};  // Time (Hz) spent in user mode with low priority (nice).
   float system_ {};  // Time (Hz) spent in system mode.
   float idle_ {};  // Time (Hz) spent in the idle task.
   float iowait_ {};  // iowait (Hz) (since Linux 2.5.41); Time waiting for I/O to complete (unreliable, see man pages).
   float irq_ {};  // Time (Hz) servicing interrupts (since Linux 2.6.0).
   float softIrq_ {};  // Time (Hz) servicing softirqs (since Linux 2.6.0).
   float steal_ {};  // Stolen time (Hz), which is the time spent in other operating systems when running in a virtualized environment (since Linux 2.6.11).
   float guest_ {};  // Time (Hz) spent running a virtual CPU for guest operating systems under the control of the Linux kernel (since Linux 2.6.24).
   float guestNice_ {};  // Time (Hz) spent running a niced guest (virtual CPU for guest operating systems under the control of the Linux kernel) (since Linux 2.6.33).
};

#endif
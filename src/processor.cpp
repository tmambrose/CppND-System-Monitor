#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
/*
    The amount of time, measured in units of USER_HZ
    (1/100ths of a second on most architectures, use
    sysconf(_SC_CLK_TCK) to obtain the right value),
    that the system ("cpu" line) or the specific CPU
    ("cpuN" line) spent in various states.
*/
float Processor::Utilization() {
    float activeJiffies = static_cast<float>(LinuxParser::ActiveJiffies());
    float totalJiffies = static_cast<float>(LinuxParser::Jiffies());
    float utilization = activeJiffies / totalJiffies;
    return utilization; 
}
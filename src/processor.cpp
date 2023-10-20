#include <string>
#include <vector>
#include <iostream>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;

// Initialize aggregate CPU processor
// TODO (unrequired): specify CPU for multi-core to return
// report for each core.
Processor::Processor() {
    vector<string> cpu_stats = LinuxParser::CpuUtilization();

    user_ = stof(cpu_stats[LinuxParser::kUser_]);
    nice_ = stof(cpu_stats[LinuxParser::kNice_]);
    system_ = stof(cpu_stats[LinuxParser::kSystem_]);
    idle_ = stof(cpu_stats[LinuxParser::kIdle_]);
    iowait_ = stof(cpu_stats[LinuxParser::kIOwait_]);
    irq_ = stof(cpu_stats[LinuxParser::kIRQ_]);
    softIrq_ = stof(cpu_stats[LinuxParser::kSoftIRQ_]);
    steal_ = stof(cpu_stats[LinuxParser::kSteal_]);
    guest_ = stof(cpu_stats[LinuxParser::kGuest_]);
    guestNice_ = stof(cpu_stats[LinuxParser::kGuestNice_]);
}

// TODO: Return the aggregate CPU utilization
/*
    The amount of time, measured in units of USER_HZ
    (1/100ths of a second on most architectures, use
    sysconf(_SC_CLK_TCK) to obtain the right value),
    that the system ("cpu" line) or the specific CPU
    ("cpuN" line) spent in various states.
*/
float Processor::Utilization() {
    Processor cpu = Processor();

    float idle = cpu.idle_ + cpu.iowait_;
    float nonIdle = cpu.user_ + cpu.nice_ + cpu.system_ + cpu.irq_ + cpu.softIrq_ + cpu.steal_;
    float total = idle + nonIdle;

    float utilization = (total - idle) / total;

    return utilization; 
}
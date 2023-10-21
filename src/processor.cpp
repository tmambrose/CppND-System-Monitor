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
    cpu_stats_ = LinuxParser::CpuUtilization();
    user_ = stof(cpu_stats_[LinuxParser::kUser_]);
    nice_ = stof(cpu_stats_[LinuxParser::kNice_]);
    system_ = stof(cpu_stats_[LinuxParser::kSystem_]);
    idle_ = stof(cpu_stats_[LinuxParser::kIdle_]);
    iowait_ = stof(cpu_stats_[LinuxParser::kIOwait_]);
    irq_ = stof(cpu_stats_[LinuxParser::kIRQ_]);
    softIrq_ = stof(cpu_stats_[LinuxParser::kSoftIRQ_]);
    steal_ = stof(cpu_stats_[LinuxParser::kSteal_]);
    guest_ = stof(cpu_stats_[LinuxParser::kGuest_]);
    guestNice_ = stof(cpu_stats_[LinuxParser::kGuestNice_]);
    float idle = idle_ + iowait_;
    float nonIdle = user_ + nice_ + system_ + irq_ + softIrq_ + steal_;
    float total = idle + nonIdle;
    float utilization = (total - idle) / total;
    return utilization; 
}
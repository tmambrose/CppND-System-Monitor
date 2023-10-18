#include <string>
#include <vector>
#include <iostream>

#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    vector<string> cpu_stats = LinuxParser::CpuUtilization();

    float kUser = stof(cpu_stats[LinuxParser::kUser_]);
    float kNice = stof(cpu_stats[LinuxParser::kNice_]);
    float kSystem = stof(cpu_stats[LinuxParser::kSystem_]);
    float kIRQ = stof(cpu_stats[LinuxParser::kIRQ_]);
    float kSoftIRQ = stof(cpu_stats[LinuxParser::kSoftIRQ_]);
    float kSteal = stof(cpu_stats[LinuxParser::kSteal_]);

    float kIdle = stof(cpu_stats[LinuxParser::kIdle_]);
    float kIOwait = stof(cpu_stats[LinuxParser::kIOwait_]);

    float idle = kIdle + kIOwait;
    float nonIdle = kUser + kNice + kSystem + kIRQ + kSoftIRQ + kSteal;
    float total = idle + nonIdle;

    float utilization = (total - idle) / total;

    return utilization; 
}
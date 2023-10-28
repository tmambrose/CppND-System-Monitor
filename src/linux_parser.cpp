#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
using namespace LinuxParser;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line, key, value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key, value;
  float memTotal{0.0}, memFree{0.0}, cached{0.0}, sReclaimable{0.0}, buffers{0.0};
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::remove(line.begin(), line.end(), ' ');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") memTotal = stof(value);
        if (key == "MemFree") memFree = stof(value);
        if (key == "Buffers") buffers = stof(value);
        if (key == "Cached") cached = stof(value);
        if (key == "SReclaimable") sReclaimable = stof(value);
      }
    }
  }
  float used = memTotal - memFree - cached - sReclaimable - buffers;
  float memUtil = used / memTotal;
  return memUtil;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string line, key;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key;
  }
  return stoi(key); 
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  unsigned long int activeJiffies = LinuxParser::ActiveJiffies();
  unsigned long int idleJiffies = LinuxParser::IdleJiffies();
  return activeJiffies + idleJiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<LinuxParser::CPUStates> states = {
    LinuxParser::kUser_,    // Time (Hz) spent in user mode.
    LinuxParser::kNice_,    // Time (Hz) spent in user mode with low priority (nice).
    LinuxParser::kSystem_,  // Time (Hz) spent in system mode.
    LinuxParser::kIRQ_,     // Time (Hz) servicing interrupts (since Linux 2.6.0).
    LinuxParser::kSoftIRQ_, // Time (Hz) servicing softirqs (since Linux 2.6.0).
    LinuxParser::kSteal_,   // Stolen time (Hz), which is the time spent in other operating systems when running in a virtualized environment (since Linux 2.6.11).
  };
  vector<string> cpu = LinuxParser::CpuUtilization();
  unsigned long int jiffies {0};
  for (auto state : states) {
    jiffies += std::stol(cpu[state]);
  }
  return jiffies; 
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<LinuxParser::CPUStates> states = {
    LinuxParser::kIdle_,    // Time (Hz) spent in the idle task.
    LinuxParser::kIOwait_,  // iowait (Hz) (since Linux 2.5.41); Time waiting for I/O to complete (unreliable, see man pages).
  };
  vector<string> cpu = LinuxParser::CpuUtilization();
  unsigned long int jiffies {0};
  for (auto state : states) {
    jiffies += std::stol(cpu[state]);
  }
  return jiffies; 
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpu_stats;
  string line, item;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> item) {
      if (item == "cpu") {
        continue;
      }
      cpu_stats.emplace_back(item);
    }
  }
  return cpu_stats; 
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return LinuxParser::Pids().size(); }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid") {
        break;
      }
    }
  }
  return value; 
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  string line, userName, xDelimiter, passwdUid; 
  std::ifstream stream(LinuxParser::kPasswordPath);
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> userName >> xDelimiter >> passwdUid;
      if (uid == passwdUid) {
        break;
      }
    }
  }
  return userName; 
}

// DONE: Read and return the uptime of a process
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
long LinuxParser::UpTime(int pid) { 
  vector<string> stats {};
  string path = LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatFilename;
  std::ifstream stream(path);
  if (stream.is_open()) {
    std::string line;
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      string value;
      while (linestream >> value) {
        stats.emplace_back(value);
      }
    }
  }
  const unsigned short int STARTTIME_IDX = 21; // #22 start time
  long starttime = stoi(stats[STARTTIME_IDX]);
  long uptime = LinuxParser::UpTime() - ( starttime / sysconf(_SC_CLK_TCK) );
  return uptime;
}
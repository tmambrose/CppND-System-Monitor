#include <string>
#include <iomanip>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
// string Format::ElapsedTime(long seconds[[maybe_unused]]) { return string(); }

string Format::ElapsedTime(long seconds) {
    std::string hh, mm, ss;
    ss = std::to_string(seconds % 60);
    mm = std::to_string((seconds / 60) % 60);
    hh = std::to_string(seconds / 3600);
    string hms = hh.length() > 1 ? hh : "0" + hh;
    hms += ":";
    hms += mm.length() > 1 ? mm : "0" + mm;
    hms += ":";
    hms += ss.length() > 1 ? ss : "0" + ss;
    return hms; 
}
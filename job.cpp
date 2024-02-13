#include <iostream>
#include <time.h>
#include "job.h"


Job::Job(int id, bool is_bg, std::string cmd, int pid) : id(id), state((is_bg)? State.BG : State.FG), process(process), pid(pid), time_stamp(time(NULL)) {}

Job::~Job() { std::cout << "Job with ID " << id << " is destroyed." << std::endl; }

Job::set_time_stamp() { this.time_stamp = time(NULL); }
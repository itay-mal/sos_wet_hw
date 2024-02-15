#include <iostream>
#include <time.h>
#include "job.h"


Job::Job(int id, std::string cmd, int pid) : id(id), cmd(cmd), pid(pid),  is_stopped(false), time_stamp(time(NULL)) {}

Job::~Job() {}

void Job::set_time_stamp() { this->time_stamp = time(NULL); }

int Job::get_elapsed_time(time_t time_now) {
    return (int)difftime(time_now, this->time_stamp);
}
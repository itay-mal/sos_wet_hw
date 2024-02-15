#ifndef JOB_H
#define JOB_H

#include <string>
#include <ctime>
#include <vector>


class Job {
    public:
    int id;
    std::string cmd;
    int pid;
    bool is_stopped;
    time_t time_stamp;

    Job(int id, std::string cmd, int pid);
    ~Job();
    void set_time_stamp();
    int get_elapsed_time(time_t time_now);

};

typedef std::vector<Job> Jobs_t;

#endif
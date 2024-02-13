#ifndef JOB_H
#define JOB_H

#include <string>
#include <ctime>


class Job {
    public:
    int id;
    bool is_stopped;
    time_t time_stamp;
    std::string cmd;
    int pid;

    Job(int id, std::string cmd, int pid);
    ~Job();
    void set_time_stamp();
    int get_elapsed_time(time_t time_now)

};

typedef std::vector<Job> Jobs_t;

#endif
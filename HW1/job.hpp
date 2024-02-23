#ifndef JOB_H
#define JOB_H

#include <string>
#include <ctime>
#include <vector>
#include <algorithm>
#include <iostream>
#include <time.h>


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
    int get_elapsed_time(const time_t time_now);
    void print_job();

};

class Jobs {
    public:
    std::vector<Job> jobs_list;
    Jobs();
    ~Jobs();
    void put_job(Job *job);
    Job& get_job_by_pid(int pid);
    Job* get_job_by_job_id(int job_id);
    void print_jobs();
    void remove_job_by_pid(int pid);
    int get_next_job_id();   
};

#endif

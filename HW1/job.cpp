#include "job.hpp"


Job::Job(int id, std::string cmd, int pid) : id(id), cmd(cmd), pid(pid),  is_stopped(false), time_stamp(time(NULL)) {}

Job::~Job() {}

void Job::set_time_stamp() { this->time_stamp = time(NULL); }

int Job::get_elapsed_time(const time_t time_now) {
    return static_cast<int>(difftime(time_now, this->time_stamp));
}

void Job::print_job() {
    std::cout << this->id << " " << this->pid << " " << this->cmd << std::endl;
}

Jobs::Jobs(): jobs_list() {}

Jobs::~Jobs() {}

// Add a job to the jobs_list
void Jobs::put_job(Job *job) {
    Job newjob = *job;
    this->jobs_list.push_back(newjob);
}

// Get a job by its JOB_ID
Job* Jobs::get_job_by_job_id(int job_id) {
    for (auto& job : this->jobs_list) {
        if (job.id == job_id) {
            return &job;
        }
    }
    throw std::runtime_error("Job with given PID not found");
}

// Get a job by its PID
Job& Jobs::get_job_by_pid(int pid) {
    for (auto& job : this->jobs_list) {
        if (job.pid == pid) {
            return job;
        }
    }
    throw std::runtime_error("Job with given PID not found");
}

// Print all jobs
void Jobs::print_jobs() {
    std::sort(this->jobs_list.begin(), this->jobs_list.end(), [](const Job& a, const Job& b) {
			return a.id < b.id;
	});

    time_t time_now = time(NULL);
    for(auto& j : this->jobs_list){
        std::cout << "[" << j.id << "] " << j.cmd << " : " << j.pid << " " << j.get_elapsed_time(time_now) << " secs" << ((j.is_stopped) ? " (stopped)": "") << std::endl;
    }
}

// Remove a job by its PID
void Jobs::remove_job_by_pid(int pid) {
    // std::cout << "removing job with pid " << pid << std::endl;
    for (auto it = this->jobs_list.begin(); it != this->jobs_list.end(); ++it) {
        if (it->pid == pid) {
            this->jobs_list.erase(it);
            return;
        }
    }
    throw std::runtime_error("Job with given PID not found");
}

// Get the next available job ID
int Jobs::get_next_job_id() {
    int max_id = 0;
    for (const auto& job : jobs_list) {
        if (job.id > max_id) {
            max_id = job.id;
        }
    }
    return max_id + 1;
}

#ifndef _LOG_HPP
#define _LOG_HPP

#include <fstream>
#include <pthread.h>
#include <iostream>
#include <sstream>
#include <type_traits>

class Logfile {
    public:
        pthread_mutex_t log_lock;
        std::ofstream logfile;
        Logfile();
        ~Logfile();
        void print_to_log(const std::stringstream &message);
};
#endif
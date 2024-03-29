#include "log.hpp"


Logfile::Logfile(){
    logfile.open("log.txt",std::ios_base::out);
    if(pthread_mutex_init(&log_lock, NULL)){
        perror("Bank error: pthread_mutex_init failed");
        exit(0);
    }
}
Logfile::~Logfile(){
    if(pthread_mutex_destroy(&log_lock)){
        perror("Bank error: pthread_mutex_destroy failed");
        exit(0);
    }
    logfile.close();
}


void Logfile::print_to_log(const std::stringstream &message) {
    // Write the message to the log file
    if(pthread_mutex_lock(&log_lock)){
        perror("Bank error: pthread_mutex_lock failed");
        exit(0);
    }
    logfile << message.str() << std::endl;
    if(pthread_mutex_unlock(&log_lock)){
        perror("Bank error: pthread_mutex_unlock failed");
        exit(0);
    }
}

#include "log.hpp"

std::ofstream logfile("log.txt");

void close_log(){
    if (logfile.is_open()) {
        logfile.close();
    }
}

void print_to_log(const std::stringstream &message) {
    // Open the log file in append mode
    std::ofstream logfile("log.txt", std::ios_base::app);
    // Write the message to the log file
    logfile << message.str() << std::endl;
    // Close the log file
    logfile.close();
}
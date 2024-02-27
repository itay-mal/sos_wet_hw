#include "log.hpp"

std::ofstream logfile("log.txt");

// template<typename T>
// void print_to_log(const T& value) {
//     // Check if the type can be printed to std::ostream
//     if(!logfile.is_open()){
//         logfile.open("log.txt", std::ios_base::app);
//     }
//     logfile << value << std::endl;
// }

// void print_to_log(std::string value){
//     if(!logfile.is_open()){
//         logfile.open("log.txt", std::ios_base::app);
//     }
//     logfile << value << std::endl;
// }

// void print_to_log(const char *value){
//     if(!logfile.is_open()){
//         logfile.open("log.txt", std::ios_base::app);
//     }
//     logfile << value << std::endl;
// }

void close_log(){
    if (logfile.is_open()) {
        logfile.close();
    }
}

void print_to_log(std::string message) {
    // Open the log file in append mode
    std::ofstream logfile("log.txt", std::ios_base::app);
    // Write the message to the log file
    logfile << message << std::endl;
    // Close the log file
    logfile.close();
}



// template<typename... Args>
// void print_to_log(const Args&... args) {
//     // Open the log file in append mode
//     std::ofstream logfile("log.txt", std::ios_base::app);
//     if (!logfile.is_open()) {
//         // Handle error if unable to open the log file
//         std::cerr << "Error: Unable to open log file." << std::endl;
//         return;
//     }

//     // Create a stringstream to format the message
//     std::stringstream ss;
//     ((ss << args), ...); // Use fold expression to append all arguments to stringstream

//     // Write the formatted message to the log file
//     logfile << ss.str() << std::endl;

//     // Close the log file
//     logfile.close();
// }
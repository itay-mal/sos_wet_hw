#ifndef _ATM_HPP
#define _ATM_HPP

#include <fstream>
#include <pthread.h>
#include "bank.hpp"
#include "log.hpp"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <typeinfo>
#include <unistd.h>

extern Bank bank;

class Atm {
public:
    std::ifstream actions_file;
    pthread_t thread;
    int identifier;
    Atm(std::string input_file, int i);
    // Atm(const Atm& other);
    ~Atm();
    void run();
};

#endif
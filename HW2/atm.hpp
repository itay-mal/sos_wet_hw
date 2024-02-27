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

extern Bank bank;

class Atm {
public:
    std::ifstream actions_file;
    pthread_t thread;
    int identifier;
    Atm(std::string input_file, int i);
    // Atm(const Atm& other);
    ~Atm();
    void open_account(int id, int password, int initial_amount);
    void close_account(int id, int password);
    void withdraw_cash(int id, int password, int amount);
    void deposit_cash(int id, int password, int amount);
    void check_balance(int id, int password);
    void transfer_between_accounts(int source_account, int password, int target_account, int amount);
    void run();
};

#endif
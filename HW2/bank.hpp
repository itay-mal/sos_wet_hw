#ifndef _BANK_HPP
#define _BANK_HPP

#include <map>
#include <iostream>
#include <sstream>
#include "account.hpp"
#include "log.hpp"
#include <algorithm>
#include <random>
#include <cmath>
#include <unistd.h>
#include <pthread.h>

class Bank {
    public:
        // TODO: consider koriim-kotvim list wise
        // account_list_lock read
        // account_list_lock write
        std::map<int, Account> account_map;
        int balance; // the bank balance
        Bank();
        void run(bool *atms_done);
        void get_commision();
        void print_all_balances();
        void open_account(int id, int password, int initial_amount, int atm_id);
        void remove_account(int id, int password, int atm_id);
        void check_balance(int id, int password, int atm_id);
        void deposit_withdraw(int id, int password, int amount, int atm_id);
        void transfer_between_accounts(int source_account, int password, int target_account, int amount, int atm_id);
};

#endif
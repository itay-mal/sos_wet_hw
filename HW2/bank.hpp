#ifndef _BANK_HPP
#define _BANK_HPP

#include <map>
#include <iostream>
#include <sstream>
#include "account.hpp"
#include "log.hpp"
#include <algorithm>
#include <random>

class Bank {
    public:
        // account_list_lock
        std::map<int, account> account_map;
        int balance; // the bank balance
        Bank();
        void get_commision();
        void print_all_balances();
        void open_account(int id, int password, int initial_amount, int atm_id);
        void remove_account(int id, int password, int atm_id);
        void check_balance(int id, int password, int atm_id);
        void deposit_withdraw(int id, int password, int amount, int atm_id);
        void transfer_between_accounts(int source_account, int password, int target_account, int amount, int atm_id);
};

#endif
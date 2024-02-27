#ifndef _BANK_HPP
#define _BANK_HPP

#include <vector>
#include "account.hpp"

class Bank {
    public:
    //  account_list_lock
    std::vector<account> accounts_list;
    account bank_account;
    void get_commision();
    void print_all_balances();
    bool is_account_exist(int account_id);
};
#endif
#include "bank.hpp"

// Define the member function get_commision
void Bank::get_commision() {
    // Implementation of get_commision goes here
    // For example:
    // calculate commission and update bank_account
}

bool Bank::is_account_exist(int account_id){
    for(auto account:this->accounts_list){
        if(account.id == account_id){ return true; }
    } 
    return false;
}

#include "bank.hpp"


Bank::Bank() : account_map(), balance(0) {} // Initialize balance to 0

// Define the member function get_commision
void Bank::get_commision() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, 5); // Define the distribution
    int random_int = dis(gen); // Generate a random integer between 1 and 5 (inclusive)
    // lock list
    // for account in list
        //lock write account
            // 
        //unlock write account
    //unlock list
}

void Bank::open_account(int id, int password, int initial_amount, int atm_id){
    //lock    
    std::stringstream msg;
    auto it = this->account_map.find(id);
    if(it != account_map.end()){ // account already exists
        msg << "Error " << atm_id << ": Your transaction failed - account with the same id exists";
        print_to_log(msg);
    } else {
        msg << atm_id << ": New account id is " << id << " with password " << password << " and initial balance " << initial_amount;
        print_to_log(msg);
        account_map[id] = {initial_amount, password};
    }
    // unlock
}

void Bank::remove_account(int id, int password, int atm_id) {
    //lock
    std::stringstream msg;
    auto it = this->account_map.find(id);
    if(it == account_map.end()){ // does not exist
        msg << "Error " << atm_id << ": Your transaction failed - account id " << id << " does not exists";
        print_to_log(msg);
    } else {
        account& acc = it->second;
        if (acc.password == password) { // Password matches
            msg << atm_id << ": Account " << id << " is now closed. Balance was " << acc.balance;
            print_to_log(msg);
            account_map.erase(it);
        } else { // Wrong password
            msg << "Error " << atm_id << ": Your transaction failed - password for account id " << id << " is incorrect";
            print_to_log(msg);
        }
    }
    //unlock
}

void Bank::deposit_withdraw(int id, int password, int amount, int atm_id){
    //lock_list
    std::stringstream msg;
    auto it = this->account_map.find(id);
    if(it == account_map.end()){ // does not exist
        // unlock_list
        msg << "Error " << atm_id << ": Your transaction failed - account id " << id << " does not exists";
        print_to_log(msg);
    } else {
        // Find the account with the given ID and password
        account& acc = it->second;
        // lock_account
        // unlock_list
        if (acc.password == password) { // Password matches, deposit cash
            if(acc.balance + amount < 0){
                // unlock account
                msg << "Error " << atm_id << ": Your transaction failed - account id " << id << " balance is lower than " << abs(amount);
                print_to_log(msg);
            } else {
                acc.balance += amount;
                // lock read
                // unlock write
                msg << atm_id << ": Account " << id << " new balance is " << acc.balance << " after " << abs(amount) << " $ was " << ((amount<0)? "withdrew":  "deposited");
                //unlock_account
                print_to_log(msg);
            }
        } else { // Wrong password
            // unlock_account
            msg << "Error " << atm_id << ": Your transaction failed - password for account id " << id << " is incorrect";
            print_to_log(msg);
        }
    }
}
void Bank::check_balance(int id, int password, int atm_id){
    //lock_list
    std::stringstream msg;
    auto it = this->account_map.find(id);
    if(it == account_map.end()){ // does not exist
        // unlock_list
        msg << "Error " << atm_id << ": Your transaction failed - account id " << id << " does not exists";
        print_to_log(msg);
    } else {
        // Find the account with the given ID and password
        account& acc = it->second;
        // lock_account_read
        // unlock_list
        if (acc.password == password) { // Password matches
                msg << atm_id << ": Account " << id << " balance is "  << acc.balance;
                print_to_log(msg);
                // unlock account_read
        } else { // Wrong password
            // unlock account_read
            msg << "Error " << atm_id << ": Your transaction failed - password for account id " << id << " is incorrect";
            print_to_log(msg);
        }
    }
}

void Bank::transfer_between_accounts(int source_account, int password, int target_account, int amount, int atm_id){
    //lock_list
    std::stringstream msg;
    auto it_source = this->account_map.find(source_account);
    auto it_target = this->account_map.find(target_account);
    if(it_source == account_map.end()){ // source does not exist
        // unlock_list
        msg << "Error " << atm_id << ": Your transaction failed - account id " << source_account << " does not exists";
        print_to_log(msg);
        return;
    }
    account *src_acc = &(it_source->second);
    // lock source read
    
    account *tgt_acc = nullptr;
    if(it_target != account_map.end()){ // target exists
        tgt_acc = &(it_target->second);
        // lock target read
    }

    // unlock_list
    if(src_acc->password != password){ // source password does not match
        // unlock source read
        if(tgt_acc != nullptr){ 
            // unlock target read
        }
        msg << "Error " << atm_id << ": Your transaction failed - password for account id " << source_account << " is incorrect";
        print_to_log(msg);
        return;
    }
    if(src_acc->balance < amount){ // source money insufficient
        // unlock source read
        if(tgt_acc != nullptr){ 
            // unlock target read
        }    
        msg << "Error " << atm_id << ": Your transaction failed - account id " << source_account << " balance is lower than " << amount;
        print_to_log(msg);
        return;
    }
    if(tgt_acc == nullptr){
        // unlock souece read
        // unlock target read
        msg << "Error " << atm_id << ": Your transaction failed - account id " << target_account << " does not exists";
        print_to_log(msg);
        return;
    }
    // unlock read
    // lock both write locks
    src_acc->balance -= amount;
    tgt_acc->balance += amount;
    msg << atm_id << ": Transfer " << amount << " from account " << source_account << " to account " << target_account << " new account balance is " << src_acc->balance << " new target account balance is " << tgt_acc->balance;
    print_to_log(msg);
    // unlock both write locks
    // unlock both read locks
}

void Bank::print_all_balances() {

    std::cout << "\033[2J" << "\033[1;1H"; // delete screen and move to top left corner
    std::cout << "Current Bank Status" << std::endl;
    //lock accounts list
    for(auto pair:this->account_map){
        std::cout << "Account " << pair.first << ": Balance - " << pair.second.balance << " $, Account Password - " << pair.second.password << std::endl;
    }
    //unlock
    // lock bank balance
    std::cout << "The Bank has " << this->balance << " $" << std::endl;
}
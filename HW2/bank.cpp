#include "bank.hpp"


Bank::Bank() : account_map(), balance(0) {} // Initialize balance to 0

// Define the member function get_commision
void Bank::get_commision() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, 5); // Define the distribution
    int random_int = dis(gen); // Generate a random integer between 1 and 5 (inclusive)
    // lock list
    // lock bank balance
    for (auto& pair : account_map) {
        // lock write account
        int commision = std::round((pair.second.balance * random_int)/100.0);
        pair.second.balance -= commision;
        this->balance += commision; 
        std::stringstream msg;
        msg << "Bank: commissions of " << random_int << " % were charged, the bank gained " << commision << " $ from account " << pair.first;
        print_to_log(msg);
        //unlock write account
    }
    // unlock bank balance
    sleep(1);
    // unlock list
}

void Bank::open_account(int id, int password, int initial_amount, int atm_id){
    //lock list    
    std::stringstream msg;
    auto it = this->account_map.find(id);
    if(it != account_map.end()){ // account already exists
        msg << "Error " << atm_id << ": Your transaction failed - account with the same id exists";
        print_to_log(msg);
        sleep(1);
        // unlock list
    } else {   
        Account new_acc(initial_amount, password); // write locked by default
        account_map[id] = new_acc;
        msg << atm_id << ": New account id is " << id << " with password " << password << " and initial balance " << initial_amount;
        print_to_log(msg);
        sleep(1);
        // unlock account write lock
        // unlock list
    }
}

void Bank::remove_account(int id, int password, int atm_id) {
    //lock list
    std::stringstream msg;
    auto it = this->account_map.find(id);
    if(it == account_map.end()){ // does not exist
        msg << "Error " << atm_id << ": Your transaction failed - account id " << id << " does not exists";
        print_to_log(msg);
    } else {
        // lock account write
        Account& acc = it->second;
        if (acc.password == password) { // Password matches
            msg << atm_id << ": Account " << id << " is now closed. Balance was " << acc.balance;
            account_map.erase(it); // account no longer exist - no lock to release
            print_to_log(msg);
        } else { // Wrong password
            msg << "Error " << atm_id << ": Your transaction failed - password for account id " << id << " is incorrect";
            print_to_log(msg);
            // unlock account write
        }
    }
    sleep(1);
    // unlock list
}

void Bank::deposit_withdraw(int id, int password, int amount, int atm_id){
    //lock_list
    std::stringstream msg;
    auto it = this->account_map.find(id);
    if(it == account_map.end()){ // does not exist
        sleep(1);
        // unlock_list
        msg << "Error " << atm_id << ": Your transaction failed - account id " << id << " does not exists";
        print_to_log(msg);
    } else {
        // Find the account with the given ID and password
        // lock_account write
        Account& acc = it->second;
        // unlock_list
        if (acc.password == password) { // Password matches, deposit cash
            if(acc.balance + amount < 0){
                sleep(1);
                // unlock account
                msg << "Error " << atm_id << ": Your transaction failed - account id " << id << " balance is lower than " << abs(amount);
                print_to_log(msg);
            } else {
                acc.balance += amount;
                msg << atm_id << ": Account " << id << " new balance is " << acc.balance << " after " << abs(amount) << " $ was " << ((amount<0)? "withdrew":  "deposited");
                print_to_log(msg);
                sleep(1);
                // unlock account write
            }
        } else { // Wrong password
            sleep(1);
            // unlock_account write
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
        msg << "Error " << atm_id << ": Your transaction failed - account id " << id << " does not exists";
        print_to_log(msg);
        sleep(1);
        // unlock_list
    } else {
        // Find the account with the given ID and password
        Account& acc = it->second;
        // lock_account_read
        // unlock_list
        if (acc.password == password) { // Password matches
                msg << atm_id << ": Account " << id << " balance is "  << acc.balance;
                print_to_log(msg);
                sleep(1);
                // unlock account_read
        } else { // Wrong password
            msg << "Error " << atm_id << ": Your transaction failed - password for account id " << id << " is incorrect";
            print_to_log(msg);
            sleep(1);
            // unlock account_read
        }
    }
}

void Bank::transfer_between_accounts(int source_account, int password, int target_account, int amount, int atm_id){
    //lock_list
    std::stringstream msg;
    auto it_source = this->account_map.find(source_account);
    auto it_target = this->account_map.find(target_account);
    if(it_source == account_map.end()){ // source does not exist
        msg << "Error " << atm_id << ": Your transaction failed - account id " << source_account << " does not exists";
        print_to_log(msg);
        sleep(1);
        // unlock_list
    } else { // source exist
        // lock source write
        Account *src_acc = &(it_source->second);
        
        Account *tgt_acc = nullptr;
        if(it_target != account_map.end()){ // target exists
            // lock target write
            tgt_acc = &(it_target->second);
        }
        // unlock_list
        if(src_acc->password != password){ // source password does not match
            // unlock source write
            if(tgt_acc != nullptr){ 
                // unlock target write
            }
            msg << "Error " << atm_id << ": Your transaction failed - password for account id " << source_account << " is incorrect";
            print_to_log(msg);
            sleep(1);
            return;
        }
        if(src_acc->balance < amount){ // source money insufficient
            // unlock source write
            if(tgt_acc != nullptr){ 
                // unlock target write
            }    
            msg << "Error " << atm_id << ": Your transaction failed - account id " << source_account << " balance is lower than " << amount;
            print_to_log(msg);
            sleep(1);
            return;
        }
        if(tgt_acc == nullptr){
            msg << "Error " << atm_id << ": Your transaction failed - account id " << target_account << " does not exists";
            print_to_log(msg);
            sleep(1);
            // unlock source write
            return;
        }
        src_acc->balance -= amount;
        tgt_acc->balance += amount;
        msg << atm_id << ": Transfer " << amount << " from account " << source_account << " to account " << target_account << " new account balance is " << src_acc->balance << " new target account balance is " << tgt_acc->balance;
        print_to_log(msg);
        sleep(1);
        // unlock both write locks
    }
}

void Bank::print_all_balances() {

    std::cout << "\033[2J" << "\033[1;1H"; // delete screen and move to top left corner
    std::cout << "Current Bank Status" << std::endl;
    //lock accounts list
    for(auto pair:this->account_map){
        // lock account read
        std::cout << "Account " << pair.first << ": Balance - " << pair.second.balance << " $, Account Password - " << pair.second.password << std::endl;
        // unlock account read
    }
    // lock bank balance
    std::cout << "The Bank has " << this->balance << " $" << std::endl;
    // unlock bank balance
    //unlock list
}

void Bank::run(bool* atms_done) {
    int counter = 0;
    while (!(*(bool*)atms_done))
    {
        sleep(0.5);
        this->print_all_balances();
        counter++;
        if(counter == 6){
            counter = 0;
            this->get_commision();
        }
    }
    sleep(1);
}
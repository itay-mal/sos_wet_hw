#include "bank.hpp"


Bank::Bank() : account_map(), balance(0) {
    if(pthread_mutex_init(&list_lock, NULL)){
        perror("Bank error: pthread_mutex_init failed");
        exit(0);
    } // initialized as open by default
    if(pthread_mutex_init(&bank_balance_lock, NULL)){ // initialized as open by default
        perror("Bank error: pthread_mutex_init failed");
        exit(0);
    }
} // Initialize balance to 0    

void Bank::get_commision() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, 5); // Define the distribution
    int random_int = dis(gen); // Generate a random integer between 1 and 5 (inclusive)
    lock_account_map();
    if(pthread_mutex_lock(&bank_balance_lock)){
        perror("Bank error: pthread_mutex_lock failed");
        exit(0);
    }
    for (auto& pair : account_map) {
        pair.second.lock_writer();
        int commision = std::round((pair.second.balance * random_int)/100.0);
        pair.second.balance -= commision;
        this->balance += commision;
        std::stringstream msg;
        msg << "Bank: commissions of " << random_int << " % were charged, the bank gained " << commision << " $ from account " << pair.first;
        print_to_log(msg);
        pair.second.unlock_writer();
    }
    if(usleep(1000*1000)){
        perror("Bank error: usleep failed");
        exit(0);
    }
    if(pthread_mutex_unlock(&bank_balance_lock)){
        perror("Bank error: pthread_mutex_lock failed");
        exit(0);
    }
    unlock_account_map();
}

void Bank::open_account(int id, int password, int initial_amount, int atm_id){
    lock_account_map();
    std::stringstream msg;
    auto it = this->account_map.find(id);
    if(it != account_map.end()){ // account already exists
        msg << "Error " << atm_id << ": Your transaction failed - account with the same id exists";
        print_to_log(msg);
        if(usleep(1000*1000)){
            perror("Bank error: usleep failed");
            exit(0);
        }
        unlock_account_map();
    } else {   
        Account new_acc(initial_amount, password);
        account_map[id] = new_acc;
        msg << atm_id << ": New account id is " << id << " with password " << password << " and initial balance " << initial_amount;
        print_to_log(msg);
        if(usleep(1000*1000)){
            perror("Bank error: usleep failed");
            exit(0);
        }
        unlock_account_map();
    }
}

void Bank::remove_account(int id, int password, int atm_id) {
    lock_account_map();
    std::stringstream msg;
    auto it = this->account_map.find(id);
    if(it == account_map.end()){ // does not exist
        msg << "Error " << atm_id << ": Your transaction failed - account id " << id << " does not exists";
        print_to_log(msg);
    } else {
        Account& acc = it->second;
        acc.lock_writer();
        if (acc.password == password) { // Password matches
            msg << atm_id << ": Account " << id << " is now closed. Balance was " << acc.balance;
            acc.unlock_writer();
            account_map.erase(it); // account no longer exist - no lock to release
            print_to_log(msg);
        } else { // Wrong password
            msg << "Error " << atm_id << ": Your transaction failed - password for account id " << id << " is incorrect";
            print_to_log(msg);
            acc.unlock_writer();
        }
    }
    if(usleep(1000*1000)){
        perror("Bank error: usleep failed");
        exit(0);
    }
    unlock_account_map();
}

void Bank::deposit_withdraw(int id, int password, int amount, int atm_id){
    lock_account_map();
    std::stringstream msg;
    auto it = this->account_map.find(id);
    if(it == account_map.end()){ // does not exist
        msg << "Error " << atm_id << ": Your transaction failed - account id " << id << " does not exists";
        print_to_log(msg);
        if(usleep(1000*1000)){
            perror("Bank error: usleep failed");
            exit(0);
        }
        unlock_account_map();
    } else {
        // Find the account with the given ID and password
        Account& acc = it->second;
        acc.lock_writer();
        unlock_account_map();
        if (acc.password == password) { // Password matches, deposit cash
            if(acc.balance + amount < 0){
                msg << "Error " << atm_id << ": Your transaction failed - account id " << id << " balance is lower than " << abs(amount);
                print_to_log(msg);
                if(usleep(1000*1000)){
                    perror("Bank error: usleep failed");
                    exit(0);
                }
                acc.unlock_writer();
            } else {
                acc.balance += amount;
                msg << atm_id << ": Account " << id << " new balance is " << acc.balance << " after " << abs(amount) << " $ was " << ((amount<0)? "withdrew":  "deposited");
                print_to_log(msg);
                if(usleep(1000*1000)){
                    perror("Bank error: usleep failed");
                    exit(0);
                }
                acc.unlock_writer();
            }
        } else { // Wrong password
            msg << "Error " << atm_id << ": Your transaction failed - password for account id " << id << " is incorrect";
            print_to_log(msg);
            if(usleep(1000*1000)){
                perror("Bank error: usleep failed");
                exit(0);
            }
            acc.unlock_writer();
        }
    }
}
void Bank::check_balance(int id, int password, int atm_id){
    lock_account_map();
    std::stringstream msg;
    // Find the account with the given ID
    auto it = this->account_map.find(id);
    if(it == account_map.end()){ // does not exist
        msg << "Error " << atm_id << ": Your transaction failed - account id " << id << " does not exists";
        print_to_log(msg);
        if(usleep(1000*1000)){
            perror("Bank error: usleep failed");
            exit(0);
        }
        unlock_account_map();
    } else {
        Account& acc = it->second;
        acc.lock_reader();
        unlock_account_map();
        if (acc.password == password) { // Password matches
            msg << atm_id << ": Account " << id << " balance is "  << acc.balance;
            print_to_log(msg);
            if(usleep(1000*1000)){
                perror("Bank error: usleep failed");
                exit(0);
            }
            acc.unlock_reader();
        } else { // Wrong password
            msg << "Error " << atm_id << ": Your transaction failed - password for account id " << id << " is incorrect";
            print_to_log(msg);
            if(usleep(1000*1000)){
                perror("Bank error: usleep failed");
                exit(0);
            }
            acc.unlock_reader();
        }
    }
}

void Bank::transfer_between_accounts(int source_account, int password, int target_account, int amount, int atm_id){
    lock_account_map();
    std::stringstream msg;
    auto it_source = this->account_map.find(source_account);
    auto it_target = this->account_map.find(target_account);
    if(it_source == account_map.end()){ // source does not exist
        msg << "Error " << atm_id << ": Your transaction failed - account id " << source_account << " does not exists";
        print_to_log(msg);
        if(usleep(1000*1000)){
            perror("Bank error: usleep failed");
            exit(0);
        }
        unlock_account_map();
    } else { // source exist
        it_source->second.lock_writer();
        Account *src_acc = &(it_source->second);
        
        Account *tgt_acc = nullptr;
        if(it_target != account_map.end()){ // target exists
            it_target->second.lock_writer();
            tgt_acc = &(it_target->second);
        }
        unlock_account_map();
        if(src_acc->password != password){ // source password does not match
            msg << "Error " << atm_id << ": Your transaction failed - password for account id " << source_account << " is incorrect";
            print_to_log(msg);
            if(usleep(1000*1000)){
                perror("Bank error: usleep failed");
                exit(0);
            }
            if(tgt_acc != nullptr){ 
                tgt_acc->unlock_writer();
            }
            src_acc->unlock_writer();
            return;
        }
        if(src_acc->balance < amount){ // source money insufficient
            msg << "Error " << atm_id << ": Your transaction failed - account id " << source_account << " balance is lower than " << amount;
            print_to_log(msg);
            if(usleep(1000*1000)){
                perror("Bank error: usleep failed");
                exit(0);
            }
            if(tgt_acc != nullptr){ 
                tgt_acc->unlock_writer();
            }    
            src_acc->unlock_writer();
            return;
        }
        if(tgt_acc == nullptr){ // target account not found
            msg << "Error " << atm_id << ": Your transaction failed - account id " << target_account << " does not exists";
            print_to_log(msg);
            if(usleep(1000*1000)){
                perror("Bank error: usleep failed");
                exit(0);
            }
            src_acc->unlock_writer();
            return;
        }
        src_acc->balance -= amount;
        tgt_acc->balance += amount;
        msg << atm_id << ": Transfer " << amount << " from account " << source_account << " to account " << target_account << " new account balance is " << src_acc->balance << " new target account balance is " << tgt_acc->balance;
        print_to_log(msg);
        if(usleep(1000*1000)){
            perror("Bank error: usleep failed");
            exit(0);
        }
        tgt_acc->unlock_writer();
        src_acc->unlock_writer();
    }
}

void Bank::print_all_balances() {

    lock_account_map();
    std::cout << "\033[2J" << "\033[1;1H"; // delete screen and move to top left corner
    std::cout << "Current Bank Status" << std::endl;
    for(auto pair:this->account_map){
        pair.second.lock_reader();
        std::cout << "Account " << pair.first << ": Balance - " << pair.second.balance << " $, Account Password - " << pair.second.password << std::endl;
        pair.second.unlock_reader();
    }
    
    if(pthread_mutex_lock(&bank_balance_lock)){
        perror("Bank error: pthread_mutex_lock failed");
        exit(0);
    }
    std::cout << "The Bank has " << this->balance << " $" << std::endl;
    if(pthread_mutex_unlock(&bank_balance_lock)){
        perror("Bank error: pthread_mutex_unlock failed");
        exit(0);
    }
    unlock_account_map();
}

void Bank::run(bool* atms_done) {
    int counter = 0;
    while (!(*(bool*)atms_done))
    {
        if(usleep(500*1000)){
            perror("Bank error: usleep failed");   
            exit(0);
        }
        counter++;
        if(counter == 6){
            counter = 0;
            this->get_commision();
        }
        this->print_all_balances();
    }
    if(usleep(2000*1000)){ // to ensure main thread reach pthread_join()
        perror("Bank error: usleep failed");
        exit(0);
    }
}

void Bank::lock_account_map(){
    if(pthread_mutex_lock(&list_lock)){
        perror("Bank error: pthread_mutex_lock failed");
        exit(0);
    }
}

void Bank::unlock_account_map(){
    if(pthread_mutex_unlock(&list_lock)){
        perror("Bank error: pthread_mutex_unlock failed");
        exit(0);
    }
}



#include "bank.hpp"


Bank::Bank() : account_map(), balance(0) {
    pthread_mutex_init(&list_lock, NULL); // initialized as open by default
    pthread_mutex_init(&bank_balance_lock, NULL); // initialized as open by default
} // Initialize balance to 0    

// Define the member function get_commision
void Bank::get_commision() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, 5); // Define the distribution
    int random_int = dis(gen); // Generate a random integer between 1 and 5 (inclusive)
    lock_account_map();
    pthread_mutex_lock(&bank_balance_lock);
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
    sleep(1);
    pthread_mutex_unlock(&bank_balance_lock);
    unlock_account_map();
}

void Bank::open_account(int id, int password, int initial_amount, int atm_id){
    lock_account_map();
    std::stringstream msg;
    auto it = this->account_map.find(id);
    if(it != account_map.end()){ // account already exists
        msg << "Error " << atm_id << ": Your transaction failed - account with the same id exists";
        print_to_log(msg);
        sleep(1);
        unlock_account_map();
    } else {   
        Account new_acc(initial_amount, password);
        account_map[id] = new_acc;
        msg << atm_id << ": New account id is " << id << " with password " << password << " and initial balance " << initial_amount;
        print_to_log(msg);
        sleep(1);
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
        it->second.lock_writer();
        Account& acc = it->second;
        if (acc.password == password) { // Password matches
            msg << atm_id << ": Account " << id << " is now closed. Balance was " << acc.balance;
            account_map.erase(it); // account no longer exist - no lock to release
            print_to_log(msg);
        } else { // Wrong password
            msg << "Error " << atm_id << ": Your transaction failed - password for account id " << id << " is incorrect";
            print_to_log(msg);
            it->second.unlock_writer();
        }
    }
    sleep(1);
    unlock_account_map();
}

void Bank::deposit_withdraw(int id, int password, int amount, int atm_id){
    lock_account_map();
    std::stringstream msg;
    auto it = this->account_map.find(id);
    if(it == account_map.end()){ // does not exist
        msg << "Error " << atm_id << ": Your transaction failed - account id " << id << " does not exists";
        print_to_log(msg);
        sleep(1);
        unlock_account_map();
    } else {
        // Find the account with the given ID and password
        it->second.lock_writer();
        Account& acc = it->second;
        unlock_account_map();
        if (acc.password == password) { // Password matches, deposit cash
            if(acc.balance + amount < 0){
                msg << "Error " << atm_id << ": Your transaction failed - account id " << id << " balance is lower than " << abs(amount);
                print_to_log(msg);
                sleep(1);
                acc.unlock_writer();
            } else {
                acc.balance += amount;
                msg << atm_id << ": Account " << id << " new balance is " << acc.balance << " after " << abs(amount) << " $ was " << ((amount<0)? "withdrew":  "deposited");
                print_to_log(msg);
                sleep(1);
                acc.unlock_writer();
            }
        } else { // Wrong password
            msg << "Error " << atm_id << ": Your transaction failed - password for account id " << id << " is incorrect";
            print_to_log(msg);
            sleep(1);
            acc.unlock_writer();
        }
    }
}
void Bank::check_balance(int id, int password, int atm_id){
    lock_account_map();
    std::stringstream msg;
    auto it = this->account_map.find(id);
    if(it == account_map.end()){ // does not exist
        msg << "Error " << atm_id << ": Your transaction failed - account id " << id << " does not exists";
        print_to_log(msg);
        sleep(1);
        unlock_account_map();
    } else {
        // Find the account with the given ID and password
        it->second.lock_reader();
        Account& acc = it->second;
        unlock_account_map();
        if (acc.password == password) { // Password matches
                msg << atm_id << ": Account " << id << " balance is "  << acc.balance;
                print_to_log(msg);
                sleep(1);
                acc.unlock_reader();
        } else { // Wrong password
            msg << "Error " << atm_id << ": Your transaction failed - password for account id " << id << " is incorrect";
            print_to_log(msg);
            sleep(1);
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
        sleep(1);
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
            sleep(1);
            src_acc->unlock_writer();
            if(tgt_acc != nullptr){ 
                tgt_acc->unlock_writer();
            }
            return;
        }
        if(src_acc->balance < amount){ // source money insufficient
            msg << "Error " << atm_id << ": Your transaction failed - account id " << source_account << " balance is lower than " << amount;
            print_to_log(msg);
            sleep(1);
            src_acc->unlock_writer();
            if(tgt_acc != nullptr){ 
                tgt_acc->unlock_writer();
            }    
            return;
        }
        if(tgt_acc == nullptr){ // target account not found
            msg << "Error " << atm_id << ": Your transaction failed - account id " << target_account << " does not exists";
            print_to_log(msg);
            sleep(1);
            src_acc->unlock_writer();
            return;
        }
        src_acc->balance -= amount;
        tgt_acc->balance += amount;
        msg << atm_id << ": Transfer " << amount << " from account " << source_account << " to account " << target_account << " new account balance is " << src_acc->balance << " new target account balance is " << tgt_acc->balance;
        print_to_log(msg);
        sleep(1);
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
    
    pthread_mutex_lock(&bank_balance_lock);
    std::cout << "The Bank has " << this->balance << " $" << std::endl;
    pthread_mutex_unlock(&bank_balance_lock);
    unlock_account_map();
}

void Bank::run(bool* atms_done) {
    int counter = 0;
    bank_thread = pthread_self();
    int times_in_print_all_balances = 0;
    auto last_print = std::chrono::steady_clock::now();
    while (!(*(bool*)atms_done))
    {
        // std::cout << "inside if counter is " << counter << std::endl;
        auto now = std::chrono::steady_clock::now();
        // struct timespec delay;
        // delay.tv_sec = 0;            // 0 seconds
        // delay.tv_nsec = 500000000;   // 500 million nanoseconds = 0.5 seconds
        // nanosleep(&delay, NULL);
        // auto after = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_print).count();
        if(duration > 500){
            times_in_print_all_balances++;
            std::cout << times_in_print_all_balances << std::endl;
            last_print = now;
	        std::cout << "slept for: " << duration << " milliseconds" << std::endl;
            bank_want_access = true;
            if(counter++ == 6){
                counter = 0;
                this->get_commision();
            }
            this->print_all_balances();
            bank_want_access = false;
        }
    }
    sleep(1); // to ensure main thread reach pthread_join()
}

void Bank::lock_account_map(){
    pthread_t calling_thread = pthread_self();
    // std::cout << "locking account map from " << (pthread_equal(calling_thread, bank_thread)? "bank": "atm") << " thread " << calling_thread << std::endl;
    while(true){
        pthread_mutex_lock(&list_lock);
        if(bank_want_access && !pthread_equal(calling_thread, bank_thread)){
            pthread_mutex_unlock(&list_lock); // give the bank chance to lock
        } else {
            break;
        }
    }
}

void Bank::unlock_account_map(){
    pthread_mutex_unlock(&list_lock);
}
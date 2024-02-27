#include "atm.hpp"

#define MAX_CMD_PARTS 5

Atm::Atm(std::string input_file, int i) {
    identifier = i;
    actions_file.open(input_file);
    if (!actions_file.is_open()) {
        std::cerr << "Error opening the file." << std::endl;
        // You might want to handle this error case more gracefully in a real application
    }
}

// Atm::Atm(const Atm& other) : identifier(other.identifier) {
//     this->actions_file = other.actions_file;
// }

Atm::~Atm() {
    if (actions_file.is_open()) {
        actions_file.close();
    }
    std::cout << "atm " << this->identifier << " is dead" << std::endl;
}

void Atm::run() {
    std::string line;
    while(std::getline(actions_file, line)) {
        std::vector<std::string> param;
        std::string s;
        std::stringstream ss(line);
        while (std::getline(ss, s, ' ')) {
            param.push_back(s);
        }
        switch (param[0][0])
        {
        case 'O':
            this->open_account(stoi(param[1]), stoi(param[2]), stoi(param[3]));
            break; 
        case 'D':
            this->deposit_cash(stoi(param[1]), stoi(param[2]), stoi(param[3]));
            break;
        case 'W':
            this->withdraw_cash(stoi(param[1]), stoi(param[2]), stoi(param[3]));
            break;
        case 'B':
            this->check_balance(stoi(param[1]), stoi(param[2]));
            break;
        case 'Q':
            this->close_account(stoi(param[1]), stoi(param[2]));
            break;
        case 'T':
            this->transfer_between_accounts(stoi(param[1]), stoi(param[2]), stoi(param[3]), stoi(param[4]));
            break;

        default:
            break;
        }
    }
}


void Atm::open_account(int id, int password, int initial_amount) {
    // Implement open_account logic here
    std::string msg("Error");
    msg += std::to_string(this->identifier);
    msg += "Your transaction failed : account with the same id exists";
    print_to_log(msg);

    if(bank.is_account_exist(id)) {
        // print_to_log(msg);
    }
}

void Atm::close_account(int id, int password) {
    // Implement close_account logic here
    print_to_log("Closing account..."); 
}

void Atm::withdraw_cash(int id, int password, int amount) {
    // Implement withdraw_cash logic here
    print_to_log("Withdrawing cash...");
}

void Atm::deposit_cash(int id, int password, int amount) {
    // Implement deposit_cash logic here
    print_to_log("Depositing cash...");
}

void Atm::check_balance(int id, int password) {
    // Implement check_balance logic here
    print_to_log("Checking balance...");
}

void Atm::transfer_between_accounts(int source_account, int password, int target_account, int amount) {
    // Implement transfer_between_accounts logic here
    print_to_log("Transferring between accounts...");
}



#include "atm.hpp"

#define MAX_CMD_PARTS 5

Atm::Atm(std::string input_file, int i) {
    identifier = i;
    actions_file.open(input_file);    
}


Atm::~Atm() {
    if(actions_file.is_open()){
        actions_file.close();
    }
}

void Atm::run() {
    std::string line;
    while(std::getline(actions_file, line)) {
        if (usleep(100*1000)){
            perror("Bank error: usleep failed");
            exit(0);
        }
        std::vector<std::string> param;
        std::string s;
        std::stringstream ss(line);
        while (std::getline(ss, s, ' ')) {
            param.push_back(s);
        }
        switch (param[0][0])
        {
        case 'O':   
            bank.open_account(stoi(param[1]), stoi(param[2]), stoi(param[3]), this->identifier);
            break; 
        case 'D':
            bank.deposit_withdraw(stoi(param[1]), stoi(param[2]), stoi(param[3]), this->identifier);
            break;
        case 'W':
            bank.deposit_withdraw(stoi(param[1]), stoi(param[2]), -stoi(param[3]), this->identifier);
            break;
        case 'B':
            bank.check_balance(stoi(param[1]), stoi(param[2]), this->identifier);
            break;
        case 'Q':
            bank.remove_account(stoi(param[1]), stoi(param[2]),this->identifier);
            break;
        case 'T':
            bank.transfer_between_accounts(stoi(param[1]),stoi(param[2]),stoi(param[3]),stoi(param[4]), this->identifier);
            break;

        default:
            break;
        }
    }
}

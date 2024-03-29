#include "account.hpp"

void Account::lock_writer() {
    if(pthread_mutex_lock(&write_lock)){
        perror("Bank error: pthread_mutex_lock failed");
        exit(0);
    }
}

void Account::unlock_writer() {
    if(pthread_mutex_unlock(&write_lock)){
        perror("Bank error: pthread_mutex_unlock failed");
        exit(0);
    }
}

void Account::lock_reader(){
    if(pthread_mutex_lock(&read_lock)){
        perror("Bank error: pthread_mutex_lock failed");
        exit(0);
    }
    readers_counter++;
    if (readers_counter==1) {
        if(pthread_mutex_lock(&write_lock)){
            perror("Bank error: pthread_mutex_lock failed");
            exit(0);
        }
    }
    if(pthread_mutex_unlock(&read_lock)){
        perror("Bank error: pthread_mutex_unlock failed");
        exit(0);
    }
}

void Account::unlock_reader(){
    if(pthread_mutex_lock(&read_lock)){
        perror("Bank error: pthread_mutex_lock failed");
        exit(0);
    }
    readers_counter--;
    if (readers_counter==0) {
        if(pthread_mutex_unlock(&write_lock)){
            perror("Bank error: pthread_mutex_unlock failed");
            exit(0);
        }
    }
    if(pthread_mutex_unlock(&read_lock)){
        perror("Bank error: pthread_mutex_unlock failed");
        exit(0);
    }
}

Account::Account(int _initial_balance, int _password) : balance(_initial_balance), password(_password) {
    readers_counter = 0;
    if(pthread_mutex_init(&read_lock, NULL)){
        perror("Bank error: pthread_mutex_init failed");
        exit(0);
    }
    if(pthread_mutex_init(&write_lock, NULL)){
        perror("Bank error: pthread_mutex_init failed");
        exit(0);
    }
}

Account::Account() : balance(0), password(0) {
    readers_counter = 0;
    if(pthread_mutex_init(&read_lock, NULL)){
        perror("Bank error: pthread_mutex_init failed");
        exit(0);
    }
    if(pthread_mutex_init(&write_lock, NULL)){
        perror("Bank error: pthread_mutex_init failed");
        exit(0);
    }
}

Account::~Account() {
    if(pthread_mutex_lock(&read_lock)){
        perror("Bank error: pthread_mutex_lock failed");
        exit(0);
    }
    if(pthread_mutex_unlock(&read_lock)){
        perror("Bank error: pthread_mutex_unlock failed");
        exit(0);
    }
    if(pthread_mutex_destroy(&read_lock)){
        perror("Bank error: pthread_mutex_destroy failed");
        exit(0);
    }
    if(pthread_mutex_destroy(&write_lock)){
        perror("Bank error: pthread_mutex_destroy failed");
        exit(0);
    }
}

Account::Account(const Account& other) : balance(other.balance), password(other.password), readers_counter(other.readers_counter) {
    if(pthread_mutex_init(&read_lock, NULL)){
        perror("Bank error: pthread_mutex_init failed");
        exit(0);
    }
    if(pthread_mutex_init(&write_lock, NULL)){
        perror("Bank error: pthread_mutex_init failed");
        exit(0);
    }
}

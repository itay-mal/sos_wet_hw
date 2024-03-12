#include "account.hpp"

void Account::lock_writer() {
    pthread_mutex_lock(&write_lock);
}

void Account::unlock_writer() {
    pthread_mutex_unlock(&write_lock);
}

void Account::lock_reader(){
    pthread_mutex_lock(&read_lock);
    readers_counter++;
    if (readers_counter==1) {
        pthread_mutex_lock(&write_lock); }
    pthread_mutex_unlock(&read_lock);
}

void Account::unlock_reader(){
    pthread_mutex_lock(&read_lock);
    readers_counter--;
    if (readers_counter==0) {
        pthread_mutex_unlock(&write_lock); }
    pthread_mutex_unlock(&read_lock);
}

Account::Account(int _initial_balance, int _password) : balance(_initial_balance), password(_password) {
    readers_counter = 0;
    pthread_mutex_init(&read_lock, NULL);
    pthread_mutex_init(&write_lock, NULL);
    }

Account::Account() : balance(0), password(0) {
    readers_counter = 0;
    pthread_mutex_init(&read_lock, NULL);
    pthread_mutex_init(&write_lock, NULL);
    }

Account::~Account() {
        pthread_mutex_destroy(&read_lock);
        pthread_mutex_destroy(&write_lock);
    }

Account::Account(const Account& other) : balance(other.balance), password(other.password), readers_counter(other.readers_counter) {
    pthread_mutex_init(&read_lock, NULL);
    pthread_mutex_init(&write_lock, NULL);
}

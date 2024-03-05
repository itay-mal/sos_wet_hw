#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <pthread.h>

class Account{
    public:
        int balance;
        int password;
        int readers_counter;
        pthread_mutex_t read_lock;
        pthread_mutex_t write_lock;
        Account(int _initial_balance, int password);
        Account();
        ~Account();
        Account(const Account& other);
        void lock_reader();
        void unlock_reader();
        void lock_writer();
        void unlock_writer();
};

#endif
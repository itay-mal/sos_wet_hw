/*	main.cpp
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include "bank.hpp"
#include "atm.hpp"
#include "account.hpp"
#include "log.hpp"

Bank bank;
Logfile logfile;
//**************************************************************************************
// function name: main
// Description: main function of bank. 
// get instruction files from args 
// and launch ATM for each instruction file 
//**************************************************************************************

void* atm_thread_runner(void* arg) {
    Atm* atm_instance = static_cast<Atm*>(arg);
    atm_instance->run();
    return nullptr;
}

void* bank_thread_runner(void* arg) {
    bank.run((bool*)arg);
    return nullptr;
}

int main(int argc, char *argv[]) {
	std::vector<Atm*> atms;
	std::vector<pthread_t> atm_threads;
	if(argc < 2){
		std::cerr << "Bank error: illegal arguments" << std::endl;
		return 0;
	}
	for(int i = 1; i<argc; i++){
		if(access(argv[i], F_OK) || access(argv[i], R_OK)){
			if(errno == ENOENT || errno == EACCES){ // no file or not readable
				std::cerr << "Bank error: illegal arguments" << std::endl;
				return 0;
			} else {
				perror("Bank error: access failed");
				exit(0);
			}
		}
	}

	for(int i = 1; i<argc; i++){
		Atm* atm_p = new Atm(argv[i], i);
		atms.push_back(atm_p);
		pthread_t atm_thread;
		if(pthread_create(&atm_thread, NULL, *atm_thread_runner, (void*)atm_p)){
			perror("Bank error: pthread_create failed");
			exit(0);
		}
		atm_threads.push_back(atm_thread);
	}
	pthread_t bank_thread;
	bool atm_threads_joined = false; 
	if(pthread_create(&bank_thread, NULL, *bank_thread_runner, &atm_threads_joined)){
		perror("Bank error: pthread_create failed");
		exit(0);
	}
	
	for (pthread_t thread : atm_threads) {
		if(pthread_join(thread, nullptr)){
			perror("Bank error: pthread_join failed");
			exit(0);
		}
	}
	atm_threads_joined = true;
	if(pthread_join(bank_thread, nullptr)){
		perror("Bank error: pthread_join failed");
		exit(0);
	}
	for(auto atm_p:atms){ delete atm_p; }

	return 0;
}

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
#include <chrono>


Bank bank;
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
	auto start = std::chrono::steady_clock::now();
	std::vector<Atm*> atms;
	std::vector<pthread_t> atm_threads;
	for(int i = 1; i<argc; i++){
		// std::cout << argv[i] << std::endl;
		Atm* atm_p = new Atm(argv[i], i);
		atms.push_back(atm_p);
		pthread_t atm_thread;
		pthread_create(&atm_thread, NULL, *atm_thread_runner, (void*)atm_p);
		atm_threads.push_back(atm_thread);
	}
	pthread_t bank_thread;
	bool atm_threads_joined = false; 
	pthread_create(&bank_thread, NULL, *bank_thread_runner, &atm_threads_joined);
	
	for (pthread_t thread : atm_threads) {
		pthread_join(thread, nullptr);
	}
	atm_threads_joined = true;
	pthread_join(bank_thread, nullptr);
	//wait();
	for(auto atm_p:atms){ delete atm_p; }

	auto end = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Time elapsed: " << duration.count() << " milliseconds" << std::endl;
	return 0;
}

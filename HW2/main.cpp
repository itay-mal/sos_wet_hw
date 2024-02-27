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

Bank bank;
//**************************************************************************************
// function name: main
// Description: main function of bank. 
// get instruction files from args 
// and launch ATM for each instruction file 
//**************************************************************************************

int main(int argc, char *argv[]) {
	std::vector<Atm*> atms;
	std::vector<pthread_t> threads;
	for(int i = 1; i<argc; i++){
		// std::cout << argv[i] << std::endl;
		Atm* atm_p = new Atm(argv[i], i);
		atms.push_back(atm_p);
		atm_p->run();
	}
	//wait();
	for(auto atm_p:atms){ delete atm_p; }
	atms.clear();
	return 0;
}

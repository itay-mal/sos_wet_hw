#ifndef _LOG_HPP
#define _LOG_HPP

#include <fstream>
#include <pthread.h>
#include <iostream>
#include <sstream>
#include <type_traits>

void print_to_log(std::string message);

void close_log();

#endif
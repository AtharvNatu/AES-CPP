#pragma once

#include <iostream>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <fstream>
#include <cstdlib>
#include <chrono>

using namespace std;

// Function Declarations
void initialize(void);
void print_log(const char* fmt, ...);
string get_current_time(void);
void uninitialize(void);


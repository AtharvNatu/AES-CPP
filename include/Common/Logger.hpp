#pragma once

#include <iostream>
#include <ctime>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <chrono>

using namespace std;

class Logger
{
    public:

    // Member Declarations
    FILE* log_file = nullptr;
    string log_message;

    // Method Declarations
    void initialize(void);
    void print_log(string);
    string get_current_time(void);
    void uninitialize(void);
};


#include "../../include/Common/Logger.hpp"

// Variable Declarations
FILE* log_file = nullptr;
string log_message;

// Code
void initialize(void)
{   
    // Code
    log_file = fopen("Log.txt", "a+");
    if (log_file == nullptr)
    {
        cerr << endl << "Failed To Open Log File ... Exiting !!!" << endl;
        exit(EXIT_FAILURE);
    }
    else
        print_log("Log File Opened");
}

void print_log(const char* fmt, ...)
{
    // Variable Declarations
    va_list arg_list;

    // Code
    if (log_file == nullptr)
        return;

    fprintf(log_file, get_current_time().c_str());
    fprintf(log_file, "\t");

    va_start(arg_list, fmt);
    vfprintf(log_file, fmt, arg_list);
    va_end(arg_list);

    fprintf(log_file, "\n");
}

string get_current_time(void)
{
    time_t current_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
    string str_time(30, '\0');
    strftime(&str_time[0], str_time.size(), "%d/%m/%Y | %H:%M:%S", localtime(&current_time));
    return str_time;
}

void uninitialize(void)
{
    if (log_file)
    {
        print_log("Log File Closed");
        fclose(log_file);
        log_file = nullptr;
    }
}

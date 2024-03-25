
#include <stdio.h>

#ifndef Second_Task_log
#define Second_Task_log

enum LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

void log(enum LogLevel type, char* msg) {
    switch (type) {
    case DEBUG:
        printf("[DEBUG]: %s\n", msg);
        break;
    case INFO:
        printf("[INFO]: %s\n", msg);
        break;
    case WARNING:
        printf("[WARNING]: %s\n", msg);
        break;
    case ERROR:
        printf("[ERROR]: %s\n", msg);
        break;
    default:
        printf("[NONE]: %s\n", msg);
        break;
    }
}
#endif
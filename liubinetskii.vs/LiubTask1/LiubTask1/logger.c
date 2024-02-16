#include <stdio.h>

enum Log_type {
    Debug,
    Info,
    Warning,
    Error
};

struct Log_message {
    enum Log_type type;
    char* msg;
};

void log(struct Log_message message) {
    switch (message.type) {
    case Debug:
        printf("[DEBUG]: %s\n", message.msg);
        break;
    case Info:
        printf("[INFO]: %s\n", message.msg);
        break;
    case Warning:
        printf("[WARNING]: %s\n", message.msg);
        break;
    case Error:
        printf("[ERROR]: %s\n", message.msg);
        break;
    default:
        printf("[NONE]: %s\n", message.msg);
        break;
    }
}

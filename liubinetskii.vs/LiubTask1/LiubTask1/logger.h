#ifndef LOGGER_H
#define LOGGER_H

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

void log(struct Log_message message);

#endif /* LOGGER_H */

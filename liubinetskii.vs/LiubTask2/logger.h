#ifndef LOGGER_H
#define LOGGER_H

enum Log_type {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

void log(enum Log_type type, char *msg);

#endif /* LOGGER_H */

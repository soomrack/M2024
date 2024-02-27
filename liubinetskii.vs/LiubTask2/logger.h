#ifndef LOGGER_H
#define LOGGER_H

enum LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

void log(enum LogLevel type, char *msg);

#endif /* LOGGER_H */

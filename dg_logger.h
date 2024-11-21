#ifndef DG_LOGGER_H
#define DG_LOGGER_H

#define DG_ERR 100
#define DG_WARN 200
#define DG_INFO 300

#define DG_LOG_OFF 0
#define DG_LOG_ALL 9999

#define DG_LOG_NAME_DEFAULT "/tmp/dourgaur.log"

int log_init(unsigned int level, char* fname);
void log_msg(unsigned int level, const char* message);
void log_msg_nano(unsigned int level, const char* message);
void log_close();

#endif
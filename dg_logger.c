#include <stdio.h>
#include <string.h>
#include <time.h>
#include "dg_logger.h"

char* log_filename;
FILE* logfile;
unsigned int verbosity;

int log_init(unsigned int v, char* fname){
  verbosity = v;
  
  if(fname == NULL || strlen(fname) < 1){
    fname = DG_LOG_NAME_DEFAULT;
  }
  log_filename = strdup(fname);
  
  logfile = fopen(log_filename, "a");
  if (logfile == NULL) {
    perror("Error opening log file");
    return 1;
  }
  fprintf(logfile, "\n====> dourgaur logfile opened <====\n");
  return 0;
}

void log_msg(unsigned int level, const char *message) {

    if(level < verbosity){

        time_t now = time(NULL);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

        fprintf(logfile, "[%s] %s\n", timestamp, message);      
    }
}

void log_msg_nano(unsigned int level, const char *message) {

    if(level < verbosity){

      struct timespec ts;

      if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
        perror("clock_gettime");
      }

      char timestamp[20];
      time_t epoch_secs = ts.tv_sec;
      strftime(timestamp, sizeof(timestamp), "%H:%M:%S", localtime(&epoch_secs));
      fprintf(logfile, "[%s.%ld] %s\n", timestamp, ts.tv_nsec, message);
    }
}

void log_close(){
  if(logfile != NULL) {
    fprintf(logfile, "====> dourgaur logfile closed <====\n\n");
    fclose(logfile);
  }
  logfile = NULL;
}

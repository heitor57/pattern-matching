#include "time.h"
#include <stdlib.h>
#define BILLION 1E9


struct algorithm_time{
  double utime,stime,total_time,real_time;// user time, system time and total time
};

void time_count(struct rusage *resources,char start,algorithm_time *algo_time){
  double utime, stime, total_time;
  struct timespec realtimet;
  int rc;
  if(start == START){
    if(clock_gettime(CLOCK_REALTIME, &realtimet) == -1) {
      perror("clock gettime");
    }
    algo_time->real_time = realtimet.tv_sec + realtimet.tv_nsec/ BILLION;
    
    if((rc = getrusage(RUSAGE_SELF,resources)) != 0) {
      perror("getrusage failed");
    }
  }else if(start == STOP){
    utime = (double) resources->ru_utime.tv_sec + 1.e-6 * (double) resources->ru_utime.tv_usec;
    stime = (double) resources->ru_stime.tv_sec + 1.e-6 * (double) resources->ru_stime.tv_usec;

    if((rc = getrusage(RUSAGE_SELF,resources)) != 0) {
      perror("getrusage failed");
    }
    utime = (double) resources->ru_utime.tv_sec + 1.e-6 * (double) resources->ru_utime.tv_usec - utime;
    stime = (double) resources->ru_stime.tv_sec + 1.e-6 * (double) resources->ru_stime.tv_usec - stime;
    total_time = utime+stime;
    algo_time->utime = utime;
    algo_time->stime = stime;
    algo_time->total_time = total_time;

    if(clock_gettime(CLOCK_REALTIME, &realtimet) == -1) {
      perror("clock gettime");
    }
    algo_time->real_time = realtimet.tv_sec + realtimet.tv_nsec/ BILLION-algo_time->real_time;
  }
}

algorithm_time* new_algorithm_time(void){
  algorithm_time* algo_time = malloc(sizeof(algorithm_time));
  if(algo_time == NULL)
    return 0;
  return algo_time;
}
void free_algorithm_time(algorithm_time *algo_time){
  if(algo_time == NULL)
    return;
  free(algo_time);
}
double get_utime(const algorithm_time *algo_time){
  return algo_time->utime;
}
double get_stime(const algorithm_time *algo_time){
  return algo_time->stime;
}
double get_total_time(const algorithm_time *algo_time){
  return algo_time->total_time;
}
double get_real_time(const algorithm_time *algo_time){
  return algo_time->real_time;
}


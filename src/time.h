#ifndef TIME_H
#define TIME_H
#include <sys/resource.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
enum {START, STOP};
typedef struct algorithm_time algorithm_time;
void time_count(struct rusage *resources,char start,algorithm_time* ai);
void free_algorithm_time(algorithm_time *algo_time);
algorithm_time* new_algorithm_time(void);
double get_utime(const algorithm_time *algo_time);
double get_stime(const algorithm_time *algo_time);
double get_total_time(const algorithm_time *algo_time);
double get_real_time(const algorithm_time *algo_time);
#endif

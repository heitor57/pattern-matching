#ifndef IO_H
#define IO_H
#define ENOENT 2 /* No such file or directory */
#define SUCCESS 1
#define FAIL 0
#include <stdio.h>
#include <stdbool.h>
#include "time.h"
#include "stringmatching.h"
#define error(str, ...) {			\
    fprintf(stderr,(str), ##__VA_ARGS__);	\
    fprintf(stderr,"\n");			\
  }
typedef enum DebugType{Disable=0,Enable,Verbose}DebugType;
extern DebugType DEBUG;
extern char IMPLEMENTATION;
extern size_t ERROR;
extern size_t NUM_THREADS;
extern bool NUM_MATCHES;
void entryRead(int argc, char* argv[],FILE** fin,FILE** fout,CharType ***P,size_t *numP);
size_t ftextSize(FILE* f);
CharType* getTextFromFile(FILE *fin,size_t n);
void printAlgorithmTime(const algorithm_time *algo_time,FILE* f);
void printMatch(bool *match,size_t n,FILE *fout);
void freePattern(CharType **P,int n);
size_t fileLines(FILE* f);
void countCharsInLines(FILE *f,size_t *charsInLines);
#endif

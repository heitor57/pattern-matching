#ifndef STRINGMATCHING_H
#define STRINGMATCHING_H
#include <stddef.h>
#include <stdbool.h>
#define ALPHABETSIZE 256
typedef unsigned char CharType;
void freeText(CharType *t);
void BMH(CharType *T,size_t n,CharType *P,size_t m,bool* match,size_t *comparations);
void BMHS(CharType *T,size_t n,CharType *P,size_t m,bool* match,size_t *comparations);
void ShiftAnd(CharType *T,size_t n,CharType *P,size_t m,bool* match,size_t error,size_t *comparations);
void PBMHS(CharType *T,size_t n,CharType *P,size_t m,bool* match,size_t numThreads,size_t *comparations);
void *PBMHSF(void *arg);
void ShiftAndWordLimit(CharType *T,size_t n,CharType *P,size_t m,bool* match,size_t error,size_t *comparations);
#endif

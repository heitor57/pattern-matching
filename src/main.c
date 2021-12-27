#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include "io.h"
#include "bitvector.h"
#include "time.h"
#include "stringmatching.h"

int main(int argc,char *argv[]){
  // structure to account for the time
  struct rusage *resources= malloc(sizeof(struct rusage));
  // auxiliar struct to get algorithm information
  algorithm_time* algo_time= new_algorithm_time();
  FILE* fin = NULL,*fout=stdout;
  CharType *T=NULL,**P=NULL;
  size_t n,m,numP,*comparations=malloc(sizeof(size_t));
  entryRead(argc,argv,&fin,&fout,&P,&numP);// Open input file and set IMPLEMENTATION, ERROR, Patterns, etc...
  n = ftextSize(fin);
  T = getTextFromFile(fin,n);
  bool *match = malloc(n*sizeof(bool));
  int aux;// getchar() auxiliar
  /* get implementation if it doesn't has */
  if(DEBUG==Verbose)
    fprintf(fout,"n:%ld\n",n);
  while(IMPLEMENTATION!='h'&&IMPLEMENTATION!='s'&&IMPLEMENTATION!='n'&&IMPLEMENTATION!='p'&&IMPLEMENTATION!='m'&&IMPLEMENTATION!='a'){
    printf("Choose one implementation\n"
	   "[h] BMH\n"
	   "[s] Serial BMHS\n"
	   "[n] Exact Shift-And\n"
	   "[p] Parallel BMHS, -n <threads>\n"
	   "[m] Approximated Shift-And, -e <error>\n"
	   "[a] All\n");
    IMPLEMENTATION = getchar();
    while ((aux = getchar()) != '\n' && aux != EOF);
  }

  /* passes all patterns running */
  for(size_t i=0;i<numP;i++){
    m = strlen((char*)P[i]);
    fprintf(fout,"Pattern:%s\n",P[i]);

    if(DEBUG==Verbose)
      fprintf(fout,"m:%ld\n",m);
    for(size_t i=0;i<n;i++) match[i]=false;
    
    switch(IMPLEMENTATION){
    case 'h':
      if(DEBUG) time_count(resources,START,algo_time);
      BMH(T,n,P[i],m,match,comparations);
      if(DEBUG){
	time_count(resources,STOP,algo_time);
	printAlgorithmTime(algo_time,fout);
	fprintf(fout,"Comparations: %ld\n",*comparations);
      }
      printMatch(match,n,fout);
      break;
    case 's':
      if(DEBUG) time_count(resources,START,algo_time);
      BMHS(T,n,P[i],m,match,comparations);
      if(DEBUG){
	time_count(resources,STOP,algo_time);
	printAlgorithmTime(algo_time,fout);
	fprintf(fout,"Comparations: %ld\n",*comparations);
      }
      printMatch(match,n,fout);
      break;
    case 'n':
      if(DEBUG)	time_count(resources,START,algo_time);
      // check if can use shiftand with limit in word | fastest
      if(m<=sizeof(WordType)*8){
	ShiftAndWordLimit(T,n,P[i],m,match,0,comparations);// fast
      }else{
	ShiftAnd(T,n,P[i],m,match,0,comparations);// slow because of bitvector lib
      }
      if(DEBUG){
	time_count(resources,STOP,algo_time);
	printAlgorithmTime(algo_time,fout);
	fprintf(fout,"Comparations: %ld\n",*comparations);
      }
      printMatch(match,n,fout);
      break;
    case 'm':
      if(DEBUG)	time_count(resources,START,algo_time);
      // check if can use shiftand with limit in word | fastest
      if(m<=sizeof(WordType)*8){
	ShiftAndWordLimit(T,n,P[i],m,match,ERROR,comparations);// fast
      }else{
	ShiftAnd(T,n,P[i],m,match,ERROR,comparations);// slow because of bitvector lib
      }
      if(DEBUG){
	time_count(resources,STOP,algo_time);
	printAlgorithmTime(algo_time,fout);
	fprintf(fout,"Comparations: %ld\n",*comparations);
      }
      printMatch(match,n,fout);
      break;
    case 'p':
      if(DEBUG) time_count(resources,START,algo_time);
      if(NUM_THREADS>0)
	PBMHS(T,n,P[i],m,match,NUM_THREADS,comparations);
      else
	PBMHS(T,n,P[i],m,match,get_nprocs(),comparations);
      if(DEBUG){
	time_count(resources,STOP,algo_time);
	printAlgorithmTime(algo_time,fout);
	fprintf(fout,"Comparations: %ld\n",*comparations);
      }
      printMatch(match,n,fout);
      break;

    case 'a':
      fprintf(fout,"--==BMH==--\n");
      if(DEBUG) time_count(resources,START,algo_time);
      BMH(T,n,P[i],m,match,comparations);
      if(DEBUG){
	time_count(resources,STOP,algo_time);
	printAlgorithmTime(algo_time,fout);
	fprintf(fout,"Comparations: %ld\n",*comparations);
      }
      printMatch(match,n,fout);

      for(size_t i=0;i<n;i++) match[i]=false;

      fprintf(fout,"--==Serial BMHS==--\n");
      if(DEBUG) time_count(resources,START,algo_time);
      BMHS(T,n,P[i],m,match,comparations);
      if(DEBUG){
	time_count(resources,STOP,algo_time);
	printAlgorithmTime(algo_time,fout);
	fprintf(fout,"Comparations: %ld\n",*comparations);
      }
      printMatch(match,n,fout);
      for(size_t i=0;i<n;i++) match[i]=false;

      fprintf(fout,"--==Exact Shift-And==--\n");
      if(DEBUG) time_count(resources,START,algo_time);
      // check if can use shiftand with limit in word | fastest
      if(m<=sizeof(WordType)*8){
	ShiftAndWordLimit(T,n,P[i],m,match,0,comparations);// fast
      }else{
	ShiftAnd(T,n,P[i],m,match,0,comparations);// slow because of bitvector lib
      }
      if(DEBUG){
	time_count(resources,STOP,algo_time);
	printAlgorithmTime(algo_time,fout);
	fprintf(fout,"Comparations: %ld\n",*comparations);
      }
      printMatch(match,n,fout);

      for(size_t i=0;i<n;i++) match[i]=false;

      fprintf(fout,"--==Approximated Shift-And==--\n");
      if(DEBUG) time_count(resources,START,algo_time);
      // check if can use shiftand with limit in word | fastest
      if(m<=sizeof(WordType)*8){
	ShiftAndWordLimit(T,n,P[i],m,match,ERROR,comparations);// fast
      }else{
	ShiftAnd(T,n,P[i],m,match,ERROR,comparations);// slow because of bitvector lib
      }
      if(DEBUG){
	time_count(resources,STOP,algo_time);
	printAlgorithmTime(algo_time,fout);
	fprintf(fout,"Comparations: %ld\n",*comparations);
      }
      printMatch(match,n,fout);

      for(size_t i=0;i<n;i++) match[i]=false;

      fprintf(fout,"--==Parallel BMHS==--\n");
      if(DEBUG) time_count(resources,START,algo_time);
      if(NUM_THREADS>0)
	PBMHS(T,n,P[i],m,match,NUM_THREADS,comparations);
      else
	PBMHS(T,n,P[i],m,match,get_nprocs(),comparations);
      if(DEBUG){
	time_count(resources,STOP,algo_time);
	printAlgorithmTime(algo_time,fout);
	fprintf(fout,"Comparations: %ld\n",*comparations);
      }
      printMatch(match,n,fout);

      break;
    }
  }
  //free
  free(comparations);
  free(match);
  freeText(T);
  freePattern(P,numP);
  free_algorithm_time(algo_time);
  free(resources);
  fclose(fin);
  fclose(fout);
}

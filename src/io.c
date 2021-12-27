#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include "io.h"
enum DebugType DEBUG = Disable;
size_t ERROR = 0,NUM_THREADS = 0;
bool NUM_MATCHES=false;
char IMPLEMENTATION = ' ';
void printUsage(){
  printf("tp4 -i <entry file> -o <out file> -d(d) (DEBUG) -p <Implementations> -t <Pattern> -e <error [0..Integer]> -f <Pattern file> -n <number of threads> -s(match format to number of matches)\n"
    "Possible implementations\n"
    "-p h(BMH) s(Serial BMHS) n(Exact Shift-And)\n"
    "-p m(Approximated Shift-And) p(Parallel BMHS) a(All)\n"
    "-n <number of threads> if (n<=0 or n>numProcessors) then n gets numProcessors\n"
    "Examples: tp4 -i in.txt -o out.txt -d -p h -t br\n"
    "tp4 -i in.txt -dd -o out.txt -t \"hello world\"\n"
    "tp4 -i in.txt -d -p a -t hello\n"
    "tp4 -i in.txt -d -p n -f patterns.txt\n");
  exit(ENOENT);
}

void entryRead(int argc, char* argv[],FILE** fin,FILE** fout,CharType ***P,size_t *numP){
  int option;
  size_t temp,numProcessors=get_nprocs();
  bool hasFin=false,hasPattern=false,hasPatternFile=false;
  FILE *fpattern;
  if(argc < 5)
  {
    printUsage();
  }
  while((option = getopt(argc,argv,"i:o:d::p:t:e:f:n:s")) != -1){
    switch(option){
    case 'i':
      if(!(*fin = fopen(optarg,"r"))){
	error("Input file open error.");
	exit(ENOENT);
      }
      hasFin=true;
      break;
    case 'o':
      if(!(*fout = fopen(optarg,"w"))){
	error("Output file open error.");
	exit(ENOENT);
      }
      break;
    case 'd':
      if(optarg!=NULL && optarg[0] == 'd')
	DEBUG=Verbose;
      else
	DEBUG=Enable;
      break;
    case 'p':
      IMPLEMENTATION= optarg[0];
      break;
    case 't':
      if(!hasPatternFile){
	temp = strlen(optarg)+1;
	if(temp==1){
	  error("Empty pattern.");
	  exit(1);
	}
	*numP=1;
	(*P) = malloc(sizeof(CharType*));
	(*P)[0] = malloc(sizeof(CharType)*temp);
	memcpy((*P)[0],optarg,sizeof(CharType)*temp);
	hasPattern=true;
      }
      break;
    case 'e':
      ERROR=atoi(optarg);
      break;
    case 'f':
      if(!hasPattern){
	if(!(fpattern = fopen(optarg,"r"))){
	  error("Pattern file open error.");
	  exit(ENOENT);
	}
	size_t lines = fileLines(fpattern);
	*numP=lines;
	*P = malloc(sizeof(CharType*)*lines);
	size_t *charsInLines= calloc(lines,sizeof(size_t));
	countCharsInLines(fpattern,charsInLines);
	for(size_t i=0;i<lines;i++){
	  (*P)[i]=malloc(sizeof(CharType)*(charsInLines[i]+1));
	  fscanf(fpattern,"%[^\n]s",(*P)[i]);
	  fscanf(fpattern,"\n");
	}
	fclose(fpattern);
	free(charsInLines);
	hasPatternFile=true;
      }
      break;
    case 'n':
    
      if(optarg == NULL){
	NUM_THREADS=numProcessors;
	break;
      }
      NUM_THREADS=atoi(optarg);
      if(NUM_THREADS<=0 || NUM_THREADS>numProcessors)
	NUM_THREADS=numProcessors;
      break;
    case 's':
      NUM_MATCHES=true;
      break;
    }
  }

  if(!hasFin || (!hasPattern && !hasPatternFile)){
    error("Do not has input file or pattern.");
    exit(ENOENT);
  }
}

size_t ftextSize(FILE* f){
  size_t size;
  fseek(f, 0L, SEEK_END);
  size = ftell(f);
  rewind(f);
  return size;
}

CharType* getTextFromFile(FILE *fin,size_t n){
  CharType *T = malloc(n*sizeof(CharType));
  size_t i=0;
  rewind(fin);
  while(i<n){
    T[i++]=fgetc(fin);
  }
  rewind(fin);
  return T;
}

void printAlgorithmTime(const algorithm_time *algo_time,FILE* f){
  fprintf(f,"User time %.6fs, System time %.6fs, Total Time %.6fs, Real Time %.6fs\n",get_utime(algo_time), get_stime(algo_time), get_total_time(algo_time),get_real_time(algo_time));
}

void printMatch(bool *match,size_t n,FILE *fout){
  fprintf(fout,"Matches: ");
  size_t count=0;
  if(NUM_MATCHES==false){
    for(size_t i=0;i<n;i++)
      if(match[i])
	fprintf(fout,"%ld ",i+1);
  }else{
    for(size_t i=0;i<n;i++)
      if(match[i]) count++;
    fprintf(fout,"%ld",count);
  }
  fprintf(fout,"\n");
}

size_t fileLines(FILE* f){
  size_t i=0;
  char c=fgetc(f);
  while(!feof(f)){
    if (c == '\n') // Increment count if this character is newline 
      i++;
    c=fgetc(f);
  }
  rewind(f);
  return i;
}

void countCharsInLines(FILE *f,size_t *charsInLines){
  size_t line=0;
  char c=fgetc(f);

  while(!feof(f)){
    if (c == '\n') // Increment count if this character is newline 
      line++;
    else
      charsInLines[line]++;
    c=fgetc(f);
  }
  rewind(f);
}

void freePattern(CharType **P,int n){
  for(size_t i=0;i<n;i++){
    free(P[i]);
  }
  free(P);
}

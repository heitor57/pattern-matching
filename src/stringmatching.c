#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "bitvector.h"
#include "stringmatching.h"
#include "io.h"
#include "math.h"

typedef struct {
  CharType *T;
  size_t n;
  CharType *P;
  size_t m;
  bool* match;
  size_t *d;
  size_t begin;
  size_t end;
  size_t comparations;
} thread_arg, *ptr_thread_arg;

int min(int a, int b) {
  return a<b?a:b;
}


void freeText(CharType *t){
  free(t);
}

void preProcessBMH(size_t *d,CharType *P,size_t m){
  size_t i;
  for(i=0;i<ALPHABETSIZE;i++)
    d[i]=m;
  for(i=0;i<m-1;i++)
    d[P[i]]=m-i-1;
}

void BMH(CharType *T,size_t n,CharType *P,size_t m,bool* match,size_t *comparations){
  *comparations=0;
  /* d -- displacement table */
  size_t d[ALPHABETSIZE],i,j,base;
  preProcessBMH(d,P,m);

  if(DEBUG==Verbose){
    bool alphabet[ALPHABETSIZE];
    for(i=0;i<ALPHABETSIZE;i++) alphabet[i]=false;
    for(i=0;i<m;i++) alphabet[P[i]]=true;
    for(i=0;i<ALPHABETSIZE;i++)
      if(alphabet[i])printf("d[%c]=%ld\n",(unsigned char)i,d[i]);
  }
      
  for(i=m;i<=n;i+=d[T[i-1]]){
    base=i;
    // check pattern
    for(j=m;j>0 && T[base-1] == P[j-1];j--,base--,(*comparations)++);
    if(j!=0)
      (*comparations)++;

    // if gets a match put in match vector
    if(j==0)
      match[base]=true;
  }
}

void preProcessBMHS(size_t *d,CharType *P,size_t m){
  size_t i;
  for(i=0;i<ALPHABETSIZE;i++)
    d[i]=m+1;
  for(i=0;i<m;i++)
    d[P[i]]=m-i;
}

void BMHS(CharType *T,size_t n,CharType *P,size_t m,bool* match,size_t *comparations){
  *comparations=0;
  /* d -- displacement table */
  size_t d[ALPHABETSIZE],i,j,base;
  preProcessBMHS(d,P,m);

  if(DEBUG==Verbose){
    bool alphabet[ALPHABETSIZE];
    for(i=0;i<ALPHABETSIZE;i++) alphabet[i]=false;
    for(i=0;i<m;i++) alphabet[P[i]]=true;
    for(i=0;i<ALPHABETSIZE;i++)
      if(alphabet[i])printf("d[%c]=%ld\n",(unsigned char)i,d[i]);
  }

  for(i=m;i<=n;){
    base=i;
    // check pattern
    for(j=m;j>0 && T[base-1] == P[j-1];j--,base--,(*comparations)++);
    if(j!=0)
      (*comparations)++;
    // if gets a match put in match vector
    if(j==0)
      match[base]=true;
    if(i<n)
      i+=d[T[i]];
    else
      break;
  }

}

void ShiftAnd(CharType *T,size_t n,CharType *P,size_t m,bool* match,size_t error,size_t *comparations){
  size_t i,j;
  *comparations=n*(error+1);
  // init variables/ bit vectors
  bitVector *mask[ALPHABETSIZE],*Rprevious,*Rnew,*R1,**R=malloc(sizeof(bitVector*)*(error+1));
  /* R1 -- constant 1000... */
  // create bit vectors, bVNew initialize with 0's
  Rprevious=bVNew(m);
  Rnew=bVNew(m);
  R1=bVNew(m);
  bVSetBit(R1,0); /* set first bit */
  for(i=0;i<ALPHABETSIZE;i++)
    mask[i]=bVNew(m);
  for(i=0;i<error+1;i++)
    R[i]=bVNew(m);
  bitVector *aux1=bVNew(m),*aux2=bVNew(m);
  /* initialize mask */
  for(i=0;i<m;i++)
    bVSetBit(mask[P[i]],i);
  // print mask
  if(DEBUG==Verbose){
    bool alphabet[ALPHABETSIZE];
    for(i=0;i<ALPHABETSIZE;i++) alphabet[i]=false;
    for(i=0;i<m;i++) alphabet[P[i]]=true;
    for(i=0;i<ALPHABETSIZE;i++)
      if(alphabet[i]){
	printf("%c ",(unsigned char)i);
	bVPrint(mask[i]);
      }
  }
  /* initialize R's*/
  bVClearAll(R[0]);//  pleonasm
  for(i=1;i<error+1;i++){
    bVCopy(R[i],R[i-1]);
    bVSetBit(R[i],i-1);
  }
  /* process of finding */
  /* the comments are the version with limitation of the width of the word */
  for(i=0;i<n;i++){
    bVCopy(Rprevious,R[0]); /* Rprevious=R[0]; */
    bVAnd(Rnew,
	  bVSetBit(bVShiftRight(aux1,Rprevious,1),0),
	  mask[T[i]]); /* Rnew=((Rprevious>>1)|R1) & mask[T[i]]; */
    bVCopy(R[0],Rnew); /* R[0]=Rnew; */

    for(j=1;j<error+1;j++){
      bVOr(Rnew,
	   bVOr(aux1,
		bVAnd(aux1,
		      bVShiftRight(aux1,R[j],1),
		      mask[T[i]]),
		Rprevious),
	   bVShiftRight(aux2,bVOr(aux2,Rprevious,Rnew),1));
      /* Rnew=((R[j]>>1) & mask[T[i]]) | Rprevious | Rprevious>>1 | Rnew>>1; */
      bVCopy(Rprevious,R[j]); /* Rprevious=R[j]; */
      bVOr(R[j],Rnew,R1);/* R[j]=Rnew|R1; */
    }

    if(bVIsSetBit(Rnew,m-1) /* (Rnew & 10^m)!=0 */ && n>i+1-m && i+1-m>=0)
      match[i+1-m]=true;
  }

  bVFree(aux1);
  bVFree(aux2);
  bVFree(Rprevious);
  bVFree(Rnew);
  bVFree(R1);
  for(i=0;i<ALPHABETSIZE;i++)
    bVFree(mask[i]);
  for(i=0;i<error+1;i++)
    bVFree(R[i]);
  free(R);

}

void PBMHS(CharType *T,size_t n,CharType *P,size_t m,bool* match,size_t numThreads,size_t *comparations){
  *comparations=0;
  /* d -- displacement table */
  size_t d[ALPHABETSIZE],i;
  preProcessBMHS(d,P,m);
  if(DEBUG==Verbose){
    bool alphabet[ALPHABETSIZE];
    for(i=0;i<ALPHABETSIZE;i++) alphabet[i]=false;
    for(i=0;i<m;i++) alphabet[P[i]]=true;
    for(i=0;i<ALPHABETSIZE;i++)
      if(alphabet[i])printf("d[%c]=%ld\n",(unsigned char)i,d[i]);
  }
  if(numThreads>ceil(n/m))
    numThreads = ceil(n/m);
  if(DEBUG == Verbose){
    printf("Using %ld threads\n",numThreads);
    printf("Size of thread space: %ld characters\n",n/numThreads+m-1);
  }
  pthread_t *THREADS= malloc(sizeof(pthread_t)*numThreads);
  thread_arg *argument = malloc(sizeof(thread_arg)*numThreads);
  for(i=0;i<numThreads;i++){
    argument[i].T=T;
    argument[i].n=n;
    argument[i].P=P;
    argument[i].m=m;
    argument[i].match=match;
    argument[i].d=d;
    argument[i].begin=floor(i*n/numThreads);
    if(i==numThreads-1)
      argument[i].end = floor((i+1)*n/numThreads);
    else
      argument[i].end=floor((i+1)*n/numThreads)-1+m;
    argument[i].comparations=0;
    pthread_create(&(THREADS[i]), NULL, PBMHSF, &(argument[i]));
  }
  
  for(i = 0; i < numThreads; i++){
    pthread_join(THREADS[i],NULL);
    *comparations+=argument[i].comparations;
  }
  free(THREADS);
  free(argument);
}


void *PBMHSF(void *arg){
  ptr_thread_arg argument = (ptr_thread_arg) arg;
  size_t i,j,base,
    n=argument->n,m=argument->m,*d=argument->d;
  CharType *T=argument->T,*P=argument->P;
  bool* match=argument->match;
  /* if(DEBUG == Verbose) */
  /*   printf("In function PBMHSF \nthread id = %ld; begin = %ld; end = %ld; begin+m = %ld\n", pthread_self(),argument->begin,argument->end,argument->begin+m);  */

  for(i=argument->begin+m;i<=argument->end;){
    base=i;
    // check pattern
    for(j=m;j>0 && T[base-1] == P[j-1];j--,base--,argument->comparations++);
    if(j!=0)
      argument->comparations++;

    // if gets a match put in match vector
    if(j==0){
      /* if(DEBUG == Verbose) */
      /* 	printf("match: %ld, thread id = %ld\n",base+1,pthread_self()); */
      match[base]=true;
    }if(i<n)
       i+=d[T[i]];
    else
      break;
  }
  return NULL;
}


void ShiftAndWordLimit(CharType *T,size_t n,CharType *P,size_t m,bool* match,size_t error,size_t *comparations){
  size_t i,j;
  *comparations=n*(error+1);
  WordType mask[ALPHABETSIZE],Rp,Rn,R1=1<<(m-1),
    *R= malloc(sizeof(WordType)*(error+1));
  /* Rp -- R old/prev, Rn -- R newest, R1 -- constant 1000... */
  
  /* initialize mask */
  for(i=0;i<ALPHABETSIZE;i++)
    mask[i]=0;
  for(i=0;i<m;i++)
    mask[P[i]] = mask[P[i]] | (1 << (m-i-1));

  /* initialize R's*/
  R[0]=0;
  for(i=1;i<error+1;i++)
    R[i]= (1<<(m-i)) | R[i-1];
  /* process of finding */
  for(i=0;i<n;i++){
    Rp=R[0];
    Rn=((Rp>>1)|R1) & mask[T[i]];
    R[0]=Rn;
    for(j=1;j<error+1;j++){
      Rn=((R[j]>>1) & mask[T[i]]) | Rp | ((Rp | Rn)>>1);
      Rp=R[j];
      R[j]=Rn|R1;
    }
    if((Rn & 1)!=0 && n>i+1-m && i+1-m>=0)
      match[i+1-m]=true;
  }
  free(R);
}

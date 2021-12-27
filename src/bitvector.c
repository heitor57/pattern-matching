#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "bitvector.h"
#define BYTESIZE 8 		/* byte size in bits */
struct bitVector{
  size_t bits;
  WordType *vector;
};

// in what word is the bit "bit"? this macro responds the question
/* in: bit = 0..MAXCAPACITYBITS-1 */
/* out: 0..MAXWORDINDEX */

#define WORD_SIZE_BITS (sizeof(WordType)*BYTESIZE)
#define BIT_WORD(bit) (bit)/(WORD_SIZE_BITS)
#define POS_IN_WORD(bit) (WORD_SIZE_BITS -1 -(bit % WORD_SIZE_BITS))
/* words needed for a number of bits */
#define WORDS_NEEDED(bits) ((bits) -1)/(WORD_SIZE_BITS) + 1
/* Word size in bits */
/* Most significant bit */
#define MSB ((WordType)1<<(WORD_SIZE_BITS-1))
#define LSB ((WordType)1)


void bVInit(bitVector* bitv,size_t bits){
  size_t wordsNeeded = WORDS_NEEDED(bits);
  bitv->bits = wordsNeeded*BYTESIZE*sizeof(WordType);
  bitv->vector = calloc(wordsNeeded,sizeof(WordType));
}

bitVector* bVNew(size_t bits){
  bitVector* bitv= malloc(sizeof(bitVector));
  bVInit(bitv,bits);
  return bitv;
}
void bVFreeVar(bitVector* bitv){
  free(bitv->vector);
}
void bVFree(bitVector* bitv){
  bVFreeVar(bitv);
  free(bitv);
}

void bVCopy(bitVector* dest,bitVector* src){
  if(dest->bits>=src->bits)
    memcpy(dest->vector, src->vector,src->bits/BYTESIZE);
}

bitVector* bVSetBit(bitVector* bitv,size_t bit){
  bitv->vector[BIT_WORD(bit)] |= LSB<<POS_IN_WORD(bit);
  return bitv;
}

bitVector* bVClearBit(bitVector* bitv,size_t bit){
  WordType mask= ~(LSB<<POS_IN_WORD(bit));
  bitv->vector[BIT_WORD(bit)] &= mask;
  return bitv;
}

bitVector* bVOr(bitVector* dest,bitVector* src1,bitVector* src2){
  for(size_t i=0;i<WORDS_NEEDED(dest->bits);i++){
    dest->vector[i] = src1->vector[i] | src2->vector[i];
  }
  return dest;
}

bitVector* bVAnd(bitVector* dest,bitVector* src1,bitVector* src2){
  for(size_t i=0;i<WORDS_NEEDED(dest->bits);i++){
    dest->vector[i] = src1->vector[i] & src2->vector[i];
  }
  return dest;
}

bitVector* bVClearAll(bitVector* target){
  memset(target->vector,0,target->bits/BYTESIZE);
  return target;
}
//O((WORD LENGTH IN BITS)*(WORDS USED))
bitVector* bVShiftLeft(bitVector* dest,bitVector* src,size_t shifts){
  bVCopy(dest,src);
  size_t charShift = shifts/WORD_SIZE_BITS;
  shifts=shifts%WORD_SIZE_BITS;
  if(shifts>=dest->bits){
    bVClearAll(dest);
    return dest;
  }
  if(charShift>0){
    /* block shift */
    for(size_t i=0;(i+charShift)<BIT_WORD(dest->bits);i++)
      dest->vector[i] = dest->vector[i + charShift];
    /* zero bytes on the right side */
    for(size_t bitword=BIT_WORD(dest->bits);charShift>0;charShift--)
      dest->vector[bitword - charShift] = 0;
  }
  /* shifts < WORD_SIZE_BITS after handle big jumps */
  for(size_t i=0;i<shifts;i++){
    for(size_t j=0;j<WORDS_NEEDED(dest->bits)-1;j++){
      dest->vector[j]<<=1;
      if(dest->vector[j+1] & MSB)
	dest->vector[j] |= LSB;
    }
    dest->vector[WORDS_NEEDED(dest->bits)-1] <<=1;
  }

  return dest;
}

//O((WORD LENGTH IN BITS)*(WORDS USED))
bitVector* bVShiftRight(bitVector* dest,bitVector* src,size_t shifts){
  bVCopy(dest,src);
  size_t charShift = shifts/WORD_SIZE_BITS;
  shifts=shifts%WORD_SIZE_BITS;
  if(shifts>=dest->bits){
    bVClearAll(dest);
    return dest;
  }
  if(charShift>0){
    /* block shift */
    for(size_t i=WORDS_NEEDED(dest->bits)-1;i>=charShift;i--)
      dest->vector[i] = dest->vector[i - charShift];
    /* zero bytes on the right side */
    do{
      charShift--;
      dest->vector[charShift] = 0;
    }while(charShift>0);
  }
  /* shifts < WORD_SIZE_BITS after handle big shifts */
  for(size_t i=0;i<shifts;i++){
    for(size_t j=WORDS_NEEDED(dest->bits)-1;j>0;j--){
      dest->vector[j]>>=1;
      if(dest->vector[j-1] & LSB)
	dest->vector[j] |= MSB;
    }
    dest->vector[0] >>=1;
  }

  return dest;
}


void bVPrint(bitVector* target){
  for(size_t i=0;i<BIT_WORD(target->bits);i++)
    for(size_t j=0;j<WORD_SIZE_BITS;j++)
      printf("%d", !!((target->vector[i] << j) & MSB));
  printf("\n");
}

int bVIsSetBit(bitVector* target,size_t bit){
  return ((target->vector[BIT_WORD(bit)] & (LSB<<POS_IN_WORD(bit))) != 0);
}

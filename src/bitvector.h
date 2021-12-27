#ifndef BITVECTOR_H
#define BITVECTOR_H
typedef struct bitVector bitVector;
typedef unsigned long long WordType;

void bVInit(bitVector* bitv,size_t bits);
bitVector* bVNew(size_t bits);
void bVFreeVar(bitVector* bitv);
void bVFree(bitVector* bitv);
void bVCopy(bitVector* dest,bitVector* src);
bitVector* bVSetBit(bitVector* bitv,size_t bit);
bitVector* bVClearBit(bitVector* bitv,size_t bit);
bitVector* bVOr(bitVector* dest,bitVector* src1,bitVector* src2);
bitVector* bVAnd(bitVector* dest,bitVector* src1,bitVector* src2);
bitVector* bVClearAll(bitVector* target);
bitVector* bVShiftLeft(bitVector* dest,bitVector* src,size_t shifts);
bitVector* bVShiftRight(bitVector* dest,bitVector* src,size_t shifts);
int bVIsSetBit(bitVector* target,size_t bit);
void bVPrint(bitVector* target);
#endif

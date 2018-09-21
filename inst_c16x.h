/*
   Permission is hereby granted, free of charge, to any person obtaining
   a copy of this software and associated documentation files (the
   "Software"), to deal in the Software without restriction, including
   without limitation the rights to use, copy, modify, merge, publish,
   distribute, sublicense, and/or sell copies of the Software, and to
   permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
   AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
   OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.
*/
#ifndef _INST_C16X_SUPPORT_H
#define _INST_C16X_SUPPORT_H
#include "utils.h"
#include "needles.h"

/*
 * Addressing Modes
 */
#define _cRw			0 // _n R0, R1, R2, ... R15
#define _cRb			1 // _n RL0, RH0, RL1, ... RH7
#define _cRb_plus		2 // _n RL0, RH0, RL1, ... RH7 ]+
#define _cregb			3 // cregb_RR SFR/GPR (byte)
#define _cregw			4 // cregw_RR SFR/GPR (word)
#define _cmem			5 // MMMM 1234H
#define _cbitaddr		6 // _QQ_q	-
#define _cbitoff		7 // _QQ
#define _cirang2		8 // (:..##)
#define _cdata3			9 // (:.###)
#define _cdata4			10 // (#)
#define _cdata8			11 // (##)
#define _cdata16		12 // (####)
#define _cmask8			13 // (@@)
#define _ccaddr			14 // (MMMM)
#define _cseg			15 // (SS)
#define _crel			16 // crel(rr)
#define _ctrap7			17 // ctrap7(
#define _ccc			18 // ccc(cc)		CC_UC, CC_NZ, ...

/*
 * Conditional's
 */							   
#define	CC_UC			 0	// 0000
#define CC_NET			 1	// 0001
#define CC_Z			 2	// 0010
#define CC_NZ			 3	// 0011
#define CC_V			 4	// 0100
#define CC_NV			 5	// 0101
#define CC_N			 6	// 0110
#define CC_NN			 7	// 0111
#define CC_C			 8	// 1000
#define CC_NC			 9	// 1001
#define CC_SGT 			10	// 1010
#define CC_SLE 			11	// 1011
#define CC_SLT 			12	// 1100
#define CC_SGE 			13	// 1101
#define CC_UGT 			14	// 1110
#define CC_ULE 			15	// 1111

/* bit helper methods macros */
#define set_bit(bits,i)   	   ((bits[i>>3] |= bit_or[i%8]))
#define clear_bit(bits,i) 	   ((bits[i>>3] &= bit_and[i%8]))
#define get_bit(bits,i)  	   ((bits[i>>3] &  bit_and[i%8]) == bits[i>>3])

#define get_bits(in, from, to) ((in & (( (1<<(to-from+1))-1) << from)) >> from)

typedef struct REGS {
 char *name;
} REGS;

typedef struct BITS {
 unsigned char op1;
 unsigned char op2;
 unsigned char op3;
 unsigned char op4;
 unsigned char op5;
} BITS;

typedef struct INST {
 unsigned char opcode;
 unsigned char len;
 struct BITS bits;
 unsigned char *name;
 struct INST *link; 
} INST;

extern INST inst_set[0xff];

void c167x_diss(unsigned char *rom_start, uint8_t *buf, int len);

#endif


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
#define UNDEFINED           0 
#define RW 					1       //  Rn
#define RW_IND 				2 	//  [Rn]
#define RW_IND_POST_INC		3 	//  [Rn]+
#define RW_IND_POST_DEC		4 	//  [Rn]-
#define RW_IND_PRE_INC		5 	// +[Rn]
#define RW_IND_PRE_DEC		6 	// -[Rn]
#define RW_IND_1_DATA16_2	7 	//  [Rn+Data16]
#define RB					8 
#define REGB				9 
#define REGW				10
#define MEM					11
#define BITADR				12
#define BITOFF				13
#define IRANG2				14
#define DATA3				15
#define DATA4				16
#define DATA8				17
#define DATA16 				18
#define MASK8 				19
#define ADR					20
#define SEG					21
#define REG 				22
#define TRAP7 				23
#define CC 					24
#define REL 				25
#define OPDATA				26
#define RW_L				27       //  Rn
#define REGB2				28
#define REGB3				29
#define BITADR_W			30
#define BITADR_W2			31

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

#define get_hi_nibble(rval)	   ((rval & 0xf0) >> 4)
#define get_lo_nibble(rval)	   ((rval & 0x0f))

#define MAX_ARGS		4

typedef struct REGB_ENTRY {
	char *name;
	unsigned char hex;
} REGB_ENTRY;

typedef struct REG_ENTRY {
	char *name;
	unsigned short hex;
} REG_ENTRY;

typedef struct REGS {
	char *name;
} REGS;

typedef struct SFR_ENTRY {
	char          *name;
	unsigned short hex_w;
	unsigned char  hex_b;
	char          *desc;
} SFR_ENTRY;

typedef struct ARG_ENTRY {
	unsigned char type;
	unsigned char pos;
} ARG_ENTRY;
 
typedef struct ARG_DEF {
	ARG_ENTRY list[MAX_ARGS];
} ARG_DEF;

typedef struct BITS {
 unsigned char ops[5];
} BITS;

typedef struct INST {
 unsigned char  opcode;
 unsigned char  len;
 char          *name;
 struct BITS    bits;
 char          *fmt;
 char           argcount;
 ARG_DEF        args;
 void          *tmp;
 struct INST   **link;
 unsigned char  addLF;
} INST;

extern REG_ENTRY CCNames[];
extern REG_ENTRY RHNames[];
extern SFR_ENTRY SFR_Entries[];
extern SFR_ENTRY SFR8_Entries[];
extern INST inst_table_0x08[];                                                      
extern INST inst_table_0x09[];                                                         
extern INST inst_table_0x18[];                                                         
extern INST inst_table_0x19[];                                                         
extern INST inst_table_0x28[];                                                         
extern INST inst_table_0x29[];                                                         
extern INST inst_table_0x38[];                                                         
extern INST inst_table_0x39[];                                                         
extern INST inst_table_0x48[];                                                         
extern INST inst_table_0x49[];                                                         
extern INST inst_table_0x58[];                                                         
extern INST inst_table_0x59[];                                                         
extern INST inst_table_0x68[];                                                         
extern INST inst_table_0x69[];                                                         
extern INST inst_table_0x78[];                                                         
extern INST inst_table_0x79[];                                                         
extern INST inst_table_0xb7[];                                                         
extern INST inst_table_0xd1[];                                                         
extern INST inst_table_0xd7[];                                                         
extern INST inst_table_0xdc[];                                                         
extern INST inst_set[256];

void c167x_diss(unsigned char *rom_start, uint8_t *buf, int len);

#endif


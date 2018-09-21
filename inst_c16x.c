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
#include "inst_c16x.h"

/* _very_ Crude C16x disassembler
 * 
 *  - This will be further developed with time. Just a little
 *    something to get started ;)
 */
void c167x_diss(unsigned char *rom_start, uint8_t *buf, int len)
{
 unsigned char chA, chB, chC, chD;
 unsigned long i=0,skip;
	
	i=0;
	while(1) 
	{
		if(len > 4) {		
			chA = *(buf+0); chB = *(buf+1); chC = *(buf+2); chD = *(buf+3);

			// show offsets and opcode name
			printf("\t%#8X [+%-4d]: %-42s",rom_start+i, i, inst_set[chA].name);

			// get length of opcode from lookup table
			skip = inst_set[chA].len;			

			// show hex dump of the opcode
			if(skip == 2) {			// opcodes can either be 2 bytes
				printf("\t[%d] %02X %02X%\n",skip, chA, chB);
			} else if(skip == 4) { 	// or 4 bytes long...
				printf("\t[%d] %02X %02X %02X %02X\n",skip, chA, chB, chC, chD);
			} else {
				break;
			}		

			// to next opcode in buffer
			buf += skip;
			i   += skip;
			len -= skip;
		} else {
			break;
		}
	}
	printf("***\n");
}

REGS RegisterNames[] = {
 "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", 
 "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15",
 "rl0","rh0","rl1","rh1","rl2","rh2","rl3","rh3","rl4",
 "rh4","rl5","rh5","rl6","rh6","rl7","rh7","acc",
 "MSW","MAH","MAS","MAL","MCW","MRW","idx0","idx1",
 "qx0","qx1","qr0","qr1","rnd","csp",
 "dpp0","dpp1","dpp2","dpp3"
};

INST inst_table_0x08[] =                                                      
{                                                                                
 { .opcode = 0x08, .len = 2, .bits = { 4,1,3 },                 .name = "add      Rw_a,   cdata3_b3_b"    },
 { .opcode = 0x08, .len = 2, .bits = { 4,2,2 },                 .name = "add      Rw_a,   [Rw_b]"  },
 { .opcode = 0x08, .len = 2, .bits = { 4,2,2 },                 .name = "add      Rw_a,   [Rw_b]+" },
};                                                                               
                                                                                 
INST inst_table_0x09[] =                                                         
{                                                                                
 { .opcode = 0x09, .len = 2, .bits = { 4,1,3 },                 .name = "addb     Rb_a,   cdata3_b3_b" },
 { .opcode = 0x09, .len = 2, .bits = { 4,2,2 },                 .name = "addb     Rb_a,   [Rw_b]" },
 { .opcode = 0x09, .len = 2, .bits = { 4,2,2 },                 .name = "addb     Rb_a,   [Rw_b]+" },
};                                                                               
                                                                                 
INST inst_table_0x18[] =                                                         
{                                                                                
 { .opcode = 0x18, .len = 2, .bits = { 4,1,3 },                 .name = "addc     Rw_a, cdata3_b3_b" },
 { .opcode = 0x18, .len = 2, .bits = { 4,2,2 },                 .name = "addc     Rw_a, [Rw_b]" },
 { .opcode = 0x18, .len = 2, .bits = { 4,2,2 },                 .name = "addc     Rw_a, [Rw_b]+" },
};                                                                               
                                                                                 
INST inst_table_0x19[] =                                                         
{                                                                                
 { .opcode = 0x19, .len = 2, .bits = { 4,1,3 },                 .name = "addcb    Rb_a, cdata3_b3_b" },
 { .opcode = 0x19, .len = 2, .bits = { 4,2,2 },                 .name = "addcb    Rb_a, [Rw_b]" },
 { .opcode = 0x19, .len = 2, .bits = { 4,2,2 },                 .name = "addcb    Rb_a, [Rw_b]+" },
};                                                                               
                                                                                 
INST inst_table_0x28[] =                                                         
{                                                                                
 { .opcode = 0x28, .len = 2, .bits = { 4,1,3 },                 .name = "sub      Rw_a, cdata3_b3_b" },
 { .opcode = 0x28, .len = 2, .bits = { 4,2,2 },                 .name = "sub      Rw_a, [Rw_b]" },
 { .opcode = 0x28, .len = 2, .bits = { 4,2,2 },                 .name = "sub      Rw_a, [Rw_b]+" },
};                                                                               
                                                                                 
INST inst_table_0x29[] =                                                         
{                                                                                
 { .opcode = 0x29, .len = 2, .bits = { 4,1,3 },                 .name = "subb     Rb_a, cdata3_b3_b" },
 { .opcode = 0x29, .len = 2, .bits = { 4,2,2 },                 .name = "subb     Rb_a, [Rw_b]" },
 { .opcode = 0x29, .len = 2, .bits = { 4,2,2 },                 .name = "subb     Rb_a, [Rw_b]+" },
};                                                                               
                                                                                 
INST inst_table_0x38[] =                                                         
{                                                                                
 { .opcode = 0x38, .len = 2, .bits = { 4,1,3 },                 .name = "subc     Rw_a, cdata3_b3_b" },
 { .opcode = 0x38, .len = 2, .bits = { 4,2,2 },                 .name = "subc     Rw_a, [Rw_b]" },
 { .opcode = 0x38, .len = 2, .bits = { 4,2,2 },                 .name = "subc     Rw_a, [Rw_b]+" },
};                                                                               
                                                                                 
INST inst_table_0x39[] =                                                         
{                                                                                
 { .opcode = 0x39, .len = 2, .bits = { 4,1,3 },                 .name = "subcb    Rb_a, cdata3_b3_b" },
 { .opcode = 0x39, .len = 2, .bits = { 4,2,2 },                 .name = "subcb    Rb_a, [Rw_b]" },
 { .opcode = 0x39, .len = 2, .bits = { 4,2,2 },                 .name = "subcb    Rb_a, [Rw_b]+" },
};                                                                               
                                                                                 
INST inst_table_0x48[] =                                                         
{                                                                                  
 { .opcode = 0x48, .len = 2, .bits = { 4,1,3 },                 .name = "cmp      Rw_a, cdata3_b3_b " },
 { .opcode = 0x48, .len = 2, .bits = { 4,2,2 },                 .name = "cmp      Rw_a, [Rw_b]" },
 { .opcode = 0x48, .len = 2, .bits = { 4,2,2 },                 .name = "cmp      Rw_a, [Rw_b]+" },
};                                                                               
                                                                                 
INST inst_table_0x49[] =                                                         
{                                                                                
 { .opcode = 0x49, .len = 2, .bits = { 4,1,3 },                 .name = "cmpb     Rb_a, cdata3_b3_b" },
 { .opcode = 0x49, .len = 2, .bits = { 4,2,2 },                 .name = "cmpb     Rb_a, [Rw_b]" },
 { .opcode = 0x49, .len = 2, .bits = { 4,2,2 },                 .name = "cmpb     Rb_a, [Rw_b]+" },
};                                                                               
                                                                                 
INST inst_table_0x58[] =                                                         
{                                                                                
 { .opcode = 0x58, .len = 2, .bits = { 4,1,3 },                 .name = "xor      Rw_a, cdata3_b3_b" },
 { .opcode = 0x58, .len = 2, .bits = { 4,2,2 },                 .name = "xor      Rw_a, [Rw_b]" },
 { .opcode = 0x58, .len = 2, .bits = { 4,2,2 },                 .name = "xor      Rw_a, [Rw_b]+" },
};                                                                               
                                                                                 
INST inst_table_0x59[] =                                                         
{                                                                                
 { .opcode = 0x59, .len = 2, .bits = { 4,1,3 },                 .name = "xorb     Rb_a, cdata3_b3_b" },
 { .opcode = 0x59, .len = 2, .bits = { 4,2,2 },                 .name = "xorb     Rb_a, [Rw_b]" },
 { .opcode = 0x59, .len = 2, .bits = { 4,2,2 },                 .name = "xorb     Rb_a, [Rw_b]+" },
};                                                                               
                                                                                 
INST inst_table_0x68[] =                                                         
{                                                                                
 { .opcode = 0x68, .len = 2, .bits = { 4,1,3 },                 .name = "and      Rw_a, cdata3_b3_b" },
 { .opcode = 0x68, .len = 2, .bits = { 4,2,2 },                 .name = "and      Rw_a, [Rw_b]" },
 { .opcode = 0x68, .len = 2, .bits = { 4,2,2 },                 .name = "and      Rw_a, [Rw_b]+" },
};                                                                               
                                                                                 
INST inst_table_0x69[] =                                                         
{                                                                                
 { .opcode = 0x69, .len = 2, .bits = { 4,1,3 },                 .name = "andb     Rb_a, cdata3_b3_b" },
 { .opcode = 0x69, .len = 2, .bits = { 4,2,2 },                 .name = "andb     Rb_a, [Rw_b]" },
 { .opcode = 0x69, .len = 2, .bits = { 4,2,2 },                 .name = "andb     Rb_a, [Rw_b]+" },
};                                                                               
                                                                                 
INST inst_table_0x78[] =                                                         
{                                                                                
 { .opcode = 0x78, .len = 2, .bits = { 4,1,3 },                 .name = "or       Rw_a, cdata3_b3_b" },
 { .opcode = 0x78, .len = 2, .bits = { 4,2,3 },                 .name = "or       Rw_a, [Rw_b]" },
 { .opcode = 0x78, .len = 2, .bits = { 4,2,3 },                 .name = "or       Rw_a, [Rw_b]+" },
};                                                                               
                                                                                 
INST inst_table_0x79[] =                                                         
{                                                                                
 { .opcode = 0x79, .len = 2, .bits = { 4,1,3 },                 .name = "orb      Rb_a, cdata3_b3_b" },
 { .opcode = 0x79, .len = 2, .bits = { 4,2,3 },                 .name = "orb      Rb_a, [Rw_b]" },
 { .opcode = 0x79, .len = 2, .bits = { 4,2,3 },                 .name = "orb      Rb_a, [Rw_b]+" },
};                                                                               
                                                                                 
INST inst_table_0xb7[] =                                                         
{                                                                                
 { .opcode = 0xb7, .len = 4, .bits = { 24 },                    .name = "srst" },
 { .opcode = 0xb7, .len = 4, .bits = { 24 },                    .name = "srvwdt" },
};                                                                               
                                                                                 
INST inst_table_0xd1[] =                                                         
{                                                                                
 { .opcode = 0xd1, .len = 2, .bits = { 2,2,4 },                 .name = "atomic   cirang2_a  " },
 { .opcode = 0xd1, .len = 2, .bits = { 2,2,4 },                 .name = "extr     cirang2_a  " },
};                                                                               
                                                                                 
INST inst_table_0xd7[] =                                                         
{                                                                                
 { .opcode = 0xd7, .len = 4, .bits = { 2,2,4,8,8 },             .name = "exts     cseg_b, cirang2eg_b, cirang2_a" },
 { .opcode = 0xd7, .len = 4, .bits = { 2,2,4 },                 .name = "iextp    Rw_b,  cirang2ng2_a" },
 { .opcode = 0xd7, .len = 4, .bits = { 2,2,4,8,8 },             .name = "extsr    cseg_b, cirang2eg_b, cirang2_a" },
 { .opcode = 0xd7, .len = 4, .bits = { 2,2,4 },                 .name = "iextpr   Rw_b,  cirang2ng2_a" },
};                                                                               
                                                                                 
INST inst_table_0xdc[] =                                                         
{                                                                                
 { .opcode = 0xdc, .len = 2, .bits = { 2,2,4 },                 .name = "exts     Rw_b, cirang2_g2_a" },
 { .opcode = 0xdc, .len = 2, .bits = { 2,2,4 },                 .name = "extp     Rw_b, cirang2_g2_a" },
 { .opcode = 0xdc, .len = 2, .bits = { 2,2,4 },                 .name = "extsr    Rw_b, cirang2_g2_a" },
 { .opcode = 0xdc, .len = 2, .bits = { 2,2,4 },                 .name = "extpr    Rw_b, cirang2_g2_a" },
};                                                                               
                                                                                 
INST inst_set[0xff] =                                                            
{                                                                                
 { .opcode = 0x00, .len = 2, .bits = { 4,4 },                   .name = "add      Rw_a,   Rw_b   " },
 { .opcode = 0x01, .len = 2, .bits = { 4,4 },                   .name = "addb     Rb_a,   Rb_b   " },
 { .opcode = 0x02, .len = 4, .bits = { 8,16 },                  .name = "add      Rw_a,   var_b" },
 { .opcode = 0x03, .len = 4, .bits = { 8,16 },                  .name = "addb     Rb_a,   var_b" },
 { .opcode = 0x04, .len = 4, .bits = { 8,16 },                  .name = "add      var_b,  Rw_a" },
 { .opcode = 0x05, .len = 4, .bits = { 8,16 },                  .name = "addb     var_b,  Rb_a" },
 { .opcode = 0x06, .len = 4, .bits = { 8,16 },                  .name = "add      Rw_a,   #cdata16_b" },
 { .opcode = 0x07, .len = 4, .bits = { 8,16 },                  .name = "addb     Rb_a,   #cdata16_b" },
 { .opcode = 0x08, .len = 2, .link = &inst_table_0x08,          .name = "add      ..." }, 
 { .opcode = 0x09, .len = 2, .link = &inst_table_0x09,          .name = "addb     ..." }, 
 { .opcode = 0x0a, .len = 4, .bits = { 8,8,8 },                 .name = "bfldl    cbitoff_a, cmasmask8_c, cdata8_b" },
 { .opcode = 0x0b, .len = 2, .bits = { 4,4 },                   .name = "mul      Rw_a,   Rw_b   " },
 { .opcode = 0x0c, .len = 2, .bits = { 4,4 },                   .name = "rol      Rw_a,   Rw_b   " },
 { .opcode = 0x0d, .len = 2, .bits = { 8 },                     .name = "jmpr     ccc_0000,Rel_l_a" },
 { .opcode = 0x0e, .len = 2, .bits = { 8 },                     .name = "bclr     cbitaddr_a_000000" },
 { .opcode = 0x0f, .len = 2, .bits = { 8 },                     .name = "bset     cbitaddr_a_000000" },
                                                                                 
 { .opcode = 0x10, .len = 2, .bits = { 4,4 },                   .name = "addc     Rw_a,   Rw_b   " },
 { .opcode = 0x11, .len = 2, .bits = { 4,4 },                   .name = "addcb    Rb_a,   Rb_b   " },
 { .opcode = 0x12, .len = 4, .bits = { 8,16 },                  .name = "addc     Rw_a,   var_b" },
 { .opcode = 0x13, .len = 4, .bits = { 8,16 },                  .name = "addcb    Rb_a,   var_b" },
 { .opcode = 0x14, .len = 4, .bits = { 8,16 },                  .name = "addc     var_b,  Rw_a" },
 { .opcode = 0x15, .len = 4, .bits = { 8,16 },                  .name = "addcb    var_b,  Rb_a" },
 { .opcode = 0x16, .len = 4, .bits = { 8,16 },                  .name = "addc     Rw_a,   #cdata16_b" },
 { .opcode = 0x17, .len = 4, .bits = { 8,16 },                  .name = "addcb    Rb_a,   #cdata16_b" },
 { .opcode = 0x18, .len = 2, .link = &inst_table_0x18,          .name = "addc     ..." }, 
 { .opcode = 0x19, .len = 2, .link = &inst_table_0x19,          .name = "addcb    ..." }, 
 { .opcode = 0x1a, .len = 4, .bits = { 8,8,8 },                 .name = "bfldh    cbitoff_a, cmasmask8_c, cdata8_b" },
 { .opcode = 0x1b, .len = 2, .bits = { 4,4 },                   .name = "mulu     Rw_a,   Rw_b" },
 { .opcode = 0x1c, .len = 2, .bits = { 4,4 },                   .name = "rol      Rw_b,   cdata4_a" },
 { .opcode = 0x1d, .len = 2, .bits = { 8 },                     .name = "jmpr     ccc_0001,Rel_l_a" },
 { .opcode = 0x1e, .len = 2, .bits = { 8 },                     .name = "bclr     cbitaddr_a_0001" },
 { .opcode = 0x1f, .len = 2, .bits = { 8 },                     .name = "bset     cbitaddr_a_0001" },   
                                                                                 
 { .opcode = 0x20, .len = 2, .bits = { 4,4 },                   .name = "sub      Rw_a,   Rw_b" },
 { .opcode = 0x21, .len = 2, .bits = { 4,4 },                   .name = "subb     Rb_a,   Rb_b" },
 { .opcode = 0x22, .len = 4, .bits = { 8,16 },                  .name = "sub      Rw_a,   cmm_b" },
 { .opcode = 0x23, .len = 4, .bits = { 8,16 },                  .name = "subb     Rb_a,   cmm_b" },
 { .opcode = 0x24, .len = 4, .bits = { 8,16 },                  .name = "sub      var_b,  Rw_a" },
 { .opcode = 0x25, .len = 4, .bits = { 8,16 },                  .name = "subb     var_b,  Rb_a" },
 { .opcode = 0x26, .len = 4, .bits = { 8,16 },                  .name = "sub      Rw_a,   #cdata16_b" },
 { .opcode = 0x27, .len = 4, .bits = { 8,16 },                  .name = "subb     Rb_a,   #cdata16_b" },
 { .opcode = 0x28, .len = 2, .link = &inst_table_0x28,          .name = "sub      ..." }, 
 { .opcode = 0x29, .len = 2, .link = &inst_table_0x29,          .name = "subb     ..." }, 
 { .opcode = 0x2a, .len = 4, .bits = { 8,8,4,4 },               .name = "bcmp     cbitaddr_b,, cbitaddr_d, cbitaddr_a,  cbitaddr_c" },
 { .opcode = 0x2b, .len = 2, .bits = { 4,4 },                   .name = "prior    Rw_a, Rw_b" },
 { .opcode = 0x2c, .len = 2, .bits = { 4,4 },                   .name = "ror      Rw_a, Rw_b" },
 { .opcode = 0x2d, .len = 2, .bits = { 8 },                     .name = "jmpr     ccc_0010, Rel_l_a" },
 { .opcode = 0x2e, .len = 2, .bits = { 8 },                     .name = "bclr     cbitaddr_a_001010" },
 { .opcode = 0x2f, .len = 2, .bits = { 8 },                     .name = "bset     cbitaddr_a_001010" },
                                                                                 
 { .opcode = 0x30, .len = 2, .bits = { 4,4 },                   .name = "subc     Rw_a,   Rw_b" },
 { .opcode = 0x31, .len = 2, .bits = { 4,4 },                   .name = "subcb    Rb_a,   Rb_b" },
 { .opcode = 0x32, .len = 4, .bits = { 8,16 },                  .name = "subc     Rw_a,   cmm_b" },
 { .opcode = 0x33, .len = 4, .bits = { 8,16 },                  .name = "subcb    Rb_a,   cmm_b" },
 { .opcode = 0x34, .len = 4, .bits = { 8,16 },                  .name = "subc     var_b,  Rw_a" },
 { .opcode = 0x35, .len = 4, .bits = { 8,16 },                  .name = "subcb    var_b,  Rb_a" },
 { .opcode = 0x36, .len = 4, .bits = { 8,16 },                  .name = "subc     Rw_a,   #cdata16_b" },
 { .opcode = 0x37, .len = 4, .bits = { 8,16 },                  .name = "subcb    Rb_a,   #cdata16_b" },
 { .opcode = 0x38, .len = 2, .link = &inst_table_0x38,          .name = "subc     ..." }, 
 { .opcode = 0x39, .len = 2, .link = &inst_table_0x39,          .name = "subcb    ..." }, 
 { .opcode = 0x3a, .len = 4, .bits = { 8,8,4,4 },               .name = "bmovn    cbitaddr_b,, cbitaddr_d, cbitaddr_a, cbitaddr_c" },
 { .opcode = 0x3b, .len = 0,                                    .name = "..."           },      
 { .opcode = 0x3c, .len = 2, .bits = { 4,4 },                   .name = "ror      Rw_b, cdata4_a" },
 { .opcode = 0x3d, .len = 2, .bits = { 8 },                     .name = "jmpr     ccc_0011, Rel_l_a" },
 { .opcode = 0x3e, .len = 2, .bits = { 8 },                     .name = "bclr     cbitaddr_a_001111" },
 { .opcode = 0x3f, .len = 2, .bits = { 8 },                     .name = "bset     cbitaddr_a_001111" },
                                                                                 
 { .opcode = 0x40, .len = 2, .bits = { 4,4 },                   .name = "cmp      Rw_a,   Rw_b" },
 { .opcode = 0x41, .len = 2, .bits = { 4,4 },                   .name = "cmpb     Rb_a,   Rb_b" },
 { .opcode = 0x42, .len = 4, .bits = { 8,16 },                  .name = "cmp      Rw_a,   cmm_b" },
 { .opcode = 0x43, .len = 4, .bits = { 8,16 },                  .name = "cmpb     Rb_a,   cmm_b" },
 { .opcode = 0x44, .len = 0,                                    .name = "..."           },     
 { .opcode = 0x45, .len = 0,                                    .name = "..."           },     
 { .opcode = 0x46, .len = 4, .bits = { 8,16 },                  .name = "cmp      Rw_a,   #cdata16_b" },
 { .opcode = 0x47, .len = 4, .bits = { 8,16 },                  .name = "cmpb     Rb_a,   #cdata16_b" },
 { .opcode = 0x48, .len = 2, .link = &inst_table_0x48,          .name = "cmp      ..." }, 
 { .opcode = 0x49, .len = 2, .link = &inst_table_0x49,          .name = "cmpb     ..." }, 
 { .opcode = 0x4a, .len = 4, .bits = { 8,8,4,4 },               .name = "bmov     cbitaddr_b, cbitaddr_d, cbitaddr_a, cbitaddr_c" },
 { .opcode = 0x4b, .len = 2, .bits = { 4,4},                    .name = "div      Rw_a" },
 { .opcode = 0x4c, .len = 2, .bits = { 4,4 },                   .name = "shl      Rw_a,   Rw_b" },
 { .opcode = 0x4d, .len = 2, .bits = { 8 },                     .name = "jmpr     ccc_0100, Rel_a" },
 { .opcode = 0x4e, .len = 2, .bits = { 8 },                     .name = "bclr     cbitaddr_a_0100" },
 { .opcode = 0x4f, .len = 2, .bits = { 8 },                     .name = "bset     cbitaddr_a_0100" },
                                                                                 
 { .opcode = 0x50, .len = 2, .bits = { 4,4 },                   .name = "xor      Rw_a,   Rw_b" },
 { .opcode = 0x51, .len = 2, .bits = { 4,4 },                   .name = "xorb     Rb_a,   Rb_b" },
 { .opcode = 0x52, .len = 4, .bits = { 8,16 },                  .name = "xor      Rw_a,   cmm_b" },
 { .opcode = 0x53, .len = 4, .bits = { 8,16 },                  .name = "xorb     Rb_a,   cmm_b" },
 { .opcode = 0x54, .len = 4, .bits = { 8,16 },                  .name = "xor      var_b,  Rw_a" },
 { .opcode = 0x55, .len = 4, .bits = { 8,16 },                  .name = "xorb     var_b,  Rb_a" },
 { .opcode = 0x56, .len = 4, .bits = { 8,16 },                  .name = "xor      Rw_a,   #cdata16_b" },
 { .opcode = 0x57, .len = 4, .bits = { 8,16 },                  .name = "xorb     Rb_a,   #cdata16_b" },
 { .opcode = 0x58, .len = 2, .link = &inst_table_0x58,          .name = "xor      ..." }, 
 { .opcode = 0x59, .len = 2, .link = &inst_table_0x59,          .name = "xorb     ..." }, 
 { .opcode = 0x5a, .len = 4, .bits = { 8,8,4,4 },               .name = "bor      cbitaddr_b, cbitaddr_d, cbitaddr_a, cbitaddr_c" },
 { .opcode = 0x5b, .len = 2, .bits = { 4,4 },                   .name = "divu     Rw_a" },
 { .opcode = 0x5c, .len = 2, .bits = { 4,4 },                   .name = "shl      Rw_b,   #cdata_a" },
 { .opcode = 0x5d, .len = 2, .bits = { 8 },                     .name = "jmpr     ccc_0101, Rel_a" },
 { .opcode = 0x5e, .len = 2, .bits = { 8 },                     .name = "bclr     cbitaddr_a_0101" },
 { .opcode = 0x5f, .len = 2, .bits = { 8 },                     .name = "bset     cbitaddr_a_0101" },
                                                                                 
 { .opcode = 0x60, .len = 2, .bits = { 4,4 },                   .name = "and      Rw_a,   Rw_b" },
 { .opcode = 0x61, .len = 2, .bits = { 4,4 },                   .name = "andb     Rb_a,   Rb_" },
 { .opcode = 0x62, .len = 4, .bits = { 8,16 },                  .name = "and      Rw_a,   cmm_b" },
 { .opcode = 0x63, .len = 4, .bits = { 8,16 },                  .name = "andb     Rb_a,   cmm_b" },
 { .opcode = 0x64, .len = 4, .bits = { 8,16 },                  .name = "and      var_b,  Rw_a" },
 { .opcode = 0x65, .len = 4, .bits = { 8,16 },                  .name = "andb     var_b,  Rb_a" },
 { .opcode = 0x66, .len = 4, .bits = { 8,16 },                  .name = "and      Rw_a,   #cdata16_b" },
 { .opcode = 0x67, .len = 4, .bits = { 8,16 },                  .name = "andb     Rb_a,   #cdata16_b" },
 { .opcode = 0x68, .len = 2, .link = &inst_table_0x68,          .name = "and      ..." }, 
 { .opcode = 0x69, .len = 2, .link = &inst_table_0x69,          .name = "andb     ..." }, 
 { .opcode = 0x6a, .len = 4, .bits = { 8,8,4,4 },               .name = "band     cbitaddr_b, cbitaddr_d, cbitaddr_a, cbitaddr_c" },
 { .opcode = 0x6b, .len = 2, .bits = { 4,4 },                   .name = "divl     Rw_a" },
 { .opcode = 0x6c, .len = 2, .bits = { 4,4 },                   .name = "shr      Rw_a, Rw_b" },
 { .opcode = 0x6d, .len = 2, .bits = { 8 },                     .name = "jmpr     ccc_0110, Rel_a" },
 { .opcode = 0x6e, .len = 2, .bits = { 8 },                     .name = "bclr     cbitaddr_a_0110" },
 { .opcode = 0x6f, .len = 2, .bits = { 8 },                     .name = "bset     cbitaddr_a_0110" },
                                                                                 
 { .opcode = 0x70, .len = 2, .bits = { 4,4 },                   .name = "or       Rw_a,   Rw_b" },
 { .opcode = 0x71, .len = 2, .bits = { 4,4 },                   .name = "orb      Rb_a,   Rb_" },
 { .opcode = 0x72, .len = 4, .bits = { 8,16 },                  .name = "or       Rw_a,   cmm_b" },
 { .opcode = 0x73, .len = 4, .bits = { 8,16 },                  .name = "orb      Rb_a,   cmm_b" },
 { .opcode = 0x74, .len = 4, .bits = { 8,16 },                  .name = "or       var_b,  Rw_a" },
 { .opcode = 0x75, .len = 4, .bits = { 8,16 },                  .name = "orb      var_b,  Rb_a" },
 { .opcode = 0x76, .len = 4, .bits = { 8,16 },                  .name = "or       Rw_a,   #cdata16_b" },
 { .opcode = 0x77, .len = 4, .bits = { 8,16 },                  .name = "orb      Rb_a,   #cdata16_b" },
 { .opcode = 0x78, .len = 2, .link = &inst_table_0x78,          .name = "or       ..." }, 
 { .opcode = 0x79, .len = 2, .link = &inst_table_0x79,          .name = "orb      ..." }, 
 { .opcode = 0x7a, .len = 4, .bits = { 8,8,4,4 },               .name = "bxor     cbitaddr_b,, cbitaddr_d, cbitaddr_a, cbitaddr_c" },
 { .opcode = 0x7b, .len = 2, .bits = { 4,4},                    .name = "divlu    Rw_a" },
 { .opcode = 0x7c, .len = 2, .bits = { 4,4 },                   .name = "shr      Rw_b,   #cdata_a" },
 { .opcode = 0x7d, .len = 2, .bits = { 8 },                     .name = "jmpr     ccc_0111 , Rel_a" },
 { .opcode = 0x7e, .len = 2, .bits = { 8 },                     .name = "bclr     cbitaddr_a_0111" },
 { .opcode = 0x7f, .len = 2, .bits = { 8 },                     .name = "bset     cbitaddr_a_0111" },
                                                                                 
 { .opcode = 0x80, .len = 2, .bits = { 4,4 },                   .name = "cmpi1    Rw_b,   cdata_a" },
 { .opcode = 0x81, .len = 2, .bits = { 4,4 },                   .name = "neg      Rw_a      " },
 { .opcode = 0x82, .len = 4, .bits = { 4,4,16 },                .name = "cmpi1    Rw_a,   var_b" },
 { .opcode = 0x83, .len = 4, .bits = { 8,8,8,8 },               .name = "coxxx    *" },
 { .opcode = 0x84, .len = 4, .bits = { 4,4,16 },                .name = "mov      Rw_a,   var_b" },
 { .opcode = 0x85, .len = 4, .bits = { 8,8,8,8 },               .name = "enwdt    *" },
 { .opcode = 0x86, .len = 4, .bits = { 4,4,16 },                .name = "cmpi1    Rw_a,   #cdata16_b" },
 { .opcode = 0x87, .len = 4, .bits = { 24},                     .name = "idle"          },     
 { .opcode = 0x88, .len = 2, .bits = { 4,4 },                   .name = "mov      Rw_b,   Rw_a" },
 { .opcode = 0x89, .len = 2, .bits = { 4,4 },                   .name = "movb     Rw_b,   Rb_a" },
 { .opcode = 0x8a, .len = 4, .bits = { 8,8,4,4 },               .name = "jb       cbitaddr_a, cbitaddr_c, Rel_b" },
 { .opcode = 0x8b, .len = 0,                                    .name = "..."           },     
 { .opcode = 0x8c, .len = 2, .bits = { 8,8 },                   .name = "sbrk     *" },
 { .opcode = 0x8d, .len = 2, .bits = { 8 },                     .name = "jmpr     ccc_1000, Rel_a" },
 { .opcode = 0x8e, .len = 2, .bits = { 8 },                     .name = "bclr     cbitaddr_a_1000" },
 { .opcode = 0x8f, .len = 2, .bits = { 8 },                     .name = "bset     cbitaddr_a_1000" },
                                                                                 
 { .opcode = 0x90, .len = 2, .bits = { 4,4 },                   .name = "cmpi2    Rw_b,   #cdata_a" },
 { .opcode = 0x91, .len = 2, .bits = { 4,4 },                   .name = "cpl      Rw_a      " },
 { .opcode = 0x92, .len = 4, .bits = { 4,4,16 },                .name = "cmpi2    Rw_a,   var_b" },
 { .opcode = 0x93, .len = 4, .bits = { 8,8,8,8 },               .name = "coxxx    *" },
 { .opcode = 0x94, .len = 4, .bits = { 4,4,16 },                .name = "mov      var_b,  Rw_a_ind" },
 { .opcode = 0x95, .len = 4, .bits = { 8,4,4,16 },              .name = "movb     *" },
 { .opcode = 0x96, .len = 4, .bits = { 4,4,16 },                .name = "cmpi2    Rw_a,   #cdata16_b" },
 { .opcode = 0x97, .len = 4, .bits = { 24 },                    .name = "pwrdn    " },
 { .opcode = 0x98, .len = 2, .bits = { 4,4 },                   .name = "mov      Rw_a,   [Rw+]" },
 { .opcode = 0x99, .len = 2, .bits = { 4,4 },                   .name = "movb     Rb_a,   [Rw]" },
 { .opcode = 0x9a, .len = 4, .bits = { 8,8,4,4 },               .name = "jnb      cbitaddr_a, cbitaddr_c, Rel_b" },
 { .opcode = 0x9b, .len = 2, .bits = { 7,1},                    .name = "trap     ctrap7_a" },
 { .opcode = 0x9c, .len = 2, .bits = { 4,4 },                   .name = "jmpi     ccc_a, Rw_ind" },
 { .opcode = 0x9d, .len = 2, .bits = { 8 },                     .name = "jmpr     ccc_1001, Rel_a" },
 { .opcode = 0x9e, .len = 2, .bits = { 8 },                     .name = "bclr     cbitaddr_a_1001" },
 { .opcode = 0x9f, .len = 2, .bits = { 8 },                     .name = "bset     cbitaddr_a_1001" },
                                                                                 
 { .opcode = 0xa0, .len = 2, .bits = { 4,4 },                   .name = "cmpd1    Rw_b,   #cdata_a" },
 { .opcode = 0xa1, .len = 2, .bits = { 4,4 },                   .name = "negb     Rb_a      " },
 { .opcode = 0xa2, .len = 4, .bits = { 4,4,16 },                .name = "cmpd1    Rw_a,   var_b" },
 { .opcode = 0xa3, .len = 4, .bits = { 8,8,8,8 },               .name = "coxxx    *" },
 { .opcode = 0xa4, .len = 4, .bits = { 4,4,16 },                .name = "movb     Rw_a,   var_b" },
 { .opcode = 0xa5, .len = 4, .bits = { 24 },                    .name = "diswdtt" },
 { .opcode = 0xa6, .len = 4, .bits = { 4,4,16 },                .name = "cmpd1    Rw_a,   #cdata16_b" },
 { .opcode = 0xa7, .len = 4, .bits = { 8,8,8,8 },               .name = "srvwdt   *" },
 { .opcode = 0xa8, .len = 2, .bits = { 4,4 },                   .name = "mov      Rw_a,   [Rw]" },
 { .opcode = 0xa9, .len = 2, .bits = { 4,4 },                   .name = "movb     Rb_a,   [Rw]" },
 { .opcode = 0xaa, .len = 4, .bits = { 8,8,4,4 },               .name = "jbc      cbitaddr_a,_a, cbitaddr_c, Rel_b" },
 { .opcode = 0xab, .len = 2, .bits = { 4,4 },                   .name = "calli    ccc_a,   Rw_ind" },
 { .opcode = 0xac, .len = 2, .bits = { 4,4 },                   .name = "ashr     Rw_a,   Rw_b" },
 { .opcode = 0xad, .len = 2, .bits = { 8 },                     .name = "jmpr     ccc_1010, Rel_a" },
 { .opcode = 0xae, .len = 2, .bits = { 8 },                     .name = "bclr     cbitaddr_a_1010" },
 { .opcode = 0xaf, .len = 2, .bits = { 8 },                     .name = "bset     cbitaddr_a_1010" },
                                                                                 
 { .opcode = 0xb0, .len = 2, .bits = { 4,4 },                   .name = "cmpd2    Rw_b,   #cdata_a" },
 { .opcode = 0xb1, .len = 2, .bits = { 4,4 },                   .name = "cpl      Rb_a      " },
 { .opcode = 0xb2, .len = 4, .bits = { 4,4,16 },                .name = "cmpd2    Rw_a,   var_b" }, 
 { .opcode = 0xb3, .len = 4, .bits = { 8,8,8,8 },               .name = "costore  *" }, 
 { .opcode = 0xb4, .len = 4, .bits = { 4,4,16 },                .name = "movb     var_b,  Rw_a_ind" },
 { .opcode = 0xb5, .len = 4, .bits = { 24 },                    .name = "einit    " },
 { .opcode = 0xb6, .len = 4, .bits = { 4,4,16 },                .name = "cmpd2    Rw_a,   #cdata16_b" },
 { .opcode = 0xb7, .len = 4, .link = &inst_table_0xb7,          .name = "..."           },      
 { .opcode = 0xb8, .len = 2, .bits = { 4,4 },                   .name = "mov      Rw_b,   Rw_a" },
 { .opcode = 0xb9, .len = 2, .bits = { 4,4 },                   .name = "movb     Rw_b,   Rb_a" },
 { .opcode = 0xba, .len = 4, .bits = { 8,8,4,4 },               .name = "jnbs     cbitaddr_a,_a, cbitaddr_c, Rel_b" },
 { .opcode = 0xbb, .len = 2, .bits = { 8 },                     .name = "callr    Rel_a" },
 { .opcode = 0xbc, .len = 2, .bits = { 4,4 },                   .name = "ashr     Rw_b,   #cdata_a" },
 { .opcode = 0xbd, .len = 2, .bits = { 8 },                     .name = "jmpr     ccc_1011, Rel_a" },
 { .opcode = 0xbe, .len = 2, .bits = { 8 },                     .name = "bclr     cbitaddr_a_1011" },
 { .opcode = 0xbf, .len = 2, .bits = { 8 },                     .name = "bset     cbitaddr_a_1011" },
                                                                                 
 { .opcode = 0xc0, .len = 2, .bits = { 4,4 },                   .name = "movbz    Rw_b,   Rw_a" },
 { .opcode = 0xc1, .len = 0,                                    .name = "..."           },     
 { .opcode = 0xc2, .len = 4, .bits = { 8,16 },                  .name = "movbz    Rb_a,   cmm_b" },
 { .opcode = 0xc3, .len = 4, .bits = { 8,8,8,8 },               .name = "costore  *" }, 
 { .opcode = 0xc4, .len = 4, .bits = { 4,4,16 },                .name = "mov      Rw_b,   #cdata16_c, Rw_a" },
 { .opcode = 0xc5, .len = 4, .bits = { 8,16 },                  .name = "movbz    var_b,  Rb_a" },
 { .opcode = 0xc6, .len = 4, .bits = { 8,16 },                  .name = "scxt     Rw_a,   #cdata16_b" },
 { .opcode = 0xc7, .len = 0,                                    .name = "..."           },     
 { .opcode = 0xc8, .len = 2, .bits = { 4,4 },                   .name = "mov      Rw_a,   [Rw]" },
 { .opcode = 0xc9, .len = 2, .bits = { 4,4 },                   .name = "movb     Rw_a,   [Rw]" },
 { .opcode = 0xca, .len = 4, .bits = { 4,4,16 },                .name = "calla    ccc_a,   ccaddr_baddr_b" },
 { .opcode = 0xcb, .len = 2, .bits = { 8 },                     .name = "ret"           },
 { .opcode = 0xcc, .len = 2, .bits = { 8 },                     .name = "nop"           },
 { .opcode = 0xcd, .len = 2, .bits = { 8 },                     .name = "jmpr     ccc_1100, Rel_a" },
 { .opcode = 0xce, .len = 2, .bits = { 8 },                     .name = "bclr     cbitaddr_a_1100" },
 { .opcode = 0xcf, .len = 2, .bits = { 8 },                     .name = "bset     cbitaddr_a_1100" },
                                                                                 
 { .opcode = 0xd0, .len = 2, .bits = { 4,4 },                   .name = "movbs    Rw_b,   Rw_a" },
 { .opcode = 0xd1, .len = 2, .link = &inst_table_0xd1,          .name = "atomic   ..." }, 
 { .opcode = 0xd2, .len = 4, .bits = { 8,16 },                  .name = "movbs    Rb_a,   cmm_b" },
 { .opcode = 0xd3, .len = 4, .bits = { 8,8,8,8 },               .name = "comov    *" }, 
 { .opcode = 0xd4, .len = 4, .bits = { 4,4,16 },                .name = "mov      Rw_a,   Rw_b, cb, cdata16_c_ind" },
 { .opcode = 0xd5, .len = 4, .bits = { 8,16 },                  .name = "movbs    var_b,  Rb_a" },
 { .opcode = 0xd6, .len = 4, .bits = { 8,16 },                  .name = "scxt     Rw_a,   cmm_b" },
 { .opcode = 0xd7, .len = 4, .link = &inst_table_0xd7,          .name = "extx     ..." }, 
 { .opcode = 0xd8, .len = 2, .bits = { 4,4 },                   .name = "mov      Rw_a_plus,w_b_ind" },
 { .opcode = 0xd9, .len = 2, .bits = { 4,4 },                   .name = "movb     Rw_a_plus,w_b_ind" },
 { .opcode = 0xda, .len = 4, .bits = { 8,16 },                  .name = "calls    cseg_a, ccadr_b" },
 { .opcode = 0xdb, .len = 2, .bits = { 8 },                     .name = "rets"          },
 { .opcode = 0xdc, .len = 2, .link = &inst_table_0xdc,          .name = "extx     ..." }, 
 { .opcode = 0xdd, .len = 2, .bits = { 8 },                     .name = "jmpr     ccc_1101, Rel_a" },
 { .opcode = 0xde, .len = 2, .bits = { 8 },                     .name = "bclr     cbitaddr_a_1101" },
 { .opcode = 0xdf, .len = 2, .bits = { 8 },                     .name = "bset     cbitaddr_a_1101" },
                                                                                 
 { .opcode = 0xe0, .len = 2, .bits = { 4,4 },                   .name = "mov      Rw_b,   #cdata_a" },
 { .opcode = 0xe1, .len = 2, .bits = { 4,4 },                   .name = "movb     Rb_b,   #cdata_a" },
 { .opcode = 0xe2, .len = 4, .bits = { 8,16 },                  .name = "pcall    Rw_a,   ccadr_b" },
 { .opcode = 0xe3, .len = 0,                                    .name = "..."           },     
 { .opcode = 0xe4, .len = 4, .bits = { 4,4,16 },                .name = "movb     Rw_b,   cdata16_ind, Rb_a" },
 { .opcode = 0xe5, .len = 0,                                    .name = "..."           },     
 { .opcode = 0xe6, .len = 4, .bits = { 8,16 },                  .name = "mov      Rw_a,   #cdata16_b" },
 { .opcode = 0xe7, .len = 4, .bits = { 8,16 },                  .name = "movb     Rb_a,   #cdata16_b" },
 { .opcode = 0xe8, .len = 2, .bits = { 4,4 },                   .name = "mov      Rw_a_ind, w_b_indp" },
 { .opcode = 0xe9, .len = 2, .bits = { 4,4 },                   .name = "movb     Rw_a_ind, w_b_indp" },
 { .opcode = 0xea, .len = 4, .bits = { 4,4,16 },                .name = "jmpa     ccc_a, ccaddr_baddr_b" },
 { .opcode = 0xeb, .len = 2, .bits = { 8 },                     .name = "retp     Rw_a" },
 { .opcode = 0xec, .len = 2, .bits = { 8 },                     .name = "push     Rw_a" },
 { .opcode = 0xed, .len = 2, .bits = { 8 },                     .name = "jmpr     ccc_1110, Rel_a" },
 { .opcode = 0xee, .len = 2, .bits = { 8 },                     .name = "bclr     cbitaddr_a_1110" },
 { .opcode = 0xef, .len = 2, .bits = { 8 },                     .name = "bset     cbitaddr_a_1110" },
                                                                                 
 { .opcode = 0xf0, .len = 2, .bits = { 4,4 },                   .name = "mov      Rw_a,  Rw_b" },
 { .opcode = 0xf1, .len = 2, .bits = { 4,4 },                   .name = "movb     Rb_a,  Rb_b" },
 { .opcode = 0xf2, .len = 4, .bits = { 8,16 },                  .name = "mov      Rw_a,  cmm_b" },
 { .opcode = 0xf3, .len = 4, .bits = { 8,16 },                  .name = "movb     Rb_a,  cmm_b" },
 { .opcode = 0xf4, .len = 4, .bits = { 4,4,16 },                .name = "movb     Rb_a,  Rw_b, cb, cdata16_c_ind" },
 { .opcode = 0xf5, .len = 0,                                    .name = "..."           },     
 { .opcode = 0xf6, .len = 4, .bits = { 8,16 },                  .name = "mov      var_b, Rw_a" },
 { .opcode = 0xf7, .len = 4, .bits = { 8,16 },                  .name = "movb     var_b, Rb_a" },
 { .opcode = 0xf8, .len = 0,                                    .name = "..."           },     
 { .opcode = 0xf9, .len = 0,                                    .name = "..."           },     
 { .opcode = 0xfa, .len = 4, .bits = { 8,16 },                  .name = "jmps     cseg_a, ccadr_b" },
 { .opcode = 0xfb, .len = 2, .bits = { 8 },                     .name = "reti"          },
 { .opcode = 0xfc, .len = 2, .bits = { 8 },                     .name = "pop      Rw_a" },
 { .opcode = 0xfd, .len = 2, .bits = { 8 },                     .name = "jmpr     ccc_1111, Rel_a" },
 { .opcode = 0xfe, .len = 2, .bits = { 8 },                     .name = "bclr     cbitaddr_a_1111" },
 { .opcode = 0xff, .len = 2, .bits = { 8 },                     .name = "bset     cbitaddr_a_1111" }
};


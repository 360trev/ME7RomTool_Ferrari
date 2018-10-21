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

REG_ENTRY RHNames[] =
{
	{ .name = "r10", 0b0000 }, 	// 0   0000
	{ .name = "rh0", 0b0001 },	// 1   0001
	{ .name = "rl1", 0b0010 },	// 2   0010
	{ .name = "rh1", 0b0011 },	// 3   0011
	{ .name = "r12", 0b0100 },	// 4   0100
	{ .name = "rh2", 0b0101 },	// 5   0101
	{ .name = "r13", 0b0110 },	// 6   0110
	{ .name = "rh3", 0b0111 },	// 7   0111
	{ .name = "r14", 0b1000 },	// 8   1000
	{ .name = "rh4", 0b1001 },	// 9   1001
	{ .name = "rl5", 0b1010 },	// A   1010
	{ .name = "rh5", 0b1011 },	// B   1011
	{ .name = "rl6", 0b1100 },	// C   1100
	{ .name = "rh6", 0b1101 },	// D   1101
	{ .name = "rl7", 0b1110 },	// E   1110
	{ .name = "rh7", 0b1111 }	// F   1111
}; 

REG_ENTRY CCNames[] =
{
	{ .name = "UC",  0b0000 },  // 0   0000
	{ .name = "NET", 0b0001 }, 	// 1   0001
	{ .name = "Z",   0b0010 },  // 2   0010
	{ .name = "NZ",  0b0011 },  // 3   0011
	{ .name = "V",   0b0100 },  // 4   0100
	{ .name = "NV",  0b0101 },  // 5   0101
	{ .name = "N",   0b0110 },  // 6   0110
	{ .name = "NN",  0b0111 },  // 7   0111
	{ .name = "C",   0b1000 },  // 8   1000
	{ .name = "NC",  0b1001 },  // 9   1001
	{ .name = "SGT", 0b1010 }, 	// A   1010
	{ .name = "SLE", 0b1011 }, 	// B   1011
	{ .name = "SLT", 0b1100 }, 	// C   1100
	{ .name = "SGE", 0b1101 }, 	// D   1101
	{ .name = "UGT", 0b1110 }, 	// E   1110
	{ .name = "ULE", 0b1111 }, 	// F   1111
};

SFR_ENTRY SFR8_Entries[] =
{
	{  .name = "DPP0",	  .hex_b = 0b00000000 },
	{  .name = "DPP1",	  .hex_b = 0b00000001 },
	{  .name = "DPP2",	  .hex_b = 0b00000010 },
	{  .name = "DPP3",	  .hex_b = 0b00000011 },
	{  .name = "CSP",	  .hex_b = 0b00000100 },
	{  .name = "MDH",	  .hex_b = 0b00000110 },
	{  .name = "MDL",	  .hex_b = 0b00000111 },
	{  .name = "CP",	  .hex_b = 0b00001000 },
	{  .name = "SP",	  .hex_b = 0b00001001 },
	{  .name = "STKOV",	  .hex_b = 0b00001010 },
	{  .name = "STKUN",	  .hex_b = 0b00001011 },
	{  .name = "ADDRSEL1",.hex_b = 0b00001100 },
	{  .name = "ADDRSEL2",.hex_b = 0b00001101 },
	{  .name = "ADDRSEL3",.hex_b = 0b00001110 },
	{  .name = "ADDRSEL4",.hex_b = 0b00001111 },
	{  .name = "PW0",	  .hex_b = 0b00011000 },
	{  .name = "PW1",	  .hex_b = 0b00011001 },
	{  .name = "PW2",	  .hex_b = 0b00011010 },
	{  .name = "PW3",	  .hex_b = 0b00011011 },
	{  .name = "T2",	  .hex_b = 0b00100000 },
	{  .name = "T3",	  .hex_b = 0b00100001 },
	{  .name = "T4",	  .hex_b = 0b00100010 },
	{  .name = "T5",	  .hex_b = 0b00100011 },
	{  .name = "T6",	  .hex_b = 0b00100100 },
	{  .name = "CAPREL",  .hex_b = 0b00100101 },
	{  .name = "T0",	  .hex_b = 0b00101000 },
	{  .name = "T1",	  .hex_b = 0b00101001 },
	{  .name = "T0REL",	  .hex_b = 0b00101010 },
	{  .name = "T1REL",	  .hex_b = 0b00101011 },
	{  .name = "CC16",	  .hex_b = 0b00110000 },
	{  .name = "CC17",	  .hex_b = 0b00110001 },
	{  .name = "CC18",	  .hex_b = 0b00110010 },
	{  .name = "CC19",	  .hex_b = 0b00110011 },
	{  .name = "CC20",	  .hex_b = 0b00110100 },
	{  .name = "CC21",	  .hex_b = 0b00110101 },
	{  .name = "CC22",	  .hex_b = 0b00110110 },
	{  .name = "CC23",	  .hex_b = 0b00110111 },
	{  .name = "CC24",	  .hex_b = 0b00111000 },
	{  .name = "CC25",	  .hex_b = 0b00111001 },
	{  .name = "CC26",	  .hex_b = 0b00111010 },
	{  .name = "CC27",	  .hex_b = 0b00111011 },
	{  .name = "CC28",	  .hex_b = 0b00111100 },
	{  .name = "CC29",	  .hex_b = 0b00111101 },
	{  .name = "CC30",	  .hex_b = 0b00111110 },
	{  .name = "CC31",	  .hex_b = 0b00111111 },
	{  .name = "CC0",	  .hex_b = 0b01000000 },
	{  .name = "CC1",	  .hex_b = 0b01000001 },
	{  .name = "CC2",	  .hex_b = 0b01000010 },
	{  .name = "CC3",	  .hex_b = 0b01000011 },
	{  .name = "CC4",	  .hex_b = 0b01000100 },
	{  .name = "CC5",	  .hex_b = 0b01000101 },
	{  .name = "CC6",	  .hex_b = 0b01000110 },
	{  .name = "CC7",	  .hex_b = 0b01000111 },
	{  .name = "CC8",	  .hex_b = 0b01001000 },
	{  .name = "CC9",	  .hex_b = 0b01001001 },
	{  .name = "CC10",	  .hex_b = 0b01001010 },
	{  .name = "CC11",	  .hex_b = 0b01001011 },
	{  .name = "CC12",	  .hex_b = 0b01001100 },
	{  .name = "CC13",	  .hex_b = 0b01001101 },
	{  .name = "CC14",	  .hex_b = 0b01001110 },
	{  .name = "CC15",	  .hex_b = 0b01001111 },
	{  .name = "ADDAT",	  .hex_b = 0b01010000 },
	{  .name = "WDT",	  .hex_b = 0b01010111 },
	{  .name = "S0TBUF",  .hex_b = 0b01011000 },
	{  .name = "S0RBUF",  .hex_b = 0b01011001 },
	{  .name = "S0BG",	  .hex_b = 0b01011010 },
	{  .name = "S1TBUF",  .hex_b = 0b01011100 },
	{  .name = "S1RBUF",  .hex_b = 0b01011101 },
	{  .name = "S1BG",	  .hex_b = 0b01011110 },
	{  .name = "PECC0",	  .hex_b = 0b01100000 },
	{  .name = "PECC1",	  .hex_b = 0b01100001 },
	{  .name = "PECC2",	  .hex_b = 0b01100010 },
	{  .name = "PECC3",	  .hex_b = 0b01100011 },
	{  .name = "PECC4",	  .hex_b = 0b01100100 },
	{  .name = "PECC5",	  .hex_b = 0b01100101 },
	{  .name = "PECC6",	  .hex_b = 0b01100110 },
	{  .name = "PECC7",	  .hex_b = 0b01100111 },
	{  .name = "P0",	  .hex_b = 0b10000000 },
	{  .name = "P0L",	  .hex_b = 0b10000000 },
	{  .name = "DP0",	  .hex_b = 0b10000001 },
	{  .name = "P0H",	  .hex_b = 0b10000001 },
	{  .name = "P1",	  .hex_b = 0b10000010 },
	{  .name = "P1L",	  .hex_b = 0b10000010 },
	{  .name = "DP1",	  .hex_b = 0b10000011 },
	{  .name = "P1H",	  .hex_b = 0b10000011 },
	{  .name = "P4",	  .hex_b = 0b10000100 },
	{  .name = "SYSCON",  .hex_b = 0b10000110 },
	{  .name = "BUSCON0", .hex_b = 0b10000110 },
	{  .name = "MDC",	  .hex_b = 0b10000111 },
	{  .name = "PSW",	  .hex_b = 0b10001000 },
	{  .name = "SYSCON",  .hex_b = 0b10001001 },
	{  .name = "BUSCON1", .hex_b = 0b10001010 },
	{  .name = "BUSCON2", .hex_b = 0b10001011 },
	{  .name = "BUSCON3", .hex_b = 0b10001100 },
	{  .name = "BUSCON4", .hex_b = 0b10001101 },
	{  .name = "ZEROS",	  .hex_b = 0b10001110 },
	{  .name = "ONES",	  .hex_b = 0b10001111 },
	{  .name = "T78CON",  .hex_b = 0b10010000 },
	{  .name = "CCM4",	  .hex_b = 0b10010001 },
	{  .name = "CCM5",	  .hex_b = 0b10010010 },
	{  .name = "CCM6",	  .hex_b = 0b10010011 },
	{  .name = "CCM7",	  .hex_b = 0b10010100 },
	{  .name = "PWMCON0", .hex_b = 0b10011000 },
	{  .name = "PWMCON1", .hex_b = 0b10011001 },
	{  .name = "T2CON",	  .hex_b = 0b10100000 },
	{  .name = "T3CON",	  .hex_b = 0b10100001 },
	{  .name = "T4CON",	  .hex_b = 0b10100010 },
	{  .name = "T5CON",	  .hex_b = 0b10100011 },
	{  .name = "T6CON",	  .hex_b = 0b10100100 },
	{  .name = "T01CON",  .hex_b = 0b10101000 },
	{  .name = "CCM0",	  .hex_b = 0b10101001 },
	{  .name = "CCM1",	  .hex_b = 0b10101010 },
	{  .name = "CCM2",	  .hex_b = 0b10101011 },
	{  .name = "CCM3",	  .hex_b = 0b10101100 },
	{  .name = "T2IC",	  .hex_b = 0b10110000 },
	{  .name = "T3IC",	  .hex_b = 0b10110001 },
	{  .name = "T4IC",	  .hex_b = 0b10110010 },
	{  .name = "T5IC",	  .hex_b = 0b10110011 },
	{  .name = "T6IC",	  .hex_b = 0b10110100 },
	{  .name = "CRIC",	  .hex_b = 0b10110101 },
	{  .name = "S0TIC",	  .hex_b = 0b10110110 },
	{  .name = "S0RIC",	  .hex_b = 0b10110111 },
	{  .name = "S0EIC",	  .hex_b = 0b10111000 },
	{  .name = "S1TIC",	  .hex_b = 0b10111001 },
	{  .name = "SSCTIC",  .hex_b = 0b10111001 },
	{  .name = "S1RIC",	  .hex_b = 0b10111010 },
	{  .name = "SSCRIC",  .hex_b = 0b10111010 },
	{  .name = "S1EIC",	  .hex_b = 0b10111011 },
	{  .name = "SSCEIC",  .hex_b = 0b10111011 },
	{  .name = "CC0IC",	  .hex_b = 0b10111100 },
	{  .name = "CC1IC",	  .hex_b = 0b10111101 },
	{  .name = "CC2IC",	  .hex_b = 0b10111110 },
	{  .name = "CC3IC",	  .hex_b = 0b10111111 },
	{  .name = "CC4IC",	  .hex_b = 0b11000000 },
	{  .name = "CC5IC",	  .hex_b = 0b11000001 },
	{  .name = "CC6IC",	  .hex_b = 0b11000010 },
	{  .name = "CC7IC",	  .hex_b = 0b11000011 },
	{  .name = "CC8IC",	  .hex_b = 0b11000100 },
	{  .name = "CC9IC",	  .hex_b = 0b11000101 },
	{  .name = "CC10IC",  .hex_b = 0b11000110 },
	{  .name = "CC11IC",  .hex_b = 0b11000111 },
	{  .name = "CC12IC",  .hex_b = 0b11001000 },
	{  .name = "CC13IC",  .hex_b = 0b11001001 },
	{  .name = "CC14IC",  .hex_b = 0b11001010 },
	{  .name = "CC15IC",  .hex_b = 0b11001011 },
	{  .name = "ADCIC",	  .hex_b = 0b11001100 },
	{  .name = "ADEIC",	  .hex_b = 0b11001101 },
	{  .name = "T0IC",	  .hex_b = 0b11001110 },
	{  .name = "T1IC",	  .hex_b = 0b11001111 },
	{  .name = "ADCON",	  .hex_b = 0b11010000 },
	{  .name = "P5",	  .hex_b = 0b11010001 },
	{  .name = "TFR",	  .hex_b = 0b11010110 },
	{  .name = "WDTCON",  .hex_b = 0b11010111 },
	{  .name = "S0CON",	  .hex_b = 0b11011000 },
	{  .name = "SSCCON",  .hex_b = 0b11011001 },
	{  .name = "S1CON",	  .hex_b = 0b11011100 },
	{  .name = "P2",	  .hex_b = 0b11100000 },
	{  .name = "DP2",	  .hex_b = 0b11100001 },
	{  .name = "P3",	  .hex_b = 0b11100010 },
	{  .name = "DP3",	  .hex_b = 0b11100011 },
	{  .name = "P4",	  .hex_b = 0b11100100 },
	{  .name = "DP4",	  .hex_b = 0b11100101 },
	{  .name = "P6",	  .hex_b = 0b11100110 },
	{  .name = "DP6",	  .hex_b = 0b11100111 },
	{  .name = "P7",	  .hex_b = 0b11101000 },
	{  .name = "DP7",	  .hex_b = 0b11101001 },
	{  .name = "P8",	  .hex_b = 0b11101010 },
	{  .name = "DP8",	  .hex_b = 0b11101011 },
#if 1
	{  .name = "r10",     .hex_b = 0b11110000 }, 	// 0   0000
	{  .name = "rh0",     .hex_b = 0b11110001 },	// 1   0001
	{  .name = "rl1",     .hex_b = 0b11110010 },	// 2   0010
	{  .name = "rh1",     .hex_b = 0b11110011 },	// 3   0011
	{  .name = "r12",     .hex_b = 0b11110100 },	// 4   0100
	{  .name = "rh2",     .hex_b = 0b11110101 },	// 5   0101
	{  .name = "r13",     .hex_b = 0b11110110 },	// 6   0110
	{  .name = "rh3",     .hex_b = 0b11110111 },	// 7   0111
	{  .name = "r14",     .hex_b = 0b11111000 },	// 8   1000
	{  .name = "rh4",     .hex_b = 0b11111001 },	// 9   1001
	{  .name = "rl5",     .hex_b = 0b11111010 },	// A   1010
	{  .name = "rh5",     .hex_b = 0b11111011 },	// B   1011
	{  .name = "rl6",     .hex_b = 0b11111100 },	// C   1100
	{  .name = "rh6",     .hex_b = 0b11111101 },	// D   1101
	{  .name = "rl7",     .hex_b = 0b11111110 },	// E   1110
	{  .name = "rh7",     .hex_b = 0b11111111 },	// F   1111
#else
	{  .name = "R0",	  .hex_b = 0b11110000 },
	{  .name = "R1",	  .hex_b = 0b11110001 },
	{  .name = "R2",	  .hex_b = 0b11110010 },
	{  .name = "R3",	  .hex_b = 0b11110011 },
	{  .name = "R4",	  .hex_b = 0b11110100 },
	{  .name = "R5",	  .hex_b = 0b11110101 },
	{  .name = "R6",	  .hex_b = 0b11110110 },
	{  .name = "R7",	  .hex_b = 0b11110111 },
	{  .name = "R8",	  .hex_b = 0b11111000 },
	{  .name = "R9",	  .hex_b = 0b11111001 },
	{  .name = "R10",	  .hex_b = 0b11111010 },
	{  .name = "R11",	  .hex_b = 0b11111011 },
	{  .name = "R12",	  .hex_b = 0b11111100 },
	{  .name = "R13",	  .hex_b = 0b11111101 },
	{  .name = "R14",	  .hex_b = 0b11111110 },
	{  .name = "R15",	  .hex_b = 0b11111111 },
#endif
	{  .name = 0, 0 },
};

/* Special Function Register Storage fields */
SFR_ENTRY SFR_Entries[] =
{
	{ .name="ADCIC",    .hex_w=0xFF98, .hex_b=0xCC, .desc="A/D Converter End of Conversion Interrupt Control Register" }, 
	{ .name="ADCON",    .hex_w=0xFFA0, .hex_b=0xD0, .desc="A/D Converter Control Register" }, 
	{ .name="ADDAT",    .hex_w=0xFEA0, .hex_b=0x50, .desc="A/D Converter Result Register" }, 
	{ .name="ADDAT2",   .hex_w=0xF0A0, .hex_b=0x50, .desc="A/D Converter 2 Result Register" }, 
	{ .name="ADDRSEL1", .hex_w=0xFE18, .hex_b=0x0C, .desc="Address Select Register 1" }, 
	{ .name="ADDRSEL2", .hex_w=0xFE1A, .hex_b=0x0D, .desc="Address Select Register 2" }, 
	{ .name="ADDRSEL3", .hex_w=0xFE1C, .hex_b=0x0E, .desc="Address Select Register 3" }, 
	{ .name="ADDRSEL4", .hex_w=0xFE1E, .hex_b=0x0F, .desc="Address Select Register 4" }, 
	{ .name="ADEIC",    .hex_w=0xFF9A, .hex_b=0xCD, .desc="A/D Converter Overrun Error Interrupt Control Register" }, 
	{ .name="BUSCON0",  .hex_w=0xFF0C, .hex_b=0x86, .desc="Bus Configuration Register 0" }, 
	{ .name="BUSCON1",  .hex_w=0xFF14, .hex_b=0x8A, .desc="Bus Configuration Register 1" }, 
	{ .name="BUSCON2",  .hex_w=0xFF16, .hex_b=0x8B, .desc="Bus Configuration Register 2" }, 
	{ .name="BUSCON3",  .hex_w=0xFF18, .hex_b=0x8C, .desc="Bus Configuration Register 3" }, 
	{ .name="BUSCON4",  .hex_w=0xFF1A, .hex_b=0x8D, .desc="Bus Configuration Register 4" }, 
	{ .name="CAPREL",   .hex_w=0xFE4A, .hex_b=0x25, .desc="GPT2 Capture/Reload Register" }, 
	{ .name="CC0",      .hex_w=0xFE80, .hex_b=0x40, .desc="CAPCOM Register 0" }, 
	{ .name="CC0IC",    .hex_w=0xFF78, .hex_b=0xBC, .desc="CAPCOM Register 0 Interrupt Control Register" }, 
	{ .name="CC1",      .hex_w=0xFE82, .hex_b=0x41, .desc="CAPCOM Register 1" }, 
	{ .name="CC1IC",    .hex_w=0xFF7A, .hex_b=0xBD, .desc="CAPCOM Register 1 Interrupt Control Register" }, 
	{ .name="CC2",      .hex_w=0xFE84, .hex_b=0x42, .desc="CAPCOM Register 2" }, 
	{ .name="CC2IC",    .hex_w=0xFF7C, .hex_b=0xBE, .desc="CAPCOM Register 2 Interrupt Control Register" }, 
	{ .name="CC3",      .hex_w=0xFE86, .hex_b=0x43, .desc="CAPCOM Register 3" }, 
	{ .name="CC3IC",    .hex_w=0xFF7E, .hex_b=0xBF, .desc="CAPCOM Register 3 Interrupt Control Register" }, 
	{ .name="CC4",      .hex_w=0xFE88, .hex_b=0x44, .desc="CAPCOM Register 4" }, 
	{ .name="CC4IC",    .hex_w=0xFF80, .hex_b=0xC0, .desc="CAPCOM Register 4 Interrupt Control Register" }, 
	{ .name="CC5",      .hex_w=0xFE8A, .hex_b=0x45, .desc="CAPCOM Register 5" }, 
	{ .name="CC5IC",    .hex_w=0xFF82, .hex_b=0xC1, .desc="CAPCOM Register 5 Interrupt Control Register" }, 
	{ .name="CC6",      .hex_w=0xFE8C, .hex_b=0x46, .desc="CAPCOM Register 6" }, 
	{ .name="CC6IC",    .hex_w=0xFF84, .hex_b=0xC2, .desc="CAPCOM Register 6 Interrupt Control Register" }, 
	{ .name="CC7",      .hex_w=0xFE8E, .hex_b=0x47, .desc="CAPCOM Register 7" }, 
	{ .name="CC7IC",    .hex_w=0xFF86, .hex_b=0xC3, .desc="CAPCOM Register 7 Interrupt Control Register" }, 
	{ .name="CC8",      .hex_w=0xFE90, .hex_b=0x48, .desc="CAPCOM Register 8" }, 
	{ .name="CC8IC",    .hex_w=0xFF88, .hex_b=0xC4, .desc="CAPCOM Register 8 Interrupt Control Register" }, 
	{ .name="CC9",      .hex_w=0xFE92, .hex_b=0x49, .desc="CAPCOM Register 9" }, 
	{ .name="CC9IC",    .hex_w=0xFF8A, .hex_b=0xC5, .desc="CAPCOM Register 9 Interrupt Control Register" }, 
	{ .name="CC10",     .hex_w=0xFE94, .hex_b=0x4A, .desc="CAPCOM Register 10" }, 
	{ .name="CC10IC",   .hex_w=0xFF8C, .hex_b=0xC6, .desc="CAPCOM Register 10 Interrupt Control Register" }, 
	{ .name="CC11",     .hex_w=0xFE96, .hex_b=0x4B, .desc="CAPCOM Register 11" }, 
	{ .name="CC11IC",   .hex_w=0xFF8E, .hex_b=0xC7, .desc="CAPCOM Register 11 Interrupt Control Register" }, 
	{ .name="CC12",     .hex_w=0xFE98, .hex_b=0x4C, .desc="CAPCOM Register 12" }, 
	{ .name="CC12IC",   .hex_w=0xFF90, .hex_b=0xC8, .desc="CAPCOM Register 12 Interrupt Control Register" }, 
	{ .name="CC13",     .hex_w=0xFE9A, .hex_b=0x4D, .desc="CAPCOM Register 13" }, 
	{ .name="CC13IC",   .hex_w=0xFF92, .hex_b=0xC9, .desc="CAPCOM Register 13 Interrupt Control Register" }, 
	{ .name="CC14",     .hex_w=0xFE9C, .hex_b=0x4E, .desc="CAPCOM Register 14" }, 
	{ .name="CC14IC",   .hex_w=0xFF94, .hex_b=0xCA, .desc="CAPCOM Register 14 Interrupt Control Register" }, 
	{ .name="CC15",     .hex_w=0xFE9E, .hex_b=0x4F, .desc="CAPCOM Register 15" }, 
	{ .name="CC15IC",   .hex_w=0xFF96, .hex_b=0xCB, .desc="CAPCOM Register 15 Interrupt Control Register" }, 
	{ .name="CC16",     .hex_w=0xFE60, .hex_b=0x30, .desc="CAPCOM Register 16" }, 
	{ .name="CC16IC",   .hex_w=0xF160, .hex_b=0xB0, .desc="CAPCOM Register 16 Interrupt Control Register" }, 
	{ .name="CC17",     .hex_w=0xFE62, .hex_b=0x31, .desc="CAPCOM Register 17" }, 
	{ .name="CC17IC",   .hex_w=0xF162, .hex_b=0xB1, .desc="CAPCOM Register 17 Interrupt Control Register" }, 
	{ .name="CC18",     .hex_w=0xFE64, .hex_b=0x32, .desc="CAPCOM Register 18" }, 
	{ .name="CC18IC",   .hex_w=0xF164, .hex_b=0xB2, .desc="CAPCOM Register 18 Interrupt Control Register" }, 
	{ .name="CC19",     .hex_w=0xFE66, .hex_b=0x33, .desc="CAPCOM Register 19" }, 
	{ .name="CC19IC",   .hex_w=0xF166, .hex_b=0xB3, .desc="CAPCOM Register 19 Interrupt Control Register" }, 
	{ .name="CC20",     .hex_w=0xFE68, .hex_b=0x34, .desc="CAPCOM Register 20" }, 
	{ .name="CC20IC",   .hex_w=0xF168, .hex_b=0xB4, .desc="CAPCOM Register 20 Interrupt Control Register" }, 
	{ .name="CC21",     .hex_w=0xFE6A, .hex_b=0x35, .desc="CAPCOM Register 21" }, 
	{ .name="CC21IC",   .hex_w=0xF16A, .hex_b=0xB5, .desc="CAPCOM Register 21 Interrupt Control Register" }, 
	{ .name="CC22",     .hex_w=0xFE6C, .hex_b=0x36, .desc="CAPCOM Register 22" }, 
	{ .name="CC22IC",   .hex_w=0xF16C, .hex_b=0xB6, .desc="CAPCOM Register 22 Interrupt Control Register" }, 
	{ .name="CC23",     .hex_w=0xFE6E, .hex_b=0x37, .desc="CAPCOM Register 23" }, 
	{ .name="CC23IC",   .hex_w=0xF16E, .hex_b=0xB7, .desc="CAPCOM Register 23 Interrupt Control Register" }, 
	{ .name="CC24",     .hex_w=0xFE70, .hex_b=0x38, .desc="CAPCOM Register 24" }, 
	{ .name="CC24IC",   .hex_w=0xF170, .hex_b=0xB8, .desc="CAPCOM Register 24 Interrupt Control Register" }, 
	{ .name="CC25",     .hex_w=0xFE72, .hex_b=0x39, .desc="CAPCOM Register 25" }, 
	{ .name="CC25IC",   .hex_w=0xF172, .hex_b=0xB9, .desc="CAPCOM Register 25 Interrupt Control Register" }, 
	{ .name="CC26",     .hex_w=0xFE74, .hex_b=0x3A, .desc="CAPCOM Register 26" }, 
	{ .name="CC26IC",   .hex_w=0xF174, .hex_b=0xBA, .desc="CAPCOM Register 26 Interrupt Control Register" }, 
	{ .name="CC27",     .hex_w=0xFE76, .hex_b=0x3B, .desc="CAPCOM Register 27" }, 
	{ .name="CC27IC",   .hex_w=0xF176, .hex_b=0xBB, .desc="CAPCOM Register 27 Interrupt Control Register" }, 
	{ .name="CC28",     .hex_w=0xFE78, .hex_b=0x3C, .desc="CAPCOM Register 28" }, 
	{ .name="CC28IC",   .hex_w=0xF178, .hex_b=0xBC, .desc="CAPCOM Register 28 Interrupt Control Register" }, 
	{ .name="CC29",     .hex_w=0xFE7A, .hex_b=0x3D, .desc="CAPCOM Register 29" }, 
	{ .name="CC29IC",   .hex_w=0xF184, .hex_b=0xC2, .desc="CAPCOM Register 29 Interrupt Control Register" }, 
	{ .name="CC30",     .hex_w=0xFE7C, .hex_b=0x3E, .desc="CAPCOM Register 30" }, 
	{ .name="CC30IC",   .hex_w=0xF18C, .hex_b=0xC6, .desc="CAPCOM Register 30 Interrupt Control Register" }, 
	{ .name="CC31",     .hex_w=0xFE7E, .hex_b=0x3F, .desc="CAPCOM Register 31" }, 
	{ .name="CC31IC",   .hex_w=0xF194, .hex_b=0xCA, .desc="CAPCOM Register 31 Interrupt Control Register" }, 
	{ .name="CCM0",     .hex_w=0xFF52, .hex_b=0xA9, .desc="CAPCOM Mode Control Register 0" }, 
	{ .name="CCM1",     .hex_w=0xFF54, .hex_b=0xAA, .desc="CAPCOM Mode Control Register 1" }, 
	{ .name="CCM2",     .hex_w=0xFF56, .hex_b=0xAB, .desc="CAPCOM Mode Control Register 2" }, 
	{ .name="CCM3",     .hex_w=0xFF58, .hex_b=0xAC, .desc="CAPCOM Mode Control Register 3" }, 
	{ .name="CCM4",     .hex_w=0xFF22, .hex_b=0x91, .desc="CAPCOM Mode Control Register 4" }, 
	{ .name="CCM5",     .hex_w=0xFF24, .hex_b=0x92, .desc="CAPCOM Mode Control Register 5" }, 
	{ .name="CCM6",     .hex_w=0xFF26, .hex_b=0x93, .desc="CAPCOM Mode Control Register 6" }, 
	{ .name="CCM7",     .hex_w=0xFF28, .hex_b=0x94, .desc="CAPCOM Mode Control Register 7" }, 
	{ .name="CP",       .hex_w=0xFE10, .hex_b=0x08, .desc="CPU Context Pointer Register FC00" }, 
	{ .name="CRIC",     .hex_w=0xFF6A, .hex_b=0xB5, .desc="GPT2 CAPREL Interrupt Control Register" }, 
	{ .name="CSP",      .hex_w=0xFE08, .hex_b=0x04, .desc="CPU Code Segment Pointer Register (read only)" }, 
	{ .name="DP0L",     .hex_w=0xF100, .hex_b=0x80, .desc="P0L Direction Control Register" }, 
	{ .name="DP0H",     .hex_w=0xF102, .hex_b=0x81, .desc="P0H Direction Control Register" }, 
	{ .name="DP1L",     .hex_w=0xF104, .hex_b=0x82, .desc="P1L Direction Control Register" }, 
	{ .name="DP1H",     .hex_w=0xF106, .hex_b=0x83, .desc="P1H Direction Control Register" }, 
	{ .name="DP2",      .hex_w=0xFFC2, .hex_b=0xE1, .desc="Port 2 Direction Control Register" }, 
	{ .name="DP3",      .hex_w=0xFFC6, .hex_b=0xE3, .desc="Port 3 Direction Control Register" }, 
	{ .name="DP4",      .hex_w=0xFFCA, .hex_b=0xE5, .desc="Port 4 Direction Control Register" }, 
	{ .name="DP6",      .hex_w=0xFFCE, .hex_b=0xE7, .desc="Port 6 Direction Control Register" }, 
	{ .name="DP7",      .hex_w=0xFFD2, .hex_b=0xE9, .desc="Port 7 Direction Control Register" }, 
	{ .name="DP8",      .hex_w=0xFFD6, .hex_b=0xEB, .desc="Port 8 Direction Control Register" }, 
	{ .name="DPP0",     .hex_w=0xFE00, .hex_b=0x00, .desc="CPU Data Page Pointer 0 Register (10 bits)" }, 
	{ .name="DPP1",     .hex_w=0xFE02, .hex_b=0x01, .desc="CPU Data Page Pointer 1 Register (10 bits)" }, 
	{ .name="DPP2",     .hex_w=0xFE04, .hex_b=0x02, .desc="CPU Data Page Pointer 2 Register (10 bits)" }, 
	{ .name="DPP3",     .hex_w=0xFE06, .hex_b=0x03, .desc="CPU Data Page Pointer 3 Register (10 bits)" }, 
	{ .name="EXICON",   .hex_w=0xF1C0, .hex_b=0xE0, .desc="External Interrupt Control Register" }, 
	{ .name="MDC",      .hex_w=0xFF0E, .hex_b=0x87, .desc="CPU Multiply Divide Control Register" }, 
	{ .name="MDH",      .hex_w=0xFE0C, .hex_b=0x06, .desc="CPU Multiply Divide Register – High Word" }, 
	{ .name="MDL",      .hex_w=0xFE0E, .hex_b=0x07, .desc="CPU Multiply Divide Register – Low Word" }, 
	{ .name="ODP2",     .hex_w=0xF1C2, .hex_b=0xE1, .desc="Port 2 Open Drain Control Register" }, 
	{ .name="ODP3",     .hex_w=0xF1C6, .hex_b=0xE3, .desc="Port 3 Open Drain Control Register" }, 
	{ .name="ODP6",     .hex_w=0xF1CE, .hex_b=0xE7, .desc="Port 6 Open Drain Control Register" }, 
	{ .name="ODP7",     .hex_w=0xF1D2, .hex_b=0xE9, .desc="Port 7 Open Drain Control Register" }, 
	{ .name="ODP8",     .hex_w=0xF1D6, .hex_b=0xEB, .desc="Port 8 Open Drain Control Register" }, 
	{ .name="ONES",     .hex_w=0xFF1E, .hex_b=0x8F, .desc="Constant Value 1’s Register (read only)" }, 
	{ .name="P0L",      .hex_w=0xFF00, .hex_b=0x80, .desc="Port 0 Low Register (Lower half of PORT0)" }, 
	{ .name="P0H",      .hex_w=0xFF02, .hex_b=0x81, .desc="Port 0 High Register (Upper half of PORT0)" }, 
	{ .name="P1L",      .hex_w=0xFF04, .hex_b=0x82, .desc="Port 1 Low Register (Lower half of PORT1)" }, 
	{ .name="P1H",      .hex_w=0xFF06, .hex_b=0x83, .desc="Port 1 High Register (Upper half of PORT1)" }, 
	{ .name="P2",       .hex_w=0xFFC0, .hex_b=0xE0, .desc="Port 2 Register" }, 
	{ .name="P3",       .hex_w=0xFFC4, .hex_b=0xE2, .desc="Port 3 Register" }, 
	{ .name="P4",       .hex_w=0xFFC8, .hex_b=0xE4, .desc="Port 4 Register (8 bits)" }, 
	{ .name="P5",       .hex_w=0xFFA2, .hex_b=0xD1, .desc="Port 5 Register (read only)" }, 
	{ .name="P6",       .hex_w=0xFFCC, .hex_b=0xE6, .desc="Port 6 Register (8 bits)" }, 
	{ .name="P7",       .hex_w=0xFFD0, .hex_b=0xE8, .desc="Port 7 Register (8 bits)" }, 
	{ .name="P8",       .hex_w=0xFFD4, .hex_b=0xEA, .desc="Port 8 Register (8 bits)" }, 
	{ .name="PECC0",    .hex_w=0xFEC0, .hex_b=0x60, .desc="PEC Channel 0 Control Register" }, 
	{ .name="PECC1",    .hex_w=0xFEC2, .hex_b=0x61, .desc="PEC Channel 1 Control Register" }, 
	{ .name="PECC2",    .hex_w=0xFEC4, .hex_b=0x62, .desc="PEC Channel 2 Control Register" }, 
	{ .name="PECC3",    .hex_w=0xFEC6, .hex_b=0x63, .desc="PEC Channel 3 Control Register" }, 
	{ .name="PECC4",    .hex_w=0xFEC8, .hex_b=0x64, .desc="PEC Channel 4 Control Register" }, 
	{ .name="PECC5",    .hex_w=0xFECA, .hex_b=0x65, .desc="PEC Channel 5 Control Register" }, 
	{ .name="PECC6",    .hex_w=0xFECC, .hex_b=0x66, .desc="PEC Channel 6 Control Register" }, 
	{ .name="PECC7",    .hex_w=0xFECE, .hex_b=0x67, .desc="PEC Channel 7 Control Register" }, 
	{ .name="PICON",    .hex_w=0xF1C4, .hex_b=0xE2, .desc="Port Input Threshold Control Register" }, 
	{ .name="PP0",      .hex_w=0xF038, .hex_b=0x1C, .desc="PWM Module Period Register 0" }, 
	{ .name="PP1",      .hex_w=0xF03A, .hex_b=0x1D, .desc="PWM Module Period Register 1" }, 
	{ .name="PP2",      .hex_w=0xF03C, .hex_b=0x1E, .desc="PWM Module Period Register 2" }, 
	{ .name="PP3",      .hex_w=0xF03E, .hex_b=0x1F, .desc="PWM Module Period Register 3" }, 
	{ .name="PSW",      .hex_w=0xFF10, .hex_b=0x88, .desc="CPU Program Status Word" }, 
	{ .name="PT0",      .hex_w=0xF030, .hex_b=0x18, .desc="PWM Module Up/Down Counter 0" }, 
	{ .name="PT1",      .hex_w=0xF032, .hex_b=0x19, .desc="PWM Module Up/Down Counter 1" }, 
	{ .name="PT2",      .hex_w=0xF034, .hex_b=0x1A, .desc="PWM Module Up/Down Counter 2" }, 
	{ .name="PT3",      .hex_w=0xF036, .hex_b=0x1B, .desc="PWM Module Up/Down Counter 3" }, 
	{ .name="PW0",      .hex_w=0xFE30, .hex_b=0x18, .desc="PWM Module Pulse Width Register 0" }, 
	{ .name="PW1",      .hex_w=0xFE32, .hex_b=0x19, .desc="PWM Module Pulse Width Register 1" }, 
	{ .name="PW2",      .hex_w=0xFE34, .hex_b=0x1A, .desc="PWM Module Pulse Width Register 2" }, 
	{ .name="PW3",      .hex_w=0xFE36, .hex_b=0x1B, .desc="PWM Module Pulse Width Register 3" }, 
	{ .name="PWMCON0",  .hex_w=0xFF30, .hex_b=0x98, .desc="PWM Module Control Register 0" }, 
	{ .name="PWMCON1",  .hex_w=0xFF32, .hex_b=0x99, .desc="PWM Module Control Register 1" }, 
	{ .name="PWMIC",    .hex_w=0xF17E, .hex_b=0xBF, .desc="PWM Module Interrupt Control Register" }, 
	{ .name="RP0H",     .hex_w=0xF108, .hex_b=0x84, .desc="System Startup Configuration Register (Rd. only)" }, 
	{ .name="S0BG",     .hex_w=0xFEB4, .hex_b=0x5A, .desc="Serial Channel 0 Baud Rate Generator Reload Register" }, 
	{ .name="S0CON",    .hex_w=0xFFB0, .hex_b=0xD8, .desc="Serial Channel 0 Control Register" }, 
	{ .name="S0EIC",    .hex_w=0xFF70, .hex_b=0xB8, .desc="Serial Channel 0 Error Interrupt Control Register" }, 
	{ .name="S0RBUF",   .hex_w=0xFEB2, .hex_b=0x59, .desc="Serial Channel 0 Receive Buffer Register (read only)" }, 
	{ .name="S0RIC",    .hex_w=0xFF6E, .hex_b=0xB7, .desc="Serial Channel 0 Receive Interrupt Control Register" }, 
	{ .name="S0TBIC",   .hex_w=0xF19C, .hex_b=0xCE, .desc="Serial Channel 0 Transmit Buffer Interrupt Control Register" }, 
	{ .name="S0TBUF",   .hex_w=0xFEB0, .hex_b=0x58, .desc="Serial Channel 0 Transmit Buffer Register (write only)" }, 
	{ .name="S0TIC",    .hex_w=0xFF6C, .hex_b=0xB6, .desc="Serial Channel 0 Transmit Interrupt Control Register" }, 
	{ .name="SP",       .hex_w=0xFE12, .hex_b=0x09, .desc="CPU System Stack Pointer Register" }, 
	{ .name="SSCBR",    .hex_w=0xF0B4, .hex_b=0x5A, .desc="SSC Baudrate Register" }, 
	{ .name="SSCCON",   .hex_w=0xFFB2, .hex_b=0xD9, .desc="SSC Control Register" }, 
	{ .name="SSCEIC",   .hex_w=0xFF76, .hex_b=0xBB, .desc="SSC Error Interrupt Control Register" }, 
	{ .name="SSCRB",    .hex_w=0xF0B2, .hex_b=0x59, .desc="SSC Receive Buffer (read only)" }, 
	{ .name="SSCRIC",   .hex_w=0xFF74, .hex_b=0xBA, .desc="SSC Receive Interrupt Control Register" }, 
	{ .name="SSCTB",    .hex_w=0xF0B0, .hex_b=0x58, .desc="SSC Transmit Buffer (write only)" }, 
	{ .name="SSCTIC",   .hex_w=0xFF72, .hex_b=0xB9, .desc="SSC Transmit Interrupt Control Register" }, 
	{ .name="STKOV",    .hex_w=0xFE14, .hex_b=0x0A, .desc="CPU Stack Overflow Pointer Register" }, 
	{ .name="STKUN",    .hex_w=0xFE16, .hex_b=0x0B, .desc="CPU Stack Underflow Pointer Register" }, 
	{ .name="SYSCON",   .hex_w=0xFF12, .hex_b=0x89, .desc="CPU System Configuration Register)" }, 
	{ .name="T0",       .hex_w=0xFE50, .hex_b=0x28, .desc="CAPCOM Timer 0 Register" }, 
	{ .name="T01CON",   .hex_w=0xFF50, .hex_b=0xA8, .desc="CAPCOM Timer 0 and Timer 1 Control Register" }, 
	{ .name="T0IC",     .hex_w=0xFF9C, .hex_b=0xCE, .desc="CAPCOM Timer 0 Interrupt Control Register" }, 
	{ .name="T0REL",    .hex_w=0xFE54, .hex_b=0x2A, .desc="CAPCOM Timer 0 Reload Register" }, 
	{ .name="T1",       .hex_w=0xFE52, .hex_b=0x29, .desc="CAPCOM Timer 1 Register" }, 
	{ .name="T1IC",     .hex_w=0xFF9E, .hex_b=0xCF, .desc="CAPCOM Timer 1 Interrupt Control Register" }, 
	{ .name="T1REL",    .hex_w=0xFE56, .hex_b=0x2B, .desc="CAPCOM Timer 1 Reload Register" }, 
	{ .name="T2",       .hex_w=0xFE40, .hex_b=0x20, .desc="GPT1 Timer 2 Register" }, 
	{ .name="T2CON",    .hex_w=0xFF40, .hex_b=0xA0, .desc="GPT1 Timer 2 Control Register" }, 
	{ .name="T2IC",     .hex_w=0xFF60, .hex_b=0xB0, .desc="GPT1 Timer 2 Interrupt Control Register" }, 
	{ .name="T3",       .hex_w=0xFE42, .hex_b=0x21, .desc="GPT1 Timer 3 Register" }, 
	{ .name="T3CON",    .hex_w=0xFF42, .hex_b=0xA1, .desc="GPT1 Timer 3 Control Register" }, 
	{ .name="T3IC",     .hex_w=0xFF62, .hex_b=0xB1, .desc="GPT1 Timer 3 Interrupt Control Register" }, 
	{ .name="T4",       .hex_w=0xFE44, .hex_b=0x22, .desc="GPT1 Timer 4 Register" }, 
	{ .name="T4CON",    .hex_w=0xFF44, .hex_b=0xA2, .desc="GPT1 Timer 4 Control Register" }, 
	{ .name="T4IC",     .hex_w=0xFF64, .hex_b=0xB2, .desc="GPT1 Timer 4 Interrupt Control Register" }, 
	{ .name="T5",       .hex_w=0xFE46, .hex_b=0x23, .desc="GPT2 Timer 5 Register" }, 
	{ .name="T5CON",    .hex_w=0xFF46, .hex_b=0xA3, .desc="GPT2 Timer 5 Control Register" }, 
	{ .name="T5IC",     .hex_w=0xFF66, .hex_b=0xB3, .desc="GPT2 Timer 5 Interrupt Control Register" }, 
	{ .name="T6",       .hex_w=0xFE48, .hex_b=0x24, .desc="GPT2 Timer 6 Register" }, 
	{ .name="T6CON",    .hex_w=0xFF48, .hex_b=0xA4, .desc="GPT2 Timer 6 Control Register" }, 
	{ .name="T6IC",     .hex_w=0xFF68, .hex_b=0xB4, .desc="GPT2 Timer 6 Interrupt Control Register" }, 
	{ .name="T7",       .hex_w=0xF050, .hex_b=0x28, .desc="CAPCOM Timer 7 Register" }, 
	{ .name="T78CON",   .hex_w=0xFF20, .hex_b=0x90, .desc="CAPCOM Timer 7 and 8 Control Register" }, 
	{ .name="T7IC",     .hex_w=0xF17A, .hex_b=0xBE, .desc="CAPCOM Timer 7 Interrupt Control Register" }, 
	{ .name="T7REL",    .hex_w=0xF054, .hex_b=0x2A, .desc="CAPCOM Timer 7 Reload Register" }, 
	{ .name="T8",       .hex_w=0xF052, .hex_b=0x29, .desc="CAPCOM Timer 8 Register" }, 
	{ .name="T8IC",     .hex_w=0xF17C, .hex_b=0xBF, .desc="CAPCOM Timer 8 Interrupt Control Register" }, 
	{ .name="T8REL",    .hex_w=0xF056, .hex_b=0x2B, .desc="CAPCOM Timer 8 Reload Register" }, 
	{ .name="TFR",      .hex_w=0xFFAC, .hex_b=0xD6, .desc="Trap Flag Register" }, 
	{ .name="WDT",      .hex_w=0xFEAE, .hex_b=0x57, .desc="Watchdog Timer Register (read only)" }, 
	{ .name="WDTCON",   .hex_w=0xFFAE, .hex_b=0xD7, .desc="Watchdog Timer Control Register" }, 
	{ .name="XP0IC",    .hex_w=0xF186, .hex_b=0xC3, .desc="CAN Module Interrupt Control Register" }, 
	{ .name="XP1IC",    .hex_w=0xF18E, .hex_b=0xC7, .desc="X-Peripheral 1 Interrupt Control Register" }, 
	{ .name="XP2IC",    .hex_w=0xF196, .hex_b=0xCB, .desc="X-Peripheral 2 Interrupt Control Register" }, 
	{ .name="XP3IC",    .hex_w=0xF19E, .hex_b=0xCF, .desc="PLL Interrupt Control Register" }, 
	{ .name="ZEROS",    .hex_w=0xFF1C, .hex_b=0x8E, .desc="Constant Value 0’s Register (read only)" },
	{ 0, 0 },
};

INST inst_table_0x08[] =                                                      
{                                                                                
 { .opcode = 0x08, .len = 2, .name = "add      ", .bits = { 4,1,3 },       .fmt = "AAAA0BBB",                .argcount = 2, .args = { RW, 1, DATA3, 2 }                             }, // rXX, 0xXXXX
 { .opcode = 0x08, .len = 2, .name = "add      ", .bits = { 4,2,2 },       .fmt = "AAAA10BB",                .argcount = 2, .args = { RW, 1, RW_IND, 2  }                           },	// rXX, [rXX]
 { .opcode = 0x08, .len = 2, .name = "add      ", .bits = { 4,2,2 },       .fmt = "AAAA11BB",                .argcount = 2, .args = { RW, 1, RW_IND_POST_INC, 2 }                   },	// rXX, [rXX]+
};                                                                               
                                                                                 
INST inst_table_0x09[] =                                                         
{                                                                                
 { .opcode = 0x09, .len = 2, .name = "addb     ", .bits = { 4,1,3 },       .fmt = "AAAA0BBB",                .argcount = 2, .args = { RB, 1, DATA3, 2  }                            },
 { .opcode = 0x09, .len = 2, .name = "addb     ", .bits = { 4,2,2 },       .fmt = "AAAA10BB",                .argcount = 2, .args = { RB, 1, RW_IND, 2 }                            },
 { .opcode = 0x09, .len = 2, .name = "addb     ", .bits = { 4,2,2 },       .fmt = "AAAA11BB",                .argcount = 2, .args = { RB, 1, RW_IND_POST_INC, 2 }                   },
};                                                                               
                                                                                 
INST inst_table_0x18[] =                                                         
{                                                                                
 { .opcode = 0x18, .len = 2, .name = "addc     ", .bits = { 4,1,3 },       .fmt = "AAAA0BBB",                .argcount = 2, .args = { RW, 1, DATA3, 2 }                             },
 { .opcode = 0x18, .len = 2, .name = "addc     ", .bits = { 4,2,2 },       .fmt = "AAAA10BB",                .argcount = 2, .args = { RW, 1, RW_IND, 2 }                            },
 { .opcode = 0x18, .len = 2, .name = "addc     ", .bits = { 4,2,2 },       .fmt = "AAAA11BB",                .argcount = 2, .args = { RW, 1, RW_IND_POST_INC, 2 }                   },
};

INST inst_table_0x19[] =                                                         
{                                                                                
 { .opcode = 0x19, .len = 2, .name = "addcb    ", .bits = { 4,1,3 },       .fmt = "AAAA0BBB",                .argcount = 2, .args = { RB, 1, DATA3, 2 }                             },
 { .opcode = 0x19, .len = 2, .name = "addcb    ", .bits = { 4,2,2 },       .fmt = "AAAA10BB",                .argcount = 2, .args = { RB, 1, RW_IND, 2 }                            },
 { .opcode = 0x19, .len = 2, .name = "addcb    ", .bits = { 4,2,2 },       .fmt = "AAAA11BB",                .argcount = 2, .args = { RB, 1, RW_IND_POST_INC, 2 }                   },
};

INST inst_table_0x28[] =                                                         
{                                                                                
 { .opcode = 0x28, .len = 2, .name = "sub      ", .bits = { 4,1,3 },       .fmt = "AAAA0BBB",                .argcount = 2, .args = { RW, 1, DATA3, 2 }                             },
 { .opcode = 0x28, .len = 2, .name = "sub      ", .bits = { 4,2,2 },       .fmt = "AAAA10BB",                .argcount = 2, .args = { RW, 1, RW_IND, 2 }                            },
 { .opcode = 0x28, .len = 2, .name = "sub      ", .bits = { 4,2,2 },       .fmt = "AAAA11BB",                .argcount = 2, .args = { RW, 1, RW_IND_POST_INC, 2 }                   },
};

INST inst_table_0x29[] =                                                         
{                                                                                
 { .opcode = 0x29, .len = 2, .name = "subb     ", .bits = { 4,1,3 },       .fmt = "AAAA0BBB",                .argcount = 2, .args = { RB, 1, DATA3, 2 }                             },
 { .opcode = 0x29, .len = 2, .name = "subb     ", .bits = { 4,2,2 },       .fmt = "AAAA10BB",                .argcount = 2, .args = { RB, 1, RW_IND, 2 }                            },
 { .opcode = 0x29, .len = 2, .name = "subb     ", .bits = { 4,2,2 },       .fmt = "AAAA11BB",                .argcount = 2, .args = { RB, 1, RW_IND_POST_INC, 2 }                   },
};

INST inst_table_0x38[] =                                                         
{                                                                                
 { .opcode = 0x38, .len = 2, .name = "subc     ", .bits = { 4,1,3 },       .fmt = "AAAA0BBB",                .argcount = 2, .args = { RW, 1, DATA3, 2 }                             },
 { .opcode = 0x38, .len = 2, .name = "subc     ", .bits = { 4,2,2 },       .fmt = "AAAA10BB",                .argcount = 2, .args = { RW, 1, RW_IND, 2 }                            },
 { .opcode = 0x38, .len = 2, .name = "subc     ", .bits = { 4,2,2 },       .fmt = "AAAA11BB",                .argcount = 2, .args = { RW, 1, RW_IND_POST_INC, 2 }                   },
};

INST inst_table_0x39[] =                                                         
{                                                                                
 { .opcode = 0x39, .len = 2, .name = "subcb    ", .bits = { 4,1,3 },       .fmt = "AAAA0BBB",                .argcount = 2, .args = { RB, 1, DATA3, 2 }                             },
 { .opcode = 0x39, .len = 2, .name = "subcb    ", .bits = { 4,2,2 },       .fmt = "AAAA10BB",                .argcount = 2, .args = { RB, 1, RW_IND, 2 }                            },
 { .opcode = 0x39, .len = 2, .name = "subcb    ", .bits = { 4,2,2 },       .fmt = "AAAA11BB",                .argcount = 2, .args = { RB, 1, RW_IND_POST_INC, 2 }                   },
};

INST inst_table_0x48[] =                                                         
{                                                                                
 { .opcode = 0x48, .len = 2, .name = "cmp      ", .bits = { 4,1,3 },       .fmt = "AAAA0BBB",                .argcount = 2, .args = { RW, 1, DATA3, 2 }                             },
 { .opcode = 0x48, .len = 2, .name = "cmp      ", .bits = { 4,2,2 },       .fmt = "AAAA10BB",                .argcount = 2, .args = { RW, 1, RW_IND, 2 }                            },
 { .opcode = 0x48, .len = 2, .name = "cmp      ", .bits = { 4,2,2 },       .fmt = "AAAA11BB",                .argcount = 2, .args = { RW, 1, RW_IND_POST_INC, 2 }                   },
};

INST inst_table_0x49[] =                                                         
{                                                                                
 { .opcode = 0x49, .len = 2, .name = "cmpb     ", .bits = { 4,1,3 },       .fmt = "AAAA0BBB",                .argcount = 2, .args = { RB, 1, DATA3, 2 }                             },
 { .opcode = 0x49, .len = 2, .name = "cmpb     ", .bits = { 4,2,2 },       .fmt = "AAAA10BB",                .argcount = 2, .args = { RB, 1, RW_IND, 2 }                            },
 { .opcode = 0x49, .len = 2, .name = "cmpb     ", .bits = { 4,2,2 },       .fmt = "AAAA11BB",                .argcount = 2, .args = { RB, 1, RW_IND_POST_INC, 2 }                   },
};

INST inst_table_0x58[] =                                                         
{                                                                                
 { .opcode = 0x58, .len = 2, .name = "xor      ", .bits = { 4,1,3 },       .fmt = "AAAA0BBB",                .argcount = 2, .args = { RW, 1, DATA3, 2 }                             },
 { .opcode = 0x58, .len = 2, .name = "xor      ", .bits = { 4,2,2 },       .fmt = "AAAA10BB",                .argcount = 2, .args = { RW, 1, RW_IND, 2 }                            },
 { .opcode = 0x58, .len = 2, .name = "xor      ", .bits = { 4,2,2 },       .fmt = "AAAA11BB",                .argcount = 2, .args = { RW, 1, RW_IND_POST_INC, 2 }                   },
};

INST inst_table_0x59[] =                                                         
{                                                                                
 { .opcode = 0x59, .len = 2, .name = "xorb     ", .bits = { 4,1,3 },       .fmt = "AAAA0BBB",                .argcount = 2, .args = { RB, 1, DATA3, 2 }                             },
 { .opcode = 0x59, .len = 2, .name = "xorb     ", .bits = { 4,2,2 },       .fmt = "AAAA10BB",                .argcount = 2, .args = { RB, 1, RW_IND, 2 }                            },
 { .opcode = 0x59, .len = 2, .name = "xorb     ", .bits = { 4,2,2 },       .fmt = "AAAA11BB",                .argcount = 2, .args = { RB, 1, RW_IND_POST_INC, 2 }                   },
};

INST inst_table_0x68[] =                                                         
{                                                                                
 { .opcode = 0x68, .len = 2, .name = "and      ", .bits = { 4,1,3 },       .fmt = "AAAA0BBB",                .argcount = 2, .args = { RW, 1, DATA3, 2 }                             },
 { .opcode = 0x68, .len = 2, .name = "and      ", .bits = { 4,2,2 },       .fmt = "AAAA10BB",                .argcount = 2, .args = { RW, 1, RW_IND, 2 }                            },
 { .opcode = 0x68, .len = 2, .name = "and      ", .bits = { 4,2,2 },       .fmt = "AAAA11BB",                .argcount = 2, .args = { RW, 1, RW_IND_POST_INC, 2 }                   },
};

INST inst_table_0x69[] =                                                         
{                                                                                
 { .opcode = 0x69, .len = 2, .name = "andb     ", .bits = { 4,1,3 },       .fmt = "AAAA0BBB",                .argcount = 2, .args = { RB, 1, DATA3, 2 }                             },
 { .opcode = 0x69, .len = 2, .name = "andb     ", .bits = { 4,2,2 },       .fmt = "AAAA10BB",                .argcount = 2, .args = { RB, 1, RW_IND, 2 }                            },
 { .opcode = 0x69, .len = 2, .name = "andb     ", .bits = { 4,2,2 },       .fmt = "AAAA11BB",                .argcount = 2, .args = { RB, 1, RW_IND_POST_INC, 2 }                   },
};

INST inst_table_0x78[] =                                                         
{                                                                                
 { .opcode = 0x78, .len = 2, .name = "or       ", .bits = { 4,1,3 },       .fmt = "AAAA0BBB",                .argcount = 2, .args = { RW, 1, DATA3, 2 }                             },
 { .opcode = 0x78, .len = 2, .name = "or       ", .bits = { 4,2,2 },       .fmt = "AAAA10BB",                .argcount = 2, .args = { RW, 1, RW_IND, 2 }                            },
 { .opcode = 0x78, .len = 2, .name = "or       ", .bits = { 4,2,2 },       .fmt = "AAAA11BB",                .argcount = 2, .args = { RW, 1, RW_IND_POST_INC, 2 }                   },
};

INST inst_table_0x79[] =                                                         
{                                                                                
 { .opcode = 0x79, .len = 2, .name = "orb      ", .bits = { 4,1,3 },       .fmt = "AAAA0BBB",                .argcount = 2, .args = { RB, 1, DATA3, 2 }                             },
 { .opcode = 0x79, .len = 2, .name = "orb      ", .bits = { 4,2,2 },       .fmt = "AAAA10BB",                .argcount = 2, .args = { RB, 1, RW_IND, 2 }                            },
 { .opcode = 0x79, .len = 2, .name = "orb      ", .bits = { 4,2,2 },       .fmt = "AAAA11BB",                .argcount = 2, .args = { RB, 1, RW_IND_POST_INC, 2 }                   },
};




INST inst_table_0xb7[] =                                                         
{                                                                                
 { .opcode = 0xb7, .len = 4, .name = "srst     ", .bits = { 24 },          .fmt = "010010001011011110110111",.argcount = 0, .args = { UNDEFINED }                                   },
 { .opcode = 0xb7, .len = 4, .name = "srvwdt   ", .bits = { 24 },          .fmt = "010110001010011110100111",.argcount = 0, .args = { UNDEFINED }                                   },
};

INST inst_table_0xd1[] =                                                         
{                                                                                
 { .opcode = 0xd1, .len = 2, .name = "atomic   ", .bits = { 2,2,4 },       .fmt = "00AA0000",                .argcount = 1, .args = { IRANG2, 1 }                                   },
 { .opcode = 0xd1, .len = 2, .name = "extr     ", .bits = { 2,2,4 },       .fmt = "10AA0000",                .argcount = 1, .args = { IRANG2, 1 }                                   },
};

INST inst_table_0xd7[] =                                                         
{                                                                                
 { .opcode = 0xd7, .len = 4, .name = "exts     ", .bits = { 2,2,4,8,8 },   .fmt = "00AA0000BBBBBBBB00000000",.argcount = 2, .args = { SEG, 2, IRANG2, 1 }                           },
 { .opcode = 0xd7, .len = 4, .name = "extp     ", .bits = { 2,2,4,8,8 },   .fmt = "01AA0000BBBBBBBB00000000",.argcount = 2, .args = {  RW, 2, IRANG2, 1 }                           },
 { .opcode = 0xd7, .len = 4, .name = "extsr    ", .bits = { 2,2,4,8,8 },   .fmt = "10AA0000BBBBBBBB00000000",.argcount = 2, .args = { SEG, 2, IRANG2, 1 }                           },
 { .opcode = 0xd7, .len = 4, .name = "extpr    ", .bits = { 2,2,4,8,8 },   .fmt = "11AA0000BBBBBBBB00000000",.argcount = 2, .args = {  RW, 2, IRANG2, 1 }                           },
};

INST inst_table_0xdc[] =                                                         
{                                                                                
 { .opcode = 0xdc, .len = 2, .name = "exts     ", .bits = { 2,2,4 },       .fmt = "00AABBBB",                .argcount = 2, .args = { RW, 2, IRANG2, 1 }                            },
 { .opcode = 0xdc, .len = 2, .name = "extp     ", .bits = { 2,2,4 },       .fmt = "01AABBBB",                .argcount = 2, .args = { RW, 2, IRANG2, 1 }                            },
 { .opcode = 0xdc, .len = 2, .name = "extsr    ", .bits = { 2,2,4 },       .fmt = "10AABBBB",                .argcount = 2, .args = { RW, 2, IRANG2, 1 }                            },
 { .opcode = 0xdc, .len = 2, .name = "extpr    ", .bits = { 2,2,4 },       .fmt = "11AABBBB",                .argcount = 2, .args = { RW, 2, IRANG2, 1 }                            },
};


INST inst_set[256] = {
 // opcode,         len,      name,               .bit defs                 mnemonic template                  mnemonic types
 { .opcode = 0x00, .len = 2, .name = "add      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 1, RW, 2 }                                },
 { .opcode = 0x01, .len = 2, .name = "addb     ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RB, 1, RB, 2 }                                },
 { .opcode = 0x02, .len = 4, .name = "add      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, MEM, 2 }                             },
 { .opcode = 0x03, .len = 4, .name = "addb     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB, 1, MEM, 2 }                             },
 { .opcode = 0x04, .len = 4, .name = "add      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, REGW, 1 }                             },
 { .opcode = 0x05, .len = 4, .name = "addb     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, REGB3, 1 }                             },
 { .opcode = 0x06, .len = 4, .name = "add      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, DATA16, 2 }                          },
 { .opcode = 0x07, .len = 4, .name = "addb     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB, 1, DATA16, 2 }                          },
 { .opcode = 0x08, .len = 2, .name = "add   ...", .link = &inst_table_0x08 }, 
 { .opcode = 0x09, .len = 2, .name = "addb  ...", .link = &inst_table_0x09 }, 
 { .opcode = 0x0a, .len = 4, .name = "bfldl    ", .bits = { 8,8,8 },       .fmt = "AAAAAAAABBBBBBBBCCCCCCCC",.argcount = 3, .args = { BITOFF, 1, MASK8, 3, DATA8, 2 }               },
 { .opcode = 0x0b, .len = 2, .name = "mul      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 1, RW, 2 }                                },
 { .opcode = 0x0c, .len = 2, .name = "rol      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 1, RW, 2 }                                },
 { .opcode = 0x0d, .len = 2, .name = "jmpr     ", .bits = { 4,4 },         .fmt = "0000AAAA",                .argcount = 2, .args = { CC, 2, REL, 1 }, .addLF=1                     },
 { .opcode = 0x0e, .len = 2, .name = "bclr     ", .bits = { 4,4 },         .fmt = "0000AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 { .opcode = 0x0f, .len = 2, .name = "bset     ", .bits = { 4,4 },         .fmt = "0000AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },                                                                                              
 
 { .opcode = 0x10, .len = 2, .name = "addc     ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 1, RW, 2 }                                },
 { .opcode = 0x11, .len = 2, .name = "addcb    ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RB, 1, RB, 2 }                                },
 { .opcode = 0x12, .len = 4, .name = "addc     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, MEM, 2 }                             },
 { .opcode = 0x13, .len = 4, .name = "addcb    ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB, 1, MEM, 2 }                             },
 { .opcode = 0x14, .len = 4, .name = "addc     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, REGW, 1 }                             },
 { .opcode = 0x15, .len = 4, .name = "addcb    ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, REGB, 1 }                             },
 { .opcode = 0x16, .len = 4, .name = "addc     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, DATA16, 2 }                          },
 { .opcode = 0x17, .len = 4, .name = "addcb    ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB, 1, DATA16, 2 }                          },
 { .opcode = 0x18, .len = 2, .name = "addc  ...", .link = &inst_table_0x18 }, 
 { .opcode = 0x19, .len = 2, .name = "addcb ...", .link = &inst_table_0x19 }, 
 { .opcode = 0x1a, .len = 4, .name = "bfldh    ", .bits = { 8,8,8 },       .fmt = "AAAAAAAABBBBBBBBCCCCCCCC",.argcount = 3, .args = { BITOFF, 1, MASK8, 3, DATA8, 2 }               },
 { .opcode = 0x1b, .len = 2, .name = "mulu     ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 1, RW, 2 }                                },
 { .opcode = 0x1c, .len = 2, .name = "rol      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 2, DATA4, 1 }                             },
 { .opcode = 0x1d, .len = 2, .name = "jmpr     ", .bits = { 4,4 },         .fmt = "0001AAAA",                .argcount = 2, .args = { CC, 2, REL, 1 }, .addLF=1                     },
 { .opcode = 0x1e, .len = 2, .name = "bclr     ", .bits = { 4,4 },         .fmt = "0001AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 { .opcode = 0x1f, .len = 2, .name = "bset     ", .bits = { 4,4 },         .fmt = "0001AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },                                                                                                   
 
 { .opcode = 0x20, .len = 2, .name = "sub      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 1, RW, 2 }                                },
 { .opcode = 0x21, .len = 2, .name = "subb     ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RB, 1, RB, 2 }                                },
 { .opcode = 0x22, .len = 4, .name = "sub      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, MEM, 2 }                             },
 { .opcode = 0x23, .len = 4, .name = "subb     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB, 1, MEM, 2 }                             },
 { .opcode = 0x24, .len = 4, .name = "sub      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, REGW, 1 }                             },
 { .opcode = 0x25, .len = 4, .name = "subb     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, REGB, 1 }                             },
 { .opcode = 0x26, .len = 4, .name = "sub      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, DATA16, 2 }                          },
 { .opcode = 0x27, .len = 4, .name = "subb     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB, 1, DATA16, 2 }                          },
 { .opcode = 0x28, .len = 2, .name = "sub   ...", .link = &inst_table_0x28 }, 
 { .opcode = 0x29, .len = 2, .name = "subb  ...", .link = &inst_table_0x29 }, 
 { .opcode = 0x2a, .len = 4, .name = "bcmp     ", .bits = { 8,8,4,4 },     .fmt = "AAAAAAAABBBBBBBBCCCCDDDD",.argcount = 4, .args = { BITADR, 2, BITADR, 4, BITADR, 1, BITADR, 3 }  },
 { .opcode = 0x2b, .len = 2, .name = "prior    ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 1, RW, 2 }                                },
 { .opcode = 0x2c, .len = 2, .name = "ror      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 1, RW, 2 }                                },
 { .opcode = 0x2d, .len = 2, .name = "jmpr     ", .bits = { 4,4 },         .fmt = "0010AAAA",                .argcount = 2, .args = { CC, 2, REL, 1 }, .addLF=1                     },
 { .opcode = 0x2e, .len = 2, .name = "bclr     ", .bits = { 4,4 },         .fmt = "0010AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 { .opcode = 0x2f, .len = 2, .name = "bset     ", .bits = { 4,4 },         .fmt = "0010AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 
 { .opcode = 0x30, .len = 2, .name = "subc     ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 1, RW, 2 }                                },
 { .opcode = 0x31, .len = 2, .name = "subcb    ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RB, 1, RB, 2 }                                },
 { .opcode = 0x32, .len = 4, .name = "subc     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, MEM, 2 }                             },
 { .opcode = 0x33, .len = 4, .name = "subcb    ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB, 1, MEM, 2 }                             },
 { .opcode = 0x34, .len = 4, .name = "subc     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, REGW, 1 }                             },
 { .opcode = 0x35, .len = 4, .name = "subcb    ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, REGB, 1 }                             },
 { .opcode = 0x36, .len = 4, .name = "subc     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, DATA16, 2 }                          },
 { .opcode = 0x37, .len = 4, .name = "subcb    ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB, 1, DATA16, 2 }                          },
 { .opcode = 0x38, .len = 2, .name = "subc  ...", .link = &inst_table_0x38 }, 
 { .opcode = 0x39, .len = 2, .name = "subcb ...", .link = &inst_table_0x39 }, 
 { .opcode = 0x3a, .len = 4, .name = "bmovn    ", .bits = { 8,8,4,4 },     .fmt = "AAAAAAAABBBBBBBBCCCCDDDD",.argcount = 4, .args = { BITADR, 2, BITADR, 4, BITADR, 1, BITADR, 3 }  },
 { .opcode = 0x3b, .len = 0 },                                                                                                                
 { .opcode = 0x3c, .len = 2, .name = "ror      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 2, DATA4, 1 }                             },
 { .opcode = 0x3d, .len = 2, .name = "jmpr     ", .bits = { 4,4 },         .fmt = "0011AAAA",                .argcount = 2, .args = { CC, 2, REL, 1 }, .addLF=1                     },
 { .opcode = 0x3e, .len = 2, .name = "bclr     ", .bits = { 4,4 },         .fmt = "0011AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 { .opcode = 0x3f, .len = 2, .name = "bset     ", .bits = { 4,4 },         .fmt = "0011AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },                                                                                                   
 
 { .opcode = 0x40, .len = 2, .name = "cmp      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 1, RW, 2 }                                },
 { .opcode = 0x41, .len = 2, .name = "cmpb     ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RB, 1, RB, 2 }                                },
 { .opcode = 0x42, .len = 4, .name = "cmp      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, MEM, 2 }                             },
 { .opcode = 0x43, .len = 4, .name = "cmpb     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB, 1, MEM, 2 }                             },
 { .opcode = 0x44, .len = 0 },                                                                                                                
 { .opcode = 0x45, .len = 0 },                                                                                                                
 { .opcode = 0x46, .len = 4, .name = "cmp      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, DATA16, 2 }                          },
 { .opcode = 0x47, .len = 4, .name = "cmpb     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB, 1, DATA16, 2 }                          },
 { .opcode = 0x48, .len = 2, .name = "cmp   ...", .link = &inst_table_0x48 }, 
 { .opcode = 0x49, .len = 2, .name = "cmpb  ...", .link = &inst_table_0x49 }, 
 { .opcode = 0x4a, .len = 4, .name = "bmov     ", .bits = { 8,8,4,4 },     .fmt = "AAAAAAAABBBBBBBBCCCCDDDD",.argcount = 4, .args = { BITADR_W, 2, BITADR_W2, 4, BITADR, 1, BITADR, 3 }  },
 { .opcode = 0x4b, .len = 2, .name = "div      ", .bits = { 4,4 },         .fmt = "AAAA0000",                .argcount = 1, .args = { RW, 1 }                                       },
 { .opcode = 0x4c, .len = 2, .name = "shl      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 1, RW, 2 }                                },
 { .opcode = 0x4d, .len = 2, .name = "jmpr     ", .bits = { 4,4 },         .fmt = "0100AAAA",                .argcount = 2, .args = { CC, 2, REL, 1 }, .addLF=1                     },
 { .opcode = 0x4e, .len = 2, .name = "bclr     ", .bits = { 4,4 },         .fmt = "0100AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 { .opcode = 0x4f, .len = 2, .name = "bset     ", .bits = { 4,4 },         .fmt = "0100AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },                                                                                                   
 
 { .opcode = 0x50, .len = 2, .name = "xor      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 1, RW, 2 }                                },
 { .opcode = 0x51, .len = 2, .name = "xorb     ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RB, 1, RB, 2 }                                },
 { .opcode = 0x52, .len = 4, .name = "xor      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, MEM, 2 }                             },
 { .opcode = 0x53, .len = 4, .name = "xorb     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB, 1, MEM, 2 }                             },
 { .opcode = 0x54, .len = 4, .name = "xor      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, REGW, 1 }                             },
 { .opcode = 0x55, .len = 4, .name = "xorb     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, REGB, 1 }                             },
 { .opcode = 0x56, .len = 4, .name = "xor      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, DATA16, 2 }                          },
 { .opcode = 0x57, .len = 4, .name = "xorb     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB, 1, DATA16, 2 }                          },
 { .opcode = 0x58, .len = 2, .name = "xor   ...", .link = &inst_table_0x58 }, 
 { .opcode = 0x59, .len = 2, .name = "xorb  ...", .link = &inst_table_0x59 }, 
 { .opcode = 0x5a, .len = 4, .name = "bor      ", .bits = { 8,8,4,4 },     .fmt = "AAAAAAAABBBBBBBBCCCCDDDD",.argcount = 4, .args = { BITADR, 2, BITADR, 4, BITADR, 1, BITADR, 3 }  },
 { .opcode = 0x5b, .len = 2, .name = "divu     ", .bits = { 4,4 },         .fmt = "AAAA0000",                .argcount = 0, .args = { UNDEFINED }                                   },
 { .opcode = 0x5c, .len = 2, .name = "shl      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 2, DATA4, 1 }                             },
 { .opcode = 0x5d, .len = 2, .name = "jmpr     ", .bits = { 4,4 },         .fmt = "0101AAAA",                .argcount = 2, .args = { CC, 2, REL, 1 }, .addLF=1                     },
 { .opcode = 0x5e, .len = 2, .name = "bclr     ", .bits = { 4,4 },         .fmt = "0101AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 { .opcode = 0x5f, .len = 2, .name = "bset     ", .bits = { 4,4 },         .fmt = "0101AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   }, 
 
 { .opcode = 0x60, .len = 2, .name = "and      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 1, RW, 2 }                                },
 { .opcode = 0x61, .len = 2, .name = "andb!    ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RB, 1, RB, 2 }                                },
 { .opcode = 0x62, .len = 4, .name = "and      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, MEM, 2 }                             },
 { .opcode = 0x63, .len = 4, .name = "andb     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB, 1, MEM, 2 }                             },
 { .opcode = 0x64, .len = 4, .name = "and      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, REGW, 1 }                             },
 { .opcode = 0x65, .len = 4, .name = "andb     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, REGB, 1 }                             },
 { .opcode = 0x66, .len = 4, .name = "and      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, DATA16, 2 }                          },
 { .opcode = 0x67, .len = 4, .name = "andb     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB, 1, DATA16, 2 }                          },
 { .opcode = 0x68, .len = 2, .name = "and   ...", .link = &inst_table_0x68 }, 
 { .opcode = 0x69, .len = 2, .name = "andb  ...", .link = &inst_table_0x69 }, 
 { .opcode = 0x6a, .len = 4, .name = "band     ", .bits = { 8,8,4,4 },     .fmt = "AAAAAAAABBBBBBBBCCCCDDDD",.argcount = 4, .args = { BITADR, 2, BITADR, 4, BITADR, 1, BITADR, 3 }  },
 { .opcode = 0x6b, .len = 2, .name = "divl     ", .bits = { 4,4 },         .fmt = "AAAA0000",                .argcount = 1, .args = { RW, 1 }                                       },
 { .opcode = 0x6c, .len = 2, .name = "shr      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 1, RW, 2 }                                },
 { .opcode = 0x6d, .len = 2, .name = "jmpr     ", .bits = { 4,4 },         .fmt = "0110AAAA",                .argcount = 2, .args = { CC, 2, REL, 1 }, .addLF=1                     },
 { .opcode = 0x6e, .len = 2, .name = "bclr     ", .bits = { 4,4 },         .fmt = "0110AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 { .opcode = 0x6f, .len = 2, .name = "bset     ", .bits = { 4,4 },         .fmt = "0110AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },                                                                                                   
 
 { .opcode = 0x70, .len = 2, .name = "or       ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 1, RW, 2 }                                },
 { .opcode = 0x71, .len = 2, .name = "orb      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RB, 1, RB, 2 }                                },
 { .opcode = 0x72, .len = 4, .name = "or       ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, MEM, 2 }                             },
 { .opcode = 0x73, .len = 4, .name = "orb      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB, 1, MEM, 2 }                             },
 { .opcode = 0x74, .len = 4, .name = "or       ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, REGW, 1 }                             },
 { .opcode = 0x75, .len = 4, .name = "orb      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, REGB, 1 }                             },
 { .opcode = 0x76, .len = 4, .name = "or       ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, DATA16, 2 }                          },
 { .opcode = 0x77, .len = 4, .name = "orb      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB, 1, DATA16, 2 }                          },
 { .opcode = 0x78, .len = 2, .name = "or    ...", .link = &inst_table_0x78 }, 
 { .opcode = 0x79, .len = 2, .name = "orb   ...", .link = &inst_table_0x79 }, 
 { .opcode = 0x7a, .len = 4, .name = "bxor     ", .bits = { 8,8,4,4 },     .fmt = "AAAAAAAABBBBBBBBCCCCDDDD",.argcount = 4, .args = { BITADR, 2, BITADR, 4, BITADR, 1, BITADR, 3 }  },
 { .opcode = 0x7b, .len = 2, .name = "divlu    ", .bits = { 4,4 },         .fmt = "AAAA0000",                .argcount = 1, .args = { RW, 1 }                                       },
 { .opcode = 0x7c, .len = 2, .name = "shr      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 2, DATA4, 1 }                             },
 { .opcode = 0x7d, .len = 2, .name = "jmpr     ", .bits = { 4,4 },         .fmt = "0111AAAA",                .argcount = 2, .args = { CC, 2, REL, 1 }, .addLF=1                     },
 { .opcode = 0x7e, .len = 2, .name = "bclr     ", .bits = { 4,4 },         .fmt = "0111AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 { .opcode = 0x7f, .len = 2, .name = "bset     ", .bits = { 4,4 },         .fmt = "0111AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },                                                                                                   
 
 { .opcode = 0x80, .len = 2, .name = "cmpi1    ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 2, DATA4, 1 }                             },
 { .opcode = 0x81, .len = 2, .name = "neg      ", .bits = { 4,4 },         .fmt = "AAAA0000",                .argcount = 1, .args = { RW, 1 }                                       },
 { .opcode = 0x82, .len = 4, .name = "cmpi1    ", .bits = { 4,4,16 },      .fmt = "1111AAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { RW, 1, MEM, 2 }                               },
 { .opcode = 0x83, .len = 0 },                                                                                          
 { .opcode = 0x84, .len = 4, .name = "mov      ", .bits = { 4,4,16 },      .fmt = "0000AAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { RW_IND, 1, MEM, 2 }                           },
 { .opcode = 0x85, .len = 0 },                                                                                           
 { .opcode = 0x86, .len = 4, .name = "cmpi1    ", .bits = { 4,4,16 },      .fmt = "1111AAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { RW, 1, DATA16, 2 }                            },
 { .opcode = 0x87, .len = 4, .name = "idle     ", .bits = { 24 },          .fmt = "011110001000011110000111",.argcount = 0, .args = { UNDEFINED }                                   },
 { .opcode = 0x88, .len = 2, .name = "mov      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW_IND_PRE_DEC, 2, RW, 1 }                    },
 { .opcode = 0x89, .len = 2, .name = "movb     ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW_IND_PRE_DEC, 2, RB, 1 }                    },
 { .opcode = 0x8a, .len = 4, .name = "jb       ", .bits = { 8,8,4,4 },     .fmt = "AAAAAAAABBBBBBBBCCCC0000",.argcount = 3, .args = { BITADR, 1, BITADR, 3, REL, 2 }, .addLF=1      },
 { .opcode = 0x8b, .len = 0 },                                                                                                                
 { .opcode = 0x8c, .len = 0 },                                                                                               
 { .opcode = 0x8d, .len = 2, .name = "jmpr     ", .bits = { 4,4 },         .fmt = "1000AAAA",                .argcount = 2, .args = { CC, 2, REL, 1 }, .addLF=1                     },
 { .opcode = 0x8e, .len = 2, .name = "bclr     ", .bits = { 4,4 },         .fmt = "1000AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 { .opcode = 0x8f, .len = 2, .name = "bset     ", .bits = { 4,4 },         .fmt = "1000AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },                                                                                                   
 
 { .opcode = 0x90, .len = 2, .name = "cmpi2    ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 2, DATA4, 1 }                             },
 { .opcode = 0x91, .len = 2, .name = "cpl      ", .bits = { 4,4 },         .fmt = "AAAA0000",                .argcount = 1, .args = { RW, 1 }                                       },
 { .opcode = 0x92, .len = 4, .name = "cmpi2    ", .bits = { 4,4,16 },      .fmt = "1111AAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { RW, 1, MEM, 2 }                               },
 { .opcode = 0x93, .len = 0 },                                                                                           
 { .opcode = 0x94, .len = 4, .name = "mov      ", .bits = { 4,4,16 },      .fmt = "0000AAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, RW_IND, 1 }                           },
 { .opcode = 0x95, .len = 0 },                                                                                          
 { .opcode = 0x96, .len = 4, .name = "cmpi2    ", .bits = { 4,4,16 },      .fmt = "1111AAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { RW, 1, DATA16, 2 }                            },
 { .opcode = 0x97, .len = 4, .name = "pwrdn    ", .bits = { 24 },          .fmt = "011010001001011110010111",.argcount = 0, .args = { UNDEFINED }                                   },
 { .opcode = 0x98, .len = 2, .name = "mov      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW_L, 1, RW_IND_POST_INC, 2 }                   },
 { .opcode = 0x99, .len = 2, .name = "movb     ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RB, 1, RW_IND_POST_INC, 2 }                   },
 { .opcode = 0x9a, .len = 4, .name = "jnb      ", .bits = { 8,8,4,4 },     .fmt = "AAAAAAAABBBBBBBBCCCC0000",.argcount = 3, .args = { BITADR, 1, BITADR, 3, REL, 2 }, .addLF=1      },
 { .opcode = 0x9b, .len = 2, .name = "trap     ", .bits = { 7,1 },         .fmt = "AAAAAAA0",                .argcount = 1, .args = { TRAP7, 1 }                                    },
 { .opcode = 0x9c, .len = 2, .name = "jmpi     ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { CC, 1, RW_IND, 2 }, .addLF=1                  },
 { .opcode = 0x9d, .len = 2, .name = "jmpr     ", .bits = { 4,4 },         .fmt = "1001AAAA",                .argcount = 2, .args = { CC, 2, REL, 1 }, .addLF=1                     },
 { .opcode = 0x9e, .len = 2, .name = "bclr     ", .bits = { 4,4 },         .fmt = "1001AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 { .opcode = 0x9f, .len = 2, .name = "bset     ", .bits = { 4,4 },         .fmt = "1001AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 
 { .opcode = 0xa0, .len = 2, .name = "cmpd1    ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 2, DATA4, 1 }                             },
 { .opcode = 0xa1, .len = 2, .name = "negb     ", .bits = { 4,4 },         .fmt = "AAAA0000",                .argcount = 1, .args = { RB, 1 }                                       },
 { .opcode = 0xa2, .len = 4, .name = "cmpd1    ", .bits = { 4,4,16 },      .fmt = "1111AAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { RW, 1, MEM, 2 }                               },
 { .opcode = 0xa3, .len = 0 },                                                                                          
 { .opcode = 0xa4, .len = 4, .name = "movb     ", .bits = { 4,4,16 },      .fmt = "0000AAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { RW_IND, 1, MEM, 2 }                           },
 { .opcode = 0xa5, .len = 4, .name = "diswdt   ", .bits = { 24 },          .fmt = "010110101010010110100101",.argcount = 0, .args = { UNDEFINED }                                   },
 { .opcode = 0xa6, .len = 4, .name = "cmpd1    ", .bits = { 4,4,16 },      .fmt = "1111AAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { RW, 1, DATA16, 2 }                            },
 { .opcode = 0xa7, .len = 0 },                                                                                           
 { .opcode = 0xa8, .len = 2, .name = "mov      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 1, RW_IND, 2 }                            },
 { .opcode = 0xa9, .len = 2, .name = "movb     ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RB, 1,RW_IND, 2 }                             },
 { .opcode = 0xaa, .len = 4, .name = "jbc      ", .bits = { 8,8,4,4 },     .fmt = "AAAAAAAABBBBBBBBCCCC0000",.argcount = 3, .args = { BITADR, 1, BITADR, 3, REL, 2 }, .addLF=1      },
 { .opcode = 0xab, .len = 2, .name = "calli    ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { CC, 1, RW_IND, 2 }, .addLF=1                  },
 { .opcode = 0xac, .len = 2, .name = "ashr     ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 1, RW, 2 }                                },
 { .opcode = 0xad, .len = 2, .name = "jmpr     ", .bits = { 4,4 },         .fmt = "1010AAAA",                .argcount = 2, .args = { CC, 2, REL, 1 }, .addLF=1                     },
 { .opcode = 0xae, .len = 2, .name = "bclr     ", .bits = { 4,4 },         .fmt = "1010AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 { .opcode = 0xaf, .len = 2, .name = "bset     ", .bits = { 4,4 },         .fmt = "1010AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },                                                                                                   
 
 { .opcode = 0xb0, .len = 2, .name = "cmpd2    ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 2, DATA4, 1 }                             },
 { .opcode = 0xb1, .len = 2, .name = "cpl      ", .bits = { 4,4 },         .fmt = "AAAA0000",                .argcount = 1, .args = { RB, 1 }                                       },
 { .opcode = 0xb2, .len = 4, .name = "cmpd2    ", .bits = { 4,4,16 },      .fmt = "1111AAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { RW, 1, MEM, 2 }                               },
 { .opcode = 0xb3, .len = 0 },                                                                                           
 { .opcode = 0xb4, .len = 4, .name = "movb     ", .bits = { 4,4,16 },      .fmt = "0000AAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, RW_IND, 1 }                           },
 { .opcode = 0xb5, .len = 4, .name = "einit    ", .bits = { 24 },          .fmt = "010010101011010110110101",.argcount = 0, .args = { UNDEFINED }                                   },
 { .opcode = 0xb6, .len = 4, .name = "cmpd2    ", .bits = { 4,4,16 },      .fmt = "1111AAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { RW, 1, DATA16, 2 }                            },
 { .opcode = 0xb7, .len = 4, .name = "srXX  ...", .link = &inst_table_0xb7 },
 { .opcode = 0xb8, .len = 2, .name = "mov      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW_IND, 2, RW, 1 }                            },
 { .opcode = 0xb9, .len = 2, .name = "movb     ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW_IND, 2, RB, 1 }                            },
 { .opcode = 0xba, .len = 4, .name = "jnbs     ", .bits = { 8,8,4,4 },     .fmt = "AAAAAAAABBBBBBBBCCCC0000",.argcount = 2, .args = { BITADR, 1, BITADR, 3, REL, 2 }, .addLF=1      },
 { .opcode = 0xbb, .len = 2, .name = "callr    ", .bits = { 8 },           .fmt = "AAAAAAAA",                .argcount = 1, .args = { REL, 1 }, .addLF=1                            },
 { .opcode = 0xbc, .len = 2, .name = "ashr     ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 2, DATA4, 1 }                             },
 { .opcode = 0xbd, .len = 2, .name = "jmpr     ", .bits = { 4,4 },         .fmt = "1011AAAA",                .argcount = 2, .args = { CC, 2,  REL, 1 }, .addLF=1                    },
 { .opcode = 0xbe, .len = 2, .name = "bclr     ", .bits = { 4,4 },         .fmt = "1011AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 { .opcode = 0xbf, .len = 2, .name = "bset     ", .bits = { 4,4 },         .fmt = "1011AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 
 { .opcode = 0xc0, .len = 2, .name = "movbz    ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 2, RB, 1 }                                },
 { .opcode = 0xc1, .len = 0 },                                                                                                                
 { .opcode = 0xc2, .len = 4, .name = "movbz    ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB2, 1, MEM, 2 }                             },
 { .opcode = 0xc3, .len = 0 },                                                                                           
 { .opcode = 0xc4, .len = 4, .name = "mov      ", .bits = { 4,4,16 },      .fmt = "AAAABBBBCCCCCCCCCCCCCCCC",.argcount = 2, .args = { RW_IND_1_DATA16_2, 2, RW, 1 }                    },
 { .opcode = 0xc5, .len = 4, .name = "movbz    ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, REGB, 1 }                             },
 { .opcode = 0xc6, .len = 4, .name = "scxt     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, DATA16, 2 }                          },
 { .opcode = 0xc7, .len = 0 },                                                                                                                
 { .opcode = 0xc8, .len = 2, .name = "mov      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW_IND, 1, RW_IND, 2 }                        },
 { .opcode = 0xc9, .len = 2, .name = "movb     ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW_IND, 1, RW_IND, 2 }                        },
 { .opcode = 0xca, .len = 4, .name = "calla    ", .bits = { 4,4,16 },      .fmt = "AAAA0000BBBBBBBBBBBBBBBB",.argcount = 2, .args = { CC, 1, ADR, 2 }, .addLF=1                     },
 { .opcode = 0xcb, .len = 2, .name = "ret      ", .bits = { 8 },           .fmt = "00000000",                .argcount = 0, .args = { UNDEFINED }, .addLF=1                         },
 { .opcode = 0xcc, .len = 2, .name = "nop      ", .bits = { 8 },           .fmt = "00000000",                .argcount = 0, .args = { UNDEFINED }                                   },
 { .opcode = 0xcd, .len = 2, .name = "jmpr     ", .bits = { 4,4 },         .fmt = "1100AAAA",                .argcount = 2, .args = { CC, 2, REL, 1 }, .addLF=1                     },
 { .opcode = 0xce, .len = 2, .name = "bclr     ", .bits = { 4,4 },         .fmt = "1100AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 { .opcode = 0xcf, .len = 2, .name = "bset     ", .bits = { 4,4 },         .fmt = "1100AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },                                                                                                   
 
 { .opcode = 0xd0, .len = 2, .name = "movbs    ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 2, RB, 1 }                                },
 { .opcode = 0xd1, .len = 2, .name = "atomic...", .link = &inst_table_0xd1 }, 
 { .opcode = 0xd2, .len = 4, .name = "movbs    ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB2, 1, MEM, 2 }                             },
 { .opcode = 0xd3, .len = 0 },                                                                                           
 { .opcode = 0xd4, .len = 4, .name = "mov      ", .bits = { 4,4,16 },      .fmt = "AAAABBBBCCCCCCCCCCCCCCCC",.argcount = 2, .args = { RW, 1, RW_IND_1_DATA16_2, 2 }                    },
 { .opcode = 0xd5, .len = 4, .name = "movbs    ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, REGB, 1 }                             },
 { .opcode = 0xd6, .len = 4, .name = "scxt     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, MEM, 2 }                             },
 { .opcode = 0xd7, .len = 4, .name = "extx  ...", .link = &inst_table_0xd7 }, 
 { .opcode = 0xd8, .len = 2, .name = "mov      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW_IND_POST_INC, 1, RW_IND, 2 }               },
 { .opcode = 0xd9, .len = 2, .name = "movb     ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW_IND_POST_INC, 1, RW_IND, 2 }               },
 { .opcode = 0xda, .len = 4, .name = "calls    ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { SEG, 1, ADR, 2 }, .addLF=1                    },
 { .opcode = 0xdb, .len = 2, .name = "rets     ", .bits = { 8 },           .fmt = "00000000",                .argcount = 0, .args = { UNDEFINED }, .addLF=1                         },
 { .opcode = 0xdc, .len = 2, .name = "extx  ...", .link = &inst_table_0xdc }, 
 { .opcode = 0xdd, .len = 2, .name = "jmpr     ", .bits = { 4,4 },         .fmt = "1101AAAA",                .argcount = 2, .args = { CC, 2, REL, 1 }, .addLF=1                     },
 { .opcode = 0xde, .len = 2, .name = "bclr     ", .bits = { 4,4 },         .fmt = "1101AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 { .opcode = 0xdf, .len = 2, .name = "bset     ", .bits = { 4,4 },         .fmt = "1101AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 
 { .opcode = 0xe0, .len = 2, .name = "mov      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW, 2, DATA4, 1 }                             },
 { .opcode = 0xe1, .len = 2, .name = "movb     ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RB, 2, DATA4, 1 }                             },
 { .opcode = 0xe2, .len = 4, .name = "pcall    ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, ADR, 2 }, .addLF=1                   },
 { .opcode = 0xe3, .len = 0 },                                                                                                                
 { .opcode = 0xe4, .len = 4, .name = "movb     ", .bits = { 4,4,16 },      .fmt = "AAAABBBBCCCCCCCCCCCCCCCC",.argcount = 2, .args = { RW_IND_1_DATA16_2, 2, RB, 1 }                    },
 { .opcode = 0xe5, .len = 0 },                                                                                                                
 { .opcode = 0xe6, .len = 4, .name = "mov      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, DATA16, 2 }                          },
 { .opcode = 0xe7, .len = 4, .name = "movb     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB, 1, DATA16, 2 }                          },
 { .opcode = 0xe8, .len = 2, .name = "mov      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW_IND, 1, RW_IND_POST_INC, 2 }               },
 { .opcode = 0xe9, .len = 2, .name = "movb     ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW_IND, 1, RW_IND_POST_INC, 2 }               },
 { .opcode = 0xea, .len = 4, .name = "jmpa     ", .bits = { 4,4,16 },      .fmt = "AAAA0000BBBBBBBBBBBBBBBB",.argcount = 2, .args = { CC, 1, ADR, 2 }, .addLF=1                     },
 { .opcode = 0xeb, .len = 2, .name = "retp     ", .bits = { 8 },           .fmt = "AAAAAAAA",                .argcount = 1, .args = { REGW, 1 }, .addLF=1                           },
 { .opcode = 0xec, .len = 2, .name = "push     ", .bits = { 8 },           .fmt = "AAAAAAAA",                .argcount = 1, .args = { REGW, 1 }                                     },
 { .opcode = 0xed, .len = 2, .name = "jmpr     ", .bits = { 4,4 },         .fmt = "1110AAAA",                .argcount = 2, .args = { CC, 2, REL, 1 }, .addLF=1                     },
 { .opcode = 0xee, .len = 2, .name = "bclr     ", .bits = { 4,4 },         .fmt = "1110AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 { .opcode = 0xef, .len = 2, .name = "bset     ", .bits = { 4,4 },         .fmt = "1110AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },                                                                                                   
 
 { .opcode = 0xf0, .len = 2, .name = "mov      ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RW_L, 1, RW, 2 }                              },
 { .opcode = 0xf1, .len = 2, .name = "movb     ", .bits = { 4,4 },         .fmt = "AAAABBBB",                .argcount = 2, .args = { RB, 1, RB, 2 }                                },
 { .opcode = 0xf2, .len = 4, .name = "mov      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGW, 1, MEM, 2 }                             },
 { .opcode = 0xf3, .len = 4, .name = "movb     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { REGB, 1, MEM, 2 }                             },
 { .opcode = 0xf4, .len = 4, .name = "movb     ", .bits = { 4,4,16 },      .fmt = "AAAABBBBCCCCCCCCCCCCCCCC",.argcount = 2, .args = { RB, 1, RW_IND_1_DATA16_2, 2 }                 },
 { .opcode = 0xf5, .len = 0 },                                                                                                                
 { .opcode = 0xf6, .len = 4, .name = "mov      ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, REGW, 1 }                             },
 { .opcode = 0xf7, .len = 4, .name = "movb     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { MEM, 2, REGB, 1 }                             },
 { .opcode = 0xf8, .len = 0 },                                                                                                                
 { .opcode = 0xf9, .len = 0 },                                                                                                                
 { .opcode = 0xfa, .len = 4, .name = "jmps     ", .bits = { 8,16 },        .fmt = "AAAAAAAABBBBBBBBBBBBBBBB",.argcount = 2, .args = { SEG, 1, ADR, 2 }, .addLF=1                    },
 { .opcode = 0xfb, .len = 2, .name = "reti     ", .bits = { 8 },           .fmt = "10001000",                .argcount = 0, .args = { UNDEFINED }, .addLF=1                         },
 { .opcode = 0xfc, .len = 2, .name = "pop      ", .bits = { 4,4 },         .fmt = "AAAAAAAA",                .argcount = 1, .args = { REGW, 1 }                                     },
 { .opcode = 0xfd, .len = 2, .name = "jmpr     ", .bits = { 4,4 },         .fmt = "1111AAAA",                .argcount = 2, .args = { CC, 2, REL, 1 }, .addLF=1                     },
 { .opcode = 0xfe, .len = 2, .name = "bclr     ", .bits = { 4,4 },         .fmt = "1111AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   },
 { .opcode = 0xff, .len = 2, .name = "bset     ", .bits = { 4,4 },         .fmt = "1111AAAA",                .argcount = 1, .args = { BITADR, 1 }                                   }

};

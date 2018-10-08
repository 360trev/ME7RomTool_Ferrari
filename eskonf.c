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
#include "eskonf.h"

extern int show_diss;
extern unsigned dpp1_value;

ESKCONF_TABLE eskconf_left_bank[] = {
	{	// [0]
	.s1 = "EV4   ", .s1_pin = "M52", .s1_desc = "Cylinder 6 injector control power output",
	.s2 = "EV3   ", .s2_pin = "M03", .s2_desc = "Cylinder 8 injector control power output",
	.s3 = "EV2   ", .s3_pin = "M35", .s3_desc = "Cylinder 7 injector control power output",
	.s4 = "EV1   ", .s4_pin = "M19", .s4_desc = "Cylinder 5 injector control power output"
	},
	{	// [1]
	.s1 = "LSHVK1", .s1_pin = "M34", .s1_desc = "LH rear Lambda sensor heater (duty cycle) Power output ",
	.s2 = "xxxx  ", .s2_pin = "M21", .s2_desc = "Not Used",
	.s3 = "TEV   ", .s3_pin = "M05", .s3_desc = "Control for LH canister purge valve (duty cycle) Power output",
	.s4 = "MIL   ", .s4_pin = "F46", .s4_desc = "Not Used"
	},
	{	// [2]
	.s1 = "EKP   ", .s1_pin = "F30", .s1_desc = "Fuel pump control Digital output ",
	.s2 = "LUE1  ", .s2_pin = "F50", .s2_desc = "Not Used",
	.s3 = "LSHVK2", .s3_pin = "M02", .s3_desc = "Not Used",
	.s4 = "MIL   ", .s4_pin = "F02", .s4_desc = "Not Used"
	},
	{	// [3]
	.s1 = "--    ", .s1_pin = "Fxx", .s1_desc = "Not Used",
	.s2 = "--    ", .s2_pin = "Fxx", .s2_desc = "Not Used",
	.s3 = "KOS   ", .s3_pin = "F13", .s3_desc = "Not Used",
	.s4 = "LUE2  ", .s4_pin = "F62", .s4_desc = "Not Used"
	},
	{	// [4]
	.s1 = "xxxx  ", .s1_pin = "M53", .s1_desc = "Not Used",
	.s2 = "SU1   ", .s2_pin = "M04", .s2_desc = "Not Used",
	.s3 = "NWS   ", .s3_pin = "M36", .s3_desc = "Not Used",
	.s4 = "xxxx  ", .s4_pin = "M20", .s4_desc = "Control for LH exhaust by-pass power output"
	},
	{	// [5]
	.s1 = "xxxx  ", .s1_pin = "F18", .s1_desc = "Not Used",
	.s2 = "xxxx  ", .s2_pin = "F33", .s2_desc = "Not Used",
	.s3 = "xxxx  ", .s3_pin = "F34", .s3_desc = "Not Used",
	.s4 = "xxxx  ", .s4_pin = "F01", .s4_desc = "Not Used"
	},
	{	// [6]
	.s1 = "xxxx  ", .s1_pin = "M13", .s1_desc = "Not Used",
	.s2 = "xxxx  ", .s2_pin = "M13", .s2_desc = "Not Used",
	.s3 = "xxxx  ", .s3_pin = "M45", .s3_desc = "Not Used",
	.s4 = "xxxx  ", .s4_pin = "M45", .s4_desc = "Not Used"
	}	
};

ESKCONF_TABLE eskconf_right_bank[] = {
	{	// [0]
	.s1 = "EV4   ", .s1_pin = "M52", .s1_desc = "Cylinder 2 injector control power output",
	.s2 = "EV3   ", .s2_pin = "M03", .s2_desc = "Cylinder 4 injector control power output",
	.s3 = "EV2   ", .s3_pin = "M35", .s3_desc = "Cylinder 3 injector control power output",
	.s4 = "EV1   ", .s4_pin = "M19", .s4_desc = "Cylinder 1 injector control power output"
	},
	{	// [1]
	.s1 = "LSHVK1", .s1_pin = "M34", .s1_desc = "RH rear Lambda sensor heater (duty cycle) power output ",
	.s2 = "xxxx  ", .s2_pin = "M21", .s2_desc = "Not Used",
	.s3 = "TEV   ", .s3_pin = "M05", .s3_desc = "Control for RH canister purge valve (duty cycle) power output",
	.s4 = "MIL   ", .s4_pin = "F46", .s4_desc = "Not Used"
	},
	{	// [2]
	.s1 = "EKP   ", .s1_pin = "F30", .s1_desc = "Fuel pump control digital output ",
	.s2 = "LUE1  ", .s2_pin = "F50", .s2_desc = "Not Used",
	.s3 = "LSHVK2", .s3_pin = "M02", .s3_desc = "Not Used",
	.s4 = "MIL   ", .s4_pin = "F02", .s4_desc = "Not Used"
	},
	{	// [3]
	.s1 = "--    ", .s1_pin = "Fxx", .s1_desc = "Not Used",
	.s2 = "--    ", .s2_pin = "Fxx", .s2_desc = "Not Used",
	.s3 = "KOS   ", .s3_pin = "F13", .s3_desc = "A/C compressor control digital output",
	.s4 = "LUE2  ", .s4_pin = "F62", .s4_desc = "Secondary air pump control digital output"
	},
	{	// [4]
	.s1 = "xxxx  ", .s1_pin = "M53", .s1_desc = "Modular manifolds control power output ",
	.s2 = "SU1   ", .s2_pin = "M04", .s2_desc = "Compensation throttle control power output ",
	.s3 = "NWS   ", .s3_pin = "M36", .s3_desc = "Timing variator control  Digital output",
	.s4 = "xxxx  ", .s4_pin = "M20", .s4_desc = "Control for RH exhaust by-pass power output"
	},
	{	// [5]
	.s1 = "xxxx  ", .s1_pin = "F18", .s1_desc = "Canister closing control power output",
	.s2 = "xxxx  ", .s2_pin = "F33", .s2_desc = "Not Used",
	.s3 = "xxxx  ", .s3_pin = "F34", .s3_desc = "Secondary air valve control digital output ",
	.s4 = "xxxx  ", .s4_pin = "F01", .s4_desc = "Not Used"
	},
	{	// [6]
	.s1 = "xxxx  ", .s1_pin = "M13", .s1_desc = "Not Used",
	.s2 = "xxxx  ", .s2_pin = "M13", .s2_desc = "Not Used",
	.s3 = "xxxx  ", .s3_pin = "M45", .s3_desc = "Not Used",
	.s4 = "xxxx  ", .s4_pin = "M45", .s4_desc = "Not Used"
	}	
};


void dump_bits(char *dst, int val, int numbits)
{
    int i, mask = (1 << numbits);
	for(i=0;i<4;i++)
	{
		mask >>= 1;
		*dst++ = (val & mask) ? '1':'0';
		mask >>= 1;
		*dst++ = (val & mask) ? '1':'0';
		*dst++ = ' ';
		*dst++ = ' ';
		*dst++ = ' ';
		*dst++ = ' ';
		*dst++ = ' ';
	}
	*dst = 0;
}

void print_eskonf_byte( ESKCONF_TABLE *p, int idx, unsigned char ch)
{
	char s_bin[64] = { "0 0 0 0 0 0 0 0" };
	dump_bits(s_bin, ch, 8);
	
	
	printf("          | %-6.6s %-6.6s %-6.6s %-6.6s\n", p->s1, p->s2, p->s3, p->s4);
	printf("[%d] 0x%2.2x  | %s\n", idx, ch, s_bin);
	printf("          | %-6.6s %-6.6s %-6.6s %-6.6s\n", p->s1_pin, p->s2_pin, p->s3_pin, p->s4_pin);
	printf("          +----------------------------------------------------------------------\n");
	printf("          | %s   %s\n", p->s1_pin, p->s1_desc);
	printf("          | %s   %s\n", p->s2_pin, p->s2_desc);
	printf("          | %s   %s\n", p->s3_pin, p->s3_desc);
	printf("          | %s   %s\n", p->s4_pin, p->s4_desc);
	printf("----------+----------------------------------------------------------------------\n");
}

int check_eskonf(ImageHandle *fh, int skip)
{
	int found = 0, byte_offset;
	char *rom_load_addr = fh->d.p;
	char *addr;
	int seg,val,i;
	char *eskonf_l_adr;
	char *eskonf_r_adr;
	int val_adr, val_l_adr, val_r_adr;
	ESKCONF_TABLE *left  = &eskconf_left_bank;
	ESKCONF_TABLE *right = &eskconf_right_bank;
	
	/*
	 * 
	 * search: *** Find ESKCONF Value by searching for correct function code byte sequence ***
	 */
	if(skip == 0) return found;		

	printf("\n-[ ESKONF Configuration of power stage (actuators) ]-------------------------------------------\n\n");
	printf(">>> Scanning for ESKONF Lookup code sequence... \n");
	byte_offset    = search_offset( rom_load_addr, fh->len, (unsigned char *)&needle_ESKONF, (unsigned char *)&mask_ESKONF, needle_ESKONF_len);
	if(byte_offset != NULL) 
	{
		addr = rom_load_addr + byte_offset;
		
		printf("\nfound needle at offset=%#x\n",(int)(byte_offset));
		// try to determine information from rom scan
		seg           = dpp1_value-1;	// get16((unsigned char *)addr+6);

		// disassemble needle found in rom
		if(show_diss) { c167x_diss(byte_offset, rom_load_addr + byte_offset, needle_ESKONF_len+16); }

		val            = get16((unsigned char *)addr+10+0);		// ESKONF_R
		eskonf_r_adr   = seg*SEGMENT_SIZE;
		eskonf_r_adr  += val;
		val_r_adr      = (unsigned long)eskonf_r_adr;			// derive phyiscal address from offset and segment
		val_r_adr     &= ~(ROM_1MB_MASK);						// convert physical address to a rom file offset we can easily work with.

		val            = get16((unsigned char *)addr+28+0);		// ESKONF_L
		eskonf_l_adr   = seg*SEGMENT_SIZE;
		eskonf_l_adr  += val;
		val_l_adr      = (unsigned long)eskonf_l_adr;			// derive phyiscal address from offset and segment
		val_l_adr     &= ~(ROM_1MB_MASK);						// convert physical address to a rom file offset we can easily work with.

		printf("\n");

		printf(" 1. Configuration of output stages\n");
		printf(" =================================\n");
		printf(" The configuration is made with the Label ESKONF_R (right bank) & ESKONF_L (left bank), each by 7 bytes.\n\n");
		
		printf(" Every byte is standing for 4 output stages. Therefore every output stage has got 2 consecutive\n");
		printf(" configuration Bits.\n\n");

		printf(" Enable of the output stages diagnosis\n");
		printf(" -------------------------------------\n");
		printf(" With the configurations-Bytes in ESKONF the functions have to be set active / inactive depending\n");
		printf(" on the available components in the car. At the same time with the 2 Bits the function of the \n");
		printf(" diagnosis is set.\n\n");
		
		printf(" Assignment of the Bit pattern:\n");
		printf(" ------------------------------\n");
		printf(" 00  Diagnosis active with OBDII-malfunction storage with test of healing\n");
		printf(" 01  Diagnosis active without OBDII-malfunction storage with test of healing\n");
		printf(" 10  Diagnosis active without OBDII-fault memory without test of healing (EKP)\n");
		printf(" 11  Diagnosis not active\n\n");

		printf("\nESKONF_L @ ADR:%#x (offset %#x) - Left Bank Configuration\n", eskonf_l_adr, val_l_adr);
		printf("----------+----------------------------------------------------------------------\n");
		printf("[i] Hex   |           Bit\n");
		printf("          | 76     54     32     10\n");
		printf("----------+----------------------------------------------------------------------\n");
		for(i=0;i<7;i++) { 
			print_eskonf_byte( &left[i], i, *(unsigned char *)(rom_load_addr + val_l_adr + i));
		}


		printf("\n\nESKONF_R @ ADR:%#x (offset %#x) - Right Bank Configuration\n", eskonf_r_adr, val_r_adr);
		printf("----------+----------------------------------------------------------------------\n");
		printf("[i] Hex   |           Bit\n");
		printf("          | 76     54     32     10\n");
		printf("----------+----------------------------------------------------------------------\n");
		for(i=0;i<7;i++) { 
			print_eskonf_byte( &right[i], i, *(unsigned char *)(rom_load_addr + val_r_adr + i));
		}
		
		printf("\n");

	} else {
		printf("Not found\n");
	}

	return found;
}


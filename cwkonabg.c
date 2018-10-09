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
#include "cwkonabg.h"
#include "needles.h"
#include "utils.h"

extern unsigned dpp1_value;
extern int show_diss;

BITFMT_TABLE cwkonabg_fmt = {
	.name = "CWKONABG:",		// formatting template 
	.b7 = "b_katfz", 		.b7_desc = "Condition bit for Catalyzer fitted in vehicle",
	.b6 = "b_kath", 		.b6_desc = "Condition bit for Catalyzer heater installed in vehicle", 
	.b5 = "b_slsfz", 		.b5_desc = "Condition bit for Secondary Air Injection System (SLS) fitted in vehicle", 
	.b4 = " ", 				.b4_desc = " ", 
	.b3 = " ", 				.b3_desc = " ", 
	.b2 = " ", 				.b2_desc = " ", 
	.b1 = " ", 				.b1_desc = " ", 
	.b0 = " ", 				.b0_desc = " "
};

int check_cwkonabg(ImageHandle *fh, int skip)
{
	char s_bin[64] = { "0 0 0 0 0 0 0 0" };
	unsigned char *cwkonabg_adr;
	unsigned long val_adr,addr;
	unsigned char cwkonabg;
	unsigned int val;
	unsigned char *rom_load_addr = fh->d.p;

	int found = 0;
	/*
	 * search: *** Find CWKONABG Codeword by searching for correct function code byte sequence ***
	 */
	if(skip == 0) return found;		

	printf("\n>>> Scanning for CWKONABG [Codeword for configuration of Exhaust emission treatment]\n");
	addr = search( fh, (unsigned char *)&needle_CWKONABG, (unsigned char *)&mask_CWKONABG, needle_CWKONABG_len, 0 );
	if(addr != NULL) {
		found = 1;
		printf("\nfound at offset=0x%x ",(int)(addr-(int)rom_load_addr) );
		val = get16((unsigned char *)addr+10);
		cwkonabg_adr  = ((dpp1_value-1)*SEGMENT_SIZE);
		cwkonabg_adr += val;
		val_adr       = (unsigned long)cwkonabg_adr;		// derive phyiscal address from offset and segment
		val_adr      &= ~(ROM_1MB_MASK);					// convert physical address to a rom file offset we can easily work with.
					
		printf("CWKONABG @ ADR:%#x\n\n", cwkonabg_adr );
		if(show_diss) { 
			printf("Dumping ...\n");
			c167x_diss(addr-(int)rom_load_addr, addr, needle_CWKONFZ1_len+4); 
		}
		cwkonabg = (unsigned char)*(rom_load_addr+val_adr);
		dump_bin(s_bin, cwkonabg, 8 );							// create formatted binary string from byte of cwkonabg
		dump_bitfmt_table(&cwkonabg_fmt, cwkonabg, s_bin);		// show it correctly formatted using template
	}

	if(found == 0) { printf("Sequence not found\n"); }
	return found;
}


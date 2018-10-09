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
#include "cwkonfz1.h"

extern unsigned dpp1_value;
extern int show_diss;

BITFMT_TABLE cwkonfz1_fmt = {
	.name = "CWKONFZ1:",		// formatting template 
	.b7 = "b_autget", .b7_desc = "Condition automatic gearbox", 
	.b6 = "b_mt",     .b6_desc = "Condition manual gearbox", 
	.b5 = "b_cvt",    .b5_desc = "Condition continously variable transmission", 
	.b4 = "b_f1getr", .b4_desc = "Condition F1-gearbox (electronic clutch control)", 
	.b3 = "",         .b3_desc = "Condition not defined for Ferrari", 
	.b2 = "",         .b2_desc = "Condition not defined for Ferrari", 
	.b1 = "b_asrfz",  .b1_desc = "Condition for ASR in the automobile", 
	.b0 = "b_4wd",    .b0_desc = "Condition 4 wheel drive"
};

int check_cwkonfz(ImageHandle *fh, int skip)
{
	char s_bin[64] = { "0 0 0 0 0 0 0 0" };
	unsigned char *cwkonfz1_adr;
	unsigned long val_adr,addr;
	unsigned char cwkonfz1;
	unsigned int val;
	unsigned char *rom_load_addr = fh->d.p;

	int found = 0;
	/*
	 * search: *** Find CWKONFZ Codeword by searching for correct function code byte sequence ***
	 */
	if(skip == 0) return found;		

	printf("\n>>> Scanning for CWKONFZ1 [Codeword for configuration of vehicle]\n");
	addr = search( fh, (unsigned char *)&needle_CWKONFZ1, (unsigned char *)&mask_CWKONFZ1, needle_CWKONFZ1_len, 0 );
	if(addr != NULL) {
		found = 1;
		printf("\nfound at offset=0x%x ",(int)(addr-(int)rom_load_addr) );
		val = get16((unsigned char *)addr+2);
		cwkonfz1_adr  = ((dpp1_value-1)*SEGMENT_SIZE);
		cwkonfz1_adr += val;
		val_adr       = (unsigned long)cwkonfz1_adr;		// derive phyiscal address from offset and segment
		val_adr      &= ~(ROM_1MB_MASK);					// convert physical address to a rom file offset we can easily work with.
					
		printf("CWKONFZ1 @ ADR:%#x\n\n", cwkonfz1_adr );
		if(show_diss) { 
			printf("Dumping ...\n");
			c167x_diss(addr-(int)rom_load_addr, addr, needle_CWKONFZ1_len+4+32); 
		}
					
		cwkonfz1 = (unsigned char)*(rom_load_addr+val_adr);
		dump_bin(s_bin, cwkonfz1, 8 );							// create formatted binary string from byte of cwkonfz1
		dump_bitfmt_table(&cwkonfz1_fmt, cwkonfz1, s_bin);		// show it correctly formatted using template

		printf("This ROM is configured for : ");
		if(s_bin[8] == '1') { 
			printf("F1-AMT Gearbox with a Transmission Control Unit.\n"); 
		} else {
			printf("H-Gate Manual gearbox.\n"); 
		}
	}

	if(found == 0) { printf("Sequence not found\n"); }
	return found;
}


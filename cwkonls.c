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
#include "cwkonls.h"
#include "cwkonfz1.h"
#include "needles.h"
#include "utils.h"

extern unsigned dpp1_value;
extern int show_diss;

BITFMT_TABLE cwkonls_fmt = {
	.name = "CWKONLS:",		// formatting template 
	.b7 = "b_lsv", 		.b7_desc = "(Bank 1) Condition 1. Lambda sensor installed upstream of cat downstream of outlet", 
	.b6 = "b_lsh",     	.b6_desc = "(Bank 1) Condition 2. Lambda sensor installed downstream of cat downstream of outlet", 
	.b5 = "b_ls3",    	.b5_desc = "(Bank 1) Condition 3. Lambda sensor installed downstream of outlet", 
	.b4 = "b_ls4", 		.b4_desc = "(Bank 1) Condition 4. Lambda sensor installed downstream of outlet", 
	.b3 = "b_lsv2",     .b3_desc = "(Bank 2) Condition 1. Lambda sensor installed upstream of cat downstream of outlet", 
	.b2 = "b_lsh2",     .b2_desc = "(Bank 2) Condition 2. Lambda sensor installed downstream of cat downstream of outlet", 
	.b1 = "b_ls32", 	.b1_desc = "(Bank 2) Condition 3. Lambda sensor installed downstream of outlet", 
	.b0 = "b_ls42", 	.b0_desc = "(Bank 2) Condition 4. Lambda sensor installed downstream of outlet"
};

int check_cwkonls(ImageHandle *fh, int skip)
{
	char s_bin[64] = { "0 0 0 0 0 0 0 0" };
	unsigned char *cwkonls_adr;
	unsigned long val_adr,addr;
	unsigned char cwkonls;
	unsigned int val;
	unsigned char *rom_load_addr = fh->d.p;

	int found = 0;
	/*
	 * search: *** Find CWKONFZ Codeword by searching for correct function code byte sequence ***
	 */
	if(skip == 0) return found;		

	printf("\n>>> Scanning for CWKONLS [Codeword for configuration of Lambda sensors]\n");
	addr = search( fh, (unsigned char *)&needle_CWKONFZ1, (unsigned char *)&mask_CWKONFZ1, needle_CWKONFZ1_len, 0 );
	if(addr != NULL) {
		found = 1;
		printf("\nfound at offset=0x%x ",(int)(addr-(int)rom_load_addr) );
		val = get16((unsigned char *)addr+92);
		cwkonls_adr  = ((dpp1_value-1)*SEGMENT_SIZE);
		cwkonls_adr += val;
		val_adr       = (unsigned long)cwkonls_adr;		// derive phyiscal address from offset and segment
		val_adr      &= ~(ROM_1MB_MASK);					// convert physical address to a rom file offset we can easily work with.
					
		printf("CWKONLS @ ADR:%#x\n\n", cwkonls_adr );
		if(show_diss) { 
			printf("Dumping ...\n");
			c167x_diss(addr-(int)rom_load_addr, addr, needle_CWKONFZ1_len+4); 
		}
		cwkonls = (unsigned char)*(rom_load_addr+val_adr);
		dump_bin(s_bin, cwkonls, 8 );							// create formatted binary string from byte of cwkonls
		dump_bitfmt_table(&cwkonls_fmt, cwkonls, s_bin);		// show it correctly formatted using template
	}

	if(found == 0) { printf("Sequence not found\n"); }
	return found;
}


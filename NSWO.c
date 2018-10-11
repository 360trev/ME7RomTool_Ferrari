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
#include "nswo.h"

extern int show_diss;
extern unsigned dpp1_value;

int check_nswo(ImageHandle *fh, int skip, int mode)
{
	MPTR _nswo1, _nswo2;
	int found = 0, byte_offset;
	char *rom_load_addr = fh->d.p;
	char *addr;
	int nswo1_val, nswo2_val;
	
	/* search: *** Find NSWO1/NSWO2 Values by searching for correct function code byte sequence */
	if(skip == 0) return found;		

	printf("\n-[ NSWO1/NSWO2 engine-speed threshold 1 & 2 for switching calculation  ]---------------------\n\n");
	printf(">>> Scanning for NSWO1/NSWO2 Lookup code sequence... \n");
	byte_offset    = search_offset( rom_load_addr, fh->len, (unsigned char *)&needle_PROKON, (unsigned char *)&mask_PROKON, needle_PROKON_len);
	if(byte_offset != 0) 
	{
		addr = rom_load_addr + byte_offset;
		printf("\nfound needle at offset=%#x\n",(int)(byte_offset));
		// disassemble needle found in rom
		if(show_diss) { c167x_diss(byte_offset, rom_load_addr + byte_offset, needle_PROKON_len+16); }

		if(mode == 1) {
			translate_seg(&_nswo1, "NSWO1", rom_load_addr, dpp1_value-1 /*seg*/, get16((unsigned char *)addr+10) /*val*/);
			show_seg(&_nswo1);
			nswo1_val = *(_nswo1.ram);
			printf("NSWO1: (0x%-2.2x) %-5.1f Upm : Speed threshold 1 switching speed for calculating time savings\n", nswo1_val, (double)nswo1_val*40.0 );
		} else if(mode == 2) {
			translate_seg(&_nswo2, "NSWO2", rom_load_addr, dpp1_value-1 /*seg*/, get16((unsigned char *)addr+30) /*val*/);
			show_seg(&_nswo2);
			nswo2_val = *(_nswo2.ram);
			printf("NSWO2: (0x%-2.2x) %-5.1f Upm : Speed threshold 2 switching speed for calculating time savings\n", nswo2_val, (double)nswo2_val*40.0 );
		}
	} else {
		printf("Not found\n");
	}

	return found;
}


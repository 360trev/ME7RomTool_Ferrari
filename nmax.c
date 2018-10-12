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
#include "nmax.h"
#include "table_spec.h"
#include "show_tables.h"

extern int show_diss;
extern unsigned dpp1_value;

int check_nmax(ImageHandle *fh, int skip)
{
	MPTR _nmax;
	int found = 0, byte_offset;
	char *rom_load_addr = fh->d.p;
	char *addr;
	unsigned int nmax;
	if(skip == 0) return found;		

	printf("\n-[ NMAX Rev limiter]-----------------\n\n");
	printf(">>> Scanning for NMAX Lookup code sequence... \n");
	/* search: *** Find NMAX Value by searching for correct function code byte sequence */
	byte_offset    = search_offset( rom_load_addr, fh->len, (unsigned char *)&needle_DFFTCNV, (unsigned char *)&mask_DFFTCNV, needle_DFFTCNV_len);
	if(byte_offset != 0) 
	{
		addr = rom_load_addr + byte_offset;
		printf("\nfound needle at offset=%#x\n",(int)(byte_offset));

		// disassemble needle found in rom
		if(show_diss) { c167x_diss(byte_offset, rom_load_addr + byte_offset, needle_DFFTCNV_len+16); }

		translate_seg(&_nmax, "NMAX", rom_load_addr, dpp1_value-1 /*seg*/, get16((unsigned char *)addr+30) /*val*/);
		show_seg(&_nmax);
		
		printf("\nNMAX:  %-4.4d rpm (u/min) limit\n", (get16(_nmax.ram))/4);
		printf("NMAXH:  %d rpm (u/min) hysteresis for hard speed limitation\n", (get16(_nmax.ram-2))/4);
	}

	return found;
}


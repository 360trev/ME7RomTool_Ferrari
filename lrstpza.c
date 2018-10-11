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
#include "lrstpza.h"
#include "table_spec.h"
#include "show_tables.h"

extern unsigned dpp1_value;
extern int show_diss;

int check_lrstpza(ImageHandle *fh, int skip)
{
	MPTR _lrstpza;
	int found = 0, val;
	unsigned char *addr;
	unsigned char *rom_load_addr = fh->d.p;

	if(skip == 0) return found;		
				
	printf("\n>>> Scanning for LRSTPZA [Period duration of the LRS forced amplitude]\n");
	addr = search( fh, (unsigned char *)&needle_LRSTPZA, (unsigned char *)&mask_LRSTPZA, needle_LRSTPZA_len, 0 );
	if(addr != NULL) {
		printf("found LRS() function at offset=0x%x.\n\n",(int)(addr-rom_load_addr) );

		// disassemble needle found in rom
		if(show_diss) { c167x_diss(addr-rom_load_addr, addr, needle_LRSTPZA_len+20); }

		// extract lrstpza from LRS() function in romcode using segment and offset directly from code
		translate_seg(&_lrstpza, "LRSTPZA", rom_load_addr, dpp1_value-1 /*seg*/, get16((unsigned char *)addr+4) /*val*/);
		show_seg(&_lrstpza);

		val = *(_lrstpza.ram);	// get byte and show it...
		printf("LRSTPZA: 0x%-2.2x (0.%d s)\n",val,val*100);

	} else {
		printf("not found\n");
	}

	return found;
}

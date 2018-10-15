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
#include "kfmsnwdk.h"
#include "table_spec.h"
#include "show_tables.h"

extern int show_diss;
extern unsigned dpp1_value;

int check_kfmsnwdk(ImageHandle *fh, int skip)
{
	int found = 0;
	MPTR _kfmsnwdk;
	unsigned char *addr;
	unsigned char *rom_load_addr = fh->d.p;

	if(skip == 0) return found;		

	printf("\n>>> Scanning for BGMSZS() for calculation of mass flows into intake manifold, to discover KFMSNWDK map ...\n");
	addr = search( fh, (unsigned char *)&needle_BGMSZS, (unsigned char *)&mask_BGMSZS, needle_BGMSZS_len, 0 );
	if(addr != NULL) {
		printf("\nfound at offset=0x%x \n\n",(int)(addr-(int)rom_load_addr) );
		// get offset to 'BGMSZS'...
		translate_seg(&_kfmsnwdk, "KFMSNWDK", rom_load_addr, dpp1_value-1 /*seg*/, get16((unsigned char *)addr+46) /*val*/);
		show_seg(&_kfmsnwdk); 
		if(show_diss) { 
			printf("Dumping ...\n");
			c167x_diss(addr-(int)rom_load_addr, addr, needle_BGMSZS_len+16); 
		}
		dump_table(addr, rom_load_addr, get16((unsigned char *)addr + 46), dpp1_value-1, &KFMSNWDK_table, 0);	

	} else {
		printf("\nNot found\n");
	}

	return found;
}

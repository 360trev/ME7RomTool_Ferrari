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
#include "kfsu.h"
#include "table_spec.h"
#include "show_tables.h"

extern int show_diss;
extern unsigned dpp1_value;

int check_kfsu(ImageHandle *fh, int skip, int mode)
{
	int found = 0, val, seg;
	MPTR _kfsu;
	MPTR _kfsu2;
	unsigned char *addr;
	unsigned char *rom_load_addr = fh->d.p;

	if(skip == 0) return found;		

	printf("\n>>> Scanning for SU() for Intake manifold switch-over, to discover KFSU/KFSU2 maps...\n");
	addr = search( fh, (unsigned char *)&needle_SU, (unsigned char *)&mask_SU, needle_SU_len, 0 );
	if(addr != NULL) {
		printf("\nfound at offset=0x%x\n",(int)(addr-(int)rom_load_addr) );
		if(show_diss) { 
			printf("Dumping ...\n");
			c167x_diss(addr-(int)rom_load_addr, addr, needle_SU_len+16); 
		}

		if(mode == 1) {
			val = get16((unsigned char *)addr+8);
			seg = get16((unsigned char *)addr+12) ;
			translate_seg(&_kfsu, "KFSU", rom_load_addr, seg, val);
			show_seg(&_kfsu); 
			dump_table(addr, rom_load_addr, val, seg, &KFSU_table, 0);
			
		} else if(mode == 2) {
			val = get16((unsigned char *)addr+30);
			seg = get16((unsigned char *)addr+34) ;
			translate_seg(&_kfsu2, "KFSU2", rom_load_addr, seg, val);
			show_seg(&_kfsu2); 
			dump_table(addr, rom_load_addr, val, seg, &KFSU2_table, 0);			
		}

	} else {
		printf("\nNot found\n");
	}

	return found;
}

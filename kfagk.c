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
#include "kfagk.h"
#include "table_spec.h"
#include "show_tables.h"

extern unsigned dpp1_value;
extern int show_diss;

int check_kfagk(ImageHandle *fh, int skip)
{
	unsigned char *addr;
	unsigned char *rom_load_addr = fh->d.p;
	int dump_len;
	int found = 0;
	if(skip == 0) return found;		

	printf("\n-[ Exhaust Valve KFAGK Table ]---------------------------------------------------------------------\n\n");
		
	printf(">>> Scanning for KFAGK Table #1 Checking sub-routine Variant #1 [manages exhaust valve/flap opening] \n");
	dump_len = KFAGK_needle_len+4;
	addr     = search( fh, (unsigned char *)&KFAGK_needle, (unsigned char *)&KFAGK_mask, KFAGK_needle_len, 0 );
	if(addr == NULL) {
		printf("Sequence not found\n");		
		printf("\n>>> Scanning for KFAGK Table #1 Checking sub-routine Variant #2 [manages exhaust valve/flap opening] \n");
		dump_len = KFAGK_needle2_len+4;
		addr     = search( fh, (unsigned char *)&KFAGK_needle2, (unsigned char *)&KFAGK_mask2, KFAGK_needle2_len, 0 );
	}
	
	if(addr != NULL) {
		printf("Found at offset=0x%x \n",(int)(addr-rom_load_addr) );
		// disassemble needle found in rom
		if(show_diss) { c167x_diss(addr-rom_load_addr, addr, dump_len); }
		// dump KFAGK table
		dump_table(addr, rom_load_addr, get16((unsigned char *)addr + 2), get16((unsigned char *)addr + 6), &KFAGK_table, 0);						
	} else {
		printf("Sequence not found\n");				
	}
	printf("\n\n");

	return found;
}

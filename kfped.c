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
#include "kfped.h"
#include "table_spec.h"
#include "show_tables.h"

extern int show_diss;
extern unsigned dpp1_value;

int check_kfped(ImageHandle *fh, int skip, int mode)
{
	int found = 0;
	unsigned char *addr;
	unsigned char *rom_load_addr = fh->d.p;

	if(skip == 0) return found;		

	printf("\n-[ Throttle Pedal KFPED/KFPEDR Table ]---------------------------------------------------------------------\n\n");
	printf(">>> Scanning for KFPED/KFPEDR Table #1 Checking sub-routine Variant #1 [manages throttle pedal torque requests] \n");
	addr = search( fh, (unsigned char *)&KFPED_needle, (unsigned char *)&KFPED_mask, KFPED_needle_len, 0 );
	if(addr != NULL) {
		printf("Found at offset=0x%x KFPEDR @+14, KFPED @ +36\n\n",(int)(addr-rom_load_addr));
		// disassemble needle found in rom
		if(show_diss) { c167x_diss(addr-rom_load_addr, addr, KFPED_needle_len+4); }

		if(mode == 2) {
			// dump KPEDR table
			dump_table(addr, rom_load_addr, get16((unsigned char *)addr + 14), dpp1_value-1, &KPEDR_table, 0);
		}
		if(mode == 1) {
			// dump KPED table (found table, rom, val, segment, table def)
			dump_table(addr, rom_load_addr, get16((unsigned char *)addr + 36), dpp1_value-1, &KPED_table, 0);
		}
	}
	printf("\n\n");

	return found;
}

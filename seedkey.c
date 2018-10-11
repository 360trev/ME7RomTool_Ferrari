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
#include "seedkey.h"

extern unsigned dpp1_value;
extern int show_diss;

int check_seedkey(ImageHandle *fh, int skip)
{
	char *rom_load_addr = fh->d.p;
	char *addr;
	int found = 0;
	if(skip == 0) return found;		

	printf("\n-[ SeedKey Security Access ]-------------------------------------------------------------\n");

	printf("\n>>> Scanning for SecurityAccessBypass() Variant #1 Checking sub-routine [allow any login seed to pass] \n");
	addr = search( fh, (unsigned char *)&needle_5, (unsigned char *)&mask_5, needle_5_len, 0 );
	if(addr != NULL) {
		printf("Found at offset=0x%x. Patch at +(0x5d) +93, 0x04 (ret=0, login failed) goes to 0x14 (ret=1, login success) \n",(int)(addr-rom_load_addr) );
		// disassemble needle found in rom
		if(show_diss) { c167x_diss(addr-rom_load_addr, addr, needle_5_len+4); }
		// do the work of patching...
		printf("Applying patch so any login seed is successful... ");
		addr[0x5d] = 0x14; 
		printf("Patched! \n");
		if(show_diss) { 
			printf("Dumping after patching to always ret 1 (login success)...\n");
			c167x_diss(addr-rom_load_addr, addr, needle_5_len+4); 
		}
	} else {
		printf("Sequence not found\n");		
		printf(">>> Scanning for SecurityAccessBypass() Variant #2 Checking sub-routine [allow any login seed to pass]\n");		
		addr = search( fh, (unsigned char *)&needle_6, (unsigned char *)&mask_6, needle_6_len, 0 );
		if(addr != NULL) {
			printf("found at offset=0x%x. Patch at +(0x64) +100, 0x04 (ret=0, login failed) goes to 0x14 (ret=1, login success) \n",(int)(addr-rom_load_addr) );
			// disassemble needle found in rom
			if(show_diss) { c167x_diss(addr-rom_load_addr, addr, needle_6_len); }
			// do the work of patching...
			printf("Applying patch so any login seed is successful... ");
			addr[0x64] = 0x14; 	// very simple patch to always return TRUE...
			printf("Patched!\n");
			if(show_diss) { 
				printf("Dumping after patching to always ret 1 (login success)...\n");
				c167x_diss(addr-rom_load_addr, addr, needle_6_len+4); 
			}
		} else {
			printf("Sequence not found\n");		
		}
		printf("\n\n");		
	}

	return found;
}

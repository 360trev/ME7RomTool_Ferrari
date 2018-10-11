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
#include "tvkup.h"

extern int show_diss;
extern unsigned dpp1_value;

int check_tvkup(ImageHandle *fh, int skip)
{
	int found = 0, byte_offset;
	char *rom_load_addr = fh->d.p;
	int seg, val;
	unsigned char *tmp_adr;
	char *addr;

	if(skip == 0) return found;		
				
	printf("\n>>> Scanning for TVKUP [Delay time for clutch pedal (b_kupplv) ]\n");
	addr = search( fh, (unsigned char *)&needle_TVKUP, (unsigned char *)&mask_TVKUP, needle_TVKUP_len, 0 );
	if(addr != NULL) {
		printf("found GGEGAS() function at offset=0x%x. ",(int)(addr-rom_load_addr) );
		seg = get16((unsigned char *)addr+20);
		seg -= 2;
		val = get16((unsigned char *)addr+32);
//		printf("seg=%-4.4x, val=%-4.4x ",seg,val);
		unsigned long str_adr = (unsigned long)(seg*SEGMENT_SIZE)+(long int)val;	// derive phyiscal address from offset and segment
		printf("TVKUP   @ ADR:%#8x ", str_adr);
		str_adr              &= ~(ROM_1MB_MASK);					// convert physical address to a rom file offset we can easily work with.
		printf("(%#8x )\n", str_adr);
		str_adr              += rom_load_addr;
		tmp_adr               = (unsigned char *)str_adr;				
		val                   = *tmp_adr;
					
		// disassemble needle found in rom
		if(show_diss) { 
			printf("\nDumping ...\n");
			c167x_diss(addr-rom_load_addr, addr, needle_TVKUP_len+20); 
		}

		printf("\nTVKUP: 0x%-2.2x (0.%d s delay)\n",val,val*50);

	} else {
		printf("not found\n");
	}
	
	return 0;
}
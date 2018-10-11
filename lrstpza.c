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
	int seg, val;
	unsigned char *tmp_adr;
	int found = 0;
	unsigned char *addr;
	unsigned char *rom_load_addr = fh->d.p;

	if(skip == 0) return found;		
				
	printf("\n>>> Scanning for LRSTPZA [Period duration of the LRS forced amplitude]\n");
	addr = search( fh, (unsigned char *)&needle_LRSTPZA, (unsigned char *)&mask_LRSTPZA, needle_LRSTPZA_len, 0 );
	if(addr != NULL) {
		printf("found    LRS() function at offset=0x%x. ",(int)(addr-rom_load_addr) );
		seg  = dpp1_value; 
		seg -= 1;
		val  = get16((unsigned char *)addr+4);
//		printf("seg=%-4.4x, val=%-4.4x ",seg,val);
		unsigned long str_adr = (unsigned long)(seg*SEGMENT_SIZE)+(long int)val;	// derive phyiscal address from offset and segment
		printf("LRSTPZA @ ADR:%#8x ", str_adr);
		str_adr              &= ~(ROM_1MB_MASK);					// convert physical address to a rom file offset we can easily work with.
		printf("(%#8x )\n", str_adr);
		str_adr              += rom_load_addr;
		tmp_adr               = (unsigned char *)str_adr;				
		val                   = get16(tmp_adr);

		// disassemble needle found in rom
		if(show_diss) { 
			printf("\nDumping ...\n");
			c167x_diss(addr-rom_load_addr, addr, needle_LRSTPZA_len+20); 
		}

		printf("\nLRSTPZA: 0x%-4.4x (0.%d s)\n",val,val*100);

	} else {
		printf("not found\n");
	}

	return found;
}

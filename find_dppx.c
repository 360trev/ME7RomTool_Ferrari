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
#include "find_dppx.h"
#include "needles.h"

extern unsigned dpp0_value;
extern unsigned dpp1_value;
extern unsigned dpp2_value;
extern unsigned dpp3_value;
extern int show_diss;

int check_dppx(ImageHandle *fh, int skip)
{
	unsigned char *addr;
	unsigned char *rom_load_addr = fh->d.p;
	int found = 0;
	if(skip == 0) return found;		

	printf("-[ DPPx Setup Analysis ]-----------------------------------------------------------------\n\n");

	printf(">>> Scanning for Main ROM DPPx setup #1 [to extract dpp0, dpp1, dpp2, dpp3 from rom] ");
	addr = search( fh, (unsigned char *)&needle_dpp, (unsigned char *)&mask_dpp, needle_dpp_len, 0 );
	if(addr == NULL) {
		printf("\nmain rom dppX byte sequence #1 not found\nProbably not an ME7.x firmware file!\n");
		exit(0);	// force quit program

	} else {
		printf("\nmain rom dppX byte sequence #1 found at offset=0x%x.\n",(int)(addr-rom_load_addr) );
		// do the work of dppx extraction...
		dpp0_value = extract_dppx(addr,0);
		dpp1_value = extract_dppx(addr,1);
		dpp2_value = extract_dppx(addr,2);
		dpp3_value = extract_dppx(addr,3);
	}
}

unsigned long extract_dppx(unsigned char *addr, int i)
{
	unsigned long value;
	switch(i)
	{
		case 0:
			value = (get16((unsigned char *)addr + 2 + i*0));	
			printf("\ndpp%d: (seg: 0x%-4.4x phy:0x%-8.8x)",i,(int)(value),(int)value*SEGMENT_SIZE );
			break;

		case 1:
			value = (get16((unsigned char *)addr + 2 + 4));	
			printf("\ndpp%d: (seg: 0x%-4.4x phy:0x%-8.8x)",i,(int)(value),(int)value*SEGMENT_SIZE );
			break;

		case 2:
			value = (get16((unsigned char *)addr + 2 + 8));	
			printf("\ndpp%d: (seg: 0x%-4.4x phy:0x%-8.8x) ram start address",i,(int)(value),(int)value*SEGMENT_SIZE );
			break;

		case 3:		
			value = (get16((unsigned char *)addr + 2 + 12));	
			printf("\ndpp%d: (seg: 0x%-4.4x phy:0x%-8.8x) cpu registers",i, value, value*SEGMENT_SIZE );
			printf("\n\nNote: dpp3 is always 3, otherwise accessing CPU register area not possible",(int)(value) );
			printf("\n");
			break;

		default:	
			printf("calle error: only dppx ranges 0-3 supported, you specified %d! out of range\n",i); 
			break;
	}
	return(value);
}

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

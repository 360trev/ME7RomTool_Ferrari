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
#include "krkte.h"
#include "table_spec.h"
#include "show_tables.h"

extern int show_diss;
extern unsigned dpp1_value;

int check_krkte(ImageHandle *fh, int skip)
{
	MPTR _krkte;
	int found = 0, byte_offset;
	char *rom_load_addr = fh->d.p;
	char *addr;
	unsigned char krkte;
	double krkte_conv;
	if(skip == 0) return found;		

	printf("\n-[ KRKTE Airmass to injector 'on time' conv. based on inj size & fuel pressure]-----------------\n\n");
	printf(">>> Scanning for KRKTE Lookup code sequence... \n");
	/* search: *** Find KRKTE Value by searching for correct function code byte sequence */
	byte_offset    = search_offset( rom_load_addr, fh->len, (unsigned char *)&needle_KRKTE, (unsigned char *)&mask_KRKTE, needle_KRKTE_len);
	if(byte_offset != 0) 
	{
		addr = rom_load_addr + byte_offset;
		printf("\nfound needle at offset=%#x\n",(int)(byte_offset));

		// disassemble needle found in rom
		if(show_diss) { c167x_diss(byte_offset, rom_load_addr + byte_offset, needle_KRKTE_len+16); }

		translate_seg(&_krkte, "KRKTE", rom_load_addr, get16((unsigned char *)addr+6) /*seg*/, get16((unsigned char *)addr+10) /*val*/);
		show_seg(&_krkte);

		krkte         = *(_krkte.ram);		// get 8-bits
		krkte_conv    = (krkte/468.75);		// conversion to ms/%
		printf("%#x (%-3.4f ms/%%) \n\n", krkte, krkte_conv );
	}

	return found;
}


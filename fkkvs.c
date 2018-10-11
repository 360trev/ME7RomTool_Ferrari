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
#include "fkkvs.h"
#include "table_spec.h"
#include "show_tables.h"

extern int show_diss;
extern unsigned dpp1_value;

int check_fkkvs(ImageHandle *fh, int skip)
{
	unsigned int val,seg,adr;
	unsigned char *addr;
	unsigned long val_adr;
	unsigned char *rom_load_addr = fh->d.p;
	MPTR _fkkvs;

	int found = 0;
	/*
	 * search: *** Find FKKVS Table by searching for correct function code byte sequence ***
	 */
	if(skip == 0) return found;		

	printf("\n-[ FKKVS Correction factor fueling system ]-------------------------------------------\n");

	printf("\n>>> Scanning for RTKI() for required fields... \n");
	addr = search( fh, (unsigned char *)&needle_RKTI, (unsigned char *)&mask_RKTI, needle_RKTI_len, 0 );
	if(addr != NULL) 
	{
		printf("found at offset=0x%x. \n",(int)(addr-rom_load_addr) );
		// disassemble needle found in rom
		if(show_diss) { c167x_diss(addr-rom_load_addr, addr, needle_RKTI_len); }

		// get offset to 'FKKVS'...
		translate_seg(&_fkkvs, "FKKVS", rom_load_addr,  get16((unsigned char *)addr+46) /*seg*/, get16((unsigned char *)addr+50) /*val*/);
		show_seg(&_fkkvs); 
		
		dump_table(addr, rom_load_addr, get16((unsigned char *)addr+50) /*val*/, get16((unsigned char *)addr+46)  /*seg*/, &FKKVS_table, 0);
		

//		set_table_overrides( _snm16zuub.ram, _srl12zuub.ram, _kfzw.ram, &KFZW_table);
//		dump_table2(rom_load_addr, FULL_OVERRIDE);
					
	} else {
		printf("Not found\n");
	}
	printf("\n");
	return 0;
}

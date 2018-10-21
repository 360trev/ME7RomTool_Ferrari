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
#include "kfzw.h"
#include "table_spec.h"
#include "show_tables.h"

extern int show_diss;
extern unsigned dpp1_value;

int check_kfzw(ImageHandle *fh, int skip, int mode)
{
	unsigned int val,seg,adr;
	unsigned char *p;
	unsigned char *addr_gru;
	unsigned char *addr;
	unsigned long val_adr;
	unsigned char *rom_load_addr = fh->d.p;
	MPTR _kfzw,_kfzw2, _snm16zuub, _srl12zuub;

	int found = 0;
	/*
	 * search: *** Find KFNW Table by searching for correct function code byte sequence ***
	 */
	if(skip == 0) return found;		

	if(mode == 1) {
		printf("\n-[ KFZW Ignition Timing ]-------------------------------------------------------------\n");
	} else if (mode == 2) {
		printf("\n-[ KFZW2 Ignition Timing Variant #2 ]-------------------------------------------------\n");
	}

	printf("\n>>> Scanning for SSTB() for required X-axis (SNM16ZUUB), Y-axis (SRL12ZUUB)... \n");
	addr = search( fh, (unsigned char *)&needle_SSTB, (unsigned char *)&mask_SSTB, needle_SSTB_len, 0 );
	if(addr != NULL) 
	{
		printf("found at offset=0x%x. \n",(int)(addr-rom_load_addr) );
		// disassemble needle found in rom
		if(show_diss) { c167x_diss(addr-rom_load_addr, addr, needle_SSTB_len); }

		// get offset to 'SNM16ZUUB'...
		translate_seg(&_snm16zuub, "SNM16ZUUB", rom_load_addr,  dpp1_value-1 /*seg*/, get16((unsigned char *)addr+150)    /*val*/);
		show_seg(&_snm16zuub); 
		// get offset to 'SRL12ZUUB'...
		translate_seg(&_srl12zuub, "SRL12ZUUB", rom_load_addr,  dpp1_value-1 /*seg*/, get16((unsigned char *)addr+170)    /*val*/);
		show_seg(&_srl12zuub); 

		printf("\n>>> Scanning for ZWGRU() method to search for KFZW table ... \n");
		addr_gru = search( fh, (unsigned char *)&needle_ZWGRU, (unsigned char *)&mask_ZWGRU, needle_ZWGRU_len, 0 );
		if(addr_gru != NULL) 
		{
			printf("found at offset=0x%x. \n",(int)(addr_gru-rom_load_addr) );
			// disassemble needle found in rom
			if(show_diss) { c167x_diss(addr_gru-rom_load_addr, addr_gru, needle_SSTB_len); }

			if( mode == 1) {
				// get offset to 'KFZW' cell data...
				translate_seg(&_kfzw,      "KFZW",      rom_load_addr,  dpp1_value-1 /*seg*/, get16((unsigned char *)addr_gru+58) /*val*/);
				show_seg(&_kfzw);
				set_table_overrides( _snm16zuub.ram, _srl12zuub.ram, _kfzw.ram, &KFZW_table);
				dump_table2(rom_load_addr, FULL_OVERRIDE);
			}
			else if( mode == 2) {
				// get offset to 'KFZW2' cell data...
				translate_seg(&_kfzw2,     "KFZW2",     rom_load_addr,  dpp1_value-1 /*seg*/, get16((unsigned char *)addr_gru+20) /*val*/);
				show_seg(&_kfzw2);
				set_table_overrides( _snm16zuub.ram, _srl12zuub.ram, _kfzw2.ram, &KFZW2_table);
				dump_table2(rom_load_addr, FULL_OVERRIDE);
			}
		}
					
	} else {
		printf("Not found\n");
	}
	printf("\n");
	return 0;
}

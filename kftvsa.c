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
#include "kftvsa.h"
#include "table_spec.h"
#include "show_tables.h"

extern int show_diss;
extern unsigned dpp1_value;
	
int check_kftvsa(ImageHandle *fh, int skip, int mode)
{
	MPTR _kftvsa, _kftvsa0, _snm08__ub, _stm05saub;
	int found = 0, byte_offset;
	char *rom_load_addr = fh->d.p;
	char *addr;

	if(skip == 0) return found;		

	if(mode == 1) {
		printf("\n-[ KFTVSA Delay time for fuel cutoff ]-----------------\n\n");
	} else if(mode == 2) { 
		printf("\n-[ KFTVSA0 Delay time for fuel cutoff ]----------------\n\n");
	}

	/* search: *** Find BBSAWE() function by searching for byte sequence */
	printf(">>> Scanning for BBSAWE() function lookup code sequence... \n");
	byte_offset    = search_offset( rom_load_addr, fh->len, (unsigned char *)&needle_BBSAWE, (unsigned char *)&mask_BBSAWE, needle_BBSAWE_len);
	if(byte_offset != NULL) 
	{
		addr = rom_load_addr + byte_offset;
		printf("\nfound needle at offset=%#x\n",(int)(byte_offset));

		// disassemble needle found in rom
		if(show_diss) { c167x_diss(byte_offset, rom_load_addr + byte_offset, needle_BBSAWE_len+16); }

		if(mode == 1) {
			// find kftvsa (for cell data)
			translate_seg(&_kftvsa, "KFTVSA", rom_load_addr, get16((unsigned char *)addr+18) /*seg*/, get16((unsigned char *)addr+14) /*val*/);
			show_seg(&_kftvsa);
		}
		else if(mode == 2) {
			// find kftvsa0  (for cell data)
			translate_seg(&_kftvsa0,"KFTVSA0", rom_load_addr, get16((unsigned char *)addr+56) /*seg*/, get16((unsigned char *)addr+52) /*val*/);
			show_seg(&_kftvsa0);
		}
		
		// find snm08__ub (for y-axis data)
		translate_seg(&_snm08__ub, "SNM08__UB", rom_load_addr, get16((unsigned char *)addr+26) /*seg*/, get16((unsigned char *)addr+22) /*val*/);
		show_seg(&_snm08__ub);

		printf("\n>>> Scanning for SSTB2() function lookup code sequence... \n");
		byte_offset    = search_offset( rom_load_addr, fh->len, (unsigned char *)&needle_SSTB2, (unsigned char *)&mask_SSTB2, needle_SSTB2_len);
		if(byte_offset != NULL) 
		{
			addr = rom_load_addr + byte_offset;
			printf("\nfound needle at offset=%#x\n",(int)(byte_offset));

			// disassemble needle found in rom
			if(show_diss) { c167x_diss(byte_offset, rom_load_addr + byte_offset, needle_SSTB2_len+16); }

			// stm05saub (for x-axis data)
			translate_seg(&_stm05saub, "STM05SAUB", rom_load_addr, get16((unsigned char *)addr+26) /*seg*/, get16((unsigned char *)addr+22) /*val*/);
			show_seg(&_stm05saub);

			// show table
			if(mode == 1) {
				set_table_overrides(_stm05saub.ram, _snm08__ub.ram, _kftvsa.ram, &KFTVSA_table);				// ktfvsa
				dump_table2(rom_load_addr, FULL_OVERRIDE);
			} else if(mode == 2) {
				set_table_overrides(_stm05saub.ram, _snm08__ub.ram, _kftvsa0.ram, &KFTVSA0_table);				// kftvsa0 
				dump_table2(rom_load_addr, FULL_OVERRIDE);
			}
		}
	}
	return found;
}

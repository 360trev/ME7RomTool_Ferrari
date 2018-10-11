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
#include "kfnw.h"
#include "table_spec.h"
#include "show_tables.h"

extern int show_diss;
extern unsigned dpp1_value;

int check_kfzw(ImageHandle *fh, int skip, int mode)
{
	unsigned int val,seg,adr;
	unsigned char *p;
	unsigned char *addr_gru;
	unsigned char *snm16zuub_adr;
	unsigned char *srl12zuub_adr;
	unsigned char *kfzw_adr;
	unsigned char *kfzw2_adr;
	unsigned char *addr;
	unsigned long val_adr;
	unsigned char *rom_load_addr = fh->d.p;

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

//	printf("Key: KF=Map, ZW=Ignition angle, MN=Minimum\n");
	printf("\n>>> Scanning for SSTB() for required X-axis (SNM16ZUUB), Y-axis (SRL12ZUUB)... \n");
	addr = search( fh, (unsigned char *)&needle_SSTB, (unsigned char *)&mask_SSTB, needle_SSTB_len, 0 );
	if(addr != NULL) 
	{
		printf("found at offset=0x%x. \n",(int)(addr-rom_load_addr) );
		// disassemble needle found in rom
		if(show_diss) { c167x_diss(addr-rom_load_addr, addr, needle_SSTB_len); }

		printf("\n>>> Scanning for ZWGRU() method to search for KFZW table ... \n");
		addr_gru = search( fh, (unsigned char *)&needle_ZWGRU, (unsigned char *)&mask_ZWGRU, needle_ZWGRU_len, 0 );
		if(addr_gru != NULL) 
		{
			printf("found at offset=0x%x. \n",(int)(addr_gru-rom_load_addr) );
			// disassemble needle found in rom
			if(show_diss) { c167x_diss(addr-rom_load_addr, addr_gru, needle_SSTB_len); }

			// get offset to 'KFZW2' cell data...
			val = get16((unsigned char *)addr_gru+58);
			seg = dpp1_value-1;
			adr = seg*SEGMENT_SIZE+val;
//			printf("\nKFZW val=0x%-4.4x seg=%-2.2x\n", val,seg);
			val_adr     = (unsigned long)adr;		// derive phyiscal address from offset and segment
			val_adr      &= ~(ROM_1MB_MASK);				// convert physical address to a rom file offset we can easily work with.
			kfzw_adr = (unsigned char *)rom_load_addr + val_adr;
			// end get KFZW...
			p             = kfzw_adr;
			if(mode==1) { printf("KFZW Cells  : 0x%p\n", adr); }

			// get offset to 'KFZW2' cell data...
			val = get16((unsigned char *)addr_gru+20);		// 14, 40
			seg = dpp1_value-1;
			adr = seg*SEGMENT_SIZE+val;
//			printf("\nKFZW2 val=0x%-4.4x seg=%-2.2x\n", val,seg);
			val_adr     = (unsigned long)adr;		// derive phyiscal address from offset and segment
			val_adr      &= ~(ROM_1MB_MASK);				// convert physical address to a rom file offset we can easily work with.
			kfzw2_adr = (unsigned char *)rom_load_addr + val_adr;
			// end get KFZW2...
			p             = kfzw2_adr;
			if(mode == 2) { printf("KFZW2 Cells : 0x%p\n", adr); }
					 
			// get offset to 'SNM16ZUUB'...
			val = get16((unsigned char *)addr+150);		// 
			seg = dpp1_value-1;
			adr = seg*SEGMENT_SIZE+val;
//			printf("\nSNM16ZUUB val=0x%-4.4x seg=%-2.2x\n", val,seg);
			val_adr     = (unsigned long)adr;		// derive phyiscal address from offset and segment
			val_adr      &= ~(ROM_1MB_MASK);				// convert physical address to a rom file offset we can easily work with.
			snm16zuub_adr = (unsigned char *)rom_load_addr + val_adr;
			// end get SNM16ZUUB...
			p             = snm16zuub_adr;
			printf("SNM16ZUUB   : 0x%p\n", adr);
//			entries = *p++;
//			for(i=0;i<entries;i++) {
//				printf("%-2.2X", *p++);
//				if(i < entries-1 ) printf(",");
//				}
			// get offset to 'SRL12ZUUB'...
			val = get16((unsigned char *)addr+170);		// 
			seg = dpp1_value-1;
			adr = seg*SEGMENT_SIZE+val;
//						printf("\n\nSRL12ZUUB val=0x%-4.4x seg=%-2.2x\n", val,seg);
			val_adr     = (unsigned long)adr;		// derive phyiscal address from offset and segment
			val_adr      &= ~(ROM_1MB_MASK);				// convert physical address to a rom file offset we can easily work with.
			srl12zuub_adr = (unsigned char *)rom_load_addr + val_adr;
			// end get SRL12ZUUB...
			p             = srl12zuub_adr;
			printf("SRL12ZUUB   : 0x%p\n", adr);
//			entries = *p++;
//			for(i=0;i<entries;i++) {
//				printf("%-2.2X", *p++);
//				if(i < entries-1 ) printf(",");
//			}

//
//						printf("** set override ***\n");

			if( mode == 1) {
				set_table_overrides(snm16zuub_adr, srl12zuub_adr, kfzw_adr);
				dump_table2(addr, rom_load_addr, get16((unsigned char *)addr + 58), dpp1_value-1, &KFZW_table, FULL_OVERRIDE);
			}
		
			if( mode == 2) {
				set_table_overrides(snm16zuub_adr, srl12zuub_adr, kfzw2_adr);
				dump_table2(addr, rom_load_addr, get16((unsigned char *)addr + 14), dpp1_value-1, &KFZW2_table, FULL_OVERRIDE);
			}
						
			printf("***\n");
		}
					
	} else {
		printf("Not found\n");
	}
	printf("\n");

	return 0;
}

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
#include "multimap.h"
#include "table_spec.h"
#include "show_tables.h"

//-[ Map Table Finder :) ] -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------			

/*  Things get quite interesting here on in...
 * 
 *  What I am about to show is how to identify 'generic' map tables directly from the rom function signatures ;)
 *  The idea is we directly find signatures for all 'known' maps, then find ALL maps and remove the known ones from the
 *  list. This yields a good 'catch all' ...
 * 
 *  The approach of masking all segment and relocation information out of the signatures means it works on any ME7x rom file 
 *  compiled for C167x cpu and works right across a huge number of rom variants.
 *
 *  This is just showing X-Axis tables (entire set of rom tables will come shortly and then we can easily match them too!)... 
 *  But ofcourse its quite simple to make this work for ALL the ROM resident tables. 
 * 
 *  This is a far better way than 'guessing' the maps knowing they reside (as some even commercial tools do) within a certain range 
 *  in the rom. This guarentee's your actually looking at real tables. The next step is to push the table start addresses into a hash 
 *  table to make it easy to de-duplicate them so you don't find calls to the lookups to the same tables (happens occasionally since 
 *  we are walking through the rom code and literaly picking up ALL of the accesses to the tables.
 * 
 *  Have fun ;)
 */
 
extern unsigned dpp1_value;
extern 

int check_multimap(ImageHandle *fh, int skip)
{
	int found = 0;
	int i;
	unsigned char *addr;
	unsigned char *rom_load_addr = fh->d.p;

	if(skip == 0) return found;		
	
	printf("-[ Generic X-Axis MAP Table Scanner! ]---------------------------------------------------------------------\n\n");
	printf(">>> Scanning for Map Tables #1 Checking sub-routine [map finder!] \n");
				
	int disabled_maps_1=1;
	if(disabled_maps_1==1)
	{
		int current_offset=0;
		int x,y1, j=0;
					
		i=0;
		while(j++ < MAX_TABLE_SEARCHES)
		{
					// search for signature for X-Axis (1 row) table..
						addr = search( fh, (unsigned char *)&mapfinder_needle, (unsigned char *)&mapfinder_mask, mapfinder_needle_len, current_offset);

						// exit the searching loop when we reach end of rom region
						if(addr-rom_load_addr > dynamic_ROM_FILESIZE-mapfinder_needle_len) { break; }

						// if we find a match lets dump it!
						if(addr != NULL) {
							printf("\n[Map #%d] 1D X-Axis  : Map function found at: offset=0x%x ",(i++)+1, (int)(addr-rom_load_addr) );
//							// disassemble needle found in rom
//							if(show_diss) { printf("\n"); c167x_diss(addr-rom_load_addr, addr, mapfinder_needle_len); }

							unsigned long val          = get16((unsigned char *)addr + 2);	// from rom routine extract value (offset in rom to table)
							unsigned long seg          = get16((unsigned char *)addr + 6);	// and segment (required to regenerate physical address from segment)
							unsigned long map_adr      = (unsigned long)(seg*SEGMENT_SIZE)+(long int)val;	// derive phyiscal address from offset and segment
							map_adr                   &= ~(ROM_1MB_MASK);					// convert physical address to a rom file offset we can easily work with.

							unsigned char *table_start = rom_load_addr+map_adr+1;			// 2 bytes to skip x and y bytes
							unsigned char x_axis       = get16(rom_load_addr+map_adr+0);		// get number of rows
	//						unsigned char x_axis       = *(rom_load_addr+map_adr+1);		// get number of colums
							
	//						printf("(seg:0x%x phy:0x%x val:0x%x), offset=0x%x x-axis=%d",seg, seg*SEGMENT_SIZE+val, val, (unsigned long)table_start-(int)rom_load_addr, x_axis);
							printf("phy:0x%x, file-offset=0x%x x-axis=%d",seg*SEGMENT_SIZE+val, (unsigned long)table_start-(int)rom_load_addr, x_axis);

							printf("\n\t");
							for(x=0;x<x_axis;x++) 
							{
								printf("%-2.2x ", (int)(*(table_start+x)) );	// show values directly out of the table
							}
						}
//						printf("\n");
						
						// continue search from next location after this match...
						current_offset = (addr-rom_load_addr)+mapfinder_needle_len;
  
						//printf("\nCurrent Offset : %x\n",current_offset);
		}
		printf("\n\n");
					
	}

	//
	// Multi-dimensional Type #1
	//
	int disabled_maps_2=1;
	if(disabled_maps_2==1)
	{ 
		int current_offset = 0;
		int new_offset=0;
		unsigned char *tmp_ptr;
		unsigned char *map_table_start;
		unsigned int map_table_adr;
		unsigned int map_table_x_axis_adr;
		unsigned int map_table_y_axis_adr;
		unsigned int map_table_x_num_adr;
		unsigned int x_num, y_num;
		unsigned int map_table_y_num_adr;
		unsigned long val, seg;

		while(1)
		{
						// search for signature for X/Y-Axis (multirow/column) table..
						current_offset = search_offset(rom_load_addr+new_offset, (fh->len)-new_offset, (unsigned char *)&mapfinder_xy2_needle, (unsigned char *)&mapfinder_xy2_mask, mapfinder_xy2_needle_len);
						if(current_offset == 0) break;

						// if we find a match lets dump it!
						if(current_offset != NULL) {
							printf("\n------------------------------------------------------------------\n[Map #%d] Multi Axis Map Type #1 function found at: offset=0x%x \n",(i++)+1, (int)(current_offset+new_offset) );
							addr = rom_load_addr+current_offset+new_offset;
							dump_table(addr, rom_load_addr, get16((unsigned char *)addr + 30), get16((unsigned char *)addr + 26), &XXXX_table, 0);
							new_offset += current_offset+mapfinder_xy2_needle_len;
						}
						printf("\n");
		}
		printf("\n\n");
	}

	//
	// Single-dimensional Type #1 
	//
	int disabled_maps_3=1;
	if(disabled_maps_3==1)
	{
		int current_offset = 0;
		int new_offset=0;
		unsigned long map_table_adr;
		unsigned long map_axis_adr;
		unsigned long val, seg;
		unsigned int j = 0;
					
		while(j++ < MAX_TABLE_SEARCHES)
		{
						// search for signature for X/Y-Axis (multirow/column) table..
						current_offset = search_offset(rom_load_addr+new_offset, (fh->len)-new_offset, (unsigned char *)&mapfinder_xy3_needle, (unsigned char *)&mapfinder_xy3_mask, mapfinder_xy3_needle_len);
						if(current_offset == 0) break;

						// if we find a match lets dump it!
						if(current_offset != NULL) {
							printf("\n\n------------------------------------------------------------------\n[Map #%d] Multi Map Type #2 lookup function found @ offset: 0x%x \n",(i++)+1, (int)(current_offset+new_offset) );

							addr = rom_load_addr+current_offset+new_offset;
							unsigned char *pos=addr;
							unsigned char val;
							int i=0;

							// lets try to determine (experimental!!! ) start of main subroutine this belongs too
							while(j++ < MAX_SEARCH_BACK_BYTES)
							{
								i++;
								if(*(pos+0) == 0xDB)				// machine code for 'rets in C167 is 0xDB00 
								{
										if(*(pos+1) == 0x00) {
											i++;
											printf("\nFound estimated function start address: 0x%-8x", pos-rom_load_addr);
											break;
										}
								}
								if(pos <= rom_load_addr) { printf("not found\n"); break; } 
								pos--;
							}
							printf("\nBacktrack offset: 0x%x (%d bytes)\n\n",(int)(current_offset+new_offset+4)-i,i);
							
							val                   = get16((unsigned char *)addr + 14 - 10) ;	// from rom routine extract value (offset in rom to table)
							seg                   = get16((unsigned char *)addr + 18 - 10);	// and segment (required to regenerate physical address from segment)
							map_table_adr         = (unsigned long)(seg*SEGMENT_SIZE)+(long int)val;	// derive phyiscal address from offset and segment
							dump_table(addr, rom_load_addr, get16((unsigned char *)addr + 22 - 10), get16((unsigned char *)addr + 26 - 10), &XXXXB_table, map_table_adr);

							new_offset += current_offset+mapfinder_xy2_needle_len;
						}
					//	printf("\n");
		}
	//	printf("\n\n");
	}
				
	return 0;
} 

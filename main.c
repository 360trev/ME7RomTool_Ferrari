/* me7romtool [ Firmware analysis tool for Bosch ME7.3H4 Ferrari firmware's]
   By 360trev. Needle lookup function borrowed from nyet (Thanks man!) from
   the ME7sum tool development (see github).
   
   See Readme for notes about current status and features.
 
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include "utils.h"
#include "needles.h"
#include "crc32.h"

#define HFM_NOTSET   0
#define HFM_READING  1
#define HFM_WRITING  2
#define HFM_IDENTIFY 3
#define SKEY_PATCH   4

int search_rom(int mode, char *filename_rom, char *filename_hfm);
	
int show_usage(char *argv[])
{
	printf("Usage: %s <mode> <rom_filename> <extra options> ...\n\n",argv[0]);

	printf(" -rhfm :  <hfm_dump_filename> : Read and extract hfm from romfile, optional dump filename to override default write name.");
	printf("e.g. %s -rhfm 360cs.bin hfm_stradale.bin\n\n", argv[0]);
	
	printf(" -whfm :  <hfm_load_filename> : Write hfm into specified romfile, mandatory load filename must be specified.");
	printf("e.g. %s -whfm 360modena.bin hfm_stradale.bin\n\n", argv[0]);

	printf(" -ihfm : Try to identify mlhfm table in specified romfile.");
	printf("e.g. %s -whfm 360modena.bin\n\n", argv[0]);

	printf(" -skey : Try to identify seedkey function and patch login so any seedkey works.");
	printf("e.g. %s -skey\n\n", argv[0]);

	return 0;
}

// shared library externs
typedef uint32_t (*calc_crc32)(uint32_t crc, const void *buf, size_t size);
int seedkey_patch=0;

int main(int argc, char *argv[])
{
    int ok;
	char *hfm_name=NULL;
	char *rom_name=NULL;
    int i,mode=HFM_NOTSET;
	printf("Ferrari 360 ME7.3H4 Rom Tool. Last Built: %s %s v1.01\n",__DATE__,__TIME__);
	printf("by 360trev.  Needle lookup function borrowed from nyet (Thanks man!) from\nthe ME7sum tool development (see github). \n\n");

	seedkey_patch = 0;
	
	/*
	 * check which mode to operate in depending on console options specified
	 */
	
    for (i = 0; i < argc; i++) {
		/* IDENTIFY a SEEDKEY in a Firmware Rom */
        if (strcmp(argv[i],"-skey")==0) {
            mode = SKEY_PATCH;
//			printf("Seedkey Bypass - This will try to identify the Seedkey login check routine and implement a bypass so any seed/key combinations work.\n\n");
            seedkey_patch = 1;
        }

		/* READ MHFM from a Firmware Rom */
		if (strcmp(argv[i],"-rhfm")==0) {
            mode = HFM_READING;
			printf("MLHFM Table Extraction Mode - This will dump and try to identify the MLHFM table from the loaded rom file.\n");
            if (i+1 <= argc) {
                rom_name = argv[++i];
            }
            if (i+1 <= argc) {
                hfm_name = argv[++i];
            } 
        }
		/* WRITE MLHFM into a Firmware Rom */
        else if (strcmp(argv[i],"-whfm")==0) {
			printf("MLHFM Table Update Mode - This will try to replace the MLHFM table in the specified rom file with a new one.\n");
            mode = HFM_WRITING;
            if (i+1 <= argc) {
                rom_name = argv[++i];
            }
            if (i+1 <= argc) {
                hfm_name = argv[++i];
            } 
        }
		/* IDENTIFY a MLHFM in a Firmware Rom */
        else if (strcmp(argv[i],"-ihfm")==0) {
			printf("MLHFM Table Identification Mode - This will try to identify the MLHFM table in the specified rom file.\nIt wont write anything\n\n");
            mode = HFM_IDENTIFY;
            if (i+1 <= argc) {
                rom_name = argv[++i];
            }
            if (i+1 <= argc) {
                hfm_name = argv[++i];
            } 
        }

    }

	/*
	 * sanity check any options for the given operational mode 
	 */
	switch(mode)
	{
		case HFM_READING:
		{
			if(hfm_name == NULL) 
			{	
				printf("You didnt specify a filename for HFM table so one will be automatically created!\n");
			} else {
				printf("HFM Output Filename: '%s'\n",hfm_name);
			}	
		} break;
		
		case HFM_WRITING:
		{
			if(hfm_name != NULL) {
				printf("HFM Input Filename: '%s'\n",hfm_name);
			} else {
				printf("You didnt specify a filename for HFM table to load.\n");
				return 0;
			}
		} break;

		case HFM_IDENTIFY:
		{
/*			printf("MLHFM Indentification Mode\n"); */
		} break;

		default:
		{
			show_usage(argv);
			return 0;
		}
		break;
	}
	
	if(argc < 2) {
		show_usage(argv);
		return 0;
	}

	if(mode == HFM_NOTSET) {
		printf("No options set. Please choose what you want to do first!\n");
		return 0;
	}
	
	/*
	 * now lets search rom for mode selected..
	 */
    ok = search_rom(mode, rom_name,hfm_name);
    if (ok) { 
		printf("File could not be found. Please check and try again.");  
	}

	return 0;
}

//
// Calculate the Bosch Motronic ME71 checksum for the given range
//
uint32_t CalcChecksumBlk(struct ImageHandle *ih, uint32_t start, uint32_t end)
{
	uint32_t sum=0, i;
	for(i = start/2; i <= end/2; i++) { 	
		sum += le16toh( ih->d.u16[i] );
	}
	return sum;
}

#define MAX_DHFM_ENTRIES     1024
#define DEFAULT_DHFM_ENTRIES 512
#define MAP_FILE_OFFSET      0x10000
#define MAX_FILENAME         256
#define ROM_FILESIZE		 512*1024
#define MAX_ROM_REGIONS      32
#define SEGMENT_SIZE         0x4000
#define ROM_BASE_ADDRESS     0x800000
#define ROM_1MB_MASK         0xFFF00000
		
unsigned long get_addr_from_rom(unsigned char *rom_start_addr, unsigned dynamic_romsize, unsigned char *lo_addr, int lo_bits, unsigned char *hi_addr, int hi_bits, int segment, int table_index)
{
	unsigned int   var_hi;
	unsigned int   var_lo;
	unsigned char *var_lo_addr;
	unsigned char *var_hi_addr;
	unsigned int   var_hi_offset;
	unsigned int   var_lo_offset;
	unsigned int   var_hi_value=0;
	unsigned int   var_lo_value=0;
	unsigned long  var_final_address=0;
	unsigned int   segment_offset;

    //
	// get segment register & high/low addresses
	// from rom function
	//
	segment_offset    = (unsigned int)get16((unsigned char *)segment); 	// get segment offset 
//		printf("segment=%x\n",segment_offset);

	if(lo_addr != 0) {
		var_lo            = (unsigned int)get16((unsigned char *)lo_addr); // get low address word
//		printf("lo_val=%x\n",var_lo);
		// calculate physical address from segment register and offset word
		var_lo_addr       = (unsigned char *)(var_lo)+(segment_offset*SEGMENT_SIZE);
//		printf("var_lo_addr=%x\n",var_lo_addr);
		// deduct rom start address to get OFFSET from 0 byte indexes (used to index loaded rom image)
//		var_lo_offset     = (int)var_lo_addr-ROM_BASE_ADDRESS;
		var_lo_offset     = (int)var_lo_addr;
		var_lo_offset    &= ~(ROM_1MB_MASK);

		// now extract (from firmware using calulated byte offsets )
		if(lo_bits == 16) {
			var_lo_value      = (unsigned int)(get16((unsigned char *)rom_start_addr + var_lo_offset + table_index));
		} else if(lo_bits == 32) {
			var_lo_value      = (unsigned int)(get32((unsigned char *)rom_start_addr + var_lo_offset + table_index));
		}		
		var_final_address = (unsigned long )var_lo_value;
	}
	
	if(hi_addr != 0) {
		var_hi            = (unsigned int)get16((unsigned char *)hi_addr);	// get high address word
//		printf("hi_val=%x\n",var_hi);
		// calculate physical address from segment register and hi/lo words
		var_hi_addr       = (unsigned char *)(var_hi)+(segment_offset*SEGMENT_SIZE);
		// deduct rom start address to get OFFSET from 0 byte indexes (used to index loaded rom image)
//		var_hi_offset     = (int)var_hi_addr-ROM_BASE_ADDRESS;
		var_hi_offset     = (int)var_hi_addr;
		var_hi_offset    &= ~(ROM_1MB_MASK);

		// now extract (from firmware using calulated byte offsets )
		if(hi_bits == 16) {
			var_hi_value      = (unsigned int)(get16((unsigned char *)rom_start_addr + var_hi_offset + table_index)); 
		} else if(hi_bits == 32) {
			var_hi_value      = (unsigned int)(get32((unsigned char *)rom_start_addr + var_hi_offset + table_index)); 
		}
		var_final_address = (unsigned long )var_hi_value;
	}

	if(hi_addr ==0)
	{
		printf("\n\tlo:0x%x (seg: 0x%x phy:0x%x) : ",(unsigned int)var_lo_offset+table_index,(int)segment_offset, (int)(var_lo_addr+table_index) );
	} else {
		if(lo_addr ==0) 
		{
//			printf("\n\thi:0x%x (seg: 0x%x phy:0x%x) : ",(unsigned int)var_hi_offset+table_index,(int)segment_offset, (int)(var_hi_addr+table_index) );
		} else {
		printf("\n\tlo:0x%x hi:0x%x (seg: 0x%x phy:0x%x) : ",(unsigned int)var_lo_offset+table_index,(unsigned int)var_hi_offset+table_index,(int)segment_offset, (int)(var_lo_addr+table_index) );
		// re-create 32-bit unsigned long from hi and low words
		var_final_address = (unsigned long )(((var_hi_value <<  16)) | var_lo_value );
		}
	}
	return(var_final_address);
}

int search_rom(int mode, char *filename_rom, char *filename_hfm)
{
	ImageHandle f_hfm;
	ImageHandle *fh_hfm = &f_hfm;
	int load_hfm_result;
	ImageHandle f;
	ImageHandle *fh = &f;
	int load_result;
	int save_result;
	char ml_filename[MAX_FILENAME];
	char newrom_filename[MAX_FILENAME];
//	int segment_offset;
	unsigned char *addr;
	unsigned char *offset_addr;
	unsigned short offset,entries;
	unsigned long masked_start_addr=0;
	unsigned long masked_end_addr=0;
	unsigned long start_addr=0;
	unsigned long end_addr=0;
	unsigned long last_end_addr=0;
	unsigned long checksum_norm;
	unsigned long checksum_comp;
	unsigned long dynamic_ROM_FILESIZE=0;
	uint32_t i, sum, final_sum=0;
	int num_entries = 0;
	int num_multipoint_entries_byte;
	int hiword, loword;
	unsigned long dpp0_value, dpp1_value, dpp2_value, dpp3_value;
	
	/* load file from storage */
	load_result = iload_file(fh, filename_rom, 0);
	if(load_result == 0) 
	{
		printf("Succeded loading file.\n\n");
		offset_addr = (unsigned char *)(fh->d.p);
		
		/* quick sanity check on firmware rom size (Ferrari 360 images must be 512kbytes/1024kbytes) */
		if(fh->len == ROM_FILESIZE*1 || fh->len == ROM_FILESIZE*2 ) 
		{		
			dynamic_ROM_FILESIZE = fh->len;	// either 512kbytes or 1024kbytes set depending on actual file size...

			if(dynamic_ROM_FILESIZE == ROM_FILESIZE*1 ) {
				printf("Loaded ROM: Tool in 512Kb Mode\n");
			} else if(dynamic_ROM_FILESIZE == ROM_FILESIZE*2 ) {
				printf("Loaded ROM: Tool in 1Mb Mode\n");
			}
			printf("\n");

//-[ DPPx Search ] -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------			
			/*
			 * search: *** Main Rom Checksum bytecode sequence #1 ***
			 */
			printf("-[ DPPx Setup Analysis ]-----------------------------------------------------------------\n\n");

			printf(">>> Scanning for Main ROM DPPx setup #1 [to extract dpp0, dpp1, dpp2, dpp3 from rom] ");
			addr = search( fh, (unsigned char *)&needle_dpp, (unsigned char *)&mask_dpp, sizeof(needle_dpp), 0 );
			if(addr == NULL) {
				printf("\nmain rom dppX byte sequence #1 not found\nProbably not an ME7.x firmware file!");
			} else {
				printf("\nmain rom dppX byte sequence #1 found at offset=0x%x.\n",(int)(addr-offset_addr) );

				dpp0_value = (get16((unsigned char *)addr + 2 + 0));
				dpp1_value = (get16((unsigned char *)addr + 2 + 4));
				dpp2_value = (get16((unsigned char *)addr + 2 + 8));
				dpp3_value = (get16((unsigned char *)addr + 2 + 12));

				printf("\ndpp0: 0x%-4.4x",(int)(dpp0_value) );
				printf("\ndpp1: 0x%-4.4x",(int)(dpp1_value) );
				printf("\ndpp2: 0x%-4.4x",(int)(dpp2_value) );
				printf("\ndpp3: 0x%-4.4x (DPP3 is always 3, otherwise accessing CPU register area not possible)",(int)(dpp3_value) );
				printf("\n");
					
			}
			printf("\n");

//-[ MAINROM <Number of Entries> ] -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------			

			printf("\n\n-[ Main-Rom Checksum Analysis ]----------------------------------------------------------\n\n");
			
			/*
			 * search: *** Main Rom Checksum bytecode sequence #1 ***
			 */
			printf(">>> Scanning for Main ROM Checksum sub-routine #1 [to extract number of entries in table] ");
			addr = search( fh, (unsigned char *)&needle_2b, (unsigned char *)&mask_2b, sizeof(needle_2b), 0 );
			if(addr == NULL) {
				printf("\nmain checksum byte sequence for number of entries not found\nGiving up.\n");
			} else {
				printf("\nmain checksum byte sequence #1 found at offset=0x%x.\n",(int)(addr-offset_addr) );
				
				int entries_byte = *(addr+27);	// offset 27 into needle_2b is the compare instruction for the number of entries, lets extract it and convert to entries
				switch(entries_byte) {
					case 0xA2:	num_entries = 1;	break;
					case 0xA4:	num_entries = 2;	break;
					case 0xA6:	num_entries = 3;	break;
					default:	num_entries = 0;	break;
				}
				if(num_entries > 0) {
					printf("Found #%d Regional Block Entries in table\n", num_entries);
				} else {
					printf("Unable to determine number of entries. Please contact developers.\n");
				}	
			}

//-[ MAINROM <Start/End Array> Version #1 ] -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------			
			
			/*
			 * search: *** Main Rom Checksum bytecode sequence #1 ***
			 */
#if 0
			printf("\n>>> Scanning for Main ROM Checksum sub-routine #2c [to extract Start/End regions] ");
			addr = search( fh, (unsigned char *)&needle_2v2, (unsigned char *)&mask_2v2, sizeof(needle_2v2), 0 );
			if(addr == NULL) {
				printf("\nmain checksum byte sequence not found\nGiving up.\n");
			} else {
				printf("\nmain checksum byte sequence #2c found at offset=0x%x.\n",(int)(addr-offset_addr) );
				final_sum = 0;
				for(i=0;i < num_entries;i++) 
				{
					// address of rom_table [8 bytes] -- Region [i]: start,end
					printf("\nMain Region Block #%d: ",i+1);		
					start_addr = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, addr+18+00, 16, addr+22+00, 16, (int)addr+14, i*8);		// extract 'start address' directly from identified checksum machine code
					start_addr &= ~(ROM_1MB_MASK);
					printf("0x%lx",(long int)start_addr );		
					
					last_end_addr = end_addr;
					end_addr   = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, addr+40+00, 16, addr+44+00, 16, (int)addr+36, i*8);		// extract 'end address' directly from identified checksum machine code
					end_addr &= ~(ROM_1MB_MASK);
					printf("0x%lx",(long int)end_addr );

					// calculate checksum for this block
					sum = CalcChecksumBlk(fh,start_addr,end_addr);
					printf(" sum=%lx ~sum=%lx : acc_sum=%lx", (unsigned long)sum, (unsigned long)~sum, (unsigned long)final_sum);

					// add this regions sum to final accumulative checksum
					final_sum += sum;
				}
				printf("\n\nFinal Main ROM Checksum calculation:  0x%-8.8lx (after %d rounds)", (unsigned long)final_sum,i);
				printf("\nFinal Main ROM Checksum calculation: ~0x%-8.8lx\n",(unsigned long)~final_sum);
				printf("\n");

			}
#endif
//-[ MAINROM <Start/End Array> Version #2 ] -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------			

			printf("\n>>> Scanning for Main ROM Checksum sub-routine #2 [to extract Start/End regions] ");
			addr = search( fh, (unsigned char *)&needle_2, (unsigned char *)&mask_2, sizeof(needle_2), 0 );
			if(addr == NULL) {
				printf("\nmain checksum byte sequence not found\nGiving up.\n");
			} else {
				printf("\nmain checksum byte sequence #2 found at offset=0x%x.\n",(int)(addr-offset_addr) );
				final_sum = 0;
				for(i=0;i < num_entries;i++) 
				{
					// address of rom_table [8 bytes] -- Region [i]: start,end
					printf("\nMain Region Block #%d: ",i+1);		
					start_addr = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, addr+18+00, 16, addr+22+00, 16, (int)addr+14, i*8);		// extract 'start address' directly from identified checksum machine code
					start_addr &= ~(ROM_1MB_MASK);
					printf("0x%lx",(long int)start_addr );		
				
					last_end_addr = end_addr;
					end_addr   = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, addr+18+26, 16, addr+22+26, 16, (int)addr+14, i*8);		// extract 'end address' directly from identified checksum machine code
					end_addr &= ~(ROM_1MB_MASK);
					printf("0x%lx",(long int)end_addr );

					// calculate checksum for this block
					sum = CalcChecksumBlk(fh,start_addr,end_addr);
					printf(" sum=%lx ~sum=%lx : acc_sum=%lx", (unsigned long)sum, (unsigned long)~sum, (unsigned long)final_sum);

					// add this regions sum to final accumulative checksum
					final_sum += sum;
				}
				printf("\n\nFinal Main ROM Checksum calculation:  0x%-8.8lx (after %d rounds)", (unsigned long)final_sum,i);
				printf("\nFinal Main ROM Checksum calculation: ~0x%-8.8lx\n",(unsigned long)~final_sum);
				printf("\n");
			}

//-[ MAINROM <Stored Checksums> Version #1 ] -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------			

			/*
			 * search: *** Main Rom Checksum bytecode sequence #3 : MAIN ROM stored HI/LO checksums ***
			 */
			printf("\n>>> Scanning for Main ROM Checksum sub-routine #3 variant #A [to extract stored checksums and locations in ROM] ");
			addr = search( fh, (unsigned char *)&needle_3, (unsigned char *)&mask_3, sizeof(needle_3), 0 );
			if(addr == NULL) {
				printf("\nmain checksum byte sequence #3 variant #A not found\nTrying different variant.\n");

				printf("\n>>> Scanning for (!) Main ROM Checksum sub-routine #3 variant #B [to extract stored checksums and locations in ROM] ");
				addr = search( fh, (unsigned char *)&needle_3b, (unsigned char *)&mask_3b, sizeof(needle_3b), 0 );
				if(addr == NULL) {
					printf("\nmain checksum byte sequence #3 variant #B not found\nTrying different variant.\n");
				} else {
					printf("\nmain checksum byte sequence #3 variant #B block found at offset=0x%x.\n",(int)(addr-offset_addr) );
					printf("\nStored Main ROM Block Checksum: ");		
					checksum_norm = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, addr+40+00, 16, addr+44+00, 16, (int)addr+36, 0);		// start
					printf("0x%lx",(long unsigned int)checksum_norm );		
						
					printf("\nStored Main ROM Block ~Checksum: ");		
					checksum_comp = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, addr+40+00, 16, addr+44+00, 16, (int)addr+36, 4);		// start
					printf("0x%lx",(long unsigned int)checksum_comp );		
					printf("\n\n");
							
					printf("MAIN STORED ROM  CHECKSUM: 0x%-8.8lx ? 0x%-8.8lx : ",(long)final_sum, (long)checksum_norm);
					if(final_sum == checksum_norm)  { printf("OK!\t"); } else {printf("BAD!\t"); }
					printf(" ~CHECKSUM: 0x%-8.8lx ? 0x%-8.8lx : ",(long)~final_sum, (long)checksum_comp);
					if(~final_sum == checksum_comp) { printf("OK!\n"); } else {printf("BAD!\n"); }
				}				
				printf("\n");
			
			} else {
				printf("\nmain checksum byte sequence #3 block found at offset=0x%x.\n",(int)(addr-offset_addr) );
				printf("\nStored Main ROM Block Checksum: ");		
				checksum_norm = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, addr+14+00, 16, addr+18+00, 16, (int)addr+10, 0);		// start
				printf("0x%lx",(long unsigned int)checksum_norm );		
					
				printf("\nStored Main ROM Block ~Checksum: ");		
				checksum_comp = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, addr+14+00, 16, addr+18+00, 16, (int)addr+10, 4);		// start
				printf("0x%lx",(long unsigned int)checksum_comp );		
				printf("\n\n");
						
				printf("MAIN STORED ROM  CHECKSUM: 0x%-8.8lx ? 0x%-8.8lx : ",(long)final_sum, (long)checksum_norm);
				if(final_sum == checksum_norm)  { printf("OK!\t"); } else {printf("BAD!\t"); }
				printf(" ~CHECKSUM: 0x%-8.8lx ? 0x%-8.8lx : ",(long)~final_sum, (long)checksum_comp);
				if(~final_sum == checksum_comp) { printf("OK!\n"); } else {printf("BAD!\n"); }
			}

//-[ Multipoint <Number of Table Entries> ] -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------			

				printf("\n\n-[ Multipoint Checksum Analysis ]--------------------------------------------------------\n");

				/*
				 * search: *** Multipoint Checksum bytecode sequence #1 variant #A : Multipoint number of entries ***
				 */
				unsigned char *get_offset_addr = 0;
				
				printf("\n>>> Scanning for Multipoint Checksum sub-routine #1 Variant A [to extract number entries in stored checksum list in ROM] \n");
				addr = search( fh, (unsigned char *)&needle_4b, (unsigned char *)&mask_4b, sizeof(needle_4b), 0 );
				if(addr != NULL) {
					printf("Found at offset=0x%x.\n",(int)(addr-offset_addr) );
					get_offset_addr = ((unsigned char *)addr + 42);
				} else {

					// if not found try alternative variant
					printf("\n>>> Scanning for Multipoint Checksum sub-routine #1 Variant B [to extract number entries in stored checksum list in ROM] \n");
					addr = search( fh, (unsigned char *)&needle_4c, (unsigned char *)&mask_4c, sizeof(needle_4c), 0 );
					if(addr != NULL) {
						printf("Found at offset=0x%x.\n",(int)(addr-offset_addr) );
						get_offset_addr = ((unsigned char *)addr + 44);
					}
				}

				if(get_offset_addr != 0) {
					// extract number of multipoint entries from needle_4bb, byte offset 44
					num_multipoint_entries_byte = get16(get_offset_addr);
					if(num_multipoint_entries_byte > 0) {
						printf("Found #%d Multipoint Entries in table\n", num_multipoint_entries_byte);
					} else {
						printf("Unable to determine number of entries. Please Contact Developer!\n");
					}				
				}


//-[ Multipoint <Stored Checksums> ] -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------			

				/*
				 * search: *** Multipoint Checksum bytecode sequence #2b : Multipoint  stored HI/LO checksum list ***
				*/
				int lo_num_bits, hi_num_bits, skip_factor;
				unsigned char *seg_addr, *lo_addr, *hi_addr;
				int do_multipoint = 0;

				printf("\n>>> Scanning for Multipoint Checksum sub-routine #2 Variant A [to extract address of stored checksum list location in ROM] \n");
				addr = search( fh, (unsigned char *)&needle_4, (unsigned char *)&mask_4, sizeof(needle_4), 0 );
				if(addr != NULL) {
					printf("Found at offset=0x%x.\n",(int)(addr-offset_addr) );
					lo_num_bits = 32;
					hi_num_bits = 0;
					seg_addr    = addr+58;
					lo_addr     = addr+54;
					hi_addr     = 0;
					skip_factor = 0;
					do_multipoint = 1;
				} else {

					// if not found try alternative variant
					printf("\n>>> Scanning for Multipoint Checksum sub-routine #2 Variant B [to extract address of stored checksum list location in ROM] \n");
					addr = search( fh, (unsigned char *)&needle_4aa, (unsigned char *)&mask_4aa, sizeof(needle_4aa), 0 );
					if(addr != NULL) {
						printf("Found at offset=0x%x.\n",(int)(addr-offset_addr) );
						
						lo_num_bits = 16;
						hi_num_bits = 16;
						seg_addr    = addr+24;
						lo_addr     = addr+28;
						hi_addr     = addr+32;
						skip_factor = 32;				// skip past 1st 2 entries which are actually CRC32's in table
						do_multipoint = 1;

						num_multipoint_entries_byte -= 2;						
						printf("***Experimental***: Note Variant #B has 2 crc32's in the table before the multipoints. Skipping the CRC's and just doing the multipoints..\n");
					}				
				}
				
				if(do_multipoint == 1)
				{
					int j, good=0, bad=0;
//					int nCalcCRC;
					for(i=0,j=1; j<= num_multipoint_entries_byte; i=i+16) 
					{
							// address of rom_table [8 bytes] -- Region [i]: start,end
							printf("\nBlk #%-2.2d: ",j++);		
							long int range;
							start_addr           = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, lo_addr, lo_num_bits, hi_addr, hi_num_bits,(int)seg_addr, i+0+skip_factor);		// extract 'start address' directly from identified multippoint table
							masked_start_addr    = start_addr;
							masked_start_addr   &= ~(ROM_1MB_MASK);
							printf("Start:   0x%-8.8lx (offset: 0x%-8.8lx)",(long int)start_addr,(long int)masked_start_addr );
							
							end_addr             = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, lo_addr, lo_num_bits, hi_addr, hi_num_bits,(int)seg_addr, i+4+skip_factor);		// extract 'end address  ' directly from identified multippoint table
							masked_end_addr      = end_addr;
							masked_end_addr     &= ~(ROM_1MB_MASK);

							// perform calculation sum for the given multipoint range
							if(masked_start_addr < masked_end_addr) {
								sum       = CalcChecksumBlk(fh, masked_start_addr, masked_end_addr);
							} else if(masked_start_addr == masked_end_addr) {
								sum       = 0;
							}
							// extract from rom original stored checksums
							printf(" End:    0x%-8.8lx (offset: 0x%-8.8lx)",(long int)end_addr, (long int)masked_end_addr );
							
							checksum_norm = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, lo_addr, lo_num_bits, hi_addr, hi_num_bits,(int)seg_addr, i+8+skip_factor);		// extract 'checksum'      directly from identified multippoint table
							printf(" Chksum: 0x%-8.8lx :  Calc: 0x%-8.8lx ",(long int)checksum_norm, (long int)sum );		

							// did the stored match the one we just calculated? (for normal version)
							if(checksum_norm == sum) { printf("OK"); } else { printf("BAD! "); } 
							checksum_comp = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, lo_addr, lo_num_bits, hi_addr, hi_num_bits,(int)seg_addr, i+12+skip_factor);		// extract '~checksum'     directly from identified multippoint table
							printf("~Chksum: 0x%-8.8lx : ~Calc: 0x%-8.8lx ",(long int)checksum_comp, (long int)~sum );								

							// did the stored match the one we just calculated? (for one's complement version)
							if(checksum_comp == ~sum) { printf("OK"); good++; } else { printf("BAD! "); bad++; } 
					}

					if(good == num_multipoint_entries_byte) {
						printf("\n\nAll Multipoint checksums are correct.\n");
					} else {
						// only show percentages if not all multipoints passed
						printf("\n\nTotal Multipoint Checksums Passed: %d (%d%%)\n", good, (good*100)/(((num_multipoint_entries_byte*100)/100)) );
						printf("Total Multipoint Checksums Failed: %d (%d%%)\n", bad,  (bad*100)/(((num_multipoint_entries_byte*100)/100)) );
						printf("\n");
					}

				}
		
//-[ Seedkey Version #1 ] -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------			
			/*
			 * search: *** Seed/Key Check Patch #1 ***
			 */
			printf("\n-[ SeedKey Security Access ]-------------------------------------------------------------\n");
			 
			if(seedkey_patch == 1) {
				printf("\n>>> Scanning for SecurityAccessBypass() Variant #1 Checking sub-routine [allow any login seed to pass] \n");
				addr = search( fh, (unsigned char *)&needle_5, (unsigned char *)&mask_5, sizeof(needle_5), 0 );
				if(addr != NULL) {
					seedkey_patch = 2;
					printf("Found at offset=0x%x. ",(int)(addr-offset_addr) );
					printf("Applying patch so any login seed is successful... ");
					addr[0x5d] = 0x14; 
					printf("Patched!\n");
				}
				printf("\n");
			} 
			
//-[ Seedkey Version #2 ] -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------			
			/*
			 * search: *** Seed/Key Check Patch #2 ***
			 */

			if(seedkey_patch == 1) {
				printf("\n>>> Scanning for SecurityAccessBypass() Variant #2 Checking sub-routine [allow any login seed to pass] \n");
				addr = search( fh, (unsigned char *)&needle_6, (unsigned char *)&mask_6, sizeof(needle_6), 0 );
				if(addr != NULL) {
					printf("Found at offset=0x%x. ",(int)(addr-offset_addr) );
					printf("Applying patch so any login seed is successful... ");
					addr[0x64] = 0x14; 
					printf("Patched!\n");
				}
				printf("\n\n");
			}

//-[ XorChecksumCalc Version #1 ] -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------			
			/*
			 * search: *** XorChecksumCalc #1 ***
			 */

			printf("-[ XorChecksumCalc ]---------------------------------------------------------------------\n\n");
			{
				int xor_table_hi;
				int xor_table_lo;
				unsigned long xor_table_addr;
				
				printf(">>> Scanning for XorChecksumCalc() Variant #1 Checking sub-routine [calculates xor from table] \n");
				addr = search( fh, (unsigned char *)&xor_needle, (unsigned char *)&xor_mask, sizeof(xor_needle), 0 );
				if(addr != NULL) {
					printf("Found at offset=0x%x. ",(int)(addr-offset_addr) );
					xor_table_lo   = (get16((unsigned char *)addr + 38));	
					xor_table_hi    = (get16((unsigned char *)addr + 42));	
					xor_table_addr  = (unsigned long)(xor_table_hi << 16 | xor_table_lo);
					xor_table_addr &= ~(ROM_1MB_MASK);
					
					printf("xor-table_located at: 0x%-4.4x%-4.4x file offset: %-8.8x\n",(int)xor_table_hi,(int)xor_table_lo, xor_table_addr );

					printf("\nunsigned short XorTable_%x[%d] = {\n", xor_table_addr, 256);
					hexdump_le32_table(offset_addr + xor_table_addr, 1024, "};\n");					
				}
				printf("\n\n");
			}

//-[ Ferrari HFM Version #1 ] -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------			
			/*
			 * search: *** HFM Linearization code sequence ***
			 */

			printf("-[ Ferrari AirFlow Meter HFM ]-----------------------------------------------------------\n\n");

			printf(">>> Scanning for HFM Linearization Table Lookup code sequence... \n");
			addr = search( fh, (unsigned char *)&needle_1, (unsigned char *)&mask_1, sizeof(needle_1), 0 );
			if(addr == NULL) {
				printf("\nhfm sequence not found\n\n");
			} else {
				/* this offset is the machine code for the check against last entry in HFM table
				 * if we extract it we know how many entries are in the table. 
				 */ 
				entries = get16(addr+4); /* using endian conversion, get XXXX offset from 'cmp r12 #XXXXh' : part of GGHFM_lookup(); */
				/* sanity check for MAX_ENTRIES that we expect to see.. */
				if(entries != 0)
				{
					if(entries > MAX_DHFM_ENTRIES) { printf("unusual entries size, defaulting to 512"); entries=DEFAULT_DHFM_ENTRIES; };
					
					/* this offset refers to the MAP storage for HFM linearization table
					 * if we extract it we know precisly where our HFM  table is located in the firmware dump
                     */					
					offset = get16(addr+14); /* using endian conversion, get XXXX offset from 'mov r5, [r4 + XXXX]' : part of GGHFM_lookup(); */

					/* lets show the sequence we looked for and found
					 * in hex (this is the machine code sequence for the GGHFM_DHFM_Lookup() function in the firmware image
					 */
					printf("\n\nFound GGHFM_DHFM_Lookup() instruction sequence at file offset: 0x%x, len=%d\n", addr-(fh->d.u8), sizeof(needle_1) );			
					hexdump(addr, sizeof(needle_1), " ");

					printf("\nExtracted MLHFM map table offset from mov instruction = 0x%x (endian compliant)\n",offset);
					printf("Extracted %d table entries from code.\n",entries);
					
					printf("\nFile offset to MLHFM table 0x%x (%d) [%d bytes]\n",(MAP_FILE_OFFSET + offset),(MAP_FILE_OFFSET + offset), entries*2 );
					
					uint32_t crc_hfm;
					crc_hfm = crc32(0, fh->d.p + MAP_FILE_OFFSET + offset, entries*2);

					if(mode == HFM_WRITING)
					{
						/* load in MLHFM table from a file */
						load_hfm_result = iload_file(fh_hfm, filename_hfm, 0);
						if(load_hfm_result == 0) 
						{
							if(fh_hfm->len != 1024) {
								ifree_file(fh_hfm);
								printf("MLHFM table is the wrong size, cannot continue. Exiting. Are you sure its a MLHFM table?");
								return 0;
							}
							printf("Correctly loaded in an MLHFM file '%s'\n", filename_hfm);
							
							uint32_t crc_hfm_file;
							crc_hfm_file = crc32(0, fh_hfm->d.p, 1024);
							
							if(crc_hfm_file == 0x4200bc1)			/* crc32 checksum of MLHFM 1024byte table */
							{
								printf("MLHFM Table Identified in loaded file: Ferrari 360 Modena/Spider/Challenge (Stock) Air Flow Meters\n");						
							} else if(crc_hfm_file == 0x87b3489a)	/* crc32 checksum of MLHFM 1024byte table */
							{
								printf("MLHFM Table Identified in loaded file: Ferrari 360 Challenge Stradale (Stock) Air Flow Meters\n");
							} else {
								printf("MLHFM Table Not Identified in loaded file: Custom or not an MLHFM file!\n");
							}
							
							/* check if firmware ALREADY matches the loaded in MLHFM table */
							if(crc_hfm == crc_hfm_file) {
								printf("\nMLHFM Table already IDENTICAL in the rom specified. Nothing to do here...\n");
								return 0;
							}

							/* copying hfm table from file into rom image in memory */
							printf("\nMerging MLHFM table into rom...\n");
							memcpy(fh->d.p + MAP_FILE_OFFSET + offset, fh_hfm->d.p, fh_hfm->len);

							/* save it.. */
							snprintf(newrom_filename, MAX_FILENAME, "%s_patched.bin", filename_rom);
							printf("\nSaving modified rom to '%s'...\n", newrom_filename);
							save_result = save_file(newrom_filename, fh->d.p, fh->len );
 
							printf("\nAll done.\n");
						}
						return 0;
					}


					if(filename_hfm != 0)
					{
						snprintf(ml_filename, MAX_FILENAME, "%s_%x.bin", filename_hfm, crc_hfm);
						printf("Saving MLHFM filename as '%s'\n", ml_filename);
					}
					else 
					{
						/*
						 * create filename for table (lets try to identify this MLHFM first
						 */
						if(crc_hfm == 0x4200bc1)			// crc32 checksum of MLHFM 1024byte table
						{
							printf("MLHFM Table Identified: Ferrari 360 Modena/Spider/Challenge (Stock) Air Flow Meters\n");						
							snprintf(ml_filename, MAX_FILENAME, "MLHFM_Modena_%x.bin", crc_hfm);
						} else if(crc_hfm == 0x87b3489a)	// crc32 checksum of MLHFM 1024byte table
						{
							printf("MLHFM Table Identified: Ferrari 360 Challenge Stradale (Stock) Air Flow Meters\n");
							snprintf(ml_filename, MAX_FILENAME, "MLHFM_Stradale_%x.bin", crc_hfm);
						}
					}
		
					/*
					 * only try to save MLHFM table to file if in DUMPING/READING mode..
					 */
					if(mode == HFM_READING)
					{
						printf("Saving raw MLHFM table (dumped with no endian conversion) to file: '%s'\n\n", ml_filename);
						save_result = save_file(ml_filename, fh->d.p + MAP_FILE_OFFSET + offset, entries*2 );
						if(save_result) {
							printf("\nFailed to save, result = %d\n", save_result);
						}

						// get offset
						printf("unsigned short MLHFM_%x[%d] = {\n", crc_hfm, entries);
						hexdump_le_table(fh->d.p + MAP_FILE_OFFSET + offset, entries, "};\n");					
	

					}
				} else {
					printf("MLHFM not found. Probaby a matching byte sequence but not in a firmware image");
				}
			}

		} else {
			printf("File size isn't a supported firmware size. Only 512kbyte and 1Mb images supported. ");
		}
/*------------------------------------------------------------------------------------------------------------------*/
	} else {
		printf("\nFailed to load, result = %d\n", load_result);
	}
	/* free file if allocated */
	load_result = ifree_file(fh);
	printf("---\n");
	return 0;
}

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
#include "fixsums.h"
#include "needles.h"
#include "utils.h"

extern int correct_checksums;	// 0 or 1
extern int force_write;			// 0 or 1
extern char *save_name;			// filename

/*
 * Calculate the Bosch Motronic ME71 checksum for the given range
 */
uint32_t CalcChecksumBlk(struct ImageHandle *ih, uint32_t start, uint32_t end)
{
	uint32_t sum=0, i;
	for(i = start/2; i <= end/2; i++) { 	
		sum += le16toh( ih->d.u16[i] );
	}
	return sum;
}

int fix_checksums(ImageHandle *fh, unsigned char *addr, char *filename_rom, unsigned long dynamic_ROM_FILESIZE, unsigned char *offset_addr)
{
	char newrom_filename[MAX_FILENAME];
	int num_entries = 0;
	int num_multipoint_entries_byte;
	uint32_t checksum_norm;
	uint32_t checksum_comp;
	uint32_t i, sum, final_sum=0;
	unsigned long masked_start_addr=0;
	unsigned long masked_end_addr=0;
	unsigned long start_addr=0;
	unsigned long end_addr=0;
	unsigned long last_end_addr=0;
	int corrected=0;
	int fixed=0;
	int exists;
	int save_result;

//-[ Checksum Correction ] -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------			

		if(correct_checksums == OPTION_SET)
		{

			//-[ CRC32_ChecksumCalc Version #1 ] -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------			
			/*
			 * search: *** CRC32_ChecksumCalc Routine #1 ***
			 */

			printf("-[ CRC32_ChecksumCalc ]---------------------------------------------------------------------\n\n");
			{
				int crc32_table_hi;
				int crc32_table_lo;
				unsigned long crc32_table_addr;
				
				printf(">>> Scanning for CRC32_ChecksumCalc() Variant #1 Checking sub-routine [calculates crc32 from polynomial table] \n");
				addr = search( fh, (unsigned char *)&crc32_needle, (unsigned char *)&crc32_mask, crc32_needle_len, 0 );
				if(addr != NULL) {
					printf("Found at offset=0x%x. ",(int)(addr-offset_addr) );
					crc32_table_lo   = (get16((unsigned char *)addr + 38));	
					crc32_table_hi    = (get16((unsigned char *)addr + 42));	
					crc32_table_addr  = (unsigned long)(crc32_table_hi << 16 | crc32_table_lo);
					crc32_table_addr &= ~(ROM_1MB_MASK);
					
					printf("CRC32 Polynomial Table located at: 0x%-4.4x%-4.4x file offset: %-8.8x\n",(int)crc32_table_hi,(int)crc32_table_lo, (int)crc32_table_addr );

					printf("\nstatic uint32_t crc32_table_addr[%d] = {\n", 256);
					hexdump_le32_table(offset_addr + crc32_table_addr, 1024, "};\n");					
				}
				printf("\n\n");
			}

			//-[ MAINROM <Number of Entries> ] -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------			
			printf("\n\n-[ Main-Rom Checksum Analysis ]----------------------------------------------------------\n\n");
			
			/*
			 * search: *** Main Rom Checksum bytecode sequence #1 ***
			 */
			printf(">>> Scanning for Main ROM Checksum sub-routine #1 [to extract number of entries in table] ");
			addr = search( fh, (unsigned char *)&needle_2b, (unsigned char *)&mask_2b, needle_2b_len, 0 );
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
					printf("\n>> Main Region Block #%d: ",i+1);  fflush(0);		
					start_addr = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, addr+18+00, 16, addr+22+00, 16, (int)addr+14, i*8);		// extract 'start address' directly from identified checksum machine code
					printf("0x%lx",(long int)start_addr ); fflush(0);
					start_addr &= ~(ROM_1MB_MASK); 
					printf("0x%lx",(long int)start_addr );	 fflush(0);	
					
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
			addr = search( fh, (unsigned char *)&needle_2, (unsigned char *)&mask_2, needle_2_len, 0 );
			if(addr == NULL) {
				printf("\nmain checksum byte sequence not found\nGiving up.\n");
			} else {
				printf("\nmain checksum byte sequence #2 found at offset=0x%x.\n",(int)(addr-offset_addr) );fflush(0);
				final_sum = 0;
				for(i=0;i < num_entries;i++) 
				{
					// address of rom_table [8 bytes] -- Region [i]: start,end
					printf("\n>>> Main Region Block #%d: ",i+1); fflush(0);		
					start_addr = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, addr+18+00, 16, addr+22+00, 16, addr+14, i*8);		// extract 'start address' directly from identified checksum machine code
					start_addr &= ~(ROM_1MB_MASK);
					printf("\n\t Start Addr : 0x%-8.8x", start_addr ); fflush(0);
				
					last_end_addr = end_addr;
					end_addr   = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, addr+18+26, 16, addr+22+26, 16, addr+14, i*8);		// extract 'end address' directly from identified checksum machine code
					end_addr &= ~(ROM_1MB_MASK);
					printf(" End Addr   : 0x%-8.8x", end_addr );fflush(0);

					// calculate checksum for this block
					sum = CalcChecksumBlk(fh,start_addr,end_addr);
					printf("\n\t sum=%lx ~sum=%lx : acc_sum=%lx", (unsigned long)sum, (unsigned long)~sum, (unsigned long)final_sum);

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
			addr = search( fh, (unsigned char *)&needle_3, (unsigned char *)&mask_3, needle_3_len, 0 );
			if(addr == NULL) {
				printf("\nmain checksum byte sequence #3 variant #A not found\nTrying different variant.\n");

				printf("\n>>> Scanning for (!) Main ROM Checksum sub-routine #3 variant #B [to extract stored checksums and locations in ROM] ");
				addr = search( fh, (unsigned char *)&needle_3b, (unsigned char *)&mask_3b, needle_3b_len, 0 );
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
			
			} 
			else 
			{
				unsigned char *adr_chksum_norm;
				unsigned char *adr_chksum_comp;
				int bad_main=0;

				printf("\nmain checksum byte sequence #3 block found at > offset=%p.\n",(long)(addr-offset_addr) );
//				checksum_norm = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, addr+14+00, 16, addr+18+00, 16, (int)addr+10, 0);		// start
//				checksum_comp = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, addr+14+00, 16, addr+18+00, 16, (int)addr+10, 4);		// start

				adr_chksum_norm = (unsigned char *)get_addr16_of_from_rom(offset_addr, dynamic_ROM_FILESIZE, addr+14+00, addr+10, 0);
				adr_chksum_comp = (unsigned char *)get_addr16_of_from_rom(offset_addr, dynamic_ROM_FILESIZE, addr+14+00, addr+10, 4);

				checksum_norm   = (unsigned long)get32(adr_chksum_norm);
				checksum_comp   = (unsigned long)get32(adr_chksum_comp);
				
				printf("MAIN STORED ROM  CHECKSUM: 0x%-8.8lx (calc) ? 0x%-8.8lx (stored) : ",(long)final_sum, (long)checksum_norm);
				if(final_sum == checksum_norm)  { printf("OK!\t"); bad_main=0; } else {printf("BAD!\t"); bad_main=1; }
				printf(" ~CHECKSUM: 0x%-8.8lx ? 0x%-8.8lx : ",(long)~final_sum, (long)checksum_comp);
				if(~final_sum == checksum_comp) { printf("OK!\n"); bad_main=0; } else {printf("BAD!\n"); bad_main=1; }

				// where the main rom checksum incorrect?
				if(bad_main == 1) {
					printf("***CORRECTING STORED MAINROM CHECKSUMS... \n");
					unsigned int *adr_chksum_norm_int = (unsigned int *)adr_chksum_norm;
					unsigned int *adr_chksum_comp_int = (unsigned int *)adr_chksum_comp;
					corrected++;
					
					// update main rom checksums them
					*adr_chksum_norm_int =  final_sum;
					*adr_chksum_comp_int = ~final_sum;
					
					// now read them back..
					checksum_norm   = (unsigned long)get32(adr_chksum_norm);
					checksum_comp   = (unsigned long)get32(adr_chksum_comp);
							
					printf("MAIN STORED ROM  CHECKSUM: 0x%-8.8lx (calc) ? 0x%-8.8lx (stored) : ",(long)final_sum, (long)checksum_norm);
					if(final_sum == checksum_norm)  { printf("OK!\t"); } else {printf("BAD!\t"); }
					printf(" ~CHECKSUM: 0x%-8.8lx ? 0x%-8.8lx : ",(long)~final_sum, (long)checksum_comp);
					if(~final_sum == checksum_comp) { printf("OK!\n"); } else {printf("BAD!\n"); }
				}
				
			}
		
			//-[ Multipoint <Number of Table Entries> ] -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------			

				printf("\n\n-[ Multipoint Checksum Analysis ]--------------------------------------------------------\n");

				/*
				 * search: *** Multipoint Checksum bytecode sequence #1 variant #A : Multipoint number of entries ***
				 */
				unsigned char *get_offset_addr = 0;
				
				printf("\n>>> Scanning for Multipoint Checksum sub-routine #1 Variant A [to extract number entries in stored checksum list in ROM] \n");
				addr = search( fh, (unsigned char *)&needle_4b, (unsigned char *)&mask_4b, needle_4b_len, 0 );
				if(addr != NULL) {
					printf("Found at offset=0x%x.\n",(int)(addr-offset_addr) );
					get_offset_addr = ((unsigned char *)addr + 42);
				} else {

					// if not found try alternative variant
					printf("\n>>> Scanning for Multipoint Checksum sub-routine #1 Variant B [to extract number entries in stored checksum list in ROM] \n");
					addr = search( fh, (unsigned char *)&needle_4c, (unsigned char *)&mask_4c, needle_4c_len, 0 );
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
				unsigned char *adr_checksum_norm;
				unsigned char *adr_checksum_comp;
				int do_multipoint = 0;

				printf("\n>>> Scanning for Multipoint Checksum sub-routine #2 Variant A [to extract address of stored checksum list location in ROM] \n");
				addr = search( fh, (unsigned char *)&needle_4, (unsigned char *)&mask_4, needle_4_len, 0 );
				if(addr != NULL) {
					printf("Found at offset=0x%x.\n",(int)(addr-offset_addr) );
					lo_num_bits = 32;
					hi_num_bits = 0;
					seg_addr    = addr+58;
					lo_addr     = addr+54;
					hi_addr     = 0;
					skip_factor = 0;
					do_multipoint = 1;			// only try multipoint checks if we find the needle!
				} else {

					// if not found try alternative variant
					printf("\n>>> Scanning for Multipoint Checksum sub-routine #2 Variant B [to extract address of stored checksum list location in ROM] \n");
					addr = search( fh, (unsigned char *)&needle_4aa, (unsigned char *)&mask_4aa, needle_4aa_len, 0 );
					if(addr != NULL) {
						printf("Found at offset=0x%x.\n",(int)(addr-offset_addr) );
						
						lo_num_bits = 16;
						hi_num_bits = 16;
						seg_addr    = addr+24;
						lo_addr     = addr+28;
						hi_addr     = addr+32;
						skip_factor = 32;		// skip past 1st 2 entries which are actually CRC32's in table
						do_multipoint = 1;		// only try multipoint checks if we find the needle!

						num_multipoint_entries_byte -= 2;						
						printf("***Experimental***: Note Variant #B has 2 crc32's in the table before the multipoints. Skipping the CRC's and just doing the multipoints..\n");
					}				
				}
								
				if(do_multipoint == 1)	// only try multipoint checks if we found the needles!
				{
					int j, good=0, bad=0;
//					int nCalcCRC;
					for(i=0,j=1; j<= num_multipoint_entries_byte; i=i+16) 
					{
							// address of rom_table [8 bytes] -- Region [i]: start,end
							printf("\nMultipoint Block #%-2.2d of #%-2.2d: ",j++, num_multipoint_entries_byte);	fflush(0);	
							long int range;
							start_addr           = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, lo_addr, lo_num_bits, hi_addr, hi_num_bits, seg_addr, i+0+skip_factor);		// extract 'start address' directly from identified multippoint table
							masked_start_addr    = start_addr;
							masked_start_addr   &= ~(ROM_1MB_MASK);
							printf("\n\t Start:   seg:0x%-3.3x phy:0x%-8.8lx (offset: 0x%-8.8lx)",start_addr/SEGMENT_SIZE,start_addr, masked_start_addr );
							
							end_addr             = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, lo_addr, lo_num_bits, hi_addr, hi_num_bits, seg_addr, i+4+skip_factor);		// extract 'end address  ' directly from identified multippoint table
							masked_end_addr      = end_addr;
							masked_end_addr     &= ~(ROM_1MB_MASK);

							// perform calculation sum for the given multipoint range
							if(masked_start_addr < masked_end_addr) {
								sum       = CalcChecksumBlk(fh, masked_start_addr, masked_end_addr);
							} else if(masked_start_addr == masked_end_addr) {
								sum       = 0;
							}
							// extract from rom original stored checksums
							printf("\n\t End:     seg:0x%-3.3x phy:0x%-8.8lx (offset: 0x%-8.8lx) ",end_addr/SEGMENT_SIZE,end_addr, masked_end_addr );
							
//							checksum_norm     = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, lo_addr, lo_num_bits, hi_addr, hi_num_bits,(int)seg_addr, i+8+skip_factor);		// extract 'checksum'      directly from identified multippoint table
							adr_checksum_norm = get_addr16_of_from_rom(offset_addr, dynamic_ROM_FILESIZE, lo_addr, seg_addr, i+8+skip_factor);
							checksum_norm     = get32(adr_checksum_norm);

							printf("\n\t Block Checksum: 0x%-8.8lx :  Calculated: 0x%-8.8lx ", checksum_norm, sum ); fflush(0);		

							// did the stored match the one we just calculated? (for normal version)
							if(checksum_norm == sum) { 
								printf("OK"); good++;
							} else { 
								bad++;
								// update the checksum
								unsigned int *adr_checksum_norm_int = (unsigned int *)adr_checksum_norm;
								*adr_checksum_norm_int            =  sum;							
								// reacquire checksum from rom
								checksum_norm     = get32(adr_checksum_norm);
								printf("FIXED!"); 
								fixed++;
							} 

//							checksum_comp     = get_addr_from_rom(offset_addr, dynamic_ROM_FILESIZE, lo_addr, lo_num_bits, hi_addr, hi_num_bits,(int)seg_addr, i+12+skip_factor);		// extract '~checksum'     directly from identified multippoint table
							adr_checksum_comp = get_addr16_of_from_rom(offset_addr, dynamic_ROM_FILESIZE, lo_addr, seg_addr, i+12+skip_factor);
							checksum_comp     = get32(adr_checksum_comp);

							printf("\n\t~Block Checksum: 0x%-8.8lx : ~Calculated: 0x%-8.8lx ",checksum_comp, ~sum );								

							// did the stored match the one we just calculated? (for one's complement version)
							if(checksum_comp == ~sum) { 
								printf("OK"); good++; 
							} else { 
//								printf("BAD! "); 
								bad++; 
								// update the checksum
								unsigned int *adr_checksum_comp_int = adr_checksum_comp;
								*adr_checksum_comp_int            =  ~sum;							
								// reacquire checksum from rom
								checksum_comp     = get32(adr_checksum_comp);
								printf("FIXED!"); 
								fixed++;
							} 


					}

					if(good == num_multipoint_entries_byte*2) {
						printf("\n\nAll Multipoint checksums are correct.\n");
					} else {
						// only show percentages if not all multipoints passed
						printf("\n\nTotal Multipoint Checksums Passed : %3d (~%d%%)\n", good, (good*100)/((((num_multipoint_entries_byte*2)*100)/100)) );
						printf("Total Multipoint Checksums Failed : %3d (~%d%%)\n", bad,  (bad*100)/((((num_multipoint_entries_byte*2)*100)/100)) );
						printf("Total Multipoint Checksums Fixed  : %3d (~%d%%)\n", fixed,  (fixed*100)/((((num_multipoint_entries_byte*2)*100)/100)) );
						printf("\n");
					}

				}

				if(corrected >0 || fixed > 0) {
					snprintf(newrom_filename, MAX_FILENAME, "%s_corrected.bin", filename_rom);
					
					printf("\nAttempting to save corrected rom to ");
					if(save_name != 0) {
						printf("'%s'...\n", save_name);
					} else {
						printf("'%s'...\n", newrom_filename);
					}
					
					// check if file exists so we don't overwrite it!
					if(save_name != 0) {
						if( CheckFileExist(save_name)) {
							printf("File already exists.\n");
							exists = 1;
						} else {
							exists = 0;
						}
					} else {
						
						if( CheckFileExist(newrom_filename)) {
							printf("File already exists.\n");
							exists = 1;
						} else {
							exists = 0;
						}
					}
					
					// added support for forced overwriting
					if(exists == 0 || force_write == 1)
					{
						if(force_write == 1) {
							printf("Overwriting old file (-force). ");
						}
						
						if(save_name != 0) {
							printf("Overriding default save filename to: '%s'\n",save_name);
							save_result = save_file(save_name, fh->d.p, fh->len );						
						} else {
							save_result = save_file(newrom_filename, fh->d.p, fh->len );						
						}

						if(save_result == 0) {
							printf("Save completed OK.\n");
						} else {
							printf("Error: Failed to save file. Check permissions!\n");
						}
					}
				}
			}	
		return 0;
}


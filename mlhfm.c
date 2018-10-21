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
#include "mlhfm.h"
#include "table_spec.h"

extern int show_diss;
extern unsigned dpp1_value;

int check_mlhfm(ImageHandle *fh, int skip)
{
	MPTR _mlhfm;
	int found = 0, byte_offset, second_offset, offset=0;
	char *rom_load_addr = fh->d.p;
	char *addr;
	int current_offset = ROM_START_OFFSET;
	int new_offset=0;
	int mlhfm_val;
	int entries=512;
	int false_positive=0;
	uint32_t crc_hfm;
	unsigned char needle_tmp[] = { 0xE6, XXXX, 0xFF, 0xFF, 0xF0, 0xBA };     // mov     r10, #0FFFFh, mov 
	unsigned char mask_tmp[]   = { MASK, XXXX, MASK, MASK, MASK, MASK };     // mov     r10, #0FFFFh, mov

	if(skip == 0) return found;		

	printf("-[ AirFlow Meter MLHFM ]----------------------------------------------------------------\n\n");
	printf(">>> Scanning for a partial MLHFM Linearization Table Lookup code sequence... \n");

	while(1)
	{
		current_offset = search_offset(rom_load_addr+new_offset, (fh->len)-new_offset, (unsigned char *)&needle_1, (unsigned char *)&mask_1, needle_1_len);
		if(current_offset == 0) break;
		if(current_offset != NULL) {
//			printf("\nPartial match found at: offset=0x%x \n",(int)(current_offset+new_offset) );
			addr = rom_load_addr+current_offset+new_offset;
			// disassemble needle found in rom
			if(show_diss) { c167x_diss(addr-rom_load_addr, addr, 0x50); }

			// within 64 bytes of the partial match we expect to find a 'mov r10, #0FFFFh
			second_offset = search_offset( addr, 0x50, (unsigned char *)&needle_tmp, (unsigned char *)&mask_tmp, 6);
			if(second_offset == 0 ) 
			{ 
				false_positive++;				// we only found a partial match, so its not going to be the mlhfm table we really want, likely referencing some other table!!!
			}  else { 
				printf("Fuzzy match found!\n"); 

				translate_seg(&_mlhfm, "MLHFM", rom_load_addr, dpp1_value-1 /*seg*/, get16((unsigned char *)addr+4) /*val*/);
				show_seg(&_mlhfm);

				crc_hfm = crc32(0, _mlhfm.ram, entries*2);
				if(crc_hfm == 0x4200bc1)			// crc32 checksum of MLHFM 1024byte table
				{
					printf("\nMLHFM Table Identified: Ferrari 360 Modena/Spider/Challenge (Stock) Air Flow Meters\n");						
				} else if(crc_hfm == 0x87b3489a)	// crc32 checksum of MLHFM 1024byte table
				{
					printf("\nMLHFM Table Identified: Ferrari 360 Challenge Stradale (Stock) Air Flow Meters\n");
				}

				printf("\nunsigned short MLHFM_%X[%d] = {\n", crc_hfm, entries);
				hexdump_le_table(_mlhfm.ram, entries, "};\n");			
				found=1;
				break;
			}
			new_offset += current_offset+needle_1_len;
		}
		// next loop around...
	}

	printf("\nFalse positives detected (and eliminated): %d\n", false_positive);
	printf("Found status: %d",found);
	
	if(found == 0) {

		printf("\n>>> Scanning for full MLHFM Linearization Table Lookup code sequence - Variant #2... ");

		new_offset = 0;	
		while(1)
		{
			printf("\nsearch...\n");
			current_offset = search_offset(rom_load_addr+new_offset, (fh->len)-new_offset, (unsigned char *)&needle_mlhfm, (unsigned char *)&mask_mlhfm, needle_mlhfm_len);
			if(current_offset == 0) break;
			if(current_offset != NULL) 
			{
				printf("\nMatch found at: offset=0x%x \n",(int)(current_offset+new_offset) );		
				addr = rom_load_addr+current_offset+new_offset;
				// disassemble needle found in rom
				if(show_diss) { c167x_diss(addr-rom_load_addr, addr, 64); }
				translate_seg(&_mlhfm, "MLHFM", rom_load_addr, dpp1_value-1 /*seg*/, get16((unsigned char *)addr+10) /*val*/);
				show_seg(&_mlhfm);

				crc_hfm = crc32(0, _mlhfm.ram, entries*2);
				if(crc_hfm == 0x4200bc1)			// crc32 checksum of MLHFM 1024byte table
				{
					printf("\nMLHFM Table Identified: Ferrari 360 Modena/Spider/Challenge (Stock) Air Flow Meters\n");						
				} else if(crc_hfm == 0x87b3489a)	// crc32 checksum of MLHFM 1024byte table
				{
					printf("\nMLHFM Table Identified: Ferrari 360 Challenge Stradale (Stock) Air Flow Meters\n");
				}
				printf("\nunsigned short MLHFM_%X[%d] = {\n", crc_hfm, entries);
				hexdump_le_table(_mlhfm.ram, entries, "};\n");			
				found=1;
				break;
			}
			new_offset += current_offset+needle_1_len;
		}
		printf("\nFound status: %d",found);
		
	}

	return found;
}

int check_mlhfm2(ImageHandle *fh, unsigned char *addr, char *filename_rom, char *filename_hfm, unsigned long dynamic_ROM_FILESIZE, unsigned char *rom_load_addr)
{
	unsigned short offset,entries;
	int save_result,seq_found=0;
	unsigned short val, seg;
	int load_hfm_result,found,opskip;
	ImageHandle f_hfm;
	ImageHandle *fh_hfm = &f_hfm;
	char ml_filename[MAX_FILENAME];
//	char *rom_load_addr = fh->d.p;
	unsigned int rom_skip_bytes;
	unsigned int byte_offset;

//-[ Ferrari/Porsche HFM Version #1 ] -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------			
			/*
			 * search: *** HFM Linearization code sequence ***
			 */
	if (find_mlhfm != 0)
	{
			printf("\n-[ AirFlow Meter MLHFM ]----------------------------------------------------------------\n\n");
			printf(">>> Scanning for full MLHFM Linearization Table Lookup code sequence... \n");

			addr = search( fh, (unsigned char *)&needle_1q, (unsigned char *)&mask_1q, needle_1q_len, 0 );
			if(addr != NULL) {

				seq_found = 1;
				opskip = 6;//14;
					if(show_diss) {
						c167x_diss(addr-rom_load_addr, addr, 64*10);
					}
//				entries = get16(addr+4); /* using endian conversion, get XXXX offset from 'cmp r12 #XXXXh' : part of GGHFM_lookup(); */
//-				if(entries > MAX_DHFM_ENTRIES) { printf("unusual entries size, defaulting to 512"); entries=DEFAULT_DHFM_ENTRIES; };
			}
			else {
				printf("\nSequence not found\n\n");
				seq_found = 0;
				printf(">>> Scanning for full MLHFM Linearization Table Lookup code sequence - Variant #2... \n");
				addr = search( fh, (unsigned char *)&needle_mlhfm, (unsigned char *)&mask_mlhfm, needle_mlhfm_len, 0 );
				if(addr != NULL) {
					printf("\nSequence Found @ %#x\n", addr);
					entries=DEFAULT_DHFM_ENTRIES;
					if(show_diss) {
						c167x_diss(addr-rom_load_addr, addr, 64);
					}
					seq_found=1;
					opskip=10;
				} else {
					printf("\nSequence not found\n\n");				
					seq_found = 0;
				}

			}
		
			if(seq_found == 1)
			{
				/* this offset is the machine code for the check against last entry in HFM table
				 * if we extract it we know how many entries are in the table. 
				 */ 
				/* sanity check for MAX_ENTRIES that we expect to see.. */
				if(entries != 0)
				{
					
					/* this offset refers to the MAP storage for HFM linearization table
					 * if we extract it we know precisly where our HFM  table is located in the firmware dump
                     */					
					offset = get16(addr+opskip); /* using endian conversion, get XXXX offset from 'mov r5, [r4 + XXXX]' : part of GGHFM_lookup(); */

					/* lets show the sequence we looked for and found
					 * in hex (this is the machine code sequence for the GGHFM_DHFM_Lookup() function in the firmware image
					 */
					printf("\n\nFound GGHFM_DHFM_Lookup() instruction sequence at file offset: 0x%x, len=%d\n", addr-(fh->d.u8), needle_1_len );			

					printf("\nExtracted MLHFM map table offset from mov instruction = 0x%x (endian compliant)\n",offset);
					printf("Extracted %d table entries from code.\n",entries);
					
					printf("\nFile offset to MLHFM table 0x%x (%d) [%d bytes]\n",(MAP_FILE_OFFSET + offset),(MAP_FILE_OFFSET + offset), entries*2 );
					
					uint32_t crc_hfm;
					crc_hfm = crc32(0, fh->d.p + MAP_FILE_OFFSET + offset, entries*2);

						// get offset
						printf("unsigned short MLHFM_%x[%d] = {\n", crc_hfm, entries);
						hexdump_le_table(fh->d.p + MAP_FILE_OFFSET + offset, entries, "};\n");					

					if(find_mlhfm == HFM_WRITING)
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

							// now that we've merged MLHFM, force a checksum re-correction (note: his will automatically re-save!)
							correct_checksums = OPTION_SET;
							// do checksum correction and autosave it.
							fix_checksums(fh, addr, filename_rom, dynamic_ROM_FILESIZE, rom_load_addr);
 
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
					 * only try to save MLHFM table to file if in DUMPING/READING_mlhfm..
					 */
					if(find_mlhfm == HFM_READING)
					{
						printf("Saving raw MLHFM table (dumped with no endian conversion) to file: '%s'\n\n", ml_filename);
						save_result = save_file(ml_filename, fh->d.p + MAP_FILE_OFFSET + offset, entries*2 );
						if(save_result) {
							printf("\nFailed to save, result = %d\n", save_result);
						}

//						// get offset
//						printf("unsigned short MLHFM_%x[%d] = {\n", crc_hfm, entries);
//						hexdump_le_table(fh->d.p + MAP_FILE_OFFSET + offset, entries, "};\n");					
	

					}
				} else {
					printf("MLHFM not found. Probaby a matching byte sequence but not in a firmware image");
				}
			}
		}

}
/* me7romtool [ Firmware analysis tool for Bosch ME7.3H4 Ferrari firmware's]
   By 360trev. Needle lookup function borrowed from nyet (Thanks man!) from
   the ME7sum tool development (see github).
     
   This tool is for analyzing Ferrari Firmwares.

   This version illustrates how to identify MAP table areas directly within a 
   firmware image that 'move around' due to conditional compilation between
   different firmware versions. The basic approach is to identify the code
   block sequences (which are common) and then extract from the machine
   code itself the offsets to the map area where they are stored. This means
   that this approach works across ALL Ferrari 360 firmware dumps. It can
   ofcourse be used to search for ALL map tables. The advantage of doing this
   vs simple byte signatures is that you can extract offsets and information
   directly out of the code. 
	
   This tool supports identification of the MLHFM table which comprises of 
   (typically) 512 entries, each 2 bytes (1024 bytes total) of linearization 
   data for the type of Bosch Air Flow Meters fitted to the Ferrari 360.
 
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
#include "crc32.h"

#define SKIP    0x00
#define XXXX    0x00
#define MASK    0xff

/* GGHFM_Lookup function signature we need to find and match, XXXX can
 * change between firmware releases 
 * 
 *   This is a small snippet of machine-code extract which forms part 
 *   of the wider GGHFM_lookup() rom function. If we match this then we 
 *   have identified the method which utilizes the MLHFM table so we 
 *   just need to pull out the relevant data from the firmware such as 
 *   offset in map region to know precisely where it exists in this 
 *   specific version of the firmware image. Since the offset 'moves 
 *   around' in different versions of the rom we have to do it this way
 *   to guarentee we always match the correct byte offset. 
 * 
 *   The XXXX's are bytes which always change between different compiled 
 *   releases so we have to ignore those bytes (mask them out) during
 *   searches.
 */
const unsigned char needle_1[] = { 
 0x7c, 0x1c,              // shr   r12, #1
 0x46, 0xfc, XXXX, XXXX,  // cmp   r12, #XXXXh
 0x9d, 0x05,              // jmpr  cc_NC, end_of_table
 0xf0, 0x4c,              // mov   r4, r12
 0x5c, 0x14,              // shl   r4, #1
 0xd4, 0x54, XXXX, XXXX   // mov   r5, [r4 + XXXX]    <--- * this offset + 0x10000 [map base in rom] gives offset (instruction is in little endian format so conversion neccesary) in file to MLHFM Linearization 1kbyte (512 entries of 16-bits) table
};

const unsigned char mask_1[] = { 
 MASK, MASK,              // shr   r12, #1
 MASK, MASK, SKIP, SKIP,  // cmp   r12, #200h
 MASK, MASK,              // jmpr  cc_NC, end_of_table
 MASK, MASK,              // mov   r4, r12
 MASK, MASK,              // shl   r4, #1
 MASK, MASK, SKIP, SKIP   // mov   r5, [r4 + 4300]
};

#define HFM_NOTSET   0
#define HFM_READING  1
#define HFM_WRITING  2
#define HFM_IDENTIFY 3

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

	return 0;
}

int main(int argc, char *argv[])
{
    int ok;
	char *hfm_name=NULL;
	char *rom_name=NULL;
    int i,mode=HFM_NOTSET;
	
	printf("Ferrari 360 ME7.3H4 Rom Tool. Last Built: %s %s v1.01\n",__DATE__,__TIME__);
	printf("by 360trev.  Needle lookup function borrowed from nyet (Thanks man!) from\nthe ME7sum tool development (see github). \n\n");

	/*
	 * check which mode to operate in depending on console options specified
	 */

    for (i = 0; i < argc; i++) {
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

#define MAX_DHFM_ENTRIES     1024
#define DEFAULT_DHFM_ENTRIES 512
#define MAP_FILE_OFFSET      0x10000
#define MAX_FILENAME         256
#define ROM_FILESIZE		 512*1024

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
	unsigned char *addr;
	unsigned short offset,entries;
	
	/* load file from storage */
	load_result = iload_file(fh, filename_rom, 0);
	if(load_result == 0) 
	{
		printf("Succeded loading file.\n\n");
		
		/* quick sanity check on firmware rom size (Ferrari 360 images must be 512kbytes) */
		if(fh->len == ROM_FILESIZE) 
		{		
			/*
			 * search: *** HFM Linearization code sequence ***
			 */
			printf(">>> Scanning for HFM Linearization Table Lookup code sequence...");
			addr = search( fh, (unsigned char *)&needle_1, (unsigned char *)&mask_1, sizeof(needle_1), 0 );
			if(addr == NULL) {
				printf("\nhfm sequence not found\nGiving up.\n");
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
			printf("File size isn't 512kbytes. This isn't a Ferrari firmware file");
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

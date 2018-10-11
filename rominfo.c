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
#include "rominfo.h"
#include "utils.h"

static char vmecuhn_str[] = { "VMECUHN [Vehicle Manufacturer ECU Hardware Number SKU]" };
static char ssecuhn_str[] = { "SSECUHN [Bosch Hardware Number]" };
static char ssecusn_str[] = { "SSECUSN [Bosch Serial Number] " };
static char erotan_str[]  = { "EROTAN  [Model Description]"  };
static char dif_str[]     = { "DIF"     };
static char brif_str[]    = { "BRIF"    };
static char engid_str[]   = { "OTHERID" };
static char dummy_str[]   = { "TESTID"  };
extern int show_diss;

int check_rominfo(ImageHandle *fh, int skip)
{
	int found = 0, byte_offset;
	char *rom_load_addr = fh->d.p;
//	char *addr;
	unsigned int rom_skip_bytes;
	
	if(skip == 0) return found;		
	
	printf("\n-[ Basic Firmware information ]-----------------------------------------------------------------\n\n");
	printf(">>> Scanning for ROM String Table Byte Sequence #1 [info] \n");

	rom_skip_bytes = ROM_START_OFFSET;			// start point for search in loaded rom buffer	
	byte_offset    = search_offset( rom_load_addr+rom_skip_bytes , (fh->len)-rom_skip_bytes , (unsigned char *)&meinfo_needle, (unsigned char *)&meinfo_mask, meinfo_needle_len);
	if(byte_offset != NULL) 
	{
		printf("\nfound needle at offset=%#x",(int)(rom_skip_bytes  + byte_offset));
		// try to determine information from rom scan
		get_rominfo(fh,  (rom_load_addr + rom_skip_bytes  + byte_offset) , byte_offset, rom_load_addr);
	}
}

int get_rominfo(ImageHandle *fh, unsigned char *addr, unsigned int offset, unsigned char *offset_addr)
{
	int i;
	unsigned char *p, *tbl;

			if(fh == 0) return 0;
			if(offset_addr == 0) return 0;
					
			unsigned long val          = get16((unsigned char *)addr - 6);	// and segment (required to regenerate physical address from segment)
//			printf("val=%-4.4x\n",val); fflush(0);
			unsigned long seg          = get16((unsigned char *)addr - 2);	// and segment (required to regenerate physical address from segment)
//			printf("seg=%-4.4x\n",seg); fflush(0);
			unsigned long struct_adr   = (unsigned long)(seg*SEGMENT_SIZE)+(long int)val;	// derive phyiscal address from offset and segment
//			printf("adr=%p\n",struct_adr); fflush(0);
			struct_adr                &= ~(ROM_1MB_MASK);					// convert physical address to a rom file offset we can easily work with.
//			printf("adr=%p\n",struct_adr); fflush(0);
			printf("\nfound table at offset=%p.\n\n",(void *)struct_adr ); fflush(0);

			p = (unsigned char *)offset_addr + struct_adr;

			if(show_diss) {
				c167x_diss(addr-offset_addr, addr, 64);
			}

//			printf("\np at =%p.\n\n",(void*)p ); fflush(0);

					int idx = 1, matches=0;
					unsigned int type=0,len=0,valu=0,segm=0,skip=0;
					char *idx_str = 0;
					char strbuf[256];
					
					for(i=idx; i < TBL_MAX_ENTRIES;i++,idx++) 
					{
						tbl = (unsigned char *)(p +    (idx*TBL_SIZEOF));
						type = *(     tbl + TBL_TYPE);		// from rom routine extract value (offset in rom to table)
						len  = *(     tbl + TBL_LEN);		// from rom routine extract value (offset in rom to table)
						valu = get16( tbl + TBL_VAL);	// from rom routine extract value (offset in rom to table)
						segm = get16( tbl + TBL_SEG);	// from rom routine extract value (offset in rom to table)

									if(type == 6 && len > 0)
									{
										// list of ids to blacklist display of
										if(idx == 0x14) { skip = 1; }
										if(idx == 22)   { skip = 1; }

										if(skip == 0)
										{	
											skip = 0;
											unsigned long str_adr = (unsigned long)(segm*SEGMENT_SIZE)+(long int)valu;	// derive phyiscal address from offset and segment
											str_adr              &= ~(ROM_1MB_MASK);					// convert physical address to a rom file offset we can easily work with.
											if(str_adr < fh->len)
											{
											str_adr             += offset_addr;
											unsigned char *p_str = (unsigned char *)str_adr;
										
											// to be safe lets filter non printable chars
											for(i=0; i < len; i++)
											{
												if(isprint((unsigned char *)(p_str[i]))) 
												{
													strbuf[i] = (unsigned char)p_str[i];
												} else {
													strbuf[i] = (unsigned char)" ";
												}
											}
											*(strbuf+len) = 0x00;	// null terminate
											
											switch(idx) {
												case 1:  { idx_str = &vmecuhn_str; break; }
												case 2:  { idx_str = &ssecuhn_str; break; }
												case 4:  { idx_str = &ssecusn_str; break; }
												case 6:  { idx_str = &erotan_str;  break; }
												case 8:  { idx_str = &dummy_str;   break; }
												case 10: { idx_str = &dif_str;     break; }
												case 11: { idx_str = &brif_str;    break; }
												case 19:
												case 21: { idx_str = &engid_str;   break; }
												default: { idx_str = 0; };
											}

				//							memcpy(strbuf,str_adr,len);
											printf("Idx=%-3d { %-24s} %#x ", idx, strbuf, (int)str_adr-(int)offset_addr );
											if(idx_str != NULL) {
												printf(": %s\n",idx_str);
											} else {
												printf("\n");
											}
											
											matches++;
											}
										}
									}
						}
						if(matches == 0) {
							printf("Sorry no entries found in table.\n");
						}

						printf("\n>>> Scanning for EPK information [info] \n");
						addr = search( fh, (unsigned char *)&kwp2000_ecu_needle, (unsigned char *)&kwp2000_ecu_mask, kwp2000_ecu_needle_len, 0 );
						if(addr != NULL) 
						{
							printf("\nfound needle at offset=0x%x.\n",(int)(addr-offset_addr) );
									unsigned long val          = get16((unsigned char *)addr + 28);// and segment (required to regenerate physical address from segment)
									int seg = dpp1_value-1;
									unsigned long map_adr      = (unsigned long)(seg*SEGMENT_SIZE)+(long int)val;	// derive phyiscal address from offset and segment
									map_adr                   &= ~(ROM_1MB_MASK);					// convert physical address to a rom file offset we can easily work with.
							printf("EPK: @ %#x { ",map_adr);

							unsigned char *adrs = map_adr+offset_addr;
							i=0;
							unsigned char len=0;
							unsigned char ch;
							len =(unsigned char *)*(adrs+i);
							i += 2;
							while(1)
							{
								ch =(unsigned char *)adrs[i];
								if (ch == 0) break;
								if(isprint(ch))
								{
									printf("%c", ch);
								} else {
									break;
								}
							   i++;
							   if(i > 64) break;
							   if(i>=len) break;
							}
							printf(" }\n");
						}
	return 0;
}
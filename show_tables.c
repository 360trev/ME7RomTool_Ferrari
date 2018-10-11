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
#include "show_tables.h"
#include "utils.h"

// defined in main.c (TODO: this needs cleaning up!)
extern int full_debug;
extern int show_hex;
extern int show_adr;
extern int show_phy;
extern int show_diss;

//get_nwidth
unsigned long get_nwidth(unsigned char *s, int nwidth)
{
	unsigned long val=0;
	
	switch(nwidth)
	{
		case 0:		// no axis...
			break;			
		case 1:		// 8-bit support
			val = (unsigned long)*(s);
			break;
		case 2:		// 16-bit support
			val = (unsigned long)get16(s);
			break;
		case 4:		// 32-bit support
			val = (unsigned long)get32(s);
			break;
		default:
//			printf("FORMAT NOT SUPPORTED! FIXME\n");
			break;
	}
	return val;
}

void show_entry_def(ENTRY_DEF *entry, int nwidth)
{
	char *conv_formula[32];

	if(nwidth == 0) return;
	
	printf("\n    %s:\n",entry->field_name);  
	printf("      Unit:                    %s\n",entry->desc);
	printf("      Conversion name:         %s\n",entry->conv_name);
	snprintf(conv_formula, 32, "f(phys) = 0.0 + %3.6f * phys", strtod(entry->conv,NULL) );
	printf("      Conversion formula:      %s\n",conv_formula);
	printf("      Data type:               ");
	switch(nwidth)
	{
		case 1:	  printf("UBYTE"); break;
		case 2:	  printf("UWORD"); break;
		case 4:	  printf("ULONG"); break;
		default:  printf("%d BYTES", nwidth);
	}
}

static unsigned char *X_AXIS_START=0;
static unsigned char *Y_AXIS_START=0;
static unsigned char *CELL_START=0;
static TABLE_DEF *TBL_DEF=0;

int set_table_overrides(char *x_axis, char* y_axis, char *cells, TABLE_DEF *td)
{
	X_AXIS_START = x_axis;
	Y_AXIS_START = y_axis;
	CELL_START   = cells;
	TBL_DEF      = td;
}

int dump_table(unsigned char *adr, unsigned char *offset_addr, unsigned long val, unsigned long seg, TABLE_DEF *td, unsigned long cell_table_override_adr)
{
	int x,y;
	int seg_start             = 0x800000;
	unsigned long rom_adr      = (unsigned long)(seg*SEGMENT_SIZE)+(long int)val;	// derive phyiscal address from offset and segment
	unsigned long map_table_adr= rom_adr;
	map_table_adr             &= ~(ROM_1MB_MASK);					// convert physical address to a rom file offset we can easily work with.

	unsigned char *table_start =   offset_addr+map_table_adr;			// 2 bytes to skip x and y bytes
	unsigned long  table_start2 =  cell_table_override_adr;			// 2 bytes to skip x and y bytes
	table_start2               &= ~(ROM_1MB_MASK);					// convert physical address to a rom file offset we can easily work with.
	table_start2              += offset_addr;
	
	double double_x_axis_value, double_cell_value, conv_value, conv2_value;
	int    i,x_pos,y_pos;
	int    entry;
	int    integer_cell_value;
	int    table_data_offset;
	double y_axis_hdr_value_raw;
	double y_axis_hdr_value_fmt;
	unsigned char  *x_num_data_start;
	unsigned char  *y_num_data_start;
	unsigned char  *x_axis_header_data_start;
	unsigned char  *y_axis_header_data_start;
	unsigned char  *cell_data_start;
	unsigned int x_num, y_num, line=0;
//	unsigned int bytes_per_x_row;
//	unsigned int bytes_per_y_row;
//	char *conv_formula[32];
	unsigned char *cell_adr;

	// get the cell data
		if(cell_table_override_adr == 0)
		{
			table_data_offset  = 0;
			x_num_data_start          = table_start + table_data_offset;
			table_data_offset        += (td->x_num_nwidth);				// x num
			x_num 					  = get_nwidth(x_num_data_start, td->x_num_nwidth);		// get number of colums   **FIXED**

			y_num_data_start          = table_start + table_data_offset;
			table_data_offset        += (td->y_num_nwidth);				// y num
			y_num                     = get_nwidth(y_num_data_start, td->y_num_nwidth);		// get number of rows	  **FIXED**

			x_axis_header_data_start  = table_start + table_data_offset;
			table_data_offset        += (x_num*td->x_axis_nwidth);		//x axis header data

			y_axis_header_data_start  = table_start + table_data_offset;
			table_data_offset        += (y_num*td->y_axis_nwidth);		// y axis header data

			cell_data_start           = table_start + table_data_offset;
//			printf("Defaulting to implide cell_data start address at %p\n", cell_data_start);

		} else {
			table_data_offset  = 0;
			x_num_data_start          = table_start + table_data_offset;
			table_data_offset        += (td->x_num_nwidth);				// x num
			printf("X_NUM  start address: %p\n", x_num_data_start - offset_addr);
			x_num 					  = get_nwidth(x_num_data_start, td->x_num_nwidth);		// get number of colums   **FIXED**

			x_axis_header_data_start  = table_start + table_data_offset;
			table_data_offset        += (x_num*td->x_axis_nwidth);		//x axis header data
			printf("X_AXIS start address: %p\n", x_axis_header_data_start - offset_addr);

			y_num_data_start          = table_start + table_data_offset;
			table_data_offset        += (td->y_num_nwidth);				// y num
			printf("Y_NUM  start address: %p\n", y_num_data_start - offset_addr);
			y_num                     = get_nwidth(y_num_data_start, td->y_num_nwidth);		// get number of rows	  **FIXED**

			y_axis_header_data_start  = table_start + table_data_offset;
			table_data_offset        += (y_num*td->y_axis_nwidth);		// y axis header data
			printf("Y_AXIS start address: %p\n", y_axis_header_data_start - offset_addr);

			cell_data_start           = table_start2 ;
			printf("Overriding cell_data start address to %p\n", cell_data_start);
			
		}

//		bytes_per_x_row           = (x_num*td->cell_nwidth);
//		bytes_per_y_row           = (y_num*td->cell_nwidth);

		// if we turn on full debug support, turn on flags to show everything		
		if(full_debug == 1) {
			show_phy = 1;
			show_adr = 1;
			show_hex = 1;
		}
//					printf("    xnum=%d (nwidth=%d), ynum=%d (nwidth=%d)\n", x_num, td->x_num_nwidth, y_num, td->y_num_nwidth);  

					printf("\n%s\n",td->table_name);
					printf("    Long identifier:           %s\n", td->table_desc);
					printf("    Display identifier:        %s\n", " ");  

//					unsigned long tmp = (unsigned long)cell_data_start;
//					tmp = tmp - (unsigned long)offset_addr;
//					printf("    Cell Address:              0x%x\n", (char *)tmp );	

					printf("    Address:                   0x%x\n", (char *)rom_adr  );	
					printf("    Value:\n\n");  

					printf(" No.           | ");
					for(i=0;i<x_num;i++) {
						printf("    %4d ", i);
					}

					if(show_phy == 1)
					{
						printf("\n            PHY| ");
						for(i=0;i<x_num;i++) {
							entry = get_nwidth(x_axis_header_data_start+(i*td->x_axis_nwidth), td->x_axis_nwidth);	//**FIXED**
//							printf(td->x_axis.fmt_PHY, entry/strtod(td->x_axis.conv,NULL) );		// convert stored table value to correct viewing format

								double_x_axis_value = (double)entry;
								conv_value          = (double)strtod(td->x_axis.conv,NULL);
								if(td->x_axis.conv2 != NULL) {
									conv2_value       = (double)strtod(td->x_axis.conv2,NULL);									
								}
								
								switch(td->x_axis.otype) {
									case 'x':	double_x_axis_value = (double)double_x_axis_value * (double)conv_value - (double)conv2_value; 
									break;
									case '*':	double_x_axis_value = (double)double_x_axis_value * (double)conv_value; break;
									case '/':
									default: 	double_x_axis_value = (double)double_x_axis_value / (double)conv_value; break;
								}
								printf(td->x_axis.fmt_PHY,   (double)double_x_axis_value );	// show values directly out of the table


						}
						line++;
					}

					if(show_hex == 1)
					{
						printf("\n            HEX| ");
						for(i=0;i<x_num;i++) {
							entry = get_nwidth(x_axis_header_data_start+(i*td->x_axis_nwidth), td->x_axis_nwidth);	//**FIXED**
							printf(td->cell.fmt_HEX, entry );						// show raw hex
						}
						line++;
					}

					if(show_adr == 1)
					{
						printf("\n            ADR| ");
						for(i=0;i<x_num;i++) {
							printf("0x%X ", (x_axis_header_data_start+(i*td->x_axis_nwidth ) - offset_addr + seg_start ));
						}
						line++;
					}
					
					printf("\n");
					printf(" --------------+");
					for(i=0;i<x_num;i++) {
						printf("---------");
					}

					if(y_num == 0) 		// this means there this is a 1-axis table, we therefore only need to show 1 row
					{
#if 1
						if(show_phy==1)
						{
							printf("\n            PHY| ");
							for(i=x_num;i<x_num*2;i++) {
								entry = get_nwidth(x_axis_header_data_start+(i*td->x_axis_nwidth), td->x_axis_nwidth);	//**FIXED**
								double_cell_value = (double)entry;
								conv_value        = (double)strtod(td->cell.conv,NULL);
								if(td->cell.conv2 != NULL) {
									conv2_value       = (double)strtod(td->cell.conv2,NULL);									
								}
								
								switch(td->cell.otype) {
									case 'x':	double_cell_value = (double)double_cell_value * (double)conv_value - (double)conv2_value; break;
									case '*':	double_cell_value = (double)double_cell_value * (double)conv_value; break;
									case '/':
									default: 	double_cell_value = (double)double_cell_value / (double)conv_value; break;
								}
								printf(td->cell.fmt_PHY,   (double)double_cell_value );	// show values directly out of the table
							}
						}					

						if(show_hex==1)
						{
							printf("\n            HEX| ");
							for(i=x_num;i<x_num*2;i++) {
								entry = get_nwidth(x_axis_header_data_start+(i*td->x_axis_nwidth), td->x_axis_nwidth);	//**FIXED**
								printf(td->cell.fmt_HEX, entry );						// show raw hex
							}
						}
						if(show_adr==1)
						{
							printf("\n            ADR| ");
							for(i=x_num;i<x_num*2;i++) {
								cell_adr = x_axis_header_data_start+(i*td->x_axis_nwidth);
								entry = get_nwidth(adr, td->x_axis_nwidth);	//**FIXED**
								printf(td->cell.fmt_ADR, cell_adr - offset_addr + seg_start );						// show raw hex
							}
						}
#endif

					}
					else 
					{
						for(y_pos=0;y_pos <y_num;y_pos++) 
						{
#if 1
							// get y_axis header data
							unsigned char *adr = y_axis_header_data_start + (y_pos*td->y_axis_nwidth);
							y_axis_hdr_value_raw = get_nwidth( adr, td->y_axis_nwidth );		//**FIXED**
							y_axis_hdr_value_fmt = y_axis_hdr_value_raw/(strtod(td->y_axis.conv,NULL));	

							if(show_phy==1)
							{
								printf("\n ");
								printf(td->y_axis.fmt_PHY, (double)y_axis_hdr_value_fmt );
								printf("    PHY| ");
								for(x_pos=0;x_pos<x_num;x_pos++) 
								{
									// get cell data
									entry = get_nwidth( (cell_data_start + (x_pos)*(y_num*td->cell_nwidth) + y_pos*td->cell_nwidth ), td->cell_nwidth ); 	//**FIXED**
									double_cell_value = (double)entry;
									conv_value        = (double)strtod(td->cell.conv,NULL);
									if(td->cell.conv2 != NULL) {
										conv2_value       = (double)strtod(td->cell.conv2,NULL);									
									}
									
									switch(td->cell.otype) {
										case 'x':	double_cell_value = (double)double_cell_value * (double)conv_value - (double)conv2_value; break;
										case '*':	double_cell_value = (double)double_cell_value * (double)conv_value; break;
										case '/':
										default: 	double_cell_value = (double)double_cell_value / (double)conv_value; break;
									}
									printf(td->cell.fmt_PHY,   (double)double_cell_value );	// show values directly out of the table


									// show floating point formatted value
//									double_cell_value = (double)entry;
//									double_cell_value = (double)double_cell_value / (strtod(td->cell.conv,NULL));
//									printf(td->cell.fmt_PHY,   (double)double_cell_value );	// show values directly out of the table
								}
							}

							if(show_hex==1)
							{
								// get y_axis formatting and display it
								y_axis_hdr_value_fmt = y_axis_hdr_value_raw/(strtod(td->y_axis.conv,NULL));	
								printf("\n  ");
								printf("%-#8.4x ", get_nwidth( (int)adr, td->y_axis_nwidth ) );						//**FIXED**
								printf(" HEX| ");
								for(x_pos=0;x_pos<x_num;x_pos++) 
								{
									cell_adr = (cell_data_start + (x_pos)*(y_num*td->cell_nwidth) + y_pos*td->cell_nwidth );	// get cell address
									entry    = get_nwidth( cell_adr, td->cell_nwidth );										//**FIXED**// get cell data
									printf(td->cell.fmt_HEX,(int)entry);						// show values directly out of the table
								}
							}

							if(show_adr==1)
							{
								// get y_axis formatting and display it
								y_axis_hdr_value_fmt = y_axis_hdr_value_raw/(strtod(td->y_axis.conv,NULL));	
								printf("\n  ");
								printf("%-#9.5x", (int)(adr - (unsigned char *)(offset_addr))+seg_start );
								printf(" ADR| ");
								for(x_pos=0;x_pos<x_num;x_pos++) 
								{	
									cell_adr = (cell_data_start + (x_pos)*(y_num*td->cell_nwidth) + y_pos*td->cell_nwidth );	// get cell address
									entry    = get_nwidth( cell_adr, td->cell_nwidth );									//**FIXED**	// get cell data
									printf("0x%x ", cell_adr - offset_addr + seg_start );			// show address
								}
							} 
							
							if(line > 1)
								printf("\n");					
#endif						
						}					
					}

		printf("\n\n");
		show_entry_def(&td->cell,   td->cell_nwidth);
		show_entry_def(&td->x_axis, td->x_axis_nwidth);
		show_entry_def(&td->y_axis, td->y_axis_nwidth);
		printf("\n\n");
		return 0;
}

//
// table formatting right is a big mess (!) and needs a major cleanup (TO DO!!!)
//

int dump_table2(unsigned char *offset_addr, unsigned long cell_table_override_adr)
{
	dump_raw_table(0, offset_addr, cell_table_override_adr);
}

int dump_raw_table(unsigned char *table_start, unsigned char *offset_addr, unsigned long cell_table_override_adr)
{
	int x,y;
	int seg_start             = 0x800000;
    TABLE_DEF *td=0;
	
//	unsigned long rom_adr      = (unsigned long)(seg*SEGMENT_SIZE)+(long int)val;	// derive phyiscal address from offset and segment
//	unsigned long map_table_adr= rom_adr;
//	map_table_adr             &= ~(ROM_1MB_MASK);					// convert physical address to a rom file offset we can easily work with.

	unsigned long  table_start2 =  cell_table_override_adr;			// 2 bytes to skip x and y bytes
	table_start2               &= ~(ROM_1MB_MASK);					// convert physical address to a rom file offset we can easily work with.
	table_start2              += offset_addr;
	
	double double_cell_value, double_x_axis_value, conv_value, conv2_value;
	int    i,x_pos,y_pos;
	int    entry;
	int    integer_cell_value;
	int    table_data_offset;
	double y_axis_hdr_value_raw;
	double y_axis_hdr_value_fmt;
	unsigned char  *x_num_data_start;
	unsigned char  *y_num_data_start;
	unsigned char  *x_axis_header_data_start;
	unsigned char  *y_axis_header_data_start;
	unsigned char  *cell_data_start;
	unsigned int x_num, y_num, line=0;
//	unsigned int bytes_per_x_row;
//	unsigned int bytes_per_y_row;
//	char *conv_formula[32];
	unsigned char *cell_adr;

	// get the cell data
		if(cell_table_override_adr == 0)
		{
			table_data_offset  = 0;
			x_num_data_start          = table_start + table_data_offset;
			table_data_offset        += (td->x_num_nwidth);				// x num
			printf("X_NUM  start address: %p\n", x_num_data_start);// - offset_addr);
			x_num 					  = get_nwidth(x_num_data_start, td->x_num_nwidth);		// get number of colums   **FIXED**

			y_num_data_start          = table_start + table_data_offset;
			table_data_offset        += (td->y_num_nwidth);				// y num
			y_num                     = get_nwidth(y_num_data_start, td->y_num_nwidth);		// get number of rows	  **FIXED**
			printf("Y_NUM  start address: %p\n", y_num_data_start);// - offset_addr);

			x_axis_header_data_start  = table_start + table_data_offset;
			table_data_offset        += (x_num*td->x_axis_nwidth);		//x axis header data
			printf("X_AXIS start address: %p\n", x_axis_header_data_start); // - offset_addr);


			y_axis_header_data_start  = table_start + table_data_offset;
			table_data_offset        += (y_num*td->y_axis_nwidth);		// y axis header data
			printf("Y_AXIS start address: %p\n", y_axis_header_data_start); // - offset_addr);

			cell_data_start           = table_start + table_data_offset;
			printf("Defaulting to implide cell_data start address at %p\n", cell_data_start);

		}
		else if(cell_table_override_adr == FULL_OVERRIDE) 
		{
//			printf("FULL OVERRIDE MODE\n");
			td                        = TBL_DEF;
		
			table_data_offset         = 0;
			x_num_data_start          = X_AXIS_START;
			table_data_offset        += (td->x_num_nwidth);				// x num
//			printf("X_NUM  start address: %p\n", x_num_data_start);
			x_num 					  = get_nwidth(x_num_data_start, td->x_num_nwidth);		// get number of colums   **FIXED**
//			printf("x_num=%d\n", x_num);

			x_axis_header_data_start  = x_num_data_start + table_data_offset;
			table_data_offset        += (x_num*td->x_axis_nwidth);		//x axis header data
//			printf("X_AXIS start address: %p\n", x_axis_header_data_start);

			table_data_offset         = 0;
			y_num_data_start          = Y_AXIS_START;
			table_data_offset        += (td->y_num_nwidth);				// y num
//			printf("Y_NUM  start address: %p\n", y_num_data_start);
			y_num                     = get_nwidth(y_num_data_start, td->y_num_nwidth);		// get number of rows	  **FIXED**
//			printf("y_num=%d\n", y_num);

			y_axis_header_data_start  = y_num_data_start + table_data_offset;
			table_data_offset        += (y_num*td->y_axis_nwidth);		// y axis header data
//			printf("Y_AXIS start address: %p\n", y_axis_header_data_start);

			cell_data_start           = CELL_START;
//			printf("Overriding cell_data start address to %p\n", cell_data_start);

		} 
		else 
		{
			table_data_offset  = 0;
			x_num_data_start          = table_start + table_data_offset;
			table_data_offset        += (td->x_num_nwidth);				// x num
			printf("X_NUM  start address: %p\n", x_num_data_start - offset_addr);
			x_num 					  = get_nwidth(x_num_data_start, td->x_num_nwidth);		// get number of colums   **FIXED**

			x_axis_header_data_start  = table_start + table_data_offset;
			table_data_offset        += (x_num*td->x_axis_nwidth);		//x axis header data
			printf("X_AXIS start address: %p\n", x_axis_header_data_start - offset_addr);

			y_num_data_start          = table_start + table_data_offset;
			table_data_offset        += (td->y_num_nwidth);				// y num
			printf("Y_NUM  start address: %p\n", y_num_data_start - offset_addr);
			y_num                     = get_nwidth(y_num_data_start, td->y_num_nwidth);		// get number of rows	  **FIXED**

			y_axis_header_data_start  = table_start + table_data_offset;
			table_data_offset        += (y_num*td->y_axis_nwidth);		// y axis header data
			printf("Y_AXIS start address: %p\n", y_axis_header_data_start - offset_addr);

			cell_data_start           = table_start2 ;
			printf("Overriding cell_data start address to %p\n", cell_data_start);
			
		}

//		bytes_per_x_row           = (x_num*td->cell_nwidth);
//		bytes_per_y_row           = (y_num*td->cell_nwidth);

		// if we turn on full debug support, turn on flags to show everything		
		if(full_debug == 1) {
			show_phy = 1;
			show_adr = 1;
			show_hex = 1;
		}
//					printf("    xnum=%d (nwidth=%d), ynum=%d (nwidth=%d)\n", x_num, td->x_num_nwidth, y_num, td->y_num_nwidth);  

					printf("\n%s\n",td->table_name);
					printf("    Long identifier:           %s\n", td->table_desc);
					printf("    Display identifier:        %s\n", " ");  

//					unsigned long tmp = (unsigned long)cell_data_start;
//					tmp = tmp - (unsigned long)offset_addr;
//					printf("    Cell Address:              0x%x\n", (char *)tmp );	

//					printf("    Address:                   0x%x\n", (char *)table_start);	//rom_adr  );	
					printf("    Value:\n\n");  

					printf(" No.           | ");
					for(i=0;i<x_num;i++) {
						printf("    %4d ", i);
					}


					if(show_phy == 1)
					{
						//
						// X-AXIS
						//
						printf("\n            PHY| ");
						for(i=0;i<x_num;i++) {
							entry = get_nwidth(x_axis_header_data_start+(i*td->x_axis_nwidth), td->x_axis_nwidth);	//**FIXED**
//							printf(td->x_axis.fmt_PHY, entry/strtod(td->x_axis.conv,NULL) );		// convert stored table value to correct viewing format
								double_x_axis_value = (double)entry;
								conv_value          = (double)strtod(td->x_axis.conv,NULL);
								if(td->x_axis.conv2 != NULL) {
									conv2_value       = (double)strtod(td->x_axis.conv2,NULL);		
								}
								
								switch(td->x_axis.otype) {
									case 'd':	double_x_axis_value = (double)double_x_axis_value / (double)conv_value - (double)conv2_value; 		break;
									case 'x':	double_x_axis_value = (double)double_x_axis_value * (double)conv_value - (double)conv2_value; 		break;
									case '*':	double_x_axis_value = (double)double_x_axis_value * (double)conv_value; break;
									case '/':
									default: 	double_x_axis_value = (double)double_x_axis_value / (double)conv_value; break;
								}
								printf(td->x_axis.fmt_PHY,   (double)double_x_axis_value );	// show values directly out of the table
						}
						line++;
					}
#if 0
					if(show_phy == 1)
					{
						//
						// X-AXIS
						//
						printf("\n            PHY| ");
						for(i=0;i<x_num;i++) {
							entry = get_nwidth(x_axis_header_data_start+(i*td->x_axis_nwidth), td->x_axis_nwidth);	//**FIXED**
							printf(td->x_axis.fmt_PHY, entry/strtod(td->x_axis.conv,NULL) );		// convert stored table value to correct viewing format
						}
						line++;
					}
#endif

					if(show_hex == 1)
					{
						//
						// X-AXIS
						//
						printf("\n            HEX| ");
						for(i=0;i<x_num;i++) {
							entry = get_nwidth(x_axis_header_data_start+(i*td->x_axis_nwidth), td->x_axis_nwidth);	//**FIXED**
							printf(td->cell.fmt_HEX, entry );						// show raw hex
						}
						line++;
					}

					if(show_adr == 1)
					{
						//
						// X-AXIS
						//
						printf("\n            ADR| ");
						for(i=0;i<x_num;i++) {
							printf("0x%X ", (x_axis_header_data_start+(i*td->x_axis_nwidth ) - offset_addr + seg_start ));
						}
						line++;
					}
					
					printf("\n");
					printf(" --------------+");
					for(i=0;i<x_num;i++) {
						printf("---------");
					}

					if(y_num == 0) 		// this means there this is a 1-axis table, we therefore only need to show 1 row
					{
#if 1

#if 0
						if(show_phy==1)
						{
							//
							// X-AXIS
							//
							printf("\n            PHY| ");
							for(i=x_num;i<x_num*2;i++) {
								entry = get_nwidth(x_axis_header_data_start+(i*td->x_axis_nwidth), td->x_axis_nwidth);	//**FIXED**
								double_cell_value = (double)entry;
								conv_value        = (double)strtod(td->cell.conv,NULL);
								
								switch(td->cell.otype) {
									case '*':	double_cell_value = (double)double_cell_value * (double)conv_value; break;
									case '/':
									default: 	double_cell_value = (double)double_cell_value / (double)conv_value; break;
								}
								printf(td->cell.fmt_PHY,   (double)double_cell_value );	// show values directly out of the table
							}
						}					

						if(show_hex==1)
						{
							printf("\n            HEX| ");
							for(i=x_num;i<x_num*2;i++) {
								entry = get_nwidth(x_axis_header_data_start+(i*td->x_axis_nwidth), td->x_axis_nwidth);	//**FIXED**
								printf(td->cell.fmt_HEX, entry );						// show raw hex
							}
						}
						if(show_adr==1)
						{
							printf("\n            ADR| ");
							for(i=x_num;i<x_num*2;i++) {
								cell_adr = x_axis_header_data_start+(i*td->x_axis_nwidth);
////FIXME:TLM
//								entry = get_nwidth(adr, td->x_axis_nwidth);	//**FIXED**
//								printf(td->cell.fmt_ADR, cell_adr - offset_addr + seg_start );						// show raw hex
							}
						}

#endif

#endif

					}
					else 
					{
						for(y_pos=0;y_pos <y_num;y_pos++) 
						{
#if 1
							// get y_axis header data
							unsigned char *adr = y_axis_header_data_start + (y_pos*td->y_axis_nwidth);
							y_axis_hdr_value_raw = get_nwidth( adr, td->y_axis_nwidth );		//**FIXED**
							y_axis_hdr_value_fmt = y_axis_hdr_value_raw/(strtod(td->y_axis.conv,NULL));	

							if(show_phy==1)
							{
								// cells
#if 1
								printf("\n ");
								printf(td->y_axis.fmt_PHY, (double)y_axis_hdr_value_fmt );
								printf("    PHY| ");
								for(x_pos=0;x_pos<x_num;x_pos++) 
								{
									// get cell data
									entry = get_nwidth( (cell_data_start + (x_pos)*(y_num*td->cell_nwidth) + y_pos*td->cell_nwidth ), td->cell_nwidth ); 	//**FIXED**

									double_cell_value = (double)entry;
									conv_value        = (double)strtod(td->cell.conv,NULL);
//									if(td->cell.conv2 != NULL) {
//										conv2_value       = (double)strtod(td->cell.conv2,NULL);									
//									}
									
									switch(td->cell.otype) {
//										case 'x':	double_cell_value = (double)double_cell_value * (double)conv_value - (double)conv2_value; break;
										case '*':	double_cell_value = (double)double_cell_value * (double)conv_value; break;
										case '/':
										default: 	double_cell_value = (double)double_cell_value / (double)conv_value; break;
									}
									printf(td->cell.fmt_PHY,   (double)double_cell_value );	// show values directly out of the table


									// show floating point formatted value
//									double_cell_value = (double)entry;
//									double_cell_value = (double)double_cell_value / (strtod(td->cell.conv,NULL));
//									printf(td->cell.fmt_PHY,   (double)double_cell_value );	// show values directly out of the table
								}
#endif
							}

							if(show_hex==1)
							{
								// get y_axis formatting and display it
								y_axis_hdr_value_fmt = y_axis_hdr_value_raw/(strtod(td->y_axis.conv,NULL));	
								printf("\n  ");
								printf("%-#8.4x ", get_nwidth( (int)adr, td->y_axis_nwidth ) );						//**FIXED**
								printf(" HEX| ");
								for(x_pos=0;x_pos<x_num;x_pos++) 
								{
									cell_adr = (cell_data_start + (x_pos)*(y_num*td->cell_nwidth) + y_pos*td->cell_nwidth );	// get cell address
									entry    = get_nwidth( cell_adr, td->cell_nwidth );										//**FIXED**// get cell data
									printf(td->cell.fmt_HEX,(int)entry);						// show values directly out of the table
								}
							}

							if(show_adr==1)
							{
								// get y_axis formatting and display it
								y_axis_hdr_value_fmt = y_axis_hdr_value_raw/(strtod(td->y_axis.conv,NULL));	
								printf("\n  ");
								printf("%-#9.5x", (int)(adr - (unsigned char *)(offset_addr))+seg_start );
								printf(" ADR| ");
								for(x_pos=0;x_pos<x_num;x_pos++) 
								{	
									cell_adr = (cell_data_start + (x_pos)*(y_num*td->cell_nwidth) + y_pos*td->cell_nwidth );	// get cell address
									entry    = get_nwidth( cell_adr, td->cell_nwidth );									//**FIXED**	// get cell data
									printf("0x%X ", cell_adr - offset_addr + seg_start );			// show address
								}
							} 
							
							if(line > 1)
								printf("\n");					
#endif						
						}					
					}

		printf("\n\n");
		show_entry_def(&td->cell,   td->cell_nwidth);
		show_entry_def(&td->x_axis, td->x_axis_nwidth);
		show_entry_def(&td->y_axis, td->y_axis_nwidth);
		printf("\n\n");
		return 0;
}


int find_dump_table_dppx(unsigned char *rom_load_addr, int rom_len, unsigned char *needle, unsigned char *needle_mask, unsigned int needle_len, int table_offset, int segment, TABLE_DEF *table_fmt)
{
	int current_offset = 0, new_offset = 0;
	int table_adr, found;
	unsigned char *addr;
	unsigned char *map_table_adr;
				
	found = 0;
	while(1) {
		// search for signature
		current_offset = search_offset(rom_load_addr+new_offset, rom_len-new_offset, (unsigned char *)needle, (unsigned char *)needle_mask, needle_len);
		if(current_offset == 0) break;
				
		// if we find a match lets dump it!
		if(current_offset != NULL) {
			found++;
			// calculate its physical address in memory
			addr = rom_load_addr+current_offset+new_offset;
			// show its physical address in rom 
			printf("Function byte-sequence found @ %#x\n\n",addr-rom_load_addr);
			// disassemble needle found in rom
			if(show_diss) {
				c167x_diss(addr-rom_load_addr, addr, needle_len+44);
			}

			// now lets extract offset from needle to where table is located
			table_adr = get16((unsigned char *)addr + table_offset);
			// now lets show the table formatted correctly
			dump_table(addr, rom_load_addr, table_adr, segment, table_fmt, 0);

			// search for next match...
			new_offset += current_offset+needle_len;
		}
	}
	printf("\n");
	return found;
}

#include "inst_c16x.h"

int find_dump_table_seg(unsigned char *rom_load_addr, int rom_len, unsigned char *needle, unsigned char *needle_mask, unsigned int needle_len, int table_offset, int segment_offset, TABLE_DEF *table_fmt)
{
	int current_offset = 0, new_offset = 0;
	int table_adr, found;
	int segm_adr;
	unsigned char opcode, found_op;
	unsigned char *addr;
	unsigned char *map_table_adr;
				
	found = 0;
	while(1) {
		// search for signature
		current_offset = search_offset(rom_load_addr+new_offset, rom_len-new_offset, (unsigned char *)needle, (unsigned char *)needle_mask, needle_len);
		if(current_offset == 0) break;
				
		// if we find a match lets dump it!
		if(current_offset != NULL) {
			found++;
			// calculate its physical address in memory
			addr = rom_load_addr+current_offset+new_offset;
			// show its physical address in rom 
			printf("Function byte-sequence found @ %#x, needle offset is +%d bytes, seg offset is +%d bytes\n\n",addr-rom_load_addr, table_offset, segment_offset);			
			table_adr = get16((unsigned char *)addr + table_offset);
			segm_adr  = get16((unsigned char *)addr + segment_offset);			

			printf("valu_adr  : %#4.4x for offset +%d\n",table_adr,table_offset);
			printf("segm_adr  : %#4.4x for offset +%d\n\n",segm_adr,segment_offset);
			
			opcode    = *(unsigned char *)((addr + segment_offset) - 2);
			found_op  = 0xE6;
			
//			if(opcode != found_op || opcode != 0xD7) {
//				printf("Segment opcode not found..., found %#2x\n\n", opcode);
//				segm_adr = 0xffff;
//				return 0;
//				
//		} else {
			{
				// disassemble needle found in rom
				if(show_diss) {
					c167x_diss(addr-rom_load_addr, addr, needle_len+44);
				}

				if(segm_adr  > 0x240) { 
					printf("rom_start : %p\n",rom_load_addr);
					printf("rom_end   : %p\n",rom_load_addr+rom_len);
					printf("table_adr : %p\n",table_adr);
					printf("opcode    : 0x%2X (%s) != 0x%2X\n",opcode, inst_set[opcode].name, found_op );
					printf("segm_adr  : %p for offset +%d\n",segm_adr,segment_offset);
					printf("Invalid segment!! Likely a false positive detection at %p!",addr-rom_load_addr);
					found = 0;
					return found;
				} else {

					// now lets show the table formatted correctly
					dump_table(addr, rom_load_addr, table_adr, segm_adr, table_fmt, 0);		
				}
			}
			
			// search for next match...
			new_offset += current_offset+needle_len;
		}
	}
	return found;
}

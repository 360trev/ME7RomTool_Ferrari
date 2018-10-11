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
#ifndef _SHOW_TABLES_SUPPORT_H
#define _SHOW_TABLES_SUPPORT_H

#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct ENTRY_DEF {
		char *field_name;	 // name of the entry field
		int  nwidth;		 // field byte width
		char *conv;          // conversion value for huamn readable, e.g. 655.35
		char *conv2;         // conversion value for huamn readable, e.g. 655.35
		unsigned char otype; // operation type. e.g. "*" or "/"
		char *desc;          // conversion description, e.g. "% PED"
		char *fmt_PHY;       // PHY: data formatting for conversion to human readable
		char *fmt_HEX;       // HEX: data formatting for raw hex values
		char *fmt_ADR;       // ADR: data formatting for physical addresses
		char *conv_name;     // conversion name
} ENTRY_DEF;

typedef struct TABLE_DEF {
		char *table_name;           // name of the table 
		char *table_desc;           // description of the table 

		int   x_num_nwidth;         // width in bytes of the field
		int   y_num_nwidth;         // width in bytes of the field

		int   x_axis_nwidth;        // width in bytes of the field
		int   y_axis_nwidth;        // width in bytes of the field 
		int   cell_nwidth;          // width in bytes of the field

		ENTRY_DEF x_axis;			// x-axis format entry
		ENTRY_DEF y_axis;			// y-axis format entry
		ENTRY_DEF cell;				// cell format entry
} TABLE_DEF;

#define FULL_OVERRIDE	1

#define UBYTE 1
#define UWORD 2

extern TABLE_DEF XXXX_table;
extern int dump_table(unsigned char *adr, unsigned char *offset_addr, unsigned long val, unsigned long seg, TABLE_DEF *td, unsigned long cell_table_override_adr);

extern int find_dump_table_dppx(unsigned char *rom_load_addr, int rom_len, unsigned char *needle, unsigned char *needle_mask, unsigned int needle_len, int table_offset, int segment, TABLE_DEF *table_fmt);
extern int find_dump_table_seg( unsigned char *rom_load_addr, int rom_len, unsigned char *needle, unsigned char *needle_mask, unsigned int needle_len, int table_offset, int segment_offset, TABLE_DEF *table_fmt);

#endif
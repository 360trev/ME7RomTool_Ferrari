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
#ifndef _ROMINFO_H
#define _ROMINFO_H
#include "utils.h"
#include "needles.h"
#include "find_dppx.h"

#define ROM_START_OFFSET	0x20000	// we start searching for our needle
									// after 1st 128kbyte to avoid any
									// false positives with the needle

#define TBL_TYPE   			0		// 0-1
#define TBL_LEN    			1		// 1-2
#define TBL_VAL    			2		// 2-4
#define TBL_SEG    			4		// 4-6
#define TBL_SIZEOF 			6

#define TBL_MAX_ENTRIES 	22		// this is the maximum number of entries
									// we traverse before giving up.
/*
 *  The above definitions are the equivalent of;
 * 
 *  typedef struct TBL {
 *  	unsigned char TBL_TYPE;		// 1 byte
 * 		unsigned char TBL_LEN;		// 1 byte
 * 		unsigned short TBL_VAL;		// 2 bytes
 * 		unsigned short TBL_SEG:		// 2 bytes
 *  } TBL;
 * 
 *  Note: We don't do it this way due to compiler
 *  padding (even using pragma pack is compiler specific)
 *  so using defines is the most safe way to accomplish
 *  the exact byte structure lengths we require.
 * 
 */
extern unsigned long dpp0_value, dpp1_value, dpp2_value, dpp3_value;
extern int check_rominfo(ImageHandle *fh, int skip);
extern int get_rominfo(ImageHandle *fh, unsigned char *addr, unsigned int offset, unsigned char *offset_addr);

#endif
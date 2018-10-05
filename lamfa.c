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
#include "lamfa.h"
#include "table_spec.h"
#include "show_tables.h"

extern unsigned dpp1_value;

int check_lamfa(ImageHandle *fh, int skip)
{
	int found = 0;
	/*
	 * search: *** Find LAMFA Table by searching for correct function code byte sequence ***
	 */
	if(skip == 0) return found;		

	printf("\n-[ LAMFA Driver Requested Lambda ]-----------------------------------------------------------\n\n");
	printf(">>> Scanning for LAMFA Table Lookup code sequence... \n");
	found = find_dump_table_seg( fh->d.p, fh->len, &needle_LAMFA, &mask_LAMFA, needle_LAMFA_len, +46, +42, &LAMFA_table);

	if(found == 0) { printf("Sequence not found\n"); }
	if(found > 1)  { printf("**** Developer Warning****: False positive detected. >1 match found. Check needle is unique!\n"); }

	return found;
}

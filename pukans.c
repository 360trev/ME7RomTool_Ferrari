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
#include "kfkhfm.h"
#include "table_spec.h"

extern unsigned dpp1_value;

int check_pukans(ImageHandle *fh, int skip)
{
	int found = 0, seg, val;

	if(skip == 0) return found;		
	/*
	 * search: *** Find KFKHFM Table by searching for correct function code byte sequence ***
	 */
	printf("\n-[ PUKANS Air Pulsation correction Table ]-----------------------------------------------\n\n");
	printf(">>> Scanning for PUKANS Table Lookup code sequence - Variant #1 - Access via seg... \n");
	val =  44;					// byte offset to extract 16-bit address value for PUKANS table from mov. instruction (see needle)
	seg =  48;					// byte offset to extract 16-bit segment value for PUKANS table from mov. instruction (see needle)
	found = find_dump_table_seg( fh->d.p, fh->len, &needle_KFKHFM, &mask_KFKHFM, needle_KFKHFM_len, val, seg, &PUKANS_table);
//	printf( "seg_found=%d\n",found);
	if(found == 0) {		
		printf(">>> Scanning for PUKANS Table Lookup code sequence - Variant #1 - Access via dppx... \n");
		val =  44;				// byte offset to extract 16-bit address value for PUKANS table from mov. instruction (see needle)
		seg =  dpp1_value-1;	// implied from dppx, not specified directly (perhaps in the IROM function call itself)
		found = find_dump_table_dppx(fh->d.p, fh->len, &needle_KFKHFM, &mask_KFKHFM, needle_KFKHFM_len, val, seg, &PUKANS_table);
//		printf( "dppx_found=%d\n",found);
	}

	if(found == 0) { printf("Sequence not found\n"); }
	if(found > 1)  { printf("**** Developer Warning****: False positive detected. >1 match found. Check needle is unique!\n"); }	
	
	return found;
}

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
#include "needles.h"

/* Ferrari GGHFM_Lookup() function signature we need to find and match, 
 * XXXX can change between firmware releases 
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
//
// this is the needle (masked) for the GGHFM_Lookup() function
//

#if 0
 f2 f4 8c 88 		// mov
 48 40 				// cmp
 fd 0e 				// jmp
 
 f2 fc 8c 88 		// mov 
 7c 1c 				// shr
 46 fc 00 02 		// cmp 
 9d 05 				// jmpr
 
 f0 4c 				// mov
 5c 14 				// shl
 d4 54 ba 48 		// mov  [memadr] <-----
 0d 02 				// jmpr
 
 f2 f5 b8 4c		// mov
 0d 02              // jmpr

 f2 f5 ba 48  		// mov
 f6 f5 a8 f9  **	// mov
 f2 f4 b6 f9  		// mov
 f2 f5 b8 f9  		// mov
 02 f4 a8 f9 
 12 f5 1c ff 
 9d 03 				// jmpr
 
 e6 f4 ff ff 		// mov
 f0 54 				// mov
 f6 f4 b6 f9 		// mov
 f6 f5 b8 f9 		// mov
 24 8f ba f9 		// sub
 f2 f2 8a 88  		// mov
 48 20 				// cmp
 fd 0e				// jmp

 f2 fc 8a 88 		// mov
 7c 1c 				// shr
 46 fc 00 02 		// cmp
 9d 05 				// jmpr
 
 f0 4c				// mov 
 5c 14 				// shl
 d4 54 ba 48 		// mov  [memadr] <-----
 0d 02 		 		// jmpr

 f2 f5 b8 4c  		// mov
 0d 02 				// jmpr
 
 f2 f5 ba 48  		// mov
 f6 f5 a6 f9 		// mov
 f2 f4 c2 f9  		// mov
 f2 f5 c4 f9  		// mov
 02 f4 a6 f9 
 12 f5 1c ff 
 9d 03 				// jmpr

 e6 f4 ff ff 		// mov
 f0 54 				// mov
 f6 f4 c2 f9 		// mov
 f6 f5 c4 f9 		// mov
 24 8f c6 f9 		// sub
 db 00				// rets
#endif

#if 0
const unsigned char full_needle_1[] = { 
 0xF2, 0xF4, XXXX, XXXX,       //GGHFM_DHFM_Lookup: mov     r4, segram_BASE_ADDRESS_810000
 0x48, 0x40,                   //                   cmp     r4, #0
 0xFD, 0x0E,                   //                   jmpr    cc_ULE, hardcoded_start
 
 0xF2, 0xFC, XXXX, XXXX,       //                   mov     r12, segram_BASE_ADDRESS_810000
 0x7C, 0x1C,                   // *                 shr     r12, #1
 0x46, 0xFC, 0x00, 0x02,       // *                 cmp     r12, #200h      ; Index cannot go out of the bounds of the linearization table
 0x9D, 0x05,                   // *                 jmpr    cc_NC, end_of_table
 
 0xF0, 0x4C,                   // *                 mov     r4, r12
 0x5C, 0x14,                   // *                 shl     r4, #1
 0xD4, 0x54, XXXX, XXXX,       // *                 mov     r5, [r4+MLHFM]
 0x0D, 0x02,                   //                   jmpr    cc_UC, relative_address
 
 0xF2, 0xF5, XXXX, XXXX,       // end_of_table:     mov     r5, MLHFM_END
 0x0D, 0x02,                   // relative_address: jmpr    cc_UC, continue_dhfm
 0xF2, 0xF5, XXXX, XXXX,       // hardcoded_start:  mov     r5, MLHFM      ; <---* MLHFM Table
 0xF2, 0xFA, XXXX, XXXX,       // continue_dhfm:    mov     r10, MLHFM_var1
 0xF2, 0xFB, XXXX, XXXX,       //                   mov     r11, MLHFM_var2
 0x00, 0xA5,                   //                   add     r10, r5
 0x12, 0xFB, 0x1C, 0xFF,       //                   addc    r11, ZEROS
 0x9D, 0x03,                   //                   jmpr    cc_NC, init_mlhfm
 0xE6, 0xFA, 0xFF, 0xFF,       //                   mov     r10, #0FFFFh
 0xF0, 0xBA,                   //                   mov     r11, r10
 0xF6, 0xFA, XXXX, XXXX,       // init_mlhfm:       mov     MLHFM_var1, r10
 0xF6, 0xFB, XXXX, XXXX,       //                   mov     MLHFM_var2, r11
 0x24, 0x8F, XXXX, XXXX,       //                   sub     MLHFM_var3, ONES
 0xDB, 0x00                    //                   rets
};

const unsigned char full_mask_1[] = { 
 MASK, MASK, SKIP, SKIP,       //GGHFM_DHFM_Lookup: mov     r4, segram_BASE_ADDRESS_810000
 MASK, MASK,                   //                   cmp     r4, #0
 MASK, MASK,                   //                   jmpr    cc_ULE, hardcoded_start
 MASK, MASK, SKIP, SKIP,       //                   mov     r12, segram_BASE_ADDRESS_810000
 MASK, MASK,                   // *                 shr     r12, #1
 MASK, MASK, MASK, MASK,       // *                 cmp     r12, #200h      ; Index cannot go out of the bounds of the linearization table
 MASK, MASK,                   // *                 jmpr    cc_NC, end_of_table
 MASK, MASK,                   // *                 mov     r4, r12
 MASK, MASK,                   // *                 shl     r4, #1
 MASK, MASK, SKIP, SKIP,       // *                 mov     r5, [r4+MLHFM]
 MASK, MASK,                   //                   jmpr    cc_UC, relative_address
 MASK, MASK, SKIP, SKIP,       // end_of_table:     mov     r5, MLHFM_END
 MASK, MASK,                   // relative_address: jmpr    cc_UC, continue_dhfm
 MASK, MASK, SKIP, SKIP,       // hardcoded_start:  mov     r5, MLHFM      ; <---* MLHFM Table
 MASK, MASK, SKIP, SKIP,       // continue_dhfm:    mov     r10, MLHFM_var1
 MASK, MASK, SKIP, SKIP,       //                   mov     r11, MLHFM_var2
 MASK, MASK,                   //                   add     r10, r5
 MASK, MASK, MASK, MASK,       //                   addc    r11, ZEROS
 MASK, MASK,                   //                   jmpr    cc_NC, init_mlhfm
 MASK, MASK, MASK, MASK,       //                   mov     r10, #0FFFFh
 MASK, MASK,                   //                   mov     r11, r10
 MASK, MASK, SKIP, SKIP,       // init_mlhfm:       mov     MLHFM_var1, r10
 MASK, MASK, SKIP, SKIP,       //                   mov     MLHFM_var2, r11
 MASK, MASK, SKIP, SKIP,       //                   sub     MLHFM_var3, ONES
 MASK, MASK                    //                   rets
};
#endif

const unsigned char needle_1[] = { 
 0x7c, 0x1c,                 // shr   r12, #1
 0x46, 0xFC, 0x00, 0x02,       // *                 cmp     r12, #200h      ; Index cannot go out of the bounds of the linearization table
// 0x46, 0xfc, XXXX, XXXX,     // cmp   r12, #XXXXh
 0x9d, 0x05,                 // jmpr  cc_NC, end_of_table
 0xf0, 0x4c,                 // mov   r4, r12
 0x5c, 0x14,                 // shl   r4, #1,
 0xd4, 0x54, XXXX, XXXX      // mov   r5, [r4 + XXXX]    <--- * this offset + 0x10000 [map base in rom] gives offset (instruction is in little endian format so conversion neccesary) in file to MLHFM Linearization 1kbyte (512 entries of 16-bits) table
};
unsigned int needle_1_len = sizeof(needle_1);

const unsigned char mask_1[] = { 
 MASK, MASK,                 // shr   r12, #1
 MASK, MASK, MASK, MASK,     // cmp   r12, #200h
// MASK, MASK, SKIP, SKIP,     // cmp   r12, #200h
 MASK, MASK,                 // jmpr  cc_NC, end_of_table
 MASK, MASK,                 // mov   r4, r12
 MASK, MASK,                 // shl   r4, #1
 MASK, MASK, SKIP, SKIP      // mov   r5, [r4 + 4300]
};

//
// this is the needle (masked) for the Main Checksum function to extract Start Address and End Address 
//

const unsigned char needle_2[] = {
 0xF6, 0x8E, XXXX, XXXX,     // mov  var_x, zeros
 0xF6, 0x8E, XXXX, XXXX,     // mov  var_x, zeros
 0xF7, 0x8E, XXXX, XXXX,     // movb var_x, zeros
 
 0xD7, 0x50, XXXX, XXXX,     // extp #XXXXh, #2       <--- * this is the segment offset
 0xF2, XXXX, YYYY, YYYY,     // mov  rX, var_y        <--- * this is the offset to the hi word (start address for main rom)
 0xF2, XXXX, YYYY, YYYY,     // mov  rX, var_y        <--- * this is the offset to the lo word (start address for main rom)
 0xF6, YYYY, XXXX, XXXX,     // mov  var_x, rY
 0xF6, YYYY, XXXX, XXXX,     // mov  var_x, rY
 
 0xE1, 0x18,                 // movb r14, #1
 0xF7, YYYY, XXXX, XXXX,     // movb var_x, rY

 0xD7, 0x50, XXXX, XXXX,     // extp #XXXXh, #2       <--- * this is the segment offset (typically 0x206 or 0x207)
 0xF2, XXXX, YYYY, YYYY,     // mov  rX, var_y        <--- * this is the offset to the hi word (end address for main rom)
 0xF2, XXXX, YYYY, YYYY,     // mov  rX, var_y        <--- * this is the offset to the lo word (end address for main rom)
 0xF6, YYYY, XXXX, XXXX,     // mov  var_x, rY
 0xF6, YYYY, XXXX, XXXX,     // mov  var_x, rY

 0xDB, 0x00                  // rets 
};
unsigned int needle_2_len = sizeof(needle_2);

const unsigned char mask_2[] = {
 MASK, MASK, SKIP, SKIP,     // mov  var_x, zeros
 MASK, MASK, SKIP, SKIP,     // mov  var_x, zeros
 MASK, MASK, SKIP, SKIP,     // movb var_x, zeros
 
 MASK, MASK, SKIP, SKIP,     // extp #XXXXh, #2
 MASK, SKIP, SKIP, SKIP,     // mov  rX, var_y
 MASK, SKIP, SKIP, SKIP,     // mov  rX, var_y
 MASK, SKIP, SKIP, SKIP,     // mov  var_x, rY
 MASK, SKIP, SKIP, SKIP,     // mov  var_x, rY
 
 MASK, MASK,                 // movb r14, #1
 MASK, SKIP, SKIP, SKIP,     // movb var_x, rY

 MASK, MASK, SKIP, SKIP,     // extp #XXXXh, #2
 MASK, SKIP, SKIP, SKIP,     // mov  rX, var_y
 MASK, SKIP, SKIP, SKIP,     // mov  rX, var_y
 MASK, SKIP, SKIP, SKIP,     // mov  var_x, rY
 MASK, SKIP, SKIP, SKIP,     // mov  var_x, rY
 
 MASK, MASK                  // rets
};

#if 0
const unsigned char needle_2v2[] = {
 0xE1, 0x28,                 // movb    rl4, #2
 0x05, 0xF8, XXXX, XXXX,     // addb    byte_380103, rl4
 0xC2, 0xF6, XXXX, XXXX,     // movbz   r6, byte_380103
 0x5C, 0x26,                 // shl     r6, #2
 0xD7, 0x60, XXXX, XXXX,     // extp    #206h, #3          +14
 0xD4, 0x46, XXXX, XXXX,     // mov     r4, [r6+start_hi]  +18
 0xD4, 0x56, XXXX, XXXX,     // mov     r5, [r6+start_lo]  +22
 0xCC, 0x00,                 // nop                        
 0xF6, 0xF4, XXXX, XXXX,     // mov     word_3800FE, r4    
 0xF6, 0xF5, XXXX, XXXX,     // mov     word_380100, r5    
 0xD7, 0x60, XXXX, XXXX,     // extp    #206h, #3          +36
 0xD4, 0x46, XXXX, XXXX,     // mov     r4, [r6+end_hi]    +40
 0xD4, 0x56, XXXX, XXXX,     // mov     r5, [r6+end_lo]    +44
 0xCC, 0x00,                 // nop
 0xF6, 0xF4, XXXX, XXXX,     // mov     word_380104, r4
 0xF6, 0xF5, XXXX, XXXX      // mov     word_380106, r5
};
unsigned int needle_2v2_len = sizeof(needle_2v2);
 
const unsigned char mask_2v2[] = {
 MASK, MASK,                 // movb    rl4, #2
 MASK, MASK, SKIP, SKIP,     // addb    byte_380103, rl4
 MASK, MASK, SKIP, SKIP,     // movbz   r6, byte_380103
 MASK, MASK,                 // shl     r6, #2
 MASK, MASK, SKIP, SKIP,     // extp    #206h, #3
 MASK, MASK, SKIP, SKIP,     // mov     r4, [r6+start_hi]
 MASK, MASK, SKIP, SKIP,     // mov     r5, [r6+start_lo]
 MASK, MASK,                 // nop
 MASK, MASK, SKIP, SKIP,     // mov     word_3800FE, r4
 MASK, MASK, SKIP, SKIP,     // mov     word_380100, r5
 MASK, MASK, SKIP, SKIP,     // extp    #206h, #3
 MASK, MASK, SKIP, SKIP,     // mov     r4, [r6+end_hi]
 MASK, MASK, SKIP, SKIP,     // mov     r5, [r6+end_lo]
 MASK, MASK,                 // nop
 MASK, MASK, SKIP, SKIP,     // mov     word_380104, r4
 MASK, MASK, SKIP, SKIP      // mov     word_380106, r5
};
#endif

//
// this is the needle for the Main Checksum function to extract the "Number of entries in the table"
//

const unsigned char needle_2b[] = {
 0x88, 0x90,                 // mov     [-r0], r9
 0x88, 0x80,                 // mov     [-r0], r8
 0x88, 0x70,                 // mov     [-r0], r7
 0x88, 0x60,                 // mov     [-r0], r6

 0xDA, XXXX, YYYY, YYYY,     // calls   XXXXh, sub_YYYY
 0xF3, 0xF8, YYYY, YYYY,     // movb    rl4, YYYY

 0x49, 0x81,                 // cmpb    rl4, #1 						<---- * start entry (always 1)
 0xEA, 0x30, YYYY, YYYY,     // jmpa    cc_NZ, sub_YYYY 

 0xF3, 0xFA, YYYY, YYYY,     // movb    rl5, YYYY						current_table_index
 0x49, YYYY,                 // cmpb    rl5, #YY                        <---- * number of entries in table (0xA2=2, 0xA4=4, 0xA6=6)
 0x9D, 0x27,                 // jmpr    cc_NC, start_sumloop

 0xF2, 0xF8, YYYY, YYYY,     // mov     r8, YYYY                        current_start_lo
 0xF2, 0xF9, YYYY, YYYY,     // mov     r9, YYYY                        current_start_hi
 0xE0, 0x07,                 // mov     r7, #0
 0x0D, 0x0A                  // jmpr    cc_UC, loc_E30BA
};
unsigned int needle_2b_len = sizeof(needle_2b);

const unsigned char mask_2b[] = {
 MASK, MASK,                 // mov     [-r0], r9
 MASK, MASK,                 // mov     [-r0], r8
 MASK, MASK,                 // mov     [-r0], r7
 MASK, MASK,                 // mov     [-r0], r6

 MASK, SKIP, SKIP, SKIP,     // calls   XXXXh, sub_YYYY
 MASK, MASK, SKIP, SKIP,     // movb    rl4, YYYY

 MASK, MASK,                 // cmpb    rl4, #1 						<---- * start entry (always 1)
 MASK, MASK, SKIP, SKIP,     // jmpa    cc_NZ, sub_YYYY 

 MASK, MASK, SKIP, SKIP,     // movb    rl5, YYYY						current_table_index
 MASK, SKIP,                 // cmpb    rl5, #YY                        <---- * number of entries in table (0xA2=2, 0xA4=4, 0xA6=6) [+28]
 MASK, MASK,                 // jmpr    cc_NC, start_sumloop

 MASK, MASK, SKIP, SKIP,     // mov     r8, YYYY                        current_start_lo
 MASK, MASK, SKIP, SKIP,     // mov     r9, YYYY                        current_start_hi
 MASK, MASK,                 // mov     r7, #0
 MASK, MASK                  // jmpr    cc_UC, loc_E30BA
};
 
 
//
// this is the needle (masked) for the Main Checksum function to extract the stored checksum Hi & Lo Words
//

const unsigned char needle_3[] = {

 0xF2, 0xF4, XXXX, XXXX, 	 // mov r4, var_Y
 0xF2, 0xF5, XXXX, XXXX, 	 // mov r5, var_Y
 0xD7, 0x50, XXXX, XXXX,	 // extp #XXXXh, #2                <--- * this is the segment offset
 0x22, 0xF4, XXXX, XXXX,	 // sub r4, var_Y                  <--- * this is offset to the word (stored checksum)
 0x32, 0xF5, XXXX, XXXX,	 // subc r5, var_Y                 <--- * this is offset to the word (stored checksum)
 0x3D, XXXX,                 // jmpr cc_NZ, offset_XXXX
 
 0xE6, 0xF4, XXXX, XXXX,     // mov r4, #XXXXh
 0xE6, 0xF5, XXXX, XXXX,     // mov r5, #XXXXh
 0xDC, 0x45,                 // extp r5, #1
 0xA9, 0x64, 			     // movb r13, [r4]
 0x77, 0xF6, 0x08, 0x00,     // orb  r13, #8
 0xE6, 0xF4, XXXX, XXXX,     // mov  r4, #XXXXh
 0xE6, 0xF5, XXXX, XXXX,     // mov  r5, #XXXXh
 0xDC, 0x45,                 // extp r5, #1
 0xB9, 0x64,                 // mov [r4], r13
 0x0D, 0x0E                  // jmpr cc_UC, +0xE
};
unsigned int needle_3_len = sizeof(needle_3);

const unsigned char mask_3[] = {

 MASK, MASK, SKIP, SKIP, 	 // mov r4, var_Y
 MASK, MASK, SKIP, SKIP, 	 // mov r5, var_Y
 MASK, MASK, SKIP, SKIP,	 // extp #XXXXh, #2                <--- * this is the segment offset (should be 0x21f in 512kb & 0x23f in 1024kb ROM)
 MASK, MASK, SKIP, SKIP,	 // sub r4, var_Y                 <--- * this is offset to the [EndFirmware_hi] word (stored checksum)
 MASK, MASK, SKIP, SKIP,	 // subc r5, var_Y                <--- * this is offset to the [EndFirmware_lo] word (stored checksum)
 MASK, SKIP,                 // jmpr cc_NZ, offset_XX
 
 MASK, MASK, SKIP, SKIP,     // mov r4, #XXXXh
 MASK, MASK, SKIP, SKIP,     // mov r5, #XXXXh
 MASK, MASK,                 // extp r5, #1
 MASK, MASK, 			     // movb r13, [r4]
 MASK, MASK, MASK, MASK,     // orb  r13, #8
 MASK, MASK, SKIP, SKIP,     // mov  r4, #XXXXh
 MASK, MASK, SKIP, SKIP,     // mov  r5, #XXXXh
 MASK, MASK,                 // extp r5, #1
 MASK, MASK,                 // mov [r4], r13
 MASK, MASK                  // jmpr cc_UC, +0xE
};

//
// variant 2
//
const unsigned char needle_3b[] = {
 0xF6, 0xF8, XXXX, XXXX,     // mov     var_X, r8
 0xF6, 0xF9, XXXX, XXXX,     // mov     var_X, r9 
 0xF0, 0x48,                 // mov     r4, r8
 0xF0, 0x59,                 // mov     r5, r9
 0x22, 0xF4, XXXX, XXXX,     // sub     r4, var_Y
 0x32, 0xF5, XXXX, XXXX,     // subc    r5, var_Y
 0xEA, 0x80, XXXX, XXXX,     // jmpa    cc_C, loc_YYYY
 0x0D, SKIP,                 // jmpr    cc_UC, loc_YYYY [26]
 
 0xF2, 0xF4, XXXX, XXXX,     // mov     r4, var_Y
 0xF2, 0xF5, XXXX, XXXX,     // mov     r5, var_Y
 0xD7, 0x50, XXXX, XXXX,     // extp    #XXXXh, #2 ; '?'         <--- * this is the segment offset [+36]
 0x22, 0xF4, XXXX, XXXX,     // sub     r4, checksum_stored_hi   <--- * this is offset to the word (stored checksum) [+40]
 0x32, 0xF5, XXXX, XXXX,     // subc    r5, checksum_stored_lo   <--- * this is offset to the word (stored checksum) [+44]
 0x3D, XXXX,                 // jmpr    cc_NZ, loc_YYYY
// 0xE1, 0x88,                 // movb    rl4, #8
// 0x75, 0xF8, XXXX, XXXX,     // orb     var_X, rl4
// 0x0D, 0x04                  // jmpr    cc_UC, loc_YYYY
};
unsigned int needle_3b_len = sizeof(needle_3b);

const unsigned char mask_3b[] = {
 MASK, MASK, SKIP, SKIP,     // mov     var_X, r8
 MASK, MASK, SKIP, SKIP,     // mov     var_X, r9
 MASK, MASK,                 // mov     r4, r8
 MASK, MASK,                 // mov     r5, r9
 MASK, MASK, SKIP, SKIP,     // sub     r4, var_Y
 MASK, MASK, SKIP, SKIP,     // subc    r5, var_Y
 MASK, MASK, SKIP, SKIP,     // jmpa    cc_C, loc_YYYY
 MASK, SKIP,                 // jmpr    cc_UC, loc_YYYY
 
 MASK, MASK, SKIP, SKIP,     // mov     r4, var_Y
 MASK, MASK, SKIP, SKIP,     // mov     r5, var_Y
 MASK, MASK, SKIP, SKIP,     // extp    #XXXXh, #2 ; '?'         <--- * this is the segment offset
 MASK, MASK, SKIP, SKIP,     // sub     r4, checksum_stored_hi   <--- * this is offset to the word (stored checksum)
 MASK, MASK, SKIP, SKIP,     // subc    r5, checksum_stored_lo   <--- * this is offset to the word (stored checksum)
 MASK, SKIP,                 // jmpr    cc_NZ, loc_YYYY
// MASK, MASK,                 // movb    rl4, #8
// MASK, MASK, SKIP, SKIP,     // orb     var_X, rl4
// MASK, MASK                  // jmpr    cc_UC, loc_YYYY
};

//
// this is the needle (masked) for the Multipoint Checksum function to extract the stored list of checksum Hi & Lo Words
//

const unsigned char needle_4[] = {
 0x98, 0x24,               // mov     r2, [r4+]          
 0xA8, 0x34,               // mov     r3, [r4]           
 0x00, 0xA2,               // add     r10, r2            
 0x10, 0xB3,               // addc    r11, r3            
 0x26, 0xFA, 0xFF, 0xFF,   // sub     r10, #0FFFFh       
 0x36, 0xFB, 0xFF, 0xFF,   // subc    r11, #0FFFFh          
 0x3D, 0x08,               // jmpr    cc_NZ, loop_calc_sums    [18]
 //
 0xE6, 0xF4, XXXX, XXXX,   // mov     r4, #XXXXh          
 0xE0, 0x05,               // mov     r5, #0             
 0xF6, 0xF4, 0x90, 0xA4,   // mov     _MultipointChecksumBlk, r4    
 0xF6, 0xF5, 0x92, 0xA4,   // mov     _h207, r5          
 0xDB, 0x00,               // rets                             [34]       
 // loop_calc_sums:
 0xE6, 0xF4, 0x00, 0x3C,   // mov     r4, #MultipointBlk        <--- *** This is the multipoint table in ram
 0xE6, 0xF5, XXXX, XXXX,   // mov     r5, #XXXXh          
 0xF6, 0xF4, 0x90, 0xA4,   // mov     _MultipointChecksumBlk, r4    
 0xF6, 0xF5, 0x92, 0xA4,   // mov     _h207, r5          
 0xDB, 0x00,               // rets                             [52]     
 // exit_chsum:
 0xE6, 0xF4, 0x00, 0x3C,   // mov     r4, #MultipointChksumBlk_81fc00 <--- *** This is the multipoint table in ROM (the offset to the table!)
 0xE6, 0xF5, 0x07, 0x02,   // mov     r5, #207h          
 0xF6, 0xF4, 0x90, 0xA4,   // mov     _MultipointChecksumBlk, r4    
 0xF6, 0xF5, 0x92, 0xA4,   // mov     _h207, r5          
 0xDB, 0x00                // rets         
};
unsigned int needle_4_len = sizeof(needle_4);

const unsigned char mask_4[] = {
 MASK, MASK,               // mov     r2, [r4+]          
 MASK, MASK,               // mov     r3, [r4]           
 MASK, MASK,               // add     r10, r2             
 MASK, MASK,               // addc    r11, r3            
 MASK, MASK, MASK, MASK,   // sub     r10, #0FFFFh       
 MASK, MASK, MASK, MASK,   // subc    r11, #0FFFFh          
 MASK, MASK,               // jmpr    cc_NZ, loop_calc_sums    
 //
 MASK, MASK, SKIP, SKIP,   // mov     r4, #1E0h          
 MASK, MASK,               // mov     r5, #0             
 MASK, MASK, SKIP, SKIP,   // mov     _MultipointChecksumBlk, r4    
 MASK, MASK, SKIP, SKIP,   // mov     _h207, r5          
 MASK, MASK,               // rets         
 // loop_calc_sums:
 MASK, MASK, SKIP, SKIP,   // mov     r4, #MultipointBlk ; <--- *** This is the multipoint table ****************
 MASK, MASK, SKIP, SKIP,   // mov     r5, #207h          
 MASK, MASK, SKIP, SKIP,   // mov     _MultipointChecksumBlk, r4    
 MASK, MASK, SKIP, SKIP,   // mov     _h207, r5          
 MASK, MASK,               // rets         
 // exit_chsum:
 MASK, MASK, SKIP, SKIP,   // mov     r4, #MultipointChksumBlk_81fc00 ; Multipoint chksum block
 MASK, MASK, SKIP, SKIP,   // mov     r5, #207h          
 MASK, MASK, SKIP, SKIP,   // mov     _MultipointChecksumBlk, r4    
 MASK, MASK, SKIP, SKIP,   // mov     _h207, r5          
 MASK, MASK                // rets         
};


const unsigned char needle_4aa[] = {
 0x9A, 0x89, XXXX, XXXX,   // jnb     ROMEN, loc_XXXX
 0xF6, 0x8E, XXXX, XXXX,   // mov     word_XXXX, ZEROS
 0x0D, XXXX,               // jmpr    cc_UC, loc_XXXX
 0xE0, 0x84,               // mov     r4, #8
 0xF6, 0xF4, XXXX, XXXX,   // mov     word_XXXX, r4
 0xF2, 0xF4, XXXX, XXXX,   // mov     r4, word_XXXX
 0x5C, 0x24,               // shl     r4, #2
 0xD7, 0x60, XXXX, XXXX,   // extp    #207h, #3             [+24] <--- * this is the segment offset
 0xD4, 0xA4, XXXX, XXXX,   // mov     r10, [r4+word_81FC00] [+28] <--- *** This is the multipoint hi table ****************
 0xD4, 0xB4, XXXX, XXXX,   // mov     r11, [r4+word_81FC02] [+32] <--- *** This is the multipoint lo table ****************
 0xCC, 0x00,               // nop
 0xF6, 0xFA, XXXX, XXXX,   // mov     XXXX, r10
 0xF6, 0xFB, XXXX, XXXX    // mov     XXXX, r11
};
unsigned int needle_4aa_len = sizeof(needle_4aa);

const unsigned char mask_4aa[] = {
 MASK, MASK, SKIP, SKIP,   // jnb     ROMEN, loc_XXXX
 MASK, MASK, SKIP, SKIP,   // mov     word_XXXX, ZEROS
 MASK, SKIP,               // jmpr    cc_UC, loc_XXXX
 MASK, MASK,               // mov     r4, #8
 MASK, MASK, SKIP, SKIP,   // mov     word_XXXX, r4
 MASK, MASK, SKIP, SKIP,   // mov     r4, word_XXXX
 MASK, MASK,               // shl     r4, #2
 MASK, MASK, SKIP, SKIP,   // extp    #207h, #3
 MASK, MASK, SKIP, SKIP,   // mov     r10, [r4+word_81FC00]
 MASK, MASK, SKIP, SKIP,   // mov     r11, [r4+word_81FC02]
 MASK, MASK,               // nop
 MASK, MASK, SKIP, SKIP,   // mov     XXXX, r10
 MASK, MASK, SKIP, SKIP    // mov     XXXX, r11
};

//
// Easily Identify the dpp0 - dpp3 register rom settings
//

const unsigned char needle_dpp[] = {
 0xE6, 0x00, XXXX, XXXX,   // mov     DPP0, #XXXXh
 0xE6, 0x01, XXXX, XXXX,   // mov     DPP1, #XXXXh
 0xE6, 0x02, XXXX, XXXX,   // mov     DPP2, #XXXXh 
 0xE6, 0x03, XXXX, XXXX   // mov     DPP3, #XXXX
};
unsigned int needle_dpp_len = sizeof(needle_dpp);

const unsigned char mask_dpp[] = {
 MASK, MASK, SKIP, SKIP,   // mov     DPP0, #XXXXh
 MASK, MASK, SKIP, SKIP,   // mov     DPP1, #XXXXh
 MASK, MASK, SKIP, SKIP,   // mov     DPP2, #XXXXh 
 MASK, MASK, SKIP, SKIP   // mov     DPP3, #XXXX
};

//
// this needle is to extract the number of entries contained in the multipoint checksum table
//

const unsigned char needle_4b[] = {
 0x98, 0x4A,               // mov     r4, [r10+]
 0xA8, 0x5A,               // mov     r5, [r10]
 0xF6, 0xF4, XXXX, XXXX,   // mov     word_XXXX, r4
 0xF6, 0xF5, XXXX, XXXX,   // mov     word_XXXX, r5
 0xF6, 0x8E, XXXX, XXXX,   // mov     word_XXXX, ZEROS
 0xF6, 0x8E, XXXX, XXXX,   // mov     word_XXXX, ZEROS
 0xF2, 0xF2, XXXX, XXXX,   // mov     r2, word_XXXX
 0x66, 0xF2, 0xFF, 0x47,   // and     r2, #47FFh
 0x76, 0xF2, 0xFF, 0x40,   // or      r2, #40FFh
 0xF6, 0xF2, XXXX, XXXX,   // mov     word_XXXX, r2
 0xF2, 0xF4, XXXX, XXXX,   // mov     r4, word_XXXX
 0x46, 0xF4, XXXX, XXXX,   // cmp     r4, #XX					<--- * this is the number of entries in the multipoint checksum oop
 0xEA, 0x90, XXXX, XXXX    // jmpa    cc_NC, loc_XXXX
};
unsigned int needle_4b_len = sizeof(needle_4b);

const unsigned char mask_4b[] = {
 MASK, MASK,               // mov     r4, [r10+]
 MASK, MASK,               // mov     r5, [r10]
 MASK, MASK, SKIP, SKIP,   // mov     word_XXXX, r4
 MASK, MASK, SKIP, SKIP,   // mov     word_XXXX, r5
 MASK, MASK, SKIP, SKIP,   // mov     word_XXXX, ZEROS
 MASK, MASK, SKIP, SKIP,   // mov     word_XXXX, ZEROS
 MASK, MASK, SKIP, SKIP,   // mov     r2, word_XXXX
 MASK, MASK, MASK, MASK,   // and     r2, #47FFh
 MASK, MASK, MASK, MASK,   // or      r2, #40FFh
 MASK, MASK, SKIP, SKIP,   // mov     word_XXXX, r2
 MASK, MASK, SKIP, SKIP,   // mov     r4, word_XXXX
 MASK, MASK, SKIP, SKIP,   // cmp     r4, #XX			[+42]		<--- * this is the number of entries in the multipoint checksum table [16 bit]
 MASK, MASK, SKIP, SKIP    // jmpa    cc_NC, loc_XXXX
};

const unsigned char needle_4c[] = {
 0xF3, 0xF8, XXXX, XXXX,      // movb    rl4, byte_XXXX
 0x43, 0xF8, XXXX, XXXX,      // cmpb    rl4, byte_XXXX
 0xEA, 0x30, XXXX, XXXX,      // jmpa    cc_NZ, multipoint_range_checks_11

 0xF3, 0xFA, XXXX, XXXX,      // movb    rl5, byte_XXXX
 0x49, 0xA1,                  // cmpb    rl5, #1 
 0xEA, 0x20, XXXX, XXXX,      // jmpa    cc_Z, multipoint_range_checks_11

 0xF7, 0x8F, XXXX, XXXX,      // movb    byte_XXXX, ONES
 0x46, 0xF6, 0x00, 0x10,      // cmp     r6, #1000h
 0xEA, 0x30, XXXX, XXXX,      // jmpa    cc_NZ, multipoint_range_checks_4

 0xF2, 0xF6, XXXX, XXXX,      // mov     r6, word_XXXX
 0x66, 0xF6, 0xFF, 0x00,      // and     r6, #0FFh
 0x46, 0xF6, 0x22, 0x00,      // cmp     r6, #22h ; '"'  [+44]         <--- * number of entries in the multipoint checksum table (16-bit hex value)
 0xEA, 0xE0, XXXX, XXXX,      // jmpa    cc_UGT, multipoint_range_checks_4

 0xF0, 0x46,                  // mov     r4, r6
};
unsigned int needle_4c_len = sizeof(needle_4c);

const unsigned char mask_4c[] = {
 MASK, MASK, SKIP, SKIP,      // movb    rl4, byte_XXXX
 MASK, MASK, SKIP, SKIP,      // cmpb    rl4, byte_XXXX
 MASK, MASK, SKIP, SKIP,      // jmpa    cc_NZ, multipoint_range_checks_11

 MASK, MASK, SKIP, SKIP,      // movb    rl5, byte_XXXX
 MASK, MASK,                  // cmpb    rl5, #1 
 MASK, MASK, SKIP, SKIP,      // jmpa    cc_Z, multipoint_range_checks_11

 MASK, MASK, SKIP, SKIP,      // movb    byte_XXXX, ONES
 MASK, MASK, MASK, MASK,      // cmp     r6, #1000h
 MASK, MASK, SKIP, SKIP,      // jmpa    cc_NZ, multipoint_range_checks_4

 MASK, MASK, SKIP, SKIP,      // mov     r6, word_XXXX
 MASK, MASK, MASK, MASK,      // and     r6, #0FFh                             masked ensures no more than 256 entries ...
 MASK, MASK, SKIP, SKIP,      // cmp     r6, #22h ; '"'              [+44]     <-------- * number of entries in table (16-bit hex value) 
 MASK, MASK, SKIP, SKIP,      // jmpa    cc_UGT, multipoint_range_checks_4

 MASK, MASK,                  // mov     r4, r6
};



//
// this is the needle (masked) for the Ferrari/Alfa Seed Key routine
//

const unsigned char needle_5[] = {
 0x88, 0xC0,               // mov     [-r0], r12
 0x88, 0x90,               // mov     [-r0], r9
 0x88, 0x80,               // mov     [-r0], r8
 0x88, 0x70,               // mov     [-r0], r7
 0x88, 0x60,               // mov     [-r0], r6
 0xF0, 0x7D,               // mov     r7, r13
 0xF0, 0x8E,               // mov     r8, r14
 0xF0, 0x9F,               // mov     r9, r15
 0x07, 0xFE, 0x23, 0x00,   // addb    rl7, #23h ; '#'
 0x47, 0xFE, 0x23, 0x00,   // cmpb    rl7, #23h ; '#'
 0x9D, 0x02,               // jmpr    cc_NC, loc_XXXX
 0xE7, 0xFE, 0xFF, 0x00,   // movb    rl7, #0FFh
 0xE1, 0x0C,               // movb    rl6, #0
 0x0D, 0x12,               // jmpr    cc_UC, loop_enter
 0x46, 0xF9, 0x00, 0x80,   // cmp     r9, #8000h
 0x3D, 0x01,               // jmpr    cc_NZ, loc_XXXX
 0x48, 0x80,               // cmp     r8, #0
 0x8D, 0x0A,               // jmpr    cc_C, loc_XXXX
 0xF4, 0x80, 0x08, 0x00,   // movb    rl4, [r0+8]
 0xC0, 0x8C,               // movbz   r12, rl4
 0xF0, 0xD8,               // mov     r13, r8
 0xF0, 0xE9,               // mov     r14, r9
 0xDA, 0x00, 0x90, 0x60,   // calls   0, unk_XXXX
 0xF0, 0x84,               // mov     r8, r4
 0xF0, 0x95,               // mov     r9, r5
 0x0D, 0x02,               // jmpr    cc_UC, loc_XXXX
 0x00, 0x88,               // add     r8, r8
 0x10, 0x99,               // addc    r9, r9
 0x09, 0xC1,               // addb    rl6, #1
 0x41, 0xCE,               // cmpb    rl6, rl7
 0x8D, 0xEC,               // jmpr    cc_C, loop_key
 0xD4, 0x40, 0x0A, 0x00,   // mov     r4, [r0+0Ah]
 0xD4, 0x50, 0x0C, 0x00,   // mov     r5, [r0+0Ch]
 0x20, 0x48,               // sub     r4, r8
 0x30, 0x59,               // subc    r5, r9
 0x3D, 0x02,               // jmpr    cc_NZ, key_bad_exit
 0xE0, 0x14,               // mov     r4, #1          ; ME7_SeedKeyCheck+5A   OUT: r4 - 0=key bad, 0x1=key matches...
 0x0D, 0x01,               // jmpr    cc_UC, key_match_exit
 0xE0, 0x04,               // mov     r4, #0          ; ME7_SeedKeyCheck+5C   OUT: r4 - 0=key bad, 0x1=key matches...  <---- *** Change this to a 0x01 to a 0x14 to defeat routine
 0x98, 0x60,               // mov     r6, [r0+]
 0x98, 0x70,               // mov     r7, [r0+]
 0x98, 0x80,               // mov     r8, [r0+]
 0x98, 0x90,               // mov     r9, [r0+]
 0x08, 0x02,               // add     r0, #2
 0xDB, 0x00                // rets
};
unsigned int needle_5_len = sizeof(needle_5);

const unsigned char mask_5[] = {
 MASK, MASK,               // mov     [-r0], r12
 MASK, MASK,               // mov     [-r0], r9
 MASK, MASK,               // mov     [-r0], r8
 MASK, MASK,               // mov     [-r0], r7
 MASK, MASK,               // mov     [-r0], r6
 MASK, MASK,               // mov     r7, r13
 MASK, MASK,               // mov     r8, r14
 MASK, MASK,               // mov     r9, r15
 MASK, MASK, MASK, MASK,   // addb    rl7, #23h ; '#'
 MASK, MASK, MASK, MASK,   // cmpb    rl7, #23h ; '#'
 MASK, MASK,               // jmpr    cc_NC, loc_XXXX
 MASK, MASK, MASK, MASK,   // movb    rl7, #0FFh
 MASK, MASK,               // movb    rl6, #0
 MASK, MASK,               // jmpr    cc_UC, loop_enter
 MASK, MASK, MASK, MASK,   // cmp     r9, #8000h
 MASK, MASK,               // jmpr    cc_NZ, loc_XXXX
 MASK, MASK,               // cmp     r8, #0
 MASK, MASK,               // jmpr    cc_C, loc_XXXX
 MASK, MASK, MASK, MASK,   // movb    rl4, [r0+8]
 MASK, MASK,               // movbz   r12, rl4
 MASK, MASK,               // mov     r13, r8
 MASK, MASK,               // mov     r14, r9
 MASK, MASK, MASK, MASK,   // calls   0, unk_XXXX
 MASK, MASK,               // mov     r8, r4
 MASK, MASK,               // mov     r9, r5
 MASK, MASK,               // jmpr    cc_UC, loc_XXXX
 MASK, MASK,               // add     r8, r8
 MASK, MASK,               // addc    r9, r9
 MASK, MASK,               // addb    rl6, #1
 MASK, MASK,               // cmpb    rl6, rl7
 MASK, MASK,               // jmpr    cc_C, loop_key
 MASK, MASK, MASK, MASK,   // mov     r4, [r0+0Ah]
 MASK, MASK, MASK, MASK,   // mov     r5, [r0+0Ch]
 MASK, MASK,               // sub     r4, r8
 MASK, MASK,               // subc    r5, r9
 MASK, MASK,               // jmpr    cc_NZ, key_bad_exit
 MASK, MASK,               // mov     r4, #1          ; ME7_SeedKeyCheck+5A   OUT: r4 - 0=key bad, 0x1=key matches...
 MASK, MASK,               // jmpr    cc_UC, key_match_exit
 MASK, SKIP,               // mov     r4, #0          ; ME7_SeedKeyCheck+5C   OUT: r4 - 0=key bad, 0x1=key matches...  <---- *** Change this to a 0x01 to a 0x14 to defeat routine
 MASK, MASK,               // mov     r6, [r0+]
 MASK, MASK,               // mov     r7, [r0+]
 MASK, MASK,               // mov     r8, [r0+]
 MASK, MASK,               // mov     r9, [r0+]
 MASK, MASK,               // add     r0, #2
 MASK, MASK                // rets
};

//
// this is the needle (masked) for the VAG/Other Seed Key routine
//

const unsigned char needle_6[] = {
 0x88, 0x60,               // mov     [-r0], r6
 0xF0, 0x4D,               // mov     r4, r13
 0x07, 0xF8, 0x23, 0x00,   // addb    rl4, #23h ; '#'
 0xF0, 0xD4,               // mov     r13, r4
 0x47, 0xF8, 0x23, 0x00,   // cmpb    rl4, #23h ; '#'
 0x9D, 0x02,               // jmpr    cc_NC, loc_475A
 0xE6, 0xFD, 0xFF, 0x00,   // mov     r13, #0FFh
 0xE1, 0x0C,               // movb    rl6, #0
 0x0D, 0x1A,               // jmpr    cc_UC, loc_4792
 0x46, 0xFF, 0x00, 0x80,   // cmp     r15, #8000h
 0x3D, 0x01,               // jmpr    cc_NZ, loc_4766
 0x48, 0xE0,               // cmp     r14, #0
 0x8D, 0x12,               // jmpr    cc_C, loc_478C
 0xF0, 0x4C,               // mov     r4, r12
 0xC0, 0x84,               // movbz   r4, rl4
 0x5C, 0x24,               // shl     r4, #2
 0xD7, 0x50, 0x00, 0x00,   // extp    #0, #2
 0xD4, 0xA4, 0x8C, 0x05,   // mov     r10, [r4+seed_hi]
 0xD4, 0xB4, 0x8E, 0x05,   // mov     r11, [r4+seed_lo]
#if 1
 0xF0, 0x4E,               // mov     r4, r14
 0xF0, 0x5F,               // mov     r5, r15
 0x00, 0x44,               // add     r4, r4
 0x10, 0x55,               // addc    r5, r5
 0x50, 0x4A,               // xor     r4, r10
 0x50, 0x5B,               // xor     r5, r11
 0xF0, 0xE4,               // mov     r14, r4
 0xF0, 0xF5,               // mov     r15, r5
 0x0D, 0x02,               // jmpr    cc_UC, loc_4790
 0x00, 0xEE,               // add     r14, r14
 0x10, 0xFF,               // addc    r15, r15
 0x09, 0xC1,               // addb    rl6, #1
 0xF0, 0x4D,               // mov     r4, r13
 0x41, 0xC8,               // cmpb    rl6, rl4
 0x8D, 0xE3,               // jmpr    cc_C, loc_475E
 0xD4, 0x40, 0x02, 0x00,   // mov     r4, [r0+2]
 0xD4, 0x50, 0x04, 0x00,   // mov     r5, [r0+4]
 0x20, 0x4E,               // sub     r4, r14
 0x30, 0x5F,               // subc    r5, r15
 0x3D, 0x02,               // jmpr    cc_NZ, loc_47AA
 0xE0, 0x14,               // mov     r4, #1
 0x0D, 0x01,               // jmpr    cc_UC, loc_47AC
 0xE0, 0x04,               // mov     r4, #0
 0x98, 0x60,               // mov     r6, [r0+]
 0xDB, 0x00                // rets
#endif
};
unsigned int needle_6_len = sizeof(needle_6);

const unsigned char mask_6[] = {
 MASK, MASK,               // mov     [-r0], r6
 MASK, MASK,               // mov     r4, r13
 MASK, MASK, MASK, MASK,   // addb    rl4, #23h ; '#'
 MASK, MASK,               // mov     r13, r4
 MASK, MASK, MASK, MASK,   // cmpb    rl4, #23h ; '#'
 MASK, MASK,               // jmpr    cc_NC, loc_475A
 MASK, MASK, MASK, MASK,   // mov     r13, #0FFh
 MASK, MASK,               // movb    rl6, #0
 MASK, MASK,               // jmpr    cc_UC, loc_4792
 MASK, MASK, MASK, MASK,   // cmp     r15, #8000h
 MASK, MASK,               // jmpr    cc_NZ, loc_4766
 MASK, MASK,               // cmp     r14, #0
 MASK, MASK,               // jmpr    cc_C, loc_478C
 MASK, MASK,               // mov     r4, r12
 MASK, MASK,               // movbz   r4, rl4
 MASK, MASK,               // shl     r4, #2
 MASK, MASK, SKIP, SKIP,   // extp    #0, #2
 MASK, MASK, SKIP, SKIP,   // mov     r10, [r4+seed_hi]
 MASK, MASK, SKIP, SKIP,   // mov     r11, [r4+seed_lo]
#if 1
 MASK, MASK,               // mov     r4, r14
 MASK, MASK,               // mov     r5, r15
 MASK, MASK,               // add     r4, r4
 MASK, MASK,               // addc    r5, r5
 MASK, MASK,               // xor     r4, r10
 MASK, MASK,               // xor     r5, r11
 MASK, MASK,               // mov     r14, r4
 MASK, MASK,               // mov     r15, r5
 MASK, MASK,               // jmpr    cc_UC, loc_4790
 MASK, MASK,               // add     r14, r14
 MASK, MASK,               // addc    r15, r15
 MASK, MASK,               // addb    rl6, #1
 MASK, MASK,               // mov     r4, r13
 MASK, MASK,               // cmpb    rl6, rl4
 MASK, MASK,               // jmpr    cc_C, loc_475E
 MASK, MASK, MASK, MASK,   // mov     r4, [r0+2]
 MASK, MASK, MASK, MASK,   // mov     r5, [r0+4]
 MASK, MASK,               // sub     r4, r14
 MASK, MASK,               // subc    r5, r15
 MASK, MASK,               // jmpr    cc_NZ, loc_47AA
 MASK, MASK,               // mov     r4, #1
 MASK, MASK,               // jmpr    cc_UC, loc_47AC
 MASK, SKIP,               // mov     r4, #0			<------ this should be a 1 to allow any seed to be OK
 MASK, MASK,               // mov     r6, [r0+]
 MASK, MASK                // rets
#endif
};

// KFAGK (Exhaust Valve Control Table)
//
// note: without the last 6 lines this is a 'common' snippet to find pretty much any map in a ME7 ;)
//
const unsigned char KFAGK_needle2[] = {
 0xE6, 0xFC, XXXX, XXXX,  // mov     r12, #(KFAGK_X_NUM - ROM_MAP_REGION_818000)   <--- * This is the Exhaust Flap Table
 0xE6, 0xFD, XXXX, XXXX,  // mov     r13, #XXXXh
 0xC2, 0xFE, XXXX, XXXX,  // movbz   r14, XXXX
 0xC2, 0xFF, XXXX, XXXX,  // movbz   r15, XXXX
 0xDA, XXXX, XXXX, XXXX,  // calls   XXXXh, Lookup_Table_Data ; References a lookup tableAE
 0xF1, 0xC8,              // movb    rl6, rl4
 0x3D, XXXX,              // jmpr    cc_NZ, loc_846218
 0xE6, 0xF4, XXXX, XXXX,  // mov     r4, #0FFBFh				// 0xE6, 0xF4, 0xBF, 0xFF,  // mov     r4, #0FFBFh
 0x64, 0xF4, XXXX, XXXX,  // and     word_9CF0, r4
 0x0D, XXXX,              // jmpr    cc_UC, loc_846224
 0x49, 0xC2,              // cmpb    rl6, #2
 0x3D, XXXX               // jmpr    cc_NZ, loc_846224
};
unsigned int KFAGK_needle2_len = sizeof(KFAGK_needle2);

const unsigned char KFAGK_mask2[] = {
 MASK, MASK, SKIP, SKIP,  // mov     r12, #(KFAGK_X_NUM - ROM_MAP_REGION_818000)   <--- * This is the Exhaust Flap Table
 MASK, MASK, SKIP, SKIP,  // mov     r13, #XXXXh
 MASK, MASK, SKIP, SKIP,  // movbz   r14, XXXX
 MASK, MASK, SKIP, SKIP,  // movbz   r15, XXXX
 MASK, SKIP, SKIP, SKIP,  // calls   XXXXh, Lookup_Table_Data ; References a lookup tableAE
 MASK, MASK,              // movb    rl6, rl4
 MASK, SKIP,              // jmpr    cc_NZ, loc_846218
 MASK, MASK, SKIP, SKIP,  // mov     r4, #0FFBFh				// MASK, MASK, MASK, MASK,  // mov     r4, #0FFBFh
 MASK, MASK, SKIP, SKIP,    // and     word_9CF0, r4
 MASK, SKIP,              // jmpr    cc_UC, loc_846224
 MASK, MASK,              // cmpb    rl6, #2
 MASK, XXXX               // jmpr    cc_NZ, loc_846224
};

// KPED (for % Throttle Pedal Position to Torque Request)
//
// Note: Using DPP1 for segment conversion since no segments specified.
//
const unsigned char KFPED_needle[] = {
 0x88, XXXX,              // mov     [-r0], r9
 0x88, XXXX,              // mov     [-r0], r6
 0xF3, 0xF8, XXXX, XXXX,  // movb    rl4, KFPED_Mode_ReversingOrNot_Bits
 0x49, 0x87,              // cmpb    rl4, #7
 0x3D, XXXX,              // jmpr    cc_NZ, XXXX
 0xE6, 0xFC, XXXX, XXXX,  // mov     r12, #KFPEDR                           <---* KFPEDR table [+14]
 0xF2, 0xFD, XXXX, XXXX,  // mov     r13, KFPED_X_AXIS_pedal_percentage
 0xF2, 0xFE, XXXX, XXXX,  // mov     r14, KFPED_Y_AXIS_rpm
 0xDA, 0x00, XXXX, XXXX,  // calls   0, Lookup_KFPED_ROM                    ; IROM tabl lookup function call
 0xF6, 0xF4, XXXX, XXXX,  // mov     KFPED_TorqueTarget_Percentage, r4
 0x0D, XXXX,              // jmpr    cc_UC, loc_XXXX
 0xE6, 0xFC, XXXX, XXXX,  // mov     r12, #KFPED                            <---* KFPEDR table [+36]
/* 0xF2, 0xFD, XXXX, XXXX,  // mov     r13, KFPED_X_AXIS_pedal_percentage
 0xF2, 0xFE, XXXX, XXXX,  // mov     r14, KFPED_Y_AXIS_rpm
 0xDA, 0x00, XXXX, XXXX,  // calls   0, Lookup_KFPED_ROM                    ; IROM table lookup function call
 0xF6, 0xF4, XXXX, XXXX,  // mov     KFPED_TorqueTarget_Percentage, r4
 0xF2, 0xF4, XXXX, XXXX,  // mov     r4, KFPED_TorqueTarget_Percentage
 0x42, 0xF4, XXXX, XXXX,  // cmp     r4, KFPED_Var
 0xFD, XXXX,              // jmpr    cc_ULE, loc_XXXX
 0xF2, 0xF4, XXXX, XXXX,  // mov     r4, KFPED_TorqueTarget_Percentage
 0x0D, 0x02,              // jmpr    cc_UC, loc_XXXX
 0xF2, 0xF4, XXXX, XXXX   // mov     r4, KFPED_Var
*/
};
unsigned int KFPED_needle_len = sizeof(KFPED_needle);

const unsigned char KFPED_mask[] = {
 MASK, SKIP,              // mov     [-r0], r9
 MASK, SKIP,              // mov     [-r0], r6
 MASK, MASK, SKIP, SKIP,  // movb    rl4, KFPED_Mode_ReversingOrNot_Bits
 MASK, MASK,              // cmpb    rl4, #7
 MASK, SKIP,              // jmpr    cc_NZ, XXXX
 MASK, MASK, SKIP, SKIP,  // mov     r12, #KFPEDR                           <---* KFPEDR table [+14]
 MASK, MASK, SKIP, SKIP,  // mov     r13, KFPED_X_AXIS_pedal_percentage
 MASK, MASK, SKIP, SKIP,  // mov     r14, KFPED_Y_AXIS_rpm
 MASK, MASK, SKIP, SKIP,  // calls   0, Lookup_KFPED_ROM                    ; IROM tabl lookup function call
 MASK, MASK, SKIP, SKIP,  // mov     KFPED_TorqueTarget_Percentage, r4
 MASK, SKIP,              // jmpr    cc_UC, loc_XXXX
 MASK, MASK, SKIP, SKIP,  // mov     r12, #KFPED                            <---* KFPEDR table [+36]
/*
 MASK, MASK, SKIP, SKIP,  // mov     r13, KFPED_X_AXIS_pedal_percentage
 MASK, MASK, SKIP, SKIP,  // mov     r14, KFPED_Y_AXIS_rpm
 MASK, MASK, SKIP, SKIP,  // calls   0, Lookup_KFPED_ROM                    ; IROM table lookup function call
 MASK, MASK, SKIP, SKIP,  // mov     KFPED_TorqueTarget_Percentage, r4
 MASK, MASK, SKIP, SKIP,  // mov     r4, KFPED_TorqueTarget_Percentage
 MASK, MASK, SKIP, SKIP,  // cmp     r4, KFPED_Var
 MASK, SKIP,              // jmpr    cc_ULE, loc_XXXX
 MASK, MASK, SKIP, SKIP,  // mov     r4, KFPED_TorqueTarget_Percentage
 MASK, MASK,              // jmpr    cc_UC, loc_XXXX
 MASK, MASK, SKIP, SKIP   // mov     r4, KFPED_Var
*/
};


const unsigned char KFAGK_needle[] = {
 0xE6, 0xFC, XXXX, XXXX,  // mov     r12, #(KFAGK_X_NUM - ROM_MAP_REGION_818000)   <--- * This is the Exhaust Flap Table
 0xE6, 0xFD, XXXX, XXXX,  // mov     r13, #XXXXh
 0xC2, 0xFE, XXXX, XXXX,  // movbz   r14, XXXX
 0xC2, 0xFF, XXXX, XXXX,  // movbz   r15, XXXX
 0xDA, XXXX, XXXX, XXXX,  // calls   XXXXh, Lookup_Table_Data ; References a lookup tableAE
 0xF1, 0xC8,              // movb    rl6, rl4
 0x49, 0xC2,              // cmpb    rl6, #2
 0x3D, XXXX,              // jmpr    cc_NZ, loc_XXXX
 0xE6, 0xF4, 0x40, 0x00,  // mov     r4, #40h 
 0x74, 0xF4, XXXX, XXXX,  // or      XXXX, r4
 0x0D, XXXX               // jmpr    cc_UC, loc_XXXX
};
unsigned int KFAGK_needle_len = sizeof(KFAGK_needle);

const unsigned char KFAGK_mask[] = {
 MASK, MASK, SKIP, SKIP,  // mov     r12, #(KFAGK_X_NUM - ROM_MAP_REGION_818000)   <--- * This is the Exhaust Flap Table
 MASK, MASK, SKIP, SKIP,  // mov     r13, #XXXXh
 MASK, MASK, SKIP, SKIP,  // movbz   r14, XXXX
 MASK, MASK, SKIP, SKIP,  // movbz   r15, XXXX
 MASK, SKIP, SKIP, SKIP,  // calls   XXXXh, Lookup_Table_Data ; References a lookup tableAE
 MASK, MASK,              // movb    rl6, rl4
 MASK, MASK,              // cmpb    rl6, #2
 MASK, SKIP,              // jmpr    cc_NZ, loc_XXXX
 MASK, MASK, MASK, MASK,  // mov     r4, #40h 
 MASK, MASK, SKIP, SKIP,  // or      XXXX, r4
 MASK, SKIP               // jmpr    cc_UC, loc_XXXX
};

const unsigned char mapfinder_needle[] = {
 0xE6, 0xFC, XXXX, XXXX,  // mov     r12, #(MAP_X_NUM - ROM_MAP_REGION_818000)   <--- * This is the MAP XXX
 0xE6, 0xFD, XXXX, XXXX,  // mov     r13, #XXXXh
 0xC2, 0xFE, XXXX, XXXX,  // movbz   r14, XXXX
// 0xC2, 0xFF, XXXX, XXXX,  // movbz   r15, XXXX
 0xDA, XXXX, XXXX, XXXX,  // calls   XXXXh, Lookup_Table_Data ; References a lookup tableAE
};
unsigned int mapfinder_needle_len = sizeof(mapfinder_needle);

const unsigned char mapfinder_mask[] = {
 MASK, MASK, SKIP, SKIP,  // mov     r12, #(MAP_X_NUM - ROM_MAP_REGION_818000)   <--- * This is the MAP XXX
 MASK, MASK, SKIP, SKIP,  // mov     r13, #XXXXh
 MASK, MASK, SKIP, SKIP,  // movbz   r14, XXXX		// x-axis
// MASK, MASK, SKIP, SKIP,  // movbz   r15, XXXX		// y-axis
 MASK, SKIP, SKIP, SKIP,  // calls   XXXXh, Lookup_Table_Data ; References a lookup tableAE
};



const unsigned char mapfinder_xy3_needle[] = {
// 0xF2, 0xF4, XXXX, XXXX,  // mov     r4, word_XXXX  
// 0x88, 0x40,              // mov     [-r0], r4
// 0xF2, 0xF5, XXXX, XXXX,  // mov     r5, word_XXXX
 0x88, 0x50,              // mov     [-r0], r5
 0xE6, 0xFC, XXXX, XXXX,  // mov     r12, #XXXX 	[+14]
 0xE6, 0xFD, XXXX, XXXX,  // mov     r13, #XXXXh    [+18]
 0xE6, 0xFE, XXXX, XXXX,  // mov     r14, #XXXX		[+22]
 0xE6, 0xFF, XXXX, XXXX,  // mov     r15, #XXXXh	[+26]
 0xDA, XXXX, XXXX, XXXX,  // calls   XXXXh, Lookup_XXXX
 0x08, 0x04               // add     r0,  #4
};
unsigned int mapfinder_xy3_needle_len = sizeof(mapfinder_xy3_needle);

const unsigned char mapfinder_xy3_mask[] = {
// MASK, MASK, SKIP, SKIP,  // mov     r4, word_XXXX
// MASK, MASK,              // mov     [-r0], r4
// MASK, MASK, SKIP, SKIP,  // mov     r5, word_XXXX
 MASK, MASK,              // mov     [-r0], r5
 MASK, MASK, SKIP, SKIP,  // mov     r12, #XXXX     <---- *table   +14
 MASK, MASK, SKIP, SKIP,  // mov     r13, #XXXXh    <---- *segment +18
 MASK, MASK, SKIP, SKIP,  // mov     r14, #XXXX     <---- *table   +24
 MASK, MASK, SKIP, SKIP,  // mov     r15, #XXXXh    <---- *segment +28
 MASK, SKIP, SKIP, SKIP,  // calls   XXXXh, Lookup_XXXX
 MASK, MASK
};



const unsigned char mapfinder_xy2_needle[] = {
 0xE6, 0xF4, XXXX, XXXX,  // mov     r4, #XXXX_DATA_TBL 	; Table Data                  [+2]
 0xE6, 0xF5, XXXX, XXXX,  // mov     r5, #XXXXh				; Segment                     [+6]
 0x88, 0x50,              // mov     [-r0], r5
 0x88, 0x40,              // mov     [-r0], r4
 0xE6, 0xF4, XXXX, XXXX,  // mov     r4, #XXXX_Y_AXIS 		; Table Y Axis Data           [+14]
 0xE6, 0xF5, XXXX, XXXX,  // mov     r5, #XXXXh				; Segment                     [+18]
 0x88, 0x50,              // mov     [-r0], r5
 0x88, 0x40,              // mov     [-r0], r4
 0xD7, 0x40, XXXX, XXXX,  // extp    #XXXXh, #1				; Segment                     [+26]
 0xC2, 0xFC, XXXX, XXXX,  // movbz   r12, XXXX_X_NUM			; Table X Number of Items [+30]
 0xE6, 0xFD, XXXX, XXXX,  // mov     r13, #XXXX_X_AXIS 		; Table X Axis Data           [+34]
 0xE6, 0xFE, XXXX, XXXX,  // mov     r14, #XXXXh				; Segment                 [+38]
 0xD7, 0x40, XXXX, XXXX,  // extp    #XXXXh, #1				; Segment                     [+42]
 0xC2, 0xFF, XXXX, XXXX,  // movbz   r15, XXXX_Y_NUM			; Table Y Number of Items [+46]
 0xDA, XXXX, XXXX, XXXX,  // calls   XXXXh, XXXX_Lookup_func	; do the lookup
};
unsigned int mapfinder_xy2_needle_len = sizeof(mapfinder_xy2_needle);

const unsigned char mapfinder_xy2_mask[] = {
 MASK, MASK, SKIP, SKIP,  // mov     r4, #XXXX_DATA_TBL 		; Table Data
 MASK, MASK, SKIP, SKIP,  // mov     r5, #XXXXh				; Segment
 MASK, MASK,              // mov     [-r0], r5
 MASK, MASK,              // mov     [-r0], r4
 MASK, MASK, SKIP, SKIP,  // mov     r4, #XXXX_Y_AXIS 		; Table Y Axis Data
 MASK, MASK, SKIP, SKIP,  // mov     r5, #XXXXh				; Segment
 MASK, MASK,              // mov     [-r0], r5
 MASK, MASK,              // mov     [-r0], r4
 MASK, MASK, SKIP, SKIP,  // extp    #XXXXh, #1				; Segment
 MASK, MASK, SKIP, SKIP,  // movbz   r12, XXXX_X_NUM			; Table X Number of Items
 MASK, MASK, SKIP, SKIP,  // mov     r13, #XXXX_X_AXIS 		; Table X Axis Data
 MASK, MASK, SKIP, SKIP,  // mov     r14, #XXXXh				; Segment
 MASK, MASK, SKIP, SKIP,  // extp    #XXXXh, #1				; Segment
 MASK, MASK, SKIP, SKIP,  // movbz   r15, XXXX_Y_NUM			; Table Y Number of Items
 MASK, SKIP, SKIP, SKIP,  // calls   XXXXh, XXXX_Lookup_func	; do the lookup
};


const unsigned char mapfinder_xy_needle[] = {
 0xE6, 0xFC, XXXX, XXXX,  // mov     r12, #(MAP_X_NUM - ROM_MAP_REGION_818000)   <--- * This is the MAP XXX
 0xE6, 0xFD, XXXX, XXXX,  // mov     r13, #XXXXh
 0xF2, 0xFE, XXXX, XXXX,  // mov     r14, word_XXXX
 0xF2, 0xFF, XXXX, XXXX,  // mov     r14, word_XXXX
 0xDA, XXXX, XXXX, XXXX,  // calls   XXXXh, Lookup_Table_Data ; References a lookup tableAE
 0xF6, 0xF4, XXXX, XXXX,  // mov     word_XXXX, r4
};
unsigned int mapfinder_xy_needle_len = sizeof(mapfinder_xy_needle);


const unsigned char mapfinder_xy_mask[] = {
 MASK, MASK, SKIP, SKIP,  // mov     r12, #(MAP_X_NUM - ROM_MAP_REGION_818000)   <--- * This is the MAP XXX
 MASK, MASK, SKIP, SKIP,  // mov     r13, #XXXXh
 MASK, MASK, SKIP, SKIP,  // mov     r14, word_XXXX		// x-axis
 MASK, MASK, SKIP, SKIP,  // mov     r15, word_XXXX		// x-axis
 MASK, SKIP, SKIP, SKIP,  // calls   XXXXh, Lookup_Table_Data ; References a lookup tableAE
 MASK, MASK, SKIP, SKIP,  // mov     word_XXXX, r4
};




const unsigned char crc32_needle[] = {
 0x88, 0x90,              // mov     [-r0], r9
 0x88, 0x80,              // mov     [-r0], r8
 0x88, 0x70,              // mov     [-r0], r7
 0x88, 0x60,              // mov     [-r0], r6
 0x28, 0x02,              // sub     r0, #2
 0xE0, 0x0F,              // mov     r15, #0
 0xF2, 0xF6, XXXX, XXXX,  // mov     r6, word_XXXX
 0xF2, 0xF7, XXXX, XXXX,  // mov     r7, word_XXXX
 0xF0, 0x8C,              // mov     r8, r12
 0xF0, 0x9D,              // mov     r9, r13
 0x0D, 0x1E,              // jmpr    cc_UC, enterloop
 0xDC, 0x09,              // sumloop: exts    r9, #1
 0xA9, 0x88,              // movb    rl4, [r8]
 0xF0, 0x56,              // mov     r5, r6
 0x51, 0xA8,              // xorb    rl5, rl4
 0xB9, 0xA0,              // movb    [r0], rl5
 0xE6, 0xF4, XXXX, XXXX,  // mov     r4, #CAB0 ; lo word 
 0xE6, 0xF5, XXXX, XXXX,  // mov     r5, #0081 ; hi word   <---- CRC32 Polynomial Table Reference 
 0xA9, 0x60,              // movb    rl3, [r0]
 0xC0, 0x62,              // movbz   r2, rl3
 0x5C, 0x22,              // shl     r2, #2
 0xE0, 0x03,              // mov     r3, #0
 0x00, 0x42,              // add     r4, r2
 0x10, 0x53,              // addc    r5, r3
 0xDA, 0x00, XXXX, XXXX,   // calls   0, word_XXXX
 0xE6, 0x00, XXXX, XXXX,   // mov     DPP0, #XXXXh
 0xF0, 0x46,              // mov     r4, r6
 0xF0, 0x57,              // mov     r5, r7
 0xF1, 0x89,              // movb    rl4, rh4
 0xF1, 0x9A,              // movb    rh4, rl5
 0x7C, 0x85,              // shr     r5, #8
 0x50, 0x4A,              // xor     r4, r10
 0x50, 0x5B,              // xor     r5, r11
 0xF0, 0x64,              // mov     r6, r4
 0xF0, 0x75,              // mov     r7, r5
 0x08, 0x81,              // add     r8, #1
 0x18, 0x90,              // addc    r9, #0
 0xF0, 0x4F,              // enterloop: mov     r4, r15
 0x08, 0xF1,              // add     r15, #1
 0x40, 0x4E,              // cmp     r4, r14
 0x8D, 0xDE,              // jmpr    cc_C, sumloop
 0xF0, 0x46,              // mov     r4, r6
 0xF0, 0x57,              // mov     r5, r7
 0x08, 0x02,              // add     r0, #2
 0x98, 0x60,              // mov     r6, [r0+]
 0x98, 0x70,              // mov     r7, [r0+]
 0x98, 0x80,              // mov     r8, [r0+]
 0x98, 0x90,              // mov     r9, [r0+]
 0xDB, 0x00               // rets
};
unsigned int crc32_needle_len = sizeof(crc32_needle);

const unsigned char crc32_mask[] = {
 MASK, MASK,              // mov     [-r0], r9
 MASK, MASK,              // mov     [-r0], r8
 MASK, MASK,              // mov     [-r0], r7
 MASK, MASK,              // mov     [-r0], r6
 MASK, MASK,              // sub     r0, #2
 MASK, MASK,              // mov     r15, #0
 MASK, MASK, SKIP, SKIP,  // mov     r6, word_XXXX
 MASK, MASK, SKIP, SKIP,  // mov     r7, word_XXXX
 MASK, MASK,              // mov     r8, r12
 MASK, MASK,              // mov     r9, r13
 MASK, MASK,              // jmpr    cc_UC, enterloop
 MASK, MASK,              // sumloop: exts    r9, #1
 MASK, MASK,              // movb    rl4, [r8]
 MASK, MASK,              // mov     r5, r6
 MASK, MASK,              // xorb    rl5, rl4
 MASK, MASK,              // movb    [r0], rl5
 MASK, MASK, SKIP, SKIP,  // mov     r4, #CAB0 ; lo word [+38]
 MASK, MASK, SKIP, SKIP,  // mov     r5, #0081 ; hi word [+40]  <---- XorTable 
 MASK, MASK,              // movb    rl3, [r0]
 MASK, MASK,              // movbz   r2, rl3
 MASK, MASK,              // shl     r2, #2
 MASK, MASK,              // mov     r3, #0
 MASK, MASK,              // add     r4, r2
 MASK, MASK,              // addc    r5, r3
 MASK, MASK, SKIP, SKIP,  // calls   0, word_XXXX
 MASK, MASK, SKIP, SKIP,  // mov     DPP0, #XXXXh
 MASK, MASK,              // mov     r4, r6
 MASK, MASK,              // mov     r5, r7
 MASK, MASK,              // movb    rl4, rh4
 MASK, MASK,              // movb    rh4, rl5
 MASK, MASK,              // shr     r5, #8
 MASK, MASK,              // xor     r4, r10
 MASK, MASK,              // xor     r5, r11
 MASK, MASK,              // mov     r6, r4
 MASK, MASK,              // mov     r7, r5
 MASK, MASK,              // add     r8, #1
 MASK, MASK,              // addc    r9, #0
 MASK, MASK,              // enterloop: mov     r4, r15
 MASK, MASK,              // add     r15, #1
 MASK, MASK,              // cmp     r4, r14
 MASK, MASK,              // jmpr    cc_C, sumloop
 MASK, MASK,              // mov     r4, r6
 MASK, MASK,              // mov     r5, r7
 MASK, MASK,              // add     r0, #2
 MASK, MASK,              // mov     r6, [r0+]
 MASK, MASK,              // mov     r7, [r0+]
 MASK, MASK,              // mov     r8, [r0+]
 MASK, MASK,              // mov     r9, [r0+]
 MASK, MASK               // rets
};



extern const unsigned char needle_1[];
extern const unsigned char mask_1[];
extern const unsigned char needle_2[];
extern const unsigned char mask_2[];
extern const unsigned char needle_2b[];
extern const unsigned char mask_2b[];
extern const unsigned char needle_3[];
extern const unsigned char mask_3[];
extern const unsigned char needle_3b[];
extern const unsigned char mask_3b[];
extern const unsigned char needle_4[];
extern const unsigned char mask_4[];
extern const unsigned char needle_5[];
extern const unsigned char mask_5[];
extern const unsigned char needle_6[];
extern const unsigned char mask_6[];

#define SKIP    0x00
#define XXXX    0x00
#define YYYY    0x00
#define MASK    0xff

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


const unsigned char needle_1[] = { 
 0x7c, 0x1c,                 // shr   r12, #1
 0x46, 0xfc, XXXX, XXXX,     // cmp   r12, #XXXXh
 0x9d, 0x05,                 // jmpr  cc_NC, end_of_table
 0xf0, 0x4c,                 // mov   r4, r12
 0x5c, 0x14,                 // shl   r4, #1,
 0xd4, 0x54, XXXX, XXXX      // mov   r5, [r4 + XXXX]    <--- * this offset + 0x10000 [map base in rom] gives offset (instruction is in little endian format so conversion neccesary) in file to MLHFM Linearization 1kbyte (512 entries of 16-bits) table
};

const unsigned char mask_1[] = { 
 MASK, MASK,                 // shr   r12, #1
 MASK, MASK, SKIP, SKIP,     // cmp   r12, #200h
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
 0xEA, 0x80, 0x5C, 0x31,     // jmpa    cc_C, loc_YYYY
 0x0D, 0x1E,                 // jmpr    cc_UC, loc_YYYY [26]
 
 0xF2, 0xF4, XXXX, XXXX,     // mov     r4, var_Y
 0xF2, 0xF5, XXXX, XXXX,     // mov     r5, var_Y
 0xD7, 0x50, XXXX, XXXX,     // extp    #XXXXh, #2 ; '?'         <--- * this is the segment offset [+36]
 0x22, 0xF4, XXXX, XXXX,     // sub     r4, checksum_stored_hi   <--- * this is offset to the word (stored checksum) [+40]
 0x32, 0xF5, XXXX, XXXX,     // subc    r5, checksum_stored_lo   <--- * this is offset to the word (stored checksum) [+44]
 0x3D, 0x04,                 // jmpr    cc_NZ, loc_YYYY
 0xE1, 0x88,                 // movb    rl4, #8
 0x75, 0xF8, XXXX, XXXX,     // orb     var_X, rl4
 0x0D, 0x04                  // jmpr    cc_UC, loc_YYYY
};

const unsigned char mask_3b[] = {
 MASK, MASK, SKIP, SKIP,     // mov     var_X, r8
 MASK, MASK, SKIP, SKIP,     // mov     var_X, r9
 MASK, MASK,                 // mov     r4, r8
 MASK, MASK,                 // mov     r5, r9
 MASK, MASK, SKIP, SKIP,     // sub     r4, var_Y
 MASK, MASK, SKIP, SKIP,     // subc    r5, var_Y
 MASK, MASK, MASK, MASK,     // jmpa    cc_C, loc_YYYY
 MASK, MASK,                 // jmpr    cc_UC, loc_YYYY
 
 MASK, MASK, SKIP, SKIP,     // mov     r4, var_Y
 MASK, MASK, SKIP, SKIP,     // mov     r5, var_Y
 MASK, MASK, SKIP, SKIP,     // extp    #XXXXh, #2 ; '?'         <--- * this is the segment offset
 MASK, MASK, SKIP, SKIP,     // sub     r4, checksum_stored_hi   <--- * this is offset to the word (stored checksum)
 MASK, MASK, SKIP, SKIP,     // subc    r5, checksum_stored_lo   <--- * this is offset to the word (stored checksum)
 MASK, MASK,                 // jmpr    cc_NZ, loc_YYYY
 MASK, MASK,                 // movb    rl4, #8
 MASK, MASK, SKIP, SKIP,     // orb     var_X, rl4
 MASK, MASK                  // jmpr    cc_UC, loc_YYYY
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
 MASK, MASK, SKIP, SKIP,   // cmp     r4, #XX					<--- * this is the number of entries in the multipoint checksum oop
 MASK, MASK, SKIP, SKIP    // jmpa    cc_NC, loc_XXXX
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
};

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
};


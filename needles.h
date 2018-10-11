#ifndef NEEDLES
#define NEEDLES
/* Needles.h
 
   These are the snippets of code masked so relocation data is ignored
   as well as specific addressing data such as segmenting code. This makes
   identification of common code have a very high success rate across
   different rom's.
   
   This file is where the magic is :)
   
   The machine code is from a 16-bit Siemens C167_SR CPU in Little Endian.

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
#ifndef _NEEDLES_SUPPORT_H
#define _NEEDLES_SUPPORT_H

unsigned char me75x_needle[];
unsigned int  me75x_needle_len;
unsigned char me75x_mask[];

extern const unsigned char kwp2000_ecu_needle[] ;
extern const unsigned char kwp2000_ecu_mask[];
extern unsigned int kwp2000_ecu_needle_len;

extern const unsigned char meinfo_needle[]; 
extern const unsigned char meinfo_mask[]; 
extern unsigned int meinfo_needle_len;

extern unsigned char needle_TVKUP[];
extern unsigned char mask_TVKUP[];
extern unsigned int needle_TVKUP_len;

extern unsigned char needle_LRSTPZA[];
extern unsigned char mask_LRSTPZA[];
extern unsigned int needle_LRSTPZA_len;

extern const unsigned char needle_1[];
extern unsigned int needle_1_len;
extern const unsigned char mask_1[];

extern const unsigned char needle_mlhfm[];
extern unsigned int needle_mlhfm_len;
extern const unsigned char mask_mlhfm[];

extern const unsigned char needle_KFKHFM[];
extern unsigned int needle_KFKHFM_len;
extern const unsigned char mask_KFKHFM[];

extern const unsigned char needle_KRKTE[];
extern unsigned int needle_KRKTE_len;
extern const unsigned char mask_KRKTE[];

extern const unsigned char needle_LAMFA[];
extern unsigned int needle_LAMFA_len;
extern const unsigned char mask_LAMFA[];

extern unsigned char needle_CWKONFZ1[];
extern unsigned int needle_CWKONFZ1_len;
extern unsigned char mask_CWKONFZ1[];

extern unsigned char needle_CWKONABG[];
extern unsigned int needle_CWKONABG_len;
extern unsigned char mask_CWKONABG[];

extern unsigned char needle_NWS[];
extern unsigned int needle_NWS_len;
extern unsigned char mask_NWS[];

extern unsigned char needle_RKTI[];
extern unsigned int needle_RKTI_len;
extern unsigned char mask_RKTI[];

extern unsigned char needle_DFFTCNV[];
extern unsigned int needle_DFFTCNV_len;
extern unsigned char mask_DFFTCNV[];

extern unsigned char needle_ZWGRU[];
extern unsigned int needle_ZWGRU_len;
extern unsigned char mask_ZWGRU[];

extern unsigned char needle_BBSAWE[];
extern unsigned int needle_BBSAWE_len;
extern unsigned char mask_BBSAWE[];
	
extern unsigned char needle_SSTB[];
extern unsigned int needle_SSTB_len;
extern unsigned char mask_SSTB[];

extern unsigned char needle_SSTB2[];
extern unsigned int needle_SSTB2_len;
extern unsigned char mask_SSTB2[];

extern unsigned char needle_PROKON[];
extern unsigned int needle_PROKON_len;
extern unsigned char mask_PROKON[];

extern unsigned char needle_ESKONF[];
extern unsigned int needle_ESKONF_len;
extern unsigned char mask_ESKONF[];

extern const unsigned char needle_2[];
extern unsigned int needle_2_len;
extern const unsigned char mask_2[];

extern const unsigned char needle_2b[];
extern const unsigned char mask_2b[];
extern unsigned int needle_2b_len;

extern const unsigned char needle_3[];
extern const unsigned char mask_3[];
extern unsigned int needle_3_len;

extern const unsigned char needle_3b[];
extern const unsigned char mask_3b[];
extern unsigned int needle_3b_len;

extern const unsigned char needle_4[];
extern const unsigned char mask_4[];
extern unsigned int needle_4_len;

extern const unsigned char needle_4aa[];
extern const unsigned char mask_4aa[];
extern unsigned int needle_4aa_len;

extern const unsigned char needle_4b[];
extern const unsigned char mask_4b[];
extern unsigned int needle_4b_len;

extern const unsigned char needle_4c[];
extern const unsigned char mask_4c[];
extern unsigned int needle_4c_len;

extern const unsigned char needle_5[];
extern const unsigned char mask_5[];
extern unsigned int needle_5_len;

extern const unsigned char needle_6[];
extern const unsigned char mask_6[];
extern unsigned int needle_6_len;

extern const unsigned char KFPED_needle[];
extern const unsigned char KFPED_mask[];
extern unsigned int KFPED_needle_len;

extern const unsigned char KFAGK_needle[];
extern const unsigned char KFAGK_mask[];
extern unsigned int KFAGK_needle_len;

extern const unsigned char KFAGK_needle2[];
extern const unsigned char KFAGK_mask2[];
extern unsigned int KFAGK_needle2_len;

extern const unsigned char needle_dpp[];
extern const unsigned char mask_dpp[];
extern unsigned int needle_dpp_len;

const unsigned char mapfinder_needle[];
const unsigned char mapfinder_mask[];
extern unsigned int mapfinder_needle_len;

const unsigned char mapfinder_xy2_needle[];
const unsigned char mapfinder_xy2_mask[];
extern unsigned int mapfinder_xy2_needle_len;

const unsigned char mapfinder_xy3_needle[];
const unsigned char mapfinder_xy3_mask[];
extern unsigned int mapfinder_xy3_needle_len;

extern const unsigned char crc32_needle[];
extern const unsigned char crc32_mask[];
extern unsigned int crc32_needle_len;

#define SKIP    0x00
#define XXXX    0x00
#define YYYY    0x00
#define MASK    0xff

#endif

#endif
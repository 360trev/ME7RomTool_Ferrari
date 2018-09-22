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

#ifndef _UTILS_H
#define _UTILS_H

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

#define OPTION_CLR   0
#define OPTION_SET   1

#define MAX_DHFM_ENTRIES     1024
#define DEFAULT_DHFM_ENTRIES 512
#define MAP_FILE_OFFSET      0x10000
#define MAX_FILENAME         256
#define ROM_FILESIZE		 512*1024
#define MAX_ROM_REGIONS      32
#define SEGMENT_SIZE         0x4000
#define ROM_BASE_ADDRESS     0x800000
#define ROM_1MB_MASK         0xFFF00000

int search_rom(int mode, char *filename_rom, char *filename_hfm);

// shared library externs
typedef uint32_t (*calc_crc32)(uint32_t crc, const void *buf, size_t size);

typedef struct OPTS_ENTRY {
	char *option_name;
	int *option_var;
	int  option_value;
	char **filename;
	int  options_bump;
	char *desc;
} OPTS_ENTRY;

#define MANDATORY  0
#define OPTIONAL   1

typedef struct ImageHandle {
	union {
//		uint32_t	*u32;
		uint16_t	*u16;
		uint8_t		*u8;
		char		*s;
		void		*p;
	} d;
	size_t	len;
} ImageHandle;

/*
 * If htole16() is missing, let's assume that other *le*() functions
 * are also missing.
 *
 * OpenBSD - htole16 & 32 exist, but not le16toh etc
 */
#if defined(__OpenBSD__)
#define le16toh(x) htole16(x)
#define le32toh(x) htole32(x)
#endif

#if !defined(htole16)
#if __BYTE_ORDER == __LITTLE_ENDIAN
#define le32toh(x) (x)
#define le16toh(x) (x)
#define htole32(x) (x)
#define htole16(x) (x)
#else
#define le32toh(x) __bswap_32(x)
#define htole16(x) __bswap_16(x)
#define le32toh(x) __bswap_32(x)
#define htole16(x) __bswap_16(x)
#endif
#endif

// they're the same.
#define memcpy_to_le32 memcpy_from_le32

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define memcpy_from_le32(dest, src, len) \
	memcpy(dest, src, len)
#else
#define memcpy_from_le32(dest, src, len) { \
	int i; \
	for (i=0;i<len/4;i++) \
		((uint32_t *)dest)[i] = __bswap_32(((uint32_t *)src)[i]); \
}
#endif

int iload_file(struct ImageHandle *ih, const char *fname, int rw);
int ifree_file(struct ImageHandle *ih);
int save_file(const char *filename, const uint8_t *filebuf, size_t filelen);
uint8_t *load_file(const char *filename, size_t *filelen);

void show_cli_usage(int argc, char *argv[], OPTS_ENTRY table[], int entrysize);
int parse_cli_options(int argc, char *argv[],int i, OPTS_ENTRY table[], int entrysize);

unsigned char *search(ImageHandle *fh, unsigned char *pNeedle, unsigned char *pMask, int needle_len, int offset);
int search_image(const struct ImageHandle *ih, int start, const void *needle, const void *mask, int len, int align);
unsigned char *search_offset(unsigned char *buf, int buflen, unsigned char *pNeedle, unsigned char *pMask, int needle_len);
unsigned long get_addr_from_rom(unsigned char *rom_start_addr, unsigned dynamic_romsize, unsigned char *lo_addr, int lo_bits, unsigned char *hi_addr, int hi_bits, unsigned char *segment, int table_index);
unsigned long get_addr16_of_from_rom(unsigned char *rom_start_addr, unsigned dynamic_romsize, unsigned char *addr, unsigned char *segment, int table_index);

void dump_bin(char *dst, int val, int numbits);
char matchString(char * test, char * pWildText, char bCaseSensitive);
void c167x_diss(unsigned char *rom_start, uint8_t *buf, int len);

void hexdump(uint8_t *buf, int len, const char *end);
void hexdump_le_table(uint8_t *buf, int len, const char *end);

unsigned short get16(unsigned char *s);
unsigned long get32(unsigned char *s);
extern void show_hex_dump(const void *adrs, unsigned long nbytes, void *offset);


#endif

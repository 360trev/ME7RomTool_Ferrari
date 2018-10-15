/* me7romtool [ namFirmware analysis tool for Bosch ME7.3H4 Ferrari firmware's]
   By 360trev. Needle lookup function borrowed from nyet (Thanks man!) from
   the ME7sum tool development (see github).
   
   See Readme for notes about current status and features.
 
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "utils.h"
#include "needles.h"
#include "crc32.h"
#include "show_tables.h"
#include "table_spec.h"
#include "fixsums.h"
#include "find_dppx.h"
#include "mlhfm.h"
#include "krkte.h"
#include "eskonf.h"
#include "rominfo.h"

// this globals will be eliminated later (fixme)
char *rom_name=NULL;
char *hfm_name=NULL;
char *save_name=NULL;
unsigned long dynamic_ROM_FILESIZE=0;
int got_romfile=0;
int got_outfile=0;
int seedkey_patch=0;
int find_x_axis_maps=0;
int correct_checksums=0;
int force_write;
int valves=0;
int show_kfped=0;
int show_dppx=1;
int find_mlhfm=0;
int find_kfkhfm=0;
int full_debug=0;
int show_hex=0;
int show_adr=0;
int show_phy=1;
int show_help=0;
int show_multimap=0;
int show_rominfo=1;
int show_pukans=0;
int show_kfkhfm=0;
int show_diss=0;
int show_cwkonfz1=0;
int show_cwkonls=0;
int show_cwkonabg=0;
int show_kfzw=0;
int show_kfzw2=0;
int show_kfnw=0;
int show_kfnwwl=0;
int show_tvkup=0;
int show_lrstpza=0;
int show_lamfa=0;
int show_krkte=0;
int show_eskonf=0;
int show_nswo1=0;
int show_kfpedr=0;
int show_nswo2=0;
int show_kftvsa=0;
int show_kftvsa0=0;
int show_fkkvs=0;
int show_nmax=0;
int show_kfmsnwdk=0;
int show_kfwdkmsn=0;

unsigned long dpp0_value, dpp1_value, dpp2_value, dpp3_value;

OPTS_ENTRY opts_table[] = {
//	  option      field to set        value to set  argument   req'd or not
	{ "-romfile", &got_romfile,       OPTION_SET,   &rom_name,  MANDATORY, "Try to identify map in the firmware. You *must* specify a romfile!\n"                               },
	{ "-outfile", &got_outfile,       OPTION_SET,   &save_name, MANDATORY, "Optional filename for saving romfiles after they have been modified (overrides default name)\n"     },
	{ "-force",   &force_write,       OPTION_SET,   0,          OPTIONAL,  "If a checksummed file needs saving overwrite it anyway even if it already exists.\n\n"              },

	{ "-KFAGK",   &valves,            OPTION_SET,   0,          OPTIONAL,  "Try to identify and show KFAGK exhaust valve opening table in the firmware.\n"                      },
	{ "-KFPED",   &show_kfped,        OPTION_SET,   0,          OPTIONAL,  "Try to identify and show KFPED pedal torque request tables.\n"                                      },
	{ "-KFPEDR",  &show_kfpedr,       OPTION_SET,   0,          OPTIONAL,  "Try to identify and show KFPEDR pedal torque request tables during reversing.\n"                    },
	{ "-KFKHFM",  &show_kfkhfm,       OPTION_SET,   0,          OPTIONAL,  "Try to identify and show KFKHFM MAF Sensor correction table.\n"                                     },
	{ "-KFMSNWDK",&show_kfmsnwdk,     OPTION_SET,   0,          OPTIONAL,  "Try to identify and show KFMSNWDK Map for scaled mass flow over throttle valve.\n"                  },
	{ "-KFWDKMSN",&show_kfwdkmsn,     OPTION_SET,   0,          OPTIONAL,  "Try to identify and show KFWDKMSN Map for desired throttle plate angle.\n"                          },
	{ "-PUKANS",  &show_pukans,       OPTION_SET,   0,          OPTIONAL,  "Try to identify and show PUKANS Air Temperature correction table.\n"                                },
	{ "-TVKUP",   &show_tvkup,        OPTION_SET,   0,          OPTIONAL,  "Try to identify and show TVKUP Delay time for B_kupplv (clutch pedal).\n"                           },
	{ "-LRSTPZA", &show_lrstpza,      OPTION_SET,   0,          OPTIONAL,  "Try to identify and show LRSTPZA Period duration of the LRS forced amplitude.\n"                    },	
	{ "-LAMFA",   &show_lamfa,        OPTION_SET,   0,          OPTIONAL,  "Try to identify and show LAMFA Driver Requested Lambda table.\n"                                    },
	{ "-FKKVS",   &show_fkkvs,        OPTION_SET,   0,          OPTIONAL,  "Try to identify and show FKKVS Correction factor fueling system.\n"						            },
	{ "-KFNW",    &show_kfnw,         OPTION_SET,   0,          OPTIONAL,  "Try to identify and show KFNW Characteristic map for variable camshaft control table.\n"            },
	{ "-NMAX",    &show_nmax,         OPTION_SET,   0,          OPTIONAL,  "Try to identify and show NMAX rev limiter.\n"                                                       },
	{ "-KFNWWL",  &show_kfnwwl,       OPTION_SET,   0,          OPTIONAL,  "Try to identify and show KFNWWL Characteristic map for variable camshaft control during warm-up.\n" },
	{ "-KFZW",    &show_kfzw,         OPTION_SET,   0,          OPTIONAL,  "Try to identify and show KFZW Ignition timing table.\n"         								    },
	{ "-KFZW2",   &show_kfzw2,        OPTION_SET,   0,          OPTIONAL,  "Try to identify and show KFZW Ignition timing variant 2 table.\n"            						},
	{ "-NSWO1",   &show_nswo1,        OPTION_SET,   0,          OPTIONAL,  "Try to identify and show NSWO1 Speed threshold 1 switching speed for calculating time savings.\n"   },
	{ "-NSWO2",   &show_nswo2,        OPTION_SET,   0,          OPTIONAL,  "Try to identify and show NSWO2 Speed threshold 2 switching speed for calculating time savings.\n"   },
	{ "-CWKONABG",&show_cwkonabg,     OPTION_SET,   0,          OPTIONAL,  "Try to identify and show CWKONABG Codeword for configuration of exhaust emission treatment.\n"      },
	{ "-CWKONFZ1",&show_cwkonfz1,     OPTION_SET,   0,          OPTIONAL,  "Try to identify and show CWKONFZ1 Codeword for vehicle configuration.\n"                            },
	{ "-CWKONLS", &show_cwkonls,      OPTION_SET,   0,          OPTIONAL,  "Try to identify and show CWKONLS Codeword for configuration of lambda sensors.\n"                   },
	{ "-ESKONF",  &show_eskonf,       OPTION_SET,   0,          OPTIONAL,  "Try to identify and show ESKONF Output Stage Configurations for left and right banks.\n"            },
	{ "-KFTVSA",  &show_kftvsa,       OPTION_SET,   0,          OPTIONAL,  "Try to identify and show KFTVSA Delay time for fuel cutoff.\n"                                      },
	{ "-KFTVSA0", &show_kftvsa0,      OPTION_SET,   0,          OPTIONAL,  "Try to identify and show KFTVSA0 Delay time for fuel cutoff.\n"                                     },
	{ "-KRKTE",   &show_krkte,        OPTION_SET,   0,          OPTIONAL,  "Try to identify and show KRKTE Conv. of relative fuel mass rk into effective injection time te.\n\n"},
	
	{ "-rhfm",    &find_mlhfm,        HFM_READING,  &hfm_name,  MANDATORY, "Read and extract hfm from romfile, optional dump filename to override default write name.\n"        },
	{ "-whfm",    &find_mlhfm,        HFM_WRITING,  &hfm_name,  MANDATORY, "Write hfm into specified romfile. A Mandatory <hfm bin filename> must be specified.\n"              },
	{ "-ihfm",    &find_mlhfm,        HFM_IDENTIFY, &hfm_name,  OPTIONAL,  "Try to identify mlhfm table in specified romfile.\n"                                                },
	{ "-maps",    &show_multimap,     OPTION_SET,   0,          OPTIONAL,  "Try to identify map in the firmware (Experimental!).\n"                                             },
	{ "-seedkey", &seedkey_patch,     OPTION_SET,   0,          OPTIONAL,  "Try to identify seedkey function and patch login so any login password works.\n\n"                  },

	{ "-fixsums", &correct_checksums, OPTION_SET,   0,          OPTIONAL,  "Try to correct checksums, if corrected it saves appending '_corrected.bin'.\n\n"                    },

	{ "-noinfo",  &show_rominfo,      OPTION_CLR,   0,          OPTIONAL,  "Disable rom information report scanning (on as default).\n"                                         },
	{ "-hex",     &show_hex,          OPTION_SET,   0,          OPTIONAL,  "Also show non formatted raw hex values in map table output.\n"                                      },
	{ "-adr",     &show_adr,          OPTION_SET,   0,          OPTIONAL,  "Also show non formatted raw hex values in map table output.\n"                                      },
	{ "-dbg",     &full_debug,        OPTION_SET,   0,          OPTIONAL,  "Show -phy (on as default), -hex and -adr in map table output.\n"                                    },
	{ "-diss",    &show_diss,         OPTION_SET,   0,          OPTIONAL,  "Show C167 diassembly traces of discovered needles to aid in debugging (Experimental!).\n"           },
	{ "-nophy",   &show_phy,          OPTION_CLR,   0,          OPTIONAL,  "Override default behaviour and dont show formatted values in map table output.\n\n"                 },
	
	{ "?",        &show_help,         OPTION_SET,   0,          OPTIONAL,  "Show this help.\n\n"                                                                                },
};
	
int show_usage(char *argv[], int argc)
{
	printf("Usage: %s <options> ...\n\n",argv[0]);
	show_cli_usage(argc, argv, (OPTS_ENTRY *)&opts_table, sizeof(opts_table)/sizeof(OPTS_ENTRY));
	return 0;
}

int main(int argc, char *argv[])
{
    int ok;
    int i=0, result;
	printf("Ferrari 360 ME7.3H4 Rom Tool. *BETA TEST* Last Built: %s %s v1.6\n",__DATE__,__TIME__);
	printf("by 360trev.  Needle lookup function borrowed from nyet (Thanks man!) from\nthe ME7sum tool development (see github). \n\n");
	printf("..Now fixed and working on 64-bit hosts, Linux, Apple and Android devices ;)\n\n");
	
	/* parse and check which options provided by console */	
    for (i=0 ; i < argc; i++) 
	{
		result = parse_cli_options(argc, argv,i, (OPTS_ENTRY *)&opts_table, sizeof(opts_table)/sizeof(OPTS_ENTRY));
		if(result == 1) { exit(0); }
	}

	/* if no arguments are specified show usage */
	if(argc < 2 || show_help == 1) {
		show_usage(argv, argc);
		return 0;
	}

	/* only proceed if we have been passed a valid romfile name */
	if(rom_name == 0) {
		printf("**No rom filename specified, e.g. -romfile rom.bin\n\n");
		return 0;		
	}

	/* sanity check any options for the given operational find_mlhfm  */
	switch(find_mlhfm)
	{
		case HFM_READING:
		{
			if(hfm_name == NULL) 
			{	
				printf("You didnt specify a filename for HFM table so one will be automatically created!\n");
			} else {
				printf("HFM Output Filename: '%s'\n",hfm_name);
			}	
		} break;
		
		case HFM_WRITING:
		{
			if(hfm_name != NULL ) {
				printf("HFM Input Filename: '%s'\n",hfm_name);
			} else {
				printf("You didnt specify a <mandatory> filename for HFM table to load.\n");
				return 0;
			}
		} break;
		default:
		break;
	}
		
	/*
	 * now lets search rom 
	 */
    ok = search_rom(find_mlhfm, rom_name,hfm_name);
    if (ok) { printf("File could not be found. Please check and try again."); }

	return 0;
}

int search_rom(int find_mlhfm, char *filename_rom, char *filename_hfm)
{
	ImageHandle f;
	ImageHandle *fh = &f;
	int load_result;
	unsigned char *addr;
	unsigned char *rom_load_addr;
	
	/* load file from storage */
	load_result = iload_file(fh, filename_rom, 0);
	if(load_result == 0) 
	{
		printf("Succeded loading file.\n\n");
		rom_load_addr = (unsigned char *)(fh->d.p);
		
		/* quick sanity check on firmware rom size (Ferrari 360 images must be 512kbytes/1024kbytes) */
		if(fh->len == ROM_FILESIZE*1 || fh->len == ROM_FILESIZE*2 ) 
		{		
			dynamic_ROM_FILESIZE = fh->len;	// either 512kbytes or 1024kbytes set depending on actual file size...
			if(dynamic_ROM_FILESIZE == ROM_FILESIZE*1 ) {
				printf("Loaded ROM: Tool in 512Kb Mode\n");
			} else if(dynamic_ROM_FILESIZE == ROM_FILESIZE*2 ) {
				printf("Loaded ROM: Tool in 1Mb Mode\n");
			}
			printf("\n");
					
			// check for dppx registers
			check_dppx(fh, show_dppx);
			// check for rom info
			check_rominfo(fh, show_rominfo);
	
			// check for display of cwkonfz1 - configuration of vehicle
			check_cwkonfz(fh, show_cwkonfz1);
			// check for display of cwkonls  - conﬁguration lambda sensor
			check_cwkonls(fh, show_cwkonls);
			// check for display of cwkonabg - configuration of exhaust emissions treatment
			check_cwkonabg(fh, show_cwkonabg);

			// check for display of pukans table
			check_pukans(fh, show_pukans);
			// check for display of kfkhfm table
			check_kfkhfm(fh, show_kfkhfm);
			// check for display of lamfa table
			check_lamfa(fh, show_lamfa);
			// check for display of kfnw table - variable camshaft control
			check_kfnw(fh, show_kfnw,   1);
			check_kfnw(fh, show_kfnwwl, 0);

			// check for display of krkte
			check_krkte(fh, show_krkte);
			// check for display of krkte
			check_fkkvs(fh, show_fkkvs);
			// check for display of eskonf
			check_eskonf(fh, show_eskonf);

			// check for display of nswo
			check_nswo(fh, show_nswo1,  1);
			check_nswo(fh, show_nswo2,  2);

			check_nmax(fh, show_nmax);

			check_kfmsnwdk(fh, show_kfmsnwdk);
			check_kfwdkmsn(fh, show_kfwdkmsn);

			// check for dislay of kfzw
			check_kfzw(fh, show_kfzw,   1);
			check_kfzw(fh, show_kfzw2,  2);

			// check for display of tvkup
			check_tvkup(fh, show_tvkup);
			// check for display of lrstpza
			check_lrstpza(fh, show_lrstpza);

			// check for seedkey_patch
			check_seedkey(fh, seedkey_patch);
			
			// check for kfagk (exhaust valves maps)
			check_kfagk(fh, valves);
			
			// check for kfed (throttle)
			check_kfped(fh, show_kfped,  1);
			check_kfped(fh, show_kfpedr, 2);

			// check for kftvsa
			check_kftvsa(fh, show_kftvsa, 1);
			// check for kftvsa0
			check_kftvsa(fh, show_kftvsa0, 2);
			
			// check for multimaps
			check_multimap(fh, show_multimap);
			// mlhfm support
			check_mlhfm(fh, addr, filename_rom, filename_hfm, dynamic_ROM_FILESIZE, rom_load_addr);
			// do correction
			fix_checksums(fh, addr, filename_rom, dynamic_ROM_FILESIZE, rom_load_addr);
			
		} else {
			printf("File size isn't a supported firmware size. Only 512kbyte and 1Mb images supported. ");
		}
	} else {
		printf("\nFailed to load, result = %d\n", load_result);
	}
	/* free file if allocated */
	load_result = ifree_file(fh);
	printf("\n\n");
	return 0;
}

# ME7RomTool_Ferrari
   Bosch ME7.3H4 RomTool for Ferrari 360's 

   By 360trev. Needle lookup function borrowed from nyet (Thanks man!) from
   the ME7sum tool development (see github).
           
   This tool is for analyzing/upgrading/modifying Ferrari 360 Firmware dumps.
   However its been re-written in a very unique way as to be compatible with
   as many Bosch ME7.x firmware dumps as possible.
   
   This version illustrates how to identify code segments, variables and data 
   table areas directly within a firmware image that 'move around' due to 
   conditional compilation and absolute addressing modes of the resultant
   machine code between different firmware versions and builds. The code explicitly
   supports both 512kbyte and 1Mb rom images despite Ferrari roms only being
   512kbytes. I note it works with Alfa and Volvo roms for the checksumming
   in the tests I've done so far (again down to the unique way I identify the
   variables and tables.)...
   
   The basic approach is to identify the code block sequences (which are common) 
   and then extract from the machine code itself the offsets to either a variable,
   map or list area where they are stored. 
   
   This means that this approach works across ALL Ferrari 360 firmware dumps and
   many more besides. The approach can ofcourse be used to search for ALL map 
   tables. The advantage of doing this vs simple byte signatures is that you can 
   extract offsets and information directly out of the code. Also since we are
   masking out all of the absolute address information directly out of the machine
   code the searching for common routines works across different generations and
   even different versions of ME7.x! 
	
   This tool currently supports identification of the Main Checksum areas;
   1. Firstly we identify the code snippet which tells us how many regions are
   being processed by the original code.
   2. Then we identify the array of start and end addresses out of the code where 
   these checksum ranges are stored.
   3. Then we identify the code area where the original (stored) checksums are
   located within the rom.
   4. We then re-calc the final checksum which is just an accumulation of all 
   regions checksums plus a 1's complement version is stored as a sanity check.
   Note: If they do not match either 1) the firmware dump is bad or its been changed
   since production. Without working main checksum the ecu won't boot the firmware.
   Since we know exactly where to store the re-calculated checksum we can put this
   directly back into the rom and save it out (step 3 discovered its location).
   At this point I haven't stored the recalculated checksum (thats easy 1 liner and
   will be added later after all checksum work is completed).
   
   The tool also can walk through the Multipoint Checksum areas too;
   1. Again we search for the code which tells us how many multipoints exist in
   this firmware rom.
   2. At this stage we search for 2 different variants of the stored checksum
   rom routine (1 for VAG and 1 for others). Once we identify the location of the
   multipoint checksum array (16 bytes per entry) comprising of start address,
   end address, crc32 and 1's complement crc32. we can walk through the list
   and check them by re-calculating them checksums.
   
   Some notes: Since the rom addresses contain rom base addresses (and often 
   all though not always the case (volvo roms start from 0) not from 0 address I
   opted to use bitmasks to eliminate the high address so we could translate from
   physical addresses to ram offsets.
   
   Furthermore the tool can also identify (currently 2) different variants of
   the SecurityAccess seed calcuation routines and patch them so that they always
   return TRUE (1) which means any login password works to grant full access over
   OBD-II. (e.g access to eeprom and flash writing).
   
   And finally it can also be used to (Ferrari only at this time) identify the
   MLHFM table which comprises of (typically) 512 entries, each 2 bytes 
   (1024 bytes total) of linearization data for the type of Bosch Air Flow Meters
   fitted to the Ferrari 360.

   The current 'modes' supported are below (cut from usage of program)
  
   me7romtool <mode> <rom_filename> <extra options> ...

   -rhfm :  <hfm_dump_filename> : Read and extract hfm from romfile, optional dump filename to override default write name.
    e.g.     me7romtool.exe -rhfm 360cs.bin hfm_stradale.bin

   -whfm :  <hfm_load_filename> : Write hfm into specified romfile, mandatory load filename must be specified.
   e.g.      me7romtool.exe -whfm 360modena.bin hfm_stradale.bin

   -ihfm : Try to identify mlhfm table in specified romfile.
   e.g.      me7romtool.exe -ihfm 360modena.bin

   -skey : Try to patch the SecurityKeyAccess to always work with any password
   e.g.      me7romtool.exe -skey -ihfm 360modena.bin

   Using this program you can upgrade your Ferrari 360 Modena firmware (after dumping)
   to use Challenge Stradale MLHFM tables. You just need a dump of both rom images.
 
   NOTE: This version DOES NOT YET update checksums for the firmware even though it
   calculates main rom and pulls multipoints. This is a work in progress and all functionality
   will be working soon. The swapping of HFM air flow meter tables on Ferrari roms fully works
   and can save out a replaced rom, you just (for now) need to update the checksums seperately
   with an external tool (such as  the original ME7Sum tool I wrote, also on my github).
   
   So, if you use this tool you'll need to update them separately after
   using it before flashing the output file to your ecu's.
   
   Have fun!
   
   Built using CodeLite.
   See https://codelite.org/


# ME7RomTool_Ferrari
   Bosch ME7.3H4 RomTool for Ferrari 360's  ** BETA TESTING **

   By 360trev. Needle lookup function borrowed from nyet (Thanks man!) from
   the ME7sum tool development (see github).

   Summary:
   This tool is for analyzing/upgrading/modifying Ferrari 360 Firmware dumps.
   However its been re-written in a very unique way as to be compatible with
   as many Bosch ME7.x firmware dumps as possible.

   Chekcksumming Feature:  '-fixsums'
   This tool currently supports identification of the Main Checksum areas & Multipoint
   Checksums;
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
   The tool also will walk through the Multipoint Checksum areas too;
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

   Seedkey Patch Feature: '-seedkey'
   The tool can also identify (currently 2) different variants of the SecurityAccess 
   seed calcuation routines and patch them so that they always return TRUE (1) which 
   means any login password works to grant full access over OBD-II. (e.g access to 
   eeprom and flash writing).

   The dppx registers are values used by the cpu to determine physical addresses
   such as where main system ram is located. By correctly setting the DPPx registers
   it will help using IDA on the given rom image. Since they are different between
   different rom's this feature will help you to quickly determine what they should be.
   This feature is enabled as default.

   MLHFM Table Swapping Features: '-rhfm' - Read & save it
   This is currently Ferrari specific but it basically identifies the
   MLHFM table which comprises of (typically) 512 entries, each 2 bytes 
   (1024 bytes total) of linearization data for the type of Bosch Air Flow Meters
   fitted to the Ferrari 360.   
   This option saves out the table and tries to recognise what table it is. e.g. for 
   a Challenge Stradale sized Air Flow Meters or Modena.
   This is a great feature for those looking to upgrade to larger AFM's as it yields
   greater peak power. The larger CS/599/F430 AFM's can flow more air at higher RPM's.

   MLHFM Table Swapping Features: '-whfm' - Write & save rom
   This option allows you to load a previously saved MLHFM binary table and swap it
   in a new rom. It also tries to recognise what table it is. e.g. for 
   a Challenge Stradale sized Air Flow Meters or Modena. After you've dumped the
   AFM's from another rom use this to upgrade the rom. If you also add '-fixsums'
   it will also automatically correct the sums and the output file is ready for
   flashing. Nothing else to do!

   MLHFM Table Swapping Features: '-ihfm' - Identify only
   This option allows you to load a identify which MLHFM binary table exists in the
   specified romfile.

   Map Dump Feature: '-maps' - Dump (generic) map locations
   This is a powerful feature that's currently work in progress, its aim is to automatically 
   identify all the maps in a given rom image so you can easily dump, edit and swap them. 
   Watch this space. Big updates on this very soon.

   Exhaust Flap Control Table : '-KFAGK' - identify and dump its location
   I did one table so far (whoop!). It allows you to see how all tables in the future will be 
   formatted. This one shows rpm vs throttle position and what happens to exhaust valves.

   Throttle Pedal Torque Table : '-KFPED' - identify and dump its location
   Throttle pedal charateristics and KFPEDR reversing pedal torque tables.

   MAF Sensor Air Temperature correction table, '-KFKHFM' - identify and dump its location
   
   Pulsation correction dependent on intake air temperature: '-PUKANS' - identify and dump
   its location.
   
Usage: me7romtool.exe <options> ...

 -romfile  : Try to identify map in the firmware. You *must* specify a romfile!
 -outfile  : Optional filename for saving romfiles after they have been modified (overrides default name)
 -force    : If a checksummed file needs saving overwrite it anyway even if it already exists.

 -KFAGK    : Try to identify and show KFAGK exhaust valve opening table in the firmware.
 -KFPED    : Try to identify and show KFPED/KFPEDR pedal torque request tables.
 -KFKHFM   : Try to identify and show KFKHFM MAF Sensor correction table.
 -PUKANS   : Try to identify and show PUKANS Air Temperature correction table.

 -CWKONFZ1 : Try to identify and show CWKONFZ1 Codeword for vehicle configuration.

 -rhfm     : Read and extract hfm from romfile, optional dump filename to override default write name.
 -whfm     : Write hfm into specified romfile. A Mandatory <hfm bin filename> must be specified.
 -ihfm     : Try to identify mlhfm table in specified romfile.
 -maps     : Try to identify map in the firmware (Experimental!).
 -seedkey  : Try to identify seedkey function and patch login so any login password works.

 -fixsums  : Try to correct checksums, if corrected it saves appending '_corrected.bin'.

 -noinfo   : Disable rom information report scanning (on as default).
 -hex      : Also show non formatted raw hex values in map table output.
 -adr      : Also show non formatted raw hex values in map table output.
 -dbg      : Show -phy (on as default), -hex and -adr in map table output.
 -diss     : Show C167 diassembly traces of discovered needles to aid in debugging (Experimental!).
 -nophy    : Override default behaviour and dont show formatted values in map table output.

 ?         : Show this help.




Q. How does this work?
   
   This tool not only finds signatures it also extract segment information directly
   out of the machine code (no guessing or manual entry required). Also since variables 
   and data table areas directly within a firmware image that 'move around' due to 
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
   
   Have fun!
   
   Built using CodeLite. 
   See https://codelite.org/


# ME7RomTool_Ferrari
   Bosch ME7.3H4 RomTool for Ferrari 360's 

   By 360trev. Needle lookup function borrowed from nyet (Thanks man!) from
   the ME7sum tool development (see github).
           
   This tool is for analyzing/upgrading/modifying Ferrari 360 Firmware dumps.
   
   This version illustrates how to identify MAP table areas directly within a 
   firmware image that 'move around' due to conditional compilation between
   different firmware versions. The basic approach is to identify the code
   block sequences (which are common) and then extract from the machine
   code itself the offsets to the map area where they are stored. This means
   that this approach works across ALL Ferrari 360 firmware dumps. It can
   ofcourse be used to search for ALL map tables. The advantage of doing this
   vs simple byte signatures is that you can extract offsets and information
   directly out of the code. 
	
   This tool supports identification of the MLHFM table which comprises of 
   (typically) 512 entries, each 2 bytes (1024 bytes total) of linearization 
   data for the type of Bosch Air Flow Meters fitted to the Ferrari 360.

   The current 'modes' supported are below (cut from usage of program)
  
   me7romtool <mode> <rom_filename> <extra options> ...

   -rhfm :  <hfm_dump_filename> : Read and extract hfm from romfile, optional dump filename to override default write name.
    e.g.     me7romtool.exe -rhfm 360cs.bin hfm_stradale.bin

   -whfm :  <hfm_load_filename> : Write hfm into specified romfile, mandatory load filename must be specified.
   e.g.      me7romtool.exe -whfm 360modena.bin hfm_stradale.bin

   -ihfm : Try to identify mlhfm table in specified romfile.
   e.g.      me7romtool.exe -ihfm 360modena.bin
   
   Using this program you can upgrade your Ferrari 360 Modena firmware (after dumping)
   to use Challenge Stradale MLHFM tables. You just need a dump of both rom images.
 
   NOTE: This version DOES NOT YET update checksums for the firmware after updating
   the MLHFM table. If you use this tool you'll need to update them separately after
   using it before flashing the output file to your ecu's.
   
   Built using CodeLite.
   See https://codelite.org/


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

   It can ofcourse be adapted and used to search for all or any types of map tables. 
   The advantage of doing this vs simple byte signatures is that you can extract 
   offsets and information directly out of the code. This simple example doesn't use 
   any configration files as its meant to be learned from. Here I only identify a single 
   table entry, the MLHFM table which comprises of (typically) 512 entries, each 2
   bytes (1024 bytes total) of linearization data for the type of Air Flow 
   Meters fitted.

   NOTE: This version DOES NOT YET update checksums for the firmware after updating
   the MLHFM table. If you use this tool you'll need to update them separately after
   using it before flashing the output file to your ecu's.
   
   Built using CodeLite.
   See https://codelite.org/


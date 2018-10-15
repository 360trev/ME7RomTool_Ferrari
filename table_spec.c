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
#include "show_tables.h"
#include "table_spec.h"

/* Below are templates which define how to format the layout and describe
 * tables we know how to correctly display. Later this will be migrated
 * to external text files and later still automatically generated!
 * 
 * For instance a "typical" raw KFAGK table from a rom file looks like below;
 * 
 *  ROM:81852D 0A                        KFAGK_Y_NUM:        db 10                   ; number of rows down
 *	ROM:81852E 06                        KFAGK_X_NUM:        db 6                    ; number of colums across        
 *	ROM:81852F 14 19 3F 44 49 5D 62 7D+  KFAGK_TABL_Y_RANGE: db 14h, 19h, 3Fh, 44h, 49h, 5Dh 62h, 7Dh, 96h, 0E1h  ; RPM (Scaled by *40 or /0.025
 *	ROM:818539 00 1B 55 5C 6C 85         KFAGK_TABL_X_RANGE: db 0, 1Bh, 55h, 5Ch, 6Ch, 85h                        ; Percentage of Throttle (% val * 1.33333)
 *	ROM:81853F 00 00 00 00 00 00 00 00+  KFAGK_TABL_DATA:    db 0, 0, 0, 0, 0, 0     ;  800 rpm
 *	           00 00 00 00 00 00 00 00+                      db 0, 0, 0, 0, 0, 0     ; 1520 rpm
 *	           00 00 00 00 01 01 01 01+                      db 0, 0, 0, 0, 0, 0     ; 2520 rpm
 *	           00 00 01 02 02 02 00 00+                      db 0, 0, 1, 1, 1, 1     ; 2720 rpm
 *	           01 02 02 02 00 00 01 02+                      db 0, 0, 1, 2, 2, 2     ; 2920 rpm
 *             02 02 00 00 01 02 02 02+                      db 0, 0, 1, 2, 2, 2     ; 3720 rpm
 *	           00 00 01 02 02 02 00 00+                      db 0, 0, 1, 2, 2, 2     ; 4120 rpm
 *	           01 02 02 02                                   db 0, 0, 1, 2, 2, 2     ; 5000 rpm
 *	                                                         db 0, 0, 1, 2, 2, 2     ; 6000 rpm
 *	                                                         db 0, 0, 1, 2, 2, 2     ; 9000 rpm
 *
 * We therefore configure the table viewer with UBYTE's for the lengths of the fields...
 * and then setup some axis information to make the table more meaningful.
 */

TABLE_DEF KFAGK_table = {
	// basic table information
	.table_name = "KFAGK",  .table_desc = "Characteristic map for exhaust flap changeover.", 

	// table data byte widths
	.x_num_nwidth  = UBYTE, .y_num_nwidth  = UBYTE,		// number of x/y item fields width
    .x_axis_nwidth = UBYTE, .y_axis_nwidth = UBYTE,		// x/y axis widths
	.cell_nwidth   = UBYTE,								// cell width

	// x-axis
	.x_axis = {
		.field_name = "X-axis",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "0.025",  		    // conversion value for huamn readable
		.desc      = "Upm",  			// conversion description
		.fmt_PHY   = "%8.2f ",     		// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "0x%X ",			// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "  %#6x ",			// ADR: x axis data formatting for physical addresses
		.conv_name = "nmot_ub_q40",		// conversion name
	},
	
	// y-axis
	.y_axis = {
		.field_name = "Y-axis",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "1.333333", 		// conversion value
		.desc      = "%", 				// conversion description
		.fmt_PHY   = " %-5.0f ",     	// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "%-#8.4x ",		// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x ",		// ADR: x axis data formatting for physical addresses
		.conv_name = "rel_uw_q0p75",  	// conversion name
	},
	
	// cells
	.cell = {
		.field_name = "Cells",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "1.0", 			// conversion value
		.desc      = " ", 				// conversion description
		.fmt_PHY   = "%8.1f ", 			// PHY: cell data formatting for conversion to human readable
		.fmt_HEX   = "  %#6x ",			// HEX: cell data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x",         // ADR: cell data formatting for physical addresses
		.conv_name = "rel_uw_b200",		// conversion name
	}
};

TABLE_DEF KPED_table = {
	// basic table information
	.table_name = "KFPED", .table_desc = "Characteristics of Throttle Pedal.", 

	// table data byte widths
	.x_num_nwidth  = UWORD, .y_num_nwidth  = UWORD,		// number of x/y item fields width
    .x_axis_nwidth = UWORD, .y_axis_nwidth = UWORD,		// x/y axis widths
	.cell_nwidth   = UWORD,								// cell width

	// x-axis
	.x_axis = {
		.field_name = "X-axis",				// field name
		.conv       = "655.35",    			// conversion value for huamn readable
		.desc       = "% PED",  			// conversion description
		.fmt_PHY    = "%8.2f ",     		// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX    = "0x%X ",				// HEX: x axis data formatting for raw hex values
		.fmt_ADR    = "  %#6x ",			// ADR: x axis data formatting for physical addresses
		.conv_name  = "relPED_uw_b100", 	// conversion name
	},

	// y-axis
	.y_axis = {
		.field_name = "Y-axis",				// field name
		.conv        = "4.0",  				// conversion value
		.desc        = "U/min", 			// conversion description
		.fmt_PHY     = " %-5.0f ",     		// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX     = "%-#8.4x ",			// HEX: x axis data formatting for raw hex values
		.fmt_ADR     = "%-#9.5x ",			// ADR: x axis data formatting for physical addresses
		.conv_name   = "nmot_uw_q0p25",  	// conversion name
	},
	
	// cells
	.cell = {
		.field_name = "Cells",				// field name
		.conv       = "327.68", 			// conversion value
		.desc       = "%", 					// conversion description
		.fmt_PHY    = "%8.1f ", 			// PHY: cell data formatting for conversion to human readable
		.fmt_HEX    = "  %#6x ",			// HEX: cell data formatting for raw hex values
		.fmt_ADR    = "%-#9.5x",       		// ADR: cell data formatting for physical addresses
		.conv_name  = "rel_uw_b200",		// conversion name
	}

};
	
TABLE_DEF KPEDR_table = {
	// basic table information
	.table_name = "KFPEDR", .table_desc = "Characteristics of Throttle Pedal Reversing.", 

	// table data byte widths
	.x_num_nwidth  = UWORD, .y_num_nwidth  = UWORD,		// number of x/y item fields width
  
	.x_axis_nwidth = UWORD, 
	.y_axis_nwidth = UWORD,		// x/y axis widths
	.cell_nwidth   = UWORD,								// cell width

	// x-axis
	.x_axis = {
		.field_name = "X-axis",			// field name
		.nwidth    = UWORD,				// field type
		.conv      = "655.35",     		// conversion value for huamn readable
		.desc      = "% PED",  			// conversion description
		.fmt_PHY   = "%8.2f ",     		// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "0x%X ",			// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "  %#6x ",			// ADR: x axis data formatting for physical addresses
		.conv_name = "relPED_uw_b100", 	// conversion name
	},
	
	// y-axis
	.y_axis = {
		.field_name = "Y-axis",			// field name
		.nwidth    = UWORD,				// field type
		.conv      = "4.0",   			// conversion value
		.desc      = "U/min", 			// conversion description
		.fmt_PHY   = " %-5.0f ",     	// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "%-#8.4x ",		// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x ",		// ADR: x axis data formatting for physical addresses
		.conv_name = "nmot_uw_q0p25",  	// conversion name
	},
	
	// cells
	.cell = {
		.field_name = "Cells",			// field name
		.nwidth    = UWORD,				// field type
		.conv      = "327.68", 			// conversion value
		.desc      = "%", 				// conversion description
		.fmt_PHY   = "%8.1f ", 			// PHY: cell data formatting for conversion to human readable
		.fmt_HEX   = "  %#6x ",			// HEX: cell data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x",         // ADR: cell data formatting for physical addresses
		.conv_name = "rel_uw_b200",		// conversion name
	}
};


TABLE_DEF XXXX_table = {
	// basic table information
	.table_name = "XXX Type #1", .table_desc = "Not yet defined.", 

	// table data byte widths
	.x_num_nwidth  = UBYTE, .y_num_nwidth  = UBYTE,		// number of x/y item fields width
  
	.x_axis_nwidth = UBYTE, 
	.y_axis_nwidth = UWORD,		// x/y axis widths
	.cell_nwidth   = UWORD,								// cell width

	// x-axis
	.x_axis = {
		.field_name = "X-axis",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "1.0",     		// conversion value for huamn readable
		.desc      = " ",  				// conversion description
		.fmt_PHY   = "%8.2f ",     		// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "0x%X ",			// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "  %#6x ",			// ADR: x axis data formatting for physical addresses
		.conv_name = " ", 				// conversion name
	},
	
	// y-axis
	.y_axis = {
		.field_name = "Y-axis",			// field name
		.nwidth    = UWORD,				// field type
		.conv      = "1.0",   			// conversion value
		.desc      = " ", 			// conversion description
		.fmt_PHY   = " %-5.0f ",     	// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "%-#8.4x ",		// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x ",		// ADR: x axis data formatting for physical addresses
		.conv_name = " ",  	// conversion name
	},
	
	// cells
	.cell = {
		.field_name = "Cells",			// field name
		.nwidth    = UWORD,				// field type
		.conv      = "1.0", 			// conversion value
		.desc      = " ", 				// conversion description
		.fmt_PHY   = "%8.0f ", 			// PHY: cell data formatting for conversion to human readable
		.fmt_HEX   = "  %#6x ",			// HEX: cell data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x",         // ADR: cell data formatting for physical addresses
		.conv_name = " ",		// conversion name
	}
};



TABLE_DEF XXXXB_table = {
	// basic table information
	.table_name = "XXX Type #2", .table_desc = "Single X axis tables.", 

	// table data byte widths
	.x_num_nwidth  = UBYTE, .y_num_nwidth  = 0,		// number of x/y item fields width
  
	.x_axis_nwidth = UBYTE, 
	.y_axis_nwidth = 0,								// x/y axis widths
	.cell_nwidth   = UBYTE,							// cell width

	// x-axis
	.x_axis = {
		.field_name = "X-axis",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "1.0",     		// conversion value for huamn readable
		.desc      = " ",  				// conversion description
		.fmt_PHY   = "%8.2f ",     		// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "0x%X ",			// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "  %#6x ",			// ADR: x axis data formatting for physical addresses
		.conv_name = " ", 				// conversion name
	},
	
	// y-axis
	.y_axis = {
		.field_name = "Y-axis",			// field name
		.nwidth    = 0,				// field type
		.conv      = "1.0",   			// conversion value
		.desc      = " ", 			// conversion description
		.fmt_PHY   = " %-5.0f ",     	// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "%-#8.4x ",		// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x ",		// ADR: x axis data formatting for physical addresses
		.conv_name = " ",  	// conversion name
	},
	
	// cells
	.cell = {
		.field_name = "Cells",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "1.0", 			// conversion value
		.desc      = " ", 				// conversion description
		.fmt_PHY   = "%8.0f ", 			// PHY: cell data formatting for conversion to human readable
		.fmt_HEX   = "  %#6x ",			// HEX: cell data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x",         // ADR: cell data formatting for physical addresses
		.conv_name = " ",		// conversion name
	}
};




TABLE_DEF KFKHFM_table = {
	// basic table information
	.table_name = "KFKHFM", .table_desc = "MAF Sensor correction by Load and RPM (Correction map for HFM)", 

	// table data byte widths
	.x_num_nwidth  = UBYTE, .y_num_nwidth  = UBYTE,		// number of x/y item fields width
  
	.x_axis_nwidth = UBYTE, 
	.y_axis_nwidth = UBYTE,		// x/y axis widths
	.cell_nwidth   = UBYTE,								// cell width

	// x-axis
	.x_axis = {
		.field_name = "X-axis",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "0.025",     		// conversion value for huamn readable
		.desc      = "Upm",  			// conversion description
		.fmt_PHY   = "%8.2f ",     		// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "0x%X ",			// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "  %#6x ",			// ADR: x axis data formatting for physical addresses
		.conv_name = " ", 				// conversion name
	},
	
	// y-axis
	.y_axis = {
		.field_name = "Y-axis",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "1.333333",		// conversion value
		.desc      = "%", 				// conversion description
		.fmt_PHY   = " %-6.2f",     	// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "%-#8.4x ",		// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x",		// ADR: x axis data formatting for physical addresses
		.conv_name = " ",  	// conversion name	
	},
	
	// cells
	.cell = {
		.field_name = "Cells",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "1.0", 			// conversion value
		.desc      = " ", 				// conversion description
		.fmt_PHY   = "%8.0f ", 			// PHY: cell data formatting for conversion to human readable
		.fmt_HEX   = "  %#6x ",			// HEX: cell data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x",         // ADR: cell data formatting for physical addresses
		.conv_name = " ",				// conversion name
	}
};



TABLE_DEF PUKANS_table = {
	// basic table information
	.table_name = "PUKANS", .table_desc = "Pulsation correction dependent on intake air temperature.", 

	// table data byte widths
	.x_num_nwidth  = UBYTE, .y_num_nwidth  = 0,		// number of x/y item fields width
  
	.x_axis_nwidth = UBYTE, 
	.y_axis_nwidth = 0,								// x/y axis widths
	.cell_nwidth   = UBYTE,								// cell width

	// x-axis
	.x_axis = {
		.field_name = "X-axis",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "1.0", 	  		// conversion value for huamn readable
		.desc      = "Grad C",			// conversion description
		.fmt_PHY   = "%8.2f ",     		// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "0x%X ",			// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "  %#6x ",			// ADR: x axis data formatting for physical addresses
		.conv_name = " ", 				// conversion name
	},
	
	// y-axis
	.y_axis = {
		.field_name = "Y-axis",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "1.0",   			// conversion value
		.desc      = " ", 				// conversion description
		.fmt_PHY   = " %-5.0f ",     	// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "%-#8.4x ",		// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x ",		// ADR: x axis data formatting for physical addresses
		.conv_name = " ",  	// conversion name
	},
	
	// cells
	.cell = {
		.field_name = "Cells",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "0.0078125", 		// conversion value
		.otype     = 0x2a,				// conversion type (override default / to use *)
		.desc      = " ", 				// conversion description
		.fmt_PHY   = "%8.4f ", 			// PHY: cell data formatting for conversion to human readable
		.fmt_HEX   = "  %#6x ",			// HEX: cell data formatting for raw hex values
		.fmt_ADR   = "%#6x ",         // ADR: cell data formatting for physical addresses
		.conv_name = " ",		// conversion name
	}
};


TABLE_DEF LAMFA_table = {
	// basic table information
	.table_name = "LAMFA", .table_desc = "Driver Requested Lambda", 

	// table data byte widths
	.x_num_nwidth  = UBYTE, .y_num_nwidth  = UBYTE,		// number of x/y item fields width
  
	.x_axis_nwidth = UBYTE, 
	.y_axis_nwidth = UWORD,		// x/y axis widths
	.cell_nwidth   = UBYTE,								// cell width

	// x-axis
	.x_axis = {
		.field_name = "X-axis",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "0.025",    		// conversion value for huamn readable
		.desc      = "Upm",  			// conversion description
		.fmt_PHY   = "%8.2f ",     		// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "0x%X ",			// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "  %#6x ",			// ADR: x axis data formatting for physical addresses
		.conv_name = " ", 				// conversion name
	},
	
	// y-axis
	.y_axis = {
		.field_name = "Y-axis",			// field name
		.nwidth    = UWORD,				// field type
		.conv      = "327.68", 			// conversion value
		.desc      = "%", 				// conversion description
		.fmt_PHY   = " %-6.2f",       	// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "%-#8.4x ",		// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x ",		// ADR: x axis data formatting for physical addresses
		.conv_name = " ",  	// conversion name
	},
	
	// cells
	.cell = {
		.field_name = "Cells",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "0.0078125", 		// conversion value
		.otype     = 0x2a,				// conversion type (override default / to use *)
		.desc      = " ", 				// conversion description
		.fmt_PHY   = "%8.4f ", 			// PHY: cell data formatting for conversion to human readable
		.fmt_HEX   = "  %#6x ",			// HEX: cell data formatting for raw hex values
		.fmt_ADR   = "%#6x ",         // ADR: cell data formatting for physical addresses
		.conv_name = " ",		// conversion name
	}
};

TABLE_DEF KFNW_table = { //
	// basic table information
	.table_name = "KFNW",  .table_desc = "Characteristic map for variable camshaft control.", 

	// table data byte widths
	.x_num_nwidth  = UBYTE, .y_num_nwidth  = UBYTE,		// number of x/y item fields width
    .x_axis_nwidth = UBYTE, .y_axis_nwidth = UBYTE,		// x/y axis widths
	.cell_nwidth   = UBYTE,								// cell width

	// x-axis
	.x_axis = {
		.field_name = "X-axis",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "0.025",  		    // conversion value for huamn readable
		.desc      = "Upm",  			// conversion description
		.fmt_PHY   = "%8.2f ",     		// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "0x%X ",			// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "  %#6x ",			// ADR: x axis data formatting for physical addresses
		.conv_name = "nmot_ub_q40",		// conversion name
	},
	
	// y-axis
	.y_axis = {
		.field_name = "Y-axis",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "1.333333", 		// conversion value
		.desc      = "%", 				// conversion description
		.fmt_PHY   = " %-5.2f ",     	// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "%-#8.4x ",		// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x ",		// ADR: x axis data formatting for physical addresses
		.conv_name = "rel_uw_q0p75",  	// conversion name
	},
	
	// cells
	.cell = {
		.field_name = "Cells",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "1.0", 			// conversion value
		.desc      = " ", 				// conversion description
		.fmt_PHY   = "%8.1f ", 			// PHY: cell data formatting for conversion to human readable
		.fmt_HEX   = "  %#6x ",			// HEX: cell data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x",         // ADR: cell data formatting for physical addresses
		.conv_name = "rel_uw_b200",		// conversion name
	}
};

TABLE_DEF KFNWWL_table = { //
	// basic table information
	.table_name = "KFNWWL",  .table_desc = "Characteristic map for variable camshaft control during warmup.", 

	// table data byte widths
	.x_num_nwidth  = UBYTE, .y_num_nwidth  = UBYTE,		// number of x/y item fields width
    .x_axis_nwidth = UBYTE, .y_axis_nwidth = UBYTE,		// x/y axis widths
	.cell_nwidth   = UBYTE,								// cell width

	// x-axis
	.x_axis = {
		.field_name = "X-axis",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "0.025",  		    // conversion value for huamn readable
		.desc      = "Upm",  			// conversion description
		.fmt_PHY   = "%8.2f ",     		// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "0x%X ",			// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "  %#6x ",			// ADR: x axis data formatting for physical addresses
		.conv_name = "nmot_ub_q40",		// conversion name
	},
	
	// y-axis
	.y_axis = {
		.field_name = "Y-axis",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "1.333333", 		// conversion value
		.desc      = "%", 				// conversion description
		.fmt_PHY   = " %-5.2f ",     	// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "%-#8.4x ",		// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x ",		// ADR: x axis data formatting for physical addresses
		.conv_name = "rel_uw_q0p75",  	// conversion name
	},
	
	// cells
	.cell = {
		.field_name = "Cells",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "1.0", 			// conversion value
		.desc      = " ", 				// conversion description
		.fmt_PHY   = "%8.1f ", 			// PHY: cell data formatting for conversion to human readable
		.fmt_HEX   = "  %#6x ",			// HEX: cell data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x",         // ADR: cell data formatting for physical addresses
		.conv_name = "rel_uw_b200",		// conversion name
	}
};


TABLE_DEF KFZW_table = { //
	// basic table information
	.table_name = "KFZW",  .table_desc = "Ignition Timing", 

	// table data byte widths
	.x_num_nwidth  = UBYTE, .y_num_nwidth  = UBYTE,		// number of x/y item fields width
    .x_axis_nwidth = UBYTE, .y_axis_nwidth = UBYTE,		// x/y axis widths
	.cell_nwidth   = UBYTE,								// cell width

	// x-axis
	.x_axis = {
		.field_name = "X-axis",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "0.025",  		    // conversion value for huamn readable
		.desc      = "Upm",  			// conversion description
		.fmt_PHY   = "%8.2f ",     		// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "0x%X ",			// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "  %#6x ",			// ADR: x axis data formatting for physical addresses
		.conv_name = "nmot_ub_q40",		// conversion name
	},
	
	// y-axis
	.y_axis = {
		.field_name = "Y-axis",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "1.333333", 		// conversion value
		.desc      = "%", 				// conversion description
		.fmt_PHY   = " %-5.2f ",     	// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "%-#8.4x ",		// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x ",		// ADR: x axis data formatting for physical addresses
		.conv_name = "rel_uw_q0p75",  	// conversion name
	},
	
	// cells
	.cell = {
		.field_name = "Cells",			// field name
		.nwidth    = UBYTE,				// field type
		.otype     = '/',				// conversion type (override default / to use *)
		.conv      = "1.3333", 			// conversion value
		.desc      = "grad KW", 		// conversion description
		.fmt_PHY   = "%8.3f ", 			// PHY: cell data formatting for conversion to human readable
		.fmt_HEX   = "  %#6x ",			// HEX: cell data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x",         // ADR: cell data formatting for physical addresses
		.conv_name = "rel_uw_b200",		// conversion name
	}
};

TABLE_DEF KFZW2_table = { //
	// basic table information
	.table_name = "KFZW2",  .table_desc = "Ignition Timing Variant #2", 

	// table data byte widths
	.x_num_nwidth  = UBYTE, .y_num_nwidth  = UBYTE,		// number of x/y item fields width
    .x_axis_nwidth = UBYTE, .y_axis_nwidth = UBYTE,		// x/y axis widths
	.cell_nwidth   = UBYTE,								// cell width

	// x-axis
	.x_axis = {
		.field_name = "X-axis",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "0.025",  		    // conversion value for huamn readable
		.desc      = "Upm",  			// conversion description
		.fmt_PHY   = "%8.2f ",     		// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "0x%X ",			// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "  %#6x ",			// ADR: x axis data formatting for physical addresses
		.conv_name = "nmot_ub_q40",		// conversion name
	},
	
	// y-axis
	.y_axis = {
		.field_name = "Y-axis",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "1.333333", 		// conversion value
		.desc      = "%", 				// conversion description
		.fmt_PHY   = " %-5.2f ",     	// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "%-#8.4x ",		// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x ",		// ADR: x axis data formatting for physical addresses
		.conv_name = "rel_uw_q0p75",  	// conversion name
	},
	
	// cells
	.cell = {
		.field_name = "Cells",			// field name
		.nwidth    = UBYTE,				// field type
		.otype     = '/',				// conversion type (override default / to use *)
		.conv      = "1.3333", 			// conversion value
		.desc      = "grad KW", 		// conversion description
		.fmt_PHY   = "%8.3f ", 			// PHY: cell data formatting for conversion to human readable
		.fmt_HEX   = "  %#6x ",			// HEX: cell data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x",         // ADR: cell data formatting for physical addresses
		.conv_name = "rel_uw_b200",		// conversion name
	}
};

TABLE_DEF KFTVSA_table = { //
	// basic table information
	.table_name = "KFTVSA",  .table_desc = "Delay time for fuel cutoff", 

	// table data byte widths
	.x_num_nwidth  = UBYTE, .y_num_nwidth  = UBYTE,		// number of x/y item fields width
    .x_axis_nwidth = UBYTE, .y_axis_nwidth = UBYTE,		// x/y axis widths
	.cell_nwidth   = UBYTE,								// cell width

	// x-axis
	.x_axis = {
		.field_name = "X-axis (stm05saub)",			// field name
		.nwidth    = UBYTE,				// field type
		.otype     = 'd',	 			// conversion value
		.conv      = "1.333333", 	    // conversion value for huamn readable
		.conv2     = "48",				// additional field
		.desc      = "Grad C",  		// conversion description
		.fmt_PHY   = "%8.2f ",     		// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "0x%X ",			// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "  %#6x ",			// ADR: x axis data formatting for physical addresses
		.conv_name = "nmot_ub_q40",		// conversion name
	},
	
	// y-axis
	.y_axis = {
		.field_name = "Y-axis (snm08__ub)",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "0.025", 		    // conversion value for huamn readable
		.desc      = "Upm", 			// conversion description
		.fmt_PHY   = " %-6.0f",     	// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "%-#8.4x ",		// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x ",		// ADR: x axis data formatting for physical addresses
		.conv_name = "rel_uw_q0p75",  	// conversion name
	},
	
	// cells
	.cell = {
		.field_name = "Cells",			// field name
		.nwidth    = UBYTE,				// field type
		.otype     = '/',				// conversion type (override default / to use *)
		.conv      = "100", 			// conversion value
		.desc      = "secs", 			// conversion description
		.fmt_PHY   = " %7.2f ", 			// PHY: cell data formatting for conversion to human readable
		.fmt_HEX   = "  %#6x ",			// HEX: cell data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x",         // ADR: cell data formatting for physical addresses
		.conv_name = "rel_uw_b200",		// conversion name
	}
};

TABLE_DEF KFTVSA0_table = { //
	// basic table information
	.table_name = "KFTVSA0",  .table_desc = "Delay time for fuel cutoff", 

	// table data byte widths
	.x_num_nwidth  = UBYTE, .y_num_nwidth  = UBYTE,		// number of x/y item fields width
    .x_axis_nwidth = UBYTE, .y_axis_nwidth = UBYTE,		// x/y axis widths
	.cell_nwidth   = UBYTE,								// cell width

	// x-axis
	.x_axis = {
		.field_name = "X-axis (stm05saub)",			// field name
		.nwidth    = UBYTE,				// field type
		.otype     = 'd',	 			// conversion value
		.conv      = "1.333333", 	    // conversion value for huamn readable
		.conv2     = "48",				// additional field
		.desc      = "Grad C",  		// conversion description
		.fmt_PHY   = "%8.2f ",     		// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "0x%X ",			// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "  %#6x ",			// ADR: x axis data formatting for physical addresses
		.conv_name = "nmot_ub_q40",		// conversion name
	},
	
	// y-axis
	.y_axis = {
		.field_name = "Y-axis (snm08__ub)",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "0.025", 		    // conversion value for huamn readable
		.desc      = "Upm", 			// conversion description
		.fmt_PHY   = " %-6.0f",     	// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "%-#8.4x ",		// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x ",		// ADR: x axis data formatting for physical addresses
		.conv_name = "rel_uw_q0p75",  	// conversion name
	},
	
	// cells
	.cell = {
		.field_name = "Cells",			// field name
		.nwidth    = UBYTE,				// field type
		.otype     = '/',				// conversion type (override default / to use *)
		.conv      = "100", 			// conversion value
		.desc      = "secs", 			// conversion description
		.fmt_PHY   = " %7.2f ", 			// PHY: cell data formatting for conversion to human readable
		.fmt_HEX   = "  %#6x ",			// HEX: cell data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x",         // ADR: cell data formatting for physical addresses
		.conv_name = "rel_uw_b200",		// conversion name
	}
};


TABLE_DEF FKKVS_table = {
	// basic table information
	.table_name = "FKKVS", .table_desc = "Correction factor fueling system", 

	// table data byte widths
	.x_num_nwidth  = UBYTE, .y_num_nwidth  = UBYTE,		// number of x/y item fields width
  
	.x_axis_nwidth = UBYTE, 
	.y_axis_nwidth = UWORD,		// x/y axis widths
	.cell_nwidth   = UWORD,								// cell width

	// x-axis
	.x_axis = {
		.field_name = "X-axis",			// field name
		.nwidth    = UBYTE,				// field type
		.conv      = "0.025",     		// conversion value for huamn readable
		.desc      = "rpm",  				// conversion description
		.fmt_PHY   = "%8.1f ",     		// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "0x%X ",			// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "  %#6x ",			// ADR: x axis data formatting for physical addresses
		.conv_name = " ", 				// conversion name
	},
	
	// y-axis
	.y_axis = {
		.field_name = "Y-axis",			// field name
		.nwidth    = UWORD,				// field type
		.conv      = "312.5",  			// conversion value
		.desc      = "ms injection", 	// conversion description
		.fmt_PHY   = " %-5.1f ",     	// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "%-#8.4x ",		// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x ",		// ADR: x axis data formatting for physical addresses
		.conv_name = " ",  	// conversion name
	},
	
	// cells
	.cell = {
		.field_name = "Cells",			// field name
		.nwidth    = UWORD,				// field type
		.conv      = "32768.0", 			// conversion value
		.desc      = " ", 				// conversion description
		.fmt_PHY   = "%8.4f ", 			// PHY: cell data formatting for conversion to human readable
		.fmt_HEX   = "  %#6x ",			// HEX: cell data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x",         // ADR: cell data formatting for physical addresses
		.conv_name = " ",		// conversion name
	}
};


TABLE_DEF KFWDKMSN_table = {
	// basic table information
	.table_name = "KFWDKMSN", .table_desc = "Map for desired throttle plate angle", 

	// table data byte widths
	.x_num_nwidth  = UWORD, .y_num_nwidth  = UWORD,		// number of x/y item fields width
  
	.x_axis_nwidth = UWORD, 
	.y_axis_nwidth = UWORD,		// x/y axis widths
	.cell_nwidth   = UWORD,								// cell width

	// x-axis
	.x_axis = {
		.field_name = "X-axis",			// field name
		.nwidth    = UWORD,				// field type
		.conv      = "655.35",     		// conversion value for huamn readable
		.desc      = "%",	  			// conversion description
		.fmt_PHY   = "%8.2f ",     		// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "0x%X ",			// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "  %#6x ",			// ADR: x axis data formatting for physical addresses
		.conv_name = "relPED_uw_b100", 	// conversion name
	},
	
	// y-axis
	.y_axis = {
		.field_name = "Y-axis",			// field name
		.nwidth    = UWORD,				// field type
		.conv      = "4.0",   			// conversion value
		.desc      = "U/min", 			// conversion description
		.fmt_PHY   = " %-5.0f ",     	// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "%-#8.4x ",		// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x ",		// ADR: x axis data formatting for physical addresses
		.conv_name = "nmot_uw_q0p25",  	// conversion name
	},
	
	// cells
	.cell = {
		.field_name = "Cells",			// field name
		.nwidth    = UWORD,				// field type
		.conv      = "10.0", 			// conversion value
		.desc      = "kg/h", 			// conversion description
		.fmt_PHY   = "%8.1f ", 			// PHY: cell data formatting for conversion to human readable
		.fmt_HEX   = "  %#6x ",			// HEX: cell data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x",         // ADR: cell data formatting for physical addresses
		.conv_name = "rel_uw_b200",		// conversion name
	}
};

TABLE_DEF KFMSNWDK_table = {
	// basic table information
	.table_name = "KFMSNWDK", .table_desc = "Map for scaled mass flow over throttle valve", 

	// table data byte widths
	.x_num_nwidth  = UWORD, .y_num_nwidth  = UWORD,		// number of x/y item fields width
  
	.x_axis_nwidth = UWORD, 
	.y_axis_nwidth = UWORD,		// x/y axis widths
	.cell_nwidth   = UWORD,								// cell width

	// x-axis
	.x_axis = {
		.field_name = "X-axis",			// field name
		.nwidth    = UWORD,				// field type
		.conv      = "655.35",     		// conversion value for huamn readable
		.desc      = "%",	  			// conversion description
		.fmt_PHY   = "%8.2f ",     		// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "0x%X ",			// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "  %#6x ",			// ADR: x axis data formatting for physical addresses
		.conv_name = "relPED_uw_b100", 	// conversion name
	},
	
	// y-axis
	.y_axis = {
		.field_name = "Y-axis",			// field name
		.nwidth    = UWORD,				// field type
		.conv      = "4.0",   			// conversion value
		.desc      = "U/min", 			// conversion description
		.fmt_PHY   = " %-5.0f ",     	// PHY: x axis data formatting for conversion to human readable
		.fmt_HEX   = "%-#8.4x ",		// HEX: x axis data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x ",		// ADR: x axis data formatting for physical addresses
		.conv_name = "nmot_uw_q0p25",  	// conversion name
	},
	
	// cells
	.cell = {
		.field_name = "Cells",			// field name
		.nwidth    = UWORD,				// field type
		.conv      = "10.0", 			// conversion value
		.desc      = "kg/h", 			// conversion description
		.fmt_PHY   = "%8.1f ", 			// PHY: cell data formatting for conversion to human readable
		.fmt_HEX   = "  %#6x ",			// HEX: cell data formatting for raw hex values
		.fmt_ADR   = "%-#9.5x",         // ADR: cell data formatting for physical addresses
		.conv_name = "rel_uw_b200",		// conversion name
	}
};

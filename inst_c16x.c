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
#include "inst_c16x.h"

/* C16x disassembler 
 *
 * Work-In-Progress
 * 
 * Note: Not all opcodes and modes yet supported!!!
 */

char *lookup16_regname(unsigned short regnum)
{
	int i=0;
	unsigned short hex_w;
	char *name;

	while(1) {
	    name   = SFR_Entries[i].name;
	    hex_w  = SFR_Entries[i].hex_w;
	    if(hex_w == regnum) { break; } // found name
	    if(name == 0)       { break; } // found end of table	    
		i++;
	}
	return(name);
}

char *lookup_regname(unsigned char regnum)
{
	int i=0;
	unsigned char hex_b;
	char *name;
	while(1) {
	    name   = SFR8_Entries[i].name;
	    hex_b  = SFR8_Entries[i].hex_b;
	    if(hex_b == regnum) { break; } // found name
	    if(name == 0)       { break; } // found end of table	    
		i++;
	}
	return(name);
}

char *lookup_rh_regname(unsigned char regnum)
{
	int i=0,hex;
	char *name;

	while(1) {
	    name = RHNames[i].name;
	    hex  = RHNames[i].hex;
	    if(hex == regnum) { break; } // found name
	    if(name == 0)     { break; } // found end of table	    
		i++;
	}
	return(name);
}

void translate_arg_type(unsigned char *rom_start, uint8_t *buf, int rval, int rdata16, INST *p, unsigned char type, unsigned char pos)
{
	long int val;
	int rval_l,num_opcodes;
	char *name;
	unsigned char nibble;
	unsigned char data4;
	unsigned char _byte;
	unsigned char hi,lo;
	int len = p->len;

	// process different opcode modes
	switch(type)
	{
		case RW_L:				// 	1       //  Rn
					nibble = get_lo_nibble(rval);
					printf("r%d",nibble);
				break;

		case RW:				// 	1       //  Rn
				rval = *(buf+1);	
				if(pos == 2)
				{
					nibble = get_lo_nibble(rval);
					printf("r%d",nibble);
					
				} else if(pos == 1) {
					nibble = get_hi_nibble(rval);
					printf("r%d",nibble);
				}
				break;

		case RW_IND:			//	2 	//  [Rn]
				printf("[r%d]",rval);
				break;

		case RW_IND_POST_INC:	//	3 	//  [Rn]+
				printf("[r%d+]",rval);
				break;

		case RW_IND_POST_DEC:	//	4 	//  [Rn]-
				printf("[r%d-]",rval);
				break;

		case RW_IND_PRE_INC:	//	5 	// +[Rn]
				printf("[+r%d]",rval);
				break;

		case RW_IND_PRE_DEC:	//	6 	// -[Rn]
				printf("[-r%d]",rval);
				break;

		case RW_IND_1_DATA16_2:	//	7 	//  [Rn+Data16]
				if(rdata16 < 10) {
					printf("[r%d+%d]",rval,rdata16);
				} else {
					printf("[r%d+%02Xh]",rval,rdata16);
				}
				break;

		case RB:				// 8 

				rval = *(buf+1);	
				
				
				if(pos == 1)
				{
//					nibble = get_hi_nibble(rval);
//					nibble = 0;
//					printf("%.2Xh", nibble);					// (if not found in list) show as a number
					nibble = get_hi_nibble(rval);
//					printf(".cRB=[%x]",nibble);
					if((name = lookup_rh_regname(nibble)) == 0) { 	// try to identify number as a named register, DPP0, etc.
						printf("^%.2Xh", rval);					// (if not found in list) show as a number
					} else {
						printf("%s",name);						// if found as a name
					}

				} 
				else if(pos == 2) 
				{
					nibble = get_lo_nibble(rval);
//					printf(".cRB=[%x]",nibble);
					if((name = lookup_rh_regname(nibble)) == 0) { 	// try to identify number as a named register, DPP0, etc.
						printf("^%.2Xh", rval);					// (if not found in list) show as a number
					} else {
						printf("%s",name);						// if found as a name
					}
				}


//				rval  = *(buf+1);	
//				printf(".cRB=%x",rval);
//					if((name = lookup_regname(rval)) == 0) { 	// try to identify number as a named register, DPP0, etc.
//						printf("%.2Xh", rval);					// (if not found in list) show as a number
//					} else 
//						printf("%s",name);						// if found as a name
//					}
				break;

// cregb(RR)              SFR/GPR byte context
		case REGB2:				// 10  [ SFR/GPR byte context ] 
//				printf("REGB[%x]", rval);
				rval  = *(buf+1);	
					rval = get_lo_nibble(rval);
//					if((name = lookup_regname(rval)) == 0) { 	// try to identify number as a named register, DPP0, etc.
//						printf("%.2Xh", rval);					// (if not found in list) show as a number
//					} else {
						printf("r%d", rval);						// if found as a name
//					}
				break;

		case REGB3:				// 10  [ SFR/GPR byte context ] 
				rval  = *(buf+1);	
				if((name = lookup_regname(rval)) == 0) { 	// try to identify number as a named register, DPP0, etc.
					printf("r%X", rval);					// (if not found in list) show as a number
				} else {
					printf("%s", name);						// if found as a name
				}
				break;


		case REGB:				// 10  [ SFR/GPR byte context ] 
//				printf(".cREGW");
				rval = *(buf+1);	
				_byte = (unsigned char)rval;
				
				if(len == 4) {
//					printf(".cREGB[4]");
					rval  = *(buf+1);	
					if((name = lookup_regname(rval)) == 0) { 	// try to identify number as a named register, DPP0, etc.
						printf("r%X", rval);					// (if not found in list) show as a number
					} else {
						printf("%s", name);						// if found as a name
					}
					
				} else if(len == 2) {
//					printf(".cREGB[2]");
				
	//				nibble = get_hi_nibble(rval);	
					nibble = _byte;
	//				printf("<%-2.2X:",nibble);
	//				printBits(sizeof(nibble), &nibble);
	//				printf(">");

	//				nibble = (_byte & 0b00001111);
					nibble = (_byte & 0b11110000) >> 4;
	///				printf("<%-2.2X:",nibble);
	///				printBits(sizeof(nibble), &nibble);
	///				printf(">");

					// 1111 set ?
					if( (((_byte & 0b11110000) >> 4) == 1) ) 
					{
						// RL or RH?
						if( (((_byte & 0b10000000) >> 7) == 0) ) {
							nibble = (_byte & 0b01110000) >> 3;
							printf("***RL%d", nibble);
						} else {
							nibble = (_byte & 0b01110000) >> 3;
							printf("***RH%d", nibble);
						
						}					
						break;
					} else {
						rval = (_byte & 0b01110000) >> 3;
						rval_l = get_lo_nibble(_byte);
	//					printf("rXX");			// ... then show it as a R0-R15 	

							if((name = lookup_rh_regname(rval_l)) == 0) { 	// try to identify number as a named register, DPP0, etc.
								printf("%.2Xh", rval);					// (if not found in list) show as a number
							} else {
								printf("%s",name);						// if found as a name
							}
						
						break;
					}
				}
//				printf("_%d",nibble);			// ... then show it as a R0-R15 	
				
				break;

		case REGW:				// 10  [ SFR/GPR byte context ] 
//				printf(".cREGW");
				rval = *(buf+1);
				nibble = get_hi_nibble(rval);
				if(nibble == 0xf) { 				// if high nibble is 1111 (0xf) 			
					printf("r%d",get_lo_nibble(rval));			// ... then show it as a R0-R15 	
				} else {
					if((name = lookup_regname(rval)) == 0) { 	// try to identify number as a named register, DPP0, etc.
						printf("%.2Xh", rval);					// (if not found in list) show as a number
					} else {
						printf("%s",name);						// if found as a name
					}
				}
				break;

		case MEM:				// 11 
				if( *(buf+0) == 0xc2 || *(buf+0) == 0xf7)
				{ 
					printf("byte_%X",rdata16);	//+0x378000);
				} else {

					if((name = lookup16_regname(rdata16)) == 0) { 	// try to identify number as a named register, DPP0, etc.
						printf("word_%X",rdata16);	//+0x378000);
					} else {
						printf("%s",name);						// if found as a name
					}


				}
//				printf("MEM");
				break;

		case BITADR_W:			// 12
					rval = *(buf+3);	
					lo = get_lo_nibble(rval);
					rval = *(buf+2);	

					printf("word_%X.%d",0xFD00+(rval*2), lo );
					break;

		case BITADR_W2:			// 12
					rval = *(buf+1);	
					printf("word_%X.%d",0xFD00+(rval*2), get_hi_nibble(*(buf+3)));
					break;
					 
		case BITADR:			// 12
#if 0
				rval = *(buf+1);	
				nibble = get_hi_nibble(rval);
				if(nibble == 0xf) { 				// if high nibble is 1111 (0xf) 			
					printf("r%d",get_lo_nibble(rval));			// ... then show it as a R0-R15 	
				} else {
					if((name = lookup_regname(rval)) == 0) { 	// try to identify number as a named register, DPP0, etc.
						printf("%.2Xh", rval);					// (if not found in list) show as a number
					} else {
						printf("%s",name);						// if found as a name
					}
				}
				printf(".");
#endif
				//exper
				if(p->len == 2) 
				{
					rval = *(buf+0);	
					hi = get_hi_nibble(rval);

					rval = *(buf+1);	
					if((name = lookup_regname(rval)) == 0) { 	// try to identify number as a named register, DPP0, etc.
						printf("%.2Xh ??????????", rval);					// (if not found in list) show as a number
					} else {
						printf("%s",name);						// if found as a name
					}

//					printf("word_%X.%d [***************]",0xFD00+(rval*2), hi);
					
				} else {
									
					if(pos == 1) {
						rval = *(buf+1);	
						printf("word_%X.%d",0xFD00+(rval*2), get_hi_nibble(*(buf+3)));
					} else {
	//					printf("BITADR");					
						num_opcodes  = (*(buf+2))+2;
						printf("loc_%X",rom_start+(num_opcodes << 1));
					}
				
				}
				break;
				
		case BITOFF:			// 13
				printf("BITOFF");
				break;

		case IRANG2:			// 14
				printf("IRANG2");
				break;

		case DATA3:				// 15
				printf("#%d",rval);	//DATA3");
				break;

		case DATA4:				// 16
				data4 = ((rval & 0b11110000) >> 4);
				if(data4 < 10) {					
					printf("#%x",data4 );		//DATA4");
				} else {
					printf("#%02Xh",data4 );	//DATA4");
				}
				break;

		case DATA8:				// 17
				printf("DATA8");
				break;

		case DATA16: 			// 18
				if(rdata16 < 10) {
					printf("#%d",rdata16);					
				} else {
					printf("#%-4.4Xh",rdata16);
				}
//				printf("DATA16");
				break;

		case MASK8: 			// 19
				printf("MASK8");
				break;

		case ADR:				// 20
				rval = get16(buf+2);	
				printf("loc_%X",rval);
				break;

		case SEG:				// 21
				rval = *(buf+1);	
				printf("%Xh",rval);
//				printf("SEG");
				break;

		case REG: 				// 22
				printf("REG");
				break;

		case TRAP7: 			// 23
				printf("TRAP7");
				break;

		case CC: 				// 24
				val = strtol(p->fmt,&p->fmt+4,2);

				printf("cc_%s",CCNames[val]);
				break;

		case REL: 				// 25
		{
				int num_opcodes, skip_bytes;
				num_opcodes  = (*(buf+1))+1;
				skip_bytes   = num_opcodes << 1;		// jump offset is measured in words so x2
				printf("loc_%X",rom_start+skip_bytes);
		}				
		break;

		default:
				printf("????");
				break;
	}
}

void c167x_diss(unsigned char *rom_start, uint8_t *buf, int len)
{
 unsigned char chA, chB, chC, chD, rval, nibble, _byte, hi, lo, pos;
 int i=0,skip,ln,argc;
 int rdata16;
 int type, numbits, argnum, bitcount;
 INST *ci;
 INST *link;
 char s_bin[8*8];
 char s_tmp[8*8];

	unsigned short val;
//	len = len*10;
	
	i=0;
	while(1) 
	{
		if(len > 4) {		
			chA = *(buf+0); chB = *(buf+1); chC = *(buf+2); chD = *(buf+3);

			ci=&inst_set[chA];
			
			// get length of opcode from lookup table
			skip = ci->len;			

			printf("0x%-8.8X: (+%-3d) ",(int)rom_start+i, (int)i);

			// show hex dump of the opcode
			if(skip == 2) {			// opcodes can either be 2 bytes
				printf(" %02X %02X       \t\t", chA, chB);
			} else if(skip == 4) { 	// or 4 bytes long...
				printf(" %02X %02X %02X %02X \t\t", chA, chB, chC, chD);
			} 

			// show offsets and opcode name
			link = ci->link;
				
			if(link == 0)
			{
				printf(" %-8s", (char *)ci->name);
				//
				// lets process arguments for this opcode
				//
				if(ci->argcount > 0)		// shows args for at least 1 argument
				{
#if 0
					for(argc=0;argc < ci->argcount; argc++ )
					{
//						argnum  = find_argnum_by_offset(ci, argc, ci->argcount);
						type    = ci->args.list[argc].type;
						pos     = ci->args.list[argc].pos;
						numbits = ci->bits.ops[argc];
						printf("\n\t\t\t{ listpos+%d: bits=%2d , type=%2d, order=%2d }, ", argc, numbits, type, pos );

					}
#endif
					rdata16 = get16((buf+2));	
					rval    = *(buf+1);	
					hi      = get_hi_nibble(rval);
					lo      = get_lo_nibble(rval);

					pos  = ci->args.list[0].pos;
					type = ci->args.list[0].type;
					if(pos == 1) {
						rval    = get_hi_nibble(*(buf+1));
					} else if(pos == 2){
						rval    = get_lo_nibble(*(buf+1));
					}
					
					translate_arg_type(rom_start+i, buf, rval, rdata16, ci, type, pos);
					if(ci->argcount > 1)
					{
						printf(", ");

						pos  = ci->args.list[1].pos;
						type = ci->args.list[1].type;
						if(pos == 1) {
							rval    = *(buf+1);
						} else if(pos == 2){
							rval    = get_lo_nibble(*(buf+1));
						}
						translate_arg_type(rom_start+i, buf, rval, rdata16, ci, type, pos);							
					}
				}
				
			} else {
				switch(chA) {
						case 0x08:case 0x09:
						case 0x18:case 0x19:
						case 0x28:case 0x29:
						case 0x38:case 0x39:
						case 0x48:case 0x49:
						case 0x58:case 0x59:
						case 0x68:case 0x69:
						case 0x78:case 0x79:
						{
							char *name;
							char regname[16];
							unsigned char type;
							//determine which instruction we are dealing with... 1 of 3
							//AAAA0BBB
							//AAAA10BB
							//AAAA11BB
							_byte = *(buf+1);	
							nibble = (_byte & 0b00001000) >> 3;
							hi      = get_hi_nibble(_byte);
							lo      = get_lo_nibble(_byte);

							if(link[0].args.list[0].type == RB) {
								type = RB;	//printf("ARG1 is RB!!!");
								if((name = lookup_rh_regname(hi)) == 0) { 	// try to identify number as a named register, DPP0, etc.
									sprintf(regname,"%.2Xh", rval);					// (if not found in list) show as a number
								} else {
									sprintf(regname,"%s",name);						// if found as a name
								}
							}
							else if(link[0].args.list[0].type == RW) {
								type = RW;	//]printf("ARG1 is RW!!!");
								sprintf(regname,"r%d",hi);						// if found as a name
							}
							
							if(nibble == 0) 
							{
								printf(" %s%s, #%d",link[0].name, regname, lo);								
							} 
							else 
							{
								nibble = (_byte & 0b00000100) >> 4;
								if(nibble == 0) {
									printf(" %s%s, #%d ; AAAA10BB {INCOMPLETE}",link[1].name, regname, lo);								
								} else {
									printf(" %s%s, #%d ; AAAA11BB {INCOMPLETE}",link[2].name, regname, lo);								
								}
							}
							
						}
						break;

						case 0xd7:		// opcode: 'extX'
						{
							int opcode;
							_byte = *(buf+1);	
							opcode = (_byte & 0b11000000) >> 6;			// instruction offset : 1st 2 bits
							printf(" %s", link[opcode].name);
							switch(opcode) 
							{
								case 0b00:	// 00AA0000BBBBBBBB00000000 : exts
								case 0b10:  // 10AA0000BBBBBBBB00000000 : extsr
									printf("#%-2.2Xh", *(buf+2));				// segment :  8 bits
									printf(", ");
									nibble = (_byte & 0b00110000) >> 4;			// irang   :  2 bits
									printf("#%d", nibble+1);
									break;
								case 0b01:  // 01AA0000BBBBBBBBBBBBBBBB : extp
								case 0b11:  // 11AA0000BBBBBBBBBBBBBBBB : extpr
									printf("#%-4.4Xh", get16(buf+2));			// segment : 16 bits
									printf(", ");
									nibble = (_byte & 0b00110000) >> 4;			// irang   :  2 bits
									printf("#%d", nibble+1);
									break;
							}
						}
						break;
						default: 
							printf(" %-8s", (char *)ci->name);
							printf("Link for [%#2x]",chA);
							break;
				}

			}
			printf("\n");
			
			// support for line feed after specific opcodes, rets, calls, jmps, etc.
			if(ci->addLF > 0) {
				printf("\n");
			} 
			
			// to next opcode in buffer
			buf += skip;
			i   += skip;
			len -= skip;
		} else {
			break;
		}
	}
	printf("***\n");
}

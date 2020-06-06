// Provided by VLSI Solutions http://www.vlsi.fi/en/home.html
// as part of demo code for the VS23S010D
// For more information see:
// http://www.vsdsp-forum.com/phpbb/viewforum.php?f=14
// http://www.vlsi.fi/en/products/vs23s010.html
// https://webstore.vlsi.fi/epages/vlsi.sf/en_GB/?ObjectID=13893093

#ifndef NTSC_H
#define NTSC_H

// Crystal frequency in MHZ (float, observe accuracy)
#define XTAL_MHZ 3.579545
// Line length in microseconds (float, observe accuracy)
#define LINE_LENGTH_US 63.5555
// Frame length in lines (visible lines + nonvisible lines)
// Amount has to be odd for NTSC and RGB colors
#define TOTAL_LINES 263
// Number of lines used after the VSYNC but before visible area.
#define FRONT_PORCH_LINES 3
// Width, in PLL clocks, of each pixel
// Used 4 to 8 for 160x120 pics
#define PLLCLKS_PER_PIXEL 4
// Extra bytes can be added to end of picture lines to prevent pic-to-proto 
// border artifacts. 8 is a good value. 0 can be tried to test, if there is 
// no need for extra bytes.		
#define BEXTRA 8

//// Protolines ////

// Reserve memory for this number of different prototype lines
// (prototype lines are used for sync timing, porch and border area)
#define PROTOLINES 3
// if your real protoline lenght is longer than one slot, you must 
// use several slots per proto and there are total 16 slots
#define PROTOLINE_LENGTH_WORDS 512

// Protoline 0 starts always at address 0
#define PROTOLINE_BYTE_ADDRESS(n) (PROTOLINE_LENGTH_WORDS) *2 *(n)) // 512 * 2 * n = 1024*n
#define PROTOLINE_WORD_ADDRESS(n) (PROTOLINE_LENGTH_WORDS * (n)) // 512 * n = 512*n

// These are for proto lines and so format is VVVVUUUUYYYYYYYY 
// Sync is always 0
#define SYNC_LEVEL  0x0000
// 285 mV to 75 ohm load
#define BLANK_LEVEL 0x0066
// 339 mV to 75 ohm load
#define BLACK_LEVEL 0x0066
// Color burst
#define BURST_LEVEL (0x0D00 + BLACK_LEVEL)
#define WHITE_LEVEL 0x00ff

//// NTSC video timing constants ////

// NTSC short sync duration is 2.35 us
#define SHORT_SYNC_US 2.542
// For the start of the line, the first 10 extra PLLCLK sync (0) cycles
// are subtracted.
#define SHORTSYNC ((u_int16)(SHORT_SYNC_US*XTAL_MHZ-10.0/8.0))
// For the middle of the line the whole duration of sync pulse is used.
#define SHORTSYNCM ((u_int16)(SHORT_SYNC_US*XTAL_MHZ))
// NTSC long sync duration is 27.3 us
#define LONG_SYNC_US 27.33275
#define LONGSYNC ((u_int16)(LONG_SYNC_US*XTAL_MHZ))
#define LONGSYNCM ((u_int16)(LONG_SYNC_US*XTAL_MHZ))
// Normal visible picture line sync length is 4.7 us
#define SYNC_US 4.7
#define SYNC ((u_int16)(SYNC_US*XTAL_MHZ-10.0/8.0))
// Color burst starts at 5.6 us
#define BURST_US 5.3
#define BURST ((u_int16)(BURST_US*XTAL_MHZ-10.0/8.0))
// Color burst duration is 2.25 us
#define BURST_DUR_US 2.67
#define BURSTDUR ((u_int16)(BURST_DUR_US*XTAL_MHZ))
// NTSC sync to blanking end time is 10.5 us
#define BLANK_END_US 9.155
#define BLANKEND ((u_int16)(BLANK_END_US*XTAL_MHZ-10.0/8.0))
// Front porch starts at the end of the line, at 62.5us 
#define FRPORCH_US 61.8105
#define FRPORCH ((u_int16)(FRPORCH_US*XTAL_MHZ-10.0/8.0))

//// Definitions for picture lines ////

// On which line the picture area begins, the Y direction.
#define STARTLINE (FRONT_PORCH_LINES + 40)
#define YPIXELS 200
// The last picture area line
#define ENDLINE STARTLINE + YPIXELS
// The first pixel of the picture area, the X direction.
#define STARTPIX (BLANKEND+16)
// The last pixel of the picture area. Set PIXELS to wanted value and suitable 
// ENDPIX value is calculated.
#define XPIXELS 320
#define ENDPIX ((u_int16)(STARTPIX+PLLCLKS_PER_PIXEL*XPIXELS/8))
		
// PLL frequency
#define PLL_MHZ (XTAL_MHZ * 8.0)
// 10 first pllclks, which are not in the counters are decremented here
#define PLLCLKS_PER_LINE ((u_int16)((LINE_LENGTH_US * PLL_MHZ)+0.5-10))
// 10 first pllclks, which are not in the counters are decremented here
#define COLORCLKS_PER_LINE ((u_int16)((LINE_LENGTH_US * XTAL_MHZ)+0.5-10.0/8.0))
#define COLORCLKS_LINE_HALF ((u_int16)((LINE_LENGTH_US * XTAL_MHZ)/2+0.5-10.0/8.0))

// Calculate picture lengths in pixels and bytes, coordinate areas for picture area
#define PICLENGTH (ENDPIX - STARTPIX)
#define PICX ((u_int16)(PICLENGTH * 8 / PLLCLKS_PER_PIXEL))
#define PICY (ENDLINE-STARTLINE)
#define PICBITS (UBITS+VBITS+YBITS)

#define PICLINE_LENGTH_BYTES (u_int16)(XPIXELS)
//#define PICLINE_LENGTH_BYTES ((u_int16)(PICX*PICBITS/8+0.5+1))

// Picture area memory start point
#define PICLINE_START ((INDEX_START_BYTES + TOTAL_LINES*3+1)+1)

// Picture area line start addresses
#define PICLINE_WORD_ADDRESS(n) (PICLINE_START/2+(PICLINE_LENGTH_BYTES/2+BEXTRA/2)*(n))
#define PICLINE_BYTE_ADDRESS(n) ((u_int32)(PICLINE_START+((u_int32)(PICLINE_LENGTH_BYTES)+BEXTRA)*(n)))

//// Index start /////

#define PROTO_AREA_WORDS (PROTOLINE_LENGTH_WORDS * PROTOLINES) 
#define INDEX_START_LONGWORDS ((PROTO_AREA_WORDS+1)/2)
#define INDEX_START_WORDS (INDEX_START_LONGWORDS * 2)
#define INDEX_START_BYTES (INDEX_START_WORDS * 2)              

//// Pattern generator microcode ////

// Bits 7:6
#define PICK_A (0<<6) // 00=a
#define PICK_B (1<<6) // 01=b
#define PICK_Y (2<<6) // 10=y
#define PICK_NOTHING (3<<6) // 11=-

// Bits 5:3
// Pick 1..8
#define PICK_BITS(a) (((a)-1)<<3) 

// Bits 2:0  
// Shift 0..6
#define SHIFT_BITS(a) (a)

// The microcode is given as a 32-bit parameter to the SpiRW-function, and must
// therefore be typecasted to u_int32. Otherwise, if using Arduino, the values 
// shifted beyond 16-bit range are lost. VS1005 and VS1010 would not require typecasting
// in this instance.
#define OP1 (u_int32)(PICK_B + PICK_BITS(2) + SHIFT_BITS(2))
#define OP2 (u_int32)(PICK_A + PICK_BITS(2) + SHIFT_BITS(2))
#define OP3 (u_int32)(PICK_Y + PICK_BITS(4) + SHIFT_BITS(4))
#define OP4 (u_int32)(PICK_NOTHING)

#define USHIFT 6
#define VSHIFT 4

#define UBITS 2
#define VBITS 2
#define YBITS 4

//// VS23 SPI Commands ////

// General VS23 commands
#define WRITE_STATUS 0x01 // Write Status Register
#define WRITE 0x02 // Write SRAM
#define READ 0x03 // Read SRAM
#define WRITE_MULTIIC 0xb8 // Write Multi-IC Access Control
#define READ_MULTIIC 0xb7 // Read Multi-IC Access Control
#define READ_ID 0x9f // Read Manufacturer and Device ID

// Bit definitions
#define VDCTRL1 0x2B
#define VDCTRL1_UVSKIP (1<<0)
#define VDCTRL1_DACDIV (1<<3)
#define VDCTRL1_PLL_ENABLE (1<<12)
#define VDCTRL1_SELECT_PLL_CLOCK (1<<13)
#define VDCTRL1_USE_UVTABLE (1<<14)
#define VDCTRL1_DIRECT_DAC (1<<15)

#define VDCTRL2 0x2D
#define VDCTRL2_LINECOUNT ( (TOTAL_LINES-1) << 0)
#define VDCTRL2_PROGRAM_LENGTH ((PLLCLKS_PER_PIXEL-1)<<10)
#define VDCTRL2_NTSC (0<<14)
#define VDCTRL2_PAL (1<<14)
#define VDCTRL2_ENABLE_VIDEO (1<<15)

#define BLOCKMVC1_PYF (1<<4)

// VS23 video commands
#define PROGRAM 0x30
#define PICSTART 0x28
#define PICEND 0x29
#define LINELEN 0x2a
#define LINELEN_VGP_OUTPUT (1<<15)
#define YUVBITS 0x2b
#define INDEXSTART 0x2c
#define LINECFG 0x2d
#define VTABLE 0x2e
#define UTABLE 0x2f
#define BLOCKMVC1 0x34
#define CURLINE 0x53
#define GPIOCTL 0x82

#endif

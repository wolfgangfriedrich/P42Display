// Provided by VLSI Solutions http://www.vlsi.fi/en/home.html
// as part of demo code for the VS23S010D
// For more information see:
// http://www.vsdsp-forum.com/phpbb/viewforum.php?f=14
// http://www.vlsi.fi/en/products/vs23s010.html
// https://webstore.vlsi.fi/epages/vlsi.sf/en_GB/?ObjectID=13893093

#ifndef PAL_H
#define PAL_H

#define u_int16 unsigned int
#define u_int32 unsigned long

/// Selects interlaced PAL picture. When commented progressive PAL is used.		
//#define INTERLACE
/// Selects byte wide pixel. When commented word wide pixel is used.
#define BYTEPIC	
		
/// Crystal frequency in MHZ (float, observe accuracy)
#define XTAL_MHZ 4.43361875
/// Line length in microseconds (float, observe accuracy)
#define LINE_LENGTH_US 64.0
/// Frame length in lines (visible lines + nonvisible lines)
/// Amount has to be odd for PAL and RGB colors
#ifdef INTERLACE
#define TOTAL_LINES 625
#else
#define TOTAL_LINES 313
#endif
		
/// Number of lines used after the VSYNC but before visible area.
#define FRONT_PORCH_LINES 22
		
/// Width, in PLL clocks, of each pixel
#define PLLCLKS_PER_PIXEL 5

/// Extra bytes can be added to end of picture lines to prevent pic-to-proto 
/// border artifacts. 8 is a good value. 0 can be tried to test, if there is 
/// no need for extra bytes.		
#define BEXTRA 8

/// Definitions for picture lines
/// On which line the picture area begins, the Y direction.
//#define STARTLINE ((u_int16)(TOTAL_LINES/4))
#define STARTLINE (FRONT_PORCH_LINES+17)
#define YPIXELS 240
/// The last picture area line
#define ENDLINE STARTLINE+YPIXELS
/// The first pixel of the picture area, the X direction.
#define STARTPIX (BLANKEND+13)
/// The last pixel of the picture area. Set PIXELS to wanted value and suitable 
/// ENDPIX value is calculated.
#define XPIXELS 300
#define ENDPIX ((u_int16)(STARTPIX+PLLCLKS_PER_PIXEL*XPIXELS/8))
//#define ENDPIX 195

/// Reserve memory for this number of different prototype lines
/// (prototype lines are used for sync timing, porch and border area)
#ifdef INTERLACE
#define PROTOLINES 8
#else
#define PROTOLINES 4
#endif

/// Define USE_SLOTTED_PROTOS if you want to use fixed length prototype "slots"
/// (allows patterned border area by defining different protolines
/// also for visible lines in addition to nonvisible lines)
//#define USE_SLOTTED_PROTOS

#ifdef USE_SLOTTED_PROTOS
/// Protoline lenght is one slot fixed in VS23S010 hardware: 512 bytes
/// (if your real protoline lenght is longer than one slot, you must 
/// use several slots per proto and there are total 16 slots)
#define PROTOLINE_LENGTH_WORDS 512
#else
/// Protoline lenght is the real lenght of protoline (optimal memory 
/// layout, but visible lines' prototype must always be proto 0)
#define PROTOLINE_LENGTH_WORDS ((u_int16)(LINE_LENGTH_US*XTAL_MHZ+0.5))
#endif
		
/// PLL frequency
#define PLL_MHZ (XTAL_MHZ * 8.0)
/// 10 first pllclks, which are not in the counters are decremented here
#define PLLCLKS_PER_LINE ((u_int16)((LINE_LENGTH_US * PLL_MHZ)+0.5-10))
/// 10 first pllclks, which are not in the counters are decremented here
#define COLORCLKS_PER_LINE ((u_int16)((LINE_LENGTH_US * XTAL_MHZ)+0.5-10.0/8.0))
#define COLORCLKS_LINE_HALF ((u_int16)((LINE_LENGTH_US * XTAL_MHZ)/2+0.5-10.0/8.0))
	
#define PROTO_AREA_WORDS (PROTOLINE_LENGTH_WORDS * PROTOLINES)
#define INDEX_START_LONGWORDS ((PROTO_AREA_WORDS+1)/2)
#define INDEX_START_WORDS (INDEX_START_LONGWORDS * 2)
#define INDEX_START_BYTES (INDEX_START_WORDS * 2)
		
/// Define PAL video timing constants
/// PAL short sync duration is 2.35 us
#define SHORT_SYNC_US 2.35
/// For the start of the line, the first 10 extra PLLCLK sync (0) cycles
/// are subtracted.
#define SHORTSYNC ((u_int16)(SHORT_SYNC_US*XTAL_MHZ-10.0/8.0))
/// For the middle of the line the whole duration of sync pulse is used.
#define SHORTSYNCM ((u_int16)(SHORT_SYNC_US*XTAL_MHZ))
/// PAL long sync duration is 27.3 us
#define LONG_SYNC_US 27.3
#define LONGSYNC ((u_int16)(LONG_SYNC_US*XTAL_MHZ))
#define LONGSYNCM ((u_int16)(LONG_SYNC_US*XTAL_MHZ))
/// Normal visible picture line sync length is 4.7 us
#define SYNC_US 4.7
#define SYNC ((u_int16)(SYNC_US*XTAL_MHZ-10.0/8.0))
/// Color burst starts at 5.6 us
#define BURST_US 5.6
#define BURST ((u_int16)(BURST_US*XTAL_MHZ-10.0/8.0))
/// Color burst duration is 2.25 us
#define BURST_DUR_US 2.25
#define BURSTDUR ((u_int16)(BURST_DUR_US*XTAL_MHZ))
/// PAL sync to blanking end time is 10.5 us
#define BLANK_END_US 10.5
#define BLANKEND ((u_int16)(BLANK_END_US*XTAL_MHZ-10.0/8.0))
/// Front porch starts at the end of the line, at 62.5us 
#define FRPORCH_US 62.5
#define FRPORCH ((u_int16)(FRPORCH_US*XTAL_MHZ-10.0/8.0))

/// Select U, V and Y bit widths for 16-bit or 8-bit wide pixels.
#ifndef BYTEPIC
#define UBITS 4
#define VBITS 4
#define YBITS 8
#else
#define UBITS 2
#define VBITS 2
#define YBITS 4
#endif

/// Protoline 0 starts always at address 0
#define PROTOLINE_BYTE_ADDRESS(n) (PROTOLINE_LENGTH_WORDS)*2*(n))
#define PROTOLINE_WORD_ADDRESS(n) (PROTOLINE_LENGTH_WORDS*(n))

/// Calculate picture lengths in pixels and bytes, coordinate areas for picture area
#define PICLENGTH (ENDPIX-STARTPIX)
#define PICX ((u_int16)(PICLENGTH*8/PLLCLKS_PER_PIXEL))
#define PICY (ENDLINE-STARTLINE)
#define PICBITS (UBITS+VBITS+YBITS)
#ifndef BYTEPIC
#define PICLINE_LENGTH_BYTES ((u_int16)(PICX*PICBITS/8+0.5))
#else
#define PICLINE_LENGTH_BYTES ((u_int16)(PICX*PICBITS/8+0.5+1))
#endif
/// Picture area memory start point
#define PICLINE_START (INDEX_START_BYTES+TOTAL_LINES*3+1)
/// Picture area line start addresses
#define PICLINE_WORD_ADDRESS(n) (PICLINE_START/2+(PICLINE_LENGTH_BYTES/2+BEXTRA/2)*(n))
#define PICLINE_BYTE_ADDRESS(n) ((u_int32)(PICLINE_START+((u_int32)(PICLINE_LENGTH_BYTES)+BEXTRA)*(n)))

/// 8-bit RGB to 8-bit YUV444 conversion
#define YRGB(r,g,b) ((76*r+150*g+29*b)>>8)
#define URGB(r,g,b) (((r<<7)-107*g-20*b)>>8)
#define VRGB(r,g,b) ((-43*r-84*g+(b<<7))>>8)

/// Pattern generator microcode
/// ---------------------------
/// Bits 7:6  a=00|b=01|y=10|-=11
/// Bits 5:3  n pick bits 1..8
/// bits 2:0  shift 0..6
#define PICK_A (0<<6)
#define PICK_B (1<<6)
#define PICK_Y (2<<6)
#define PICK_NOTHING (3<<6)
#define PICK_BITS(a)(((a)-1)<<3)
#define SHIFT_BITS(a)(a)

#ifndef BYTEPIC
/// 16 bits per pixel, U4 V4 Y8
/// PICK_B is U
#define OP1 (PICK_B + PICK_BITS(4) + SHIFT_BITS(4))
/// PICK_A is V
#define OP2 (PICK_A + PICK_BITS(4) + SHIFT_BITS(4))
#define OP3 (PICK_Y + PICK_BITS(8) + SHIFT_BITS(6))
#define OP4 (PICK_NOTHING + SHIFT_BITS(2))
/// U & V data shift values in a 16-bit pixel are defined.
#define USHIFT 12
#define VSHIFT 8
#else
/// 8 bits per pixel, U2 V2 Y4
#define OP1 (unsigned long)(PICK_A + PICK_BITS(2) + SHIFT_BITS(2))
#define OP2 (unsigned long)(PICK_B + PICK_BITS(2) + SHIFT_BITS(2))
#define OP3 (unsigned long)(PICK_Y + PICK_BITS(4) + SHIFT_BITS(4))
#define OP4 (unsigned long)(PICK_NOTHING)

/// U & V data shift values in a 8-bit pixel are defined.
#define USHIFT 6
#define VSHIFT 4
#endif

/// General VS23 commands
#define WRITE_STATUS 0x01
#define WRITE_MULTIIC 0xb8

/// Bit definitions
#define VDCTRL1 0x2B
#define VDCTRL1_UVSKIP (1<<0)
#define VDCTRL1_DACDIV (1<<3)
#define VDCTRL1_PLL_ENABLE (1<<12)
#define VDCTRL1_SELECT_PLL_CLOCK (1<<13)
#define VDCTRL1_USE_UVTABLE (1<<14)
#define VDCTRL1_DIRECT_DAC (1<<15)

#define VDCTRL2 0x2D
#define VDCTRL2_LINECOUNT (1<<0)
#define VDCTRL2_PIXEL_WIDTH (1<<10)
#define VDCTRL2_NTSC (0<<14)
#define VDCTRL2_PAL (1<<14)
#define VDCTRL2_ENABLE_VIDEO (1<<15)

#define BLOCKMVC1_PYF (1<<4)

/// VS23 video commands
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

/// Sync, blank, burst and white level definitions, here are several options
/// These are for proto lines and so format is VVVVUUUUYYYYYYYY 
/// Sync is always 0
#define SYNC_LEVEL  0x0000
/// Blank level corresponds to 43/143 part of 357 (maximum dc level in pic 255+102)
//#define PICMAX
#ifdef PICMAX
#define BLANK_LEVEL 0x006b // 43 IRE
#define BLACK_LEVEL 0x006b
/// For correct colors in this system BURST_LEVEL has to start with negative V value (4 bits) and 
/// then followed by a positive U value (4 bits).
#define BURST_LEVEL 0xee6b // burst level should be at least 38 IRE, is here only 32 IRE 
#define WHITE_LEVEL 0x00FF // 143 IRE is 357 which is max in picture area.
#endif
/// Max blank level possible when burst is the smallest in relation to blank
//#define BLANKMAX
#ifdef BLANKMAX
#define BLANK_LEVEL 0x005b // 43 IRE
#define BLACK_LEVEL 0x005b
#define BURST_LEVEL 0x975b // burst level 38 IRE  
//#define BURST_LEVEL 0xb55b // smaller burst level   
#define WHITE_LEVEL 0x00FF // 143 IRE should be 0x12e 
#endif
/// Max burst level and blank level, white level a bit low in proto lines. In pic area its ok,
/// because there is added 102 to value.
//#define PROTOMAX
#ifdef PROTOMAX
#define BLANK_LEVEL 0x0050 // 43 IRE
#define BLACK_LEVEL 0x0050
#define BURST_LEVEL 0x9750 // burst level 43 IRE  
#define WHITE_LEVEL 0x00FF // 143 IRE should be 0x10a 
#endif
/// Burst, blank and white ratios ok in proto line
//#define PROTOSMALL
#ifdef PROTOSMALL
#define BLANK_LEVEL 0x0043 // 43 IRE
#define BLACK_LEVEL 0x0043
#define BURST_LEVEL 0xa643 // burst level 43+-5 IRE  
#define WHITE_LEVEL 0x00DE // 143 IRE
#endif
/// The original values
//#define THE_ORIG
#ifdef THE_ORIG
#define BLANK_LEVEL 0x003a
#define BLACK_LEVEL 0x003a
#define BURST_LEVEL 0xee3a
#define WHITE_LEVEL 0x00FF
#endif

/// The test values
#define THE_TEST
#ifdef THE_TEST
#define BLANK_LEVEL 0x0058
#define BLACK_LEVEL 0x0058
#define BURST_LEVEL 0xee58

#define WHITE_LEVEL 0x00FF
#endif

#endif

// VS23S010D-L header file for 
// P42 Video Display Shield for Arduino
// 2018/Feb/11
// Wolfgang Friedrich
// https://hackaday.io/project/21097-arduino-video-display-shield
// https://github.com/wolfgangfriedrich/P42Display

#ifndef VS23S010D-L_h
#define VS23S010D-L_h

// *** Select Video Resolution here ***
#define NTSC320x200
//#define PAL300x240

// *** Uncomment this line if target system is an Arduino MEGA ***
//#define MEGA

// *** Uncomment this line to get extra debug information on the serial interface ***
//#define SERIAL_DEBUG

#ifdef NTSC320x200
	#include "ntsc320x200.h"
#endif
#ifdef PAL300x240
	#include "pal300x240.h"
#endif

const int slaveSelectPin	= 10;
const int MemSelectPin		=  9;
const int nHOLDPin			=  8;
const int nWPPin			=  7;

// Command definitions incl read / write designation


// SRAM Read Commands
const int ReadSRAM						= 0x03;
const int DORead						= 0x3B;		// Dual Output Read
const int DORead_DualAddress			= 0xBB;
const int QORead						= 0x6B;		// Quad Output Write
const int QORead_QuadAddress			= 0xEB;
// SRAM Write Commands	
const int WriteSRAM						= 0x02;
const int DIWrite						= 0xA2;		// Dual Input Read
const int DIWrite_DualAddress			= 0x22;
const int QIWrite						= 0x32;		// Quad Input Read
const int QIWrite_QuadAddress			= 0xB2;
// Miscellaneous Commands	
const int ReadStatus					= 0x05;
const int WriteStatus					= 0x01;
const int ReadDeviceID					= 0x9F;
const int ReadGPIOControl				= 0x84;
const int WriteGPIOControl				= 0x82;
const int ReadGPIOState					= 0x86;
const int ReadMultiICAccessControl		= 0xB7;
const int WriteMultiICAccessControl		= 0xB8;
// Video Display Controller Commands	
const int WritePictureStart				= 0x28;
const int WritePictureEnd				= 0x29;
const int WriteLineLength				= 0x2A;
const int WriteVideoDisplayControl1		= 0x2B;
const int WritePictureIndexStartAddress	= 0x2C;
const int WriteVideoDisplayControl2		= 0x2D;
const int WriteVTable					= 0x2E;
const int WriteUTable					= 0x2F;
const int WriteProgram					= 0x30;
const int ReadCurrentLinePLL			= 0x53;
const int WriteBlockMoveControl1		= 0x34;
const int WriteBlockMoveControl2		= 0x35;
const int StartBlockMove				= 0x36;


// GPIO Control Register
const byte PIO4Dir	= 0x10;
const byte PIO4High	= 0x01;
const byte PIO4Low	= 0xFE;
const byte PIO5Dir	= 0x20;
const byte PIO5High	= 0x02;
const byte PIO5Low	= 0xFD;
const byte PIO6Dir	= 0x40;
const byte PIO6High	= 0x04;
const byte PIO6Low	= 0xFC;
const byte PIO7Dir	= 0x80;
const byte PIO7High	= 0x08;
const byte PIO7Low	= 0xF7;

// WriteVideoDisplayControl1 Register
const word PLLEnable = 0x1000;
const word SelPLLCLK = 0x2000;
// WriteVideoDisplayControl1 Register
const word LineCount		= ( (TOTAL_LINES-1) << 0);
const word Program_Length	= ((PLLCLKS_PER_PIXEL-1)<<10);
const word NTSC				= 0x0000;
const word PAL				= 0x4000;
const word Enable_Video		= 0x8000;
// WriteBlockMoveControl1 Register
const word BMVC_PYF			= 0x0010;


class P42Display
{
	public:
		P42Display();
		byte SPIReadRegister (byte address, boolean debug);
		word SPIReadRegister16 (byte address, boolean debug);
		void SPIWriteRegister (byte address, byte value, boolean debug);
		void SPIWriteRegister16 (byte address, word value, boolean debug );
		void SPIWriteRegister32 (byte address, unsigned long value, boolean debug );
		void SPIWriteRegister40 (byte address, word source, word target, byte control, boolean debug );
		word Config();
//		byte SPITransferByte (unsigned long address, byte value, boolean debug);
//		word SPITransferWord (unsigned long address, byte value, boolean debug);
		byte SPIReadByte (unsigned long address);
		word SPIReadWord (unsigned long address);
		void SPIWriteByte (unsigned long address, byte value, boolean debug );
		void SPIWriteWord (unsigned long address, word value, boolean debug );
		// Graphics commands
		void FilledRectangle (u_int16 x1, u_int16 y1, u_int16 x2, u_int16 y2, u_int16 color);
		void ClearScreen ( byte colour );
		void SetRGBPixel (word x, word y, unsigned long colour);	// colour is 0x00rrggbb
		void SetYUVPixel (word x, word y, byte colour);   			// colour is single word
		void PrintChar (char Letter, word x, word y, byte colour);	// YUV colour
		void PrintString (char* Text, word x, word y, byte colour);	// YUV colour
		// block move 
	private:
		void _protoline(u_int16 line, u_int16 offset, u_int16 limit, u_int16 data);
		void _printdebug (byte address, unsigned long value);
	
};


#endif

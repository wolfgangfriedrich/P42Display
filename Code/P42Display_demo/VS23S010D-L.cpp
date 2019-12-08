// VS23S010D-L library file for 
// P42 Video Display Shield for Arduino
// Wolfgang Friedrich
// Last change: 2018/Mar/30
// https://hackaday.io/project/21097-ntscpal-video-display-shield
// https://github.com/wolfgangfriedrich/P42Display

#include "Arduino.h"
#include <SPI.h>

#ifdef NTSC320x200
	#include "ntsc320x200.h"
#endif
#ifdef PAL300x240
	#include "pal300x240.h"
	// To draw some patterns into protoline which will be visible in the frame of the image
	//	#define PAL_PROTOLINE_DEBUG 1
#endif

#include "VS23S010D-L.h"


// Config SPI for communication witht the VLSI VS23S010D-L chip
P42Display::P42Display()
{
	//Serial.begin(115200);
	//Serial.println("");
	//Serial.println(F("P42 Display Shield Test"));


}


// ----------------------------------------------
// Read 8bit register and return value
// if the debug parameter is set to TRUE, the register value is printed in format
// SPI read adress: 0x0 : h 0x4 : l 0x24
// SPI read result 0x424
byte P42Display::SPIReadRegister(byte address, boolean debug = false)
{
	byte result = 0;

	// take the SS pin low to select the chip:
	digitalWrite(slaveSelectPin,LOW);
	// send in the address and return value via SPI:
	SPI.transfer( address );
	result = SPI.transfer(0x00);
	// take the SS pin high to de-select the chip:
	digitalWrite(slaveSelectPin,HIGH); 

if (debug) {
	Serial.print("Read ");
	_printdebug(address,result);
}
	return (result);
}

// ----------------------------------------------
// Read 16bit register and return value
word P42Display::SPIReadRegister16(byte address, boolean debug = false)
{
	word result = 0;
    
	// take the SS pin low to select the chip:
	digitalWrite(slaveSelectPin,LOW);
	//  send in the address and return value via SPI:
	SPI.transfer( address );
	result = SPI.transfer16(0x0000);
	// take the SS pin high to de-select the chip:
	digitalWrite(slaveSelectPin,HIGH); 

if (debug) {
	Serial.print(F("Read "));
	_printdebug(address,result);
}
	return (result);
}

// ----------------------------------------------
// Write 8bit register
void P42Display::SPIWriteRegister(byte address, byte value, boolean debug = false)
{
	byte value_hi =0;

	// take the SS pin low to select the chip:
	digitalWrite(slaveSelectPin,LOW);
	// send in the address and value via SPI:
	SPI.transfer( address );
	SPI.transfer( value );
//	SPI.transfer( byte( value & 0xFF));
	// take the SS pin high to de-select the chip:
	digitalWrite(slaveSelectPin,HIGH); 

if (debug) {
	Serial.print(F("Write "));
	_printdebug(address,value);
}
	return; 
}

// ----------------------------------------------
// Write 16bit register
// hi data byte is sent first, lo data byte last.
void P42Display::SPIWriteRegister16(byte address, word value, boolean debug = false)
{
	byte value_hi =0;

	// take the SS pin low to select the chip:
	digitalWrite(slaveSelectPin,LOW);
	// send in the address and value via SPI:
	SPI.transfer( address );
	SPI.transfer( byte(value >> 8) );
	SPI.transfer( byte( value & 0xFF));
	// take the SS pin high to de-select the chip:
	digitalWrite(slaveSelectPin,HIGH); 

if (debug) {
	Serial.print(F("Write "));
	_printdebug(address,value);
}
	return; 
}

// ----------------------------------------------
// Write 32bit register
// hi data byte is sent first, lo data byte last.
void P42Display::SPIWriteRegister32(byte address, u_int32 value, boolean debug = false)
{
	byte value_hi =0;

	// take the SS pin low to select the chip:
	digitalWrite(slaveSelectPin,LOW);
	// send in the address and value via SPI:
	SPI.transfer( address );
	SPI.transfer( byte( ((u_int32)value >> 24) & 0xFF));
	SPI.transfer( byte( ((u_int32)value >> 16) & 0xFF));
	SPI.transfer( byte( ((u_int32)value >>  8) & 0xFF));
	SPI.transfer( byte(  (u_int32)value        & 0xFF));
	// take the SS pin high to de-select the chip:
	digitalWrite(slaveSelectPin,HIGH); 

if (debug) {
	Serial.print(F("Write "));
	_printdebug(address, (u_int32)value);
}
	return; 
}

// ----------------------------------------------
// Write 40bit(5 byte) register
// hi data byte is sent first, lo data byte last.
void P42Display::SPIWriteRegister40(byte address, word source, word target, byte control, boolean debug = false)
{
	byte value_hi =0;

	// take the SS pin low to select the chip:
	digitalWrite(slaveSelectPin,LOW);
	// send in the address and value via SPI:
	SPI.transfer( address );
	SPI.transfer( byte( (source >> 8) & 0xFF));
	SPI.transfer( byte( (source     ) & 0xFF));
	SPI.transfer( byte( (target >> 8) & 0xFF));
	SPI.transfer( byte(  target       & 0xFF));
	SPI.transfer(        control             );
	// take the SS pin high to de-select the chip:
	digitalWrite(slaveSelectPin,HIGH); 

if (debug) {
	Serial.print(F("Write "));
	_printdebug(address, source);
	_printdebug(address, target);
	_printdebug(address, control);
}
	return; 
}

// ----------------------------------------------
// Protoline Generator
// used from VLSI Arduino Examples
void P42Display::_protoline(u_int16 line, u_int16 offset, u_int16 limit, u_int16 data) {
	u_int16 i = 0;
	u_int32 w = (PROTOLINE_WORD_ADDRESS(line) + offset);

	if (offset == BLANKEND) i = BLANKEND;
	for (; i<=limit; i++) SPIWriteWord((u_int32)w++, data, false);
}

// ----------------------------------------------
// Debug output of SPI address and value
void P42Display::_printdebug(byte address, u_int32 value) {
	
	Serial.print(F("SPI address: 0x"));
	Serial.print(address,HEX);
	Serial.print(F(" : 0x"));
	Serial.println(value,HEX);
}

// ----------------------------------------------
// General Config for the Display Shield
// used from VLSI Arduino Examples
word P42Display::Config()
{
	word result = 0;
	u_int32 i,j = 0;

	// Read ID register to make sure the shield is connected.
	result = SPIReadRegister16( ReadDeviceID, false );
	if ((result & 0xff00) == 0x2B00) {

#ifdef SERIAL_DEBUG
		Serial.print(F("Found P42 Display module ID: 0x"));
		Serial.println(result,HEX);
#endif

#ifdef NTSC320x200 
	#ifdef SERIAL_DEBUG
		Serial.println(F("Resolution: NTSC 320x200 8bit"));
	#endif
#endif
#ifdef PAL300x240 
	#ifdef SERIAL_DEBUG
		Serial.println(F("Resolution: PAL 300x240 8bit"));
	#endif
#endif

		// disable multi V23s just in case
		SPIWriteRegister (WriteMultiICAccessControl, 0x0e, false);
		SPIReadRegister(ReadMultiICAccessControl, false);
		// SPI mem address auto increment
		SPIWriteRegister (WriteStatus, 0x40, false);
		// left limit of visible picture
		SPIWriteRegister16 (WritePictureStart, STARTPIX-1, false );
		SPIWriteRegister16 (WritePictureEnd, ENDPIX-1, false );
		// enable and selct PLL
		SPIWriteRegister16 (WriteVideoDisplayControl1, PLLEnable | SelPLLCLK, false );

		// Clear memory, could be more elegant with autoincrement
#ifdef SERIAL_DEBUG
		Serial.println(F("Clear memory"));
#endif
		for ( i=0; i < 65536; i++)
			SPIWriteWord (i, 0x0000, false);

		// set length of complete line in PLL clocks
		SPIWriteRegister16 (WriteLineLength, PLLCLKS_PER_LINE, false );
		// microcode
		SPIWriteRegister32 (WriteProgram, ((OP4 << 24) | (OP3 << 16) | (OP2 << 8) | (OP1)), false );
		// mem location of line indexes
		SPIWriteRegister16 (WritePictureIndexStartAddress, INDEX_START_LONGWORDS, false );

#ifdef NTSC320x200 
		// set all line indexes to point to protoline 0
	#ifdef SERIAL_DEBUG
		Serial.println(F("set line index to protoline 0"));
	#endif
		for ( i=0; i <= TOTAL_LINES-1; i++) {
			SPIWriteByte (INDEX_START_BYTES + i*3    , 0x00, false);
			SPIWriteByte (INDEX_START_BYTES + i*3 + 1, PROTOLINE_WORD_ADDRESS(0), false);
			SPIWriteByte (INDEX_START_BYTES + i*3 + 2, PROTOLINE_WORD_ADDRESS(0)>>8, false);
		}
		// Construct protoline 0
	#ifdef SERIAL_DEBUG
		Serial.println(F("Construct protoline 0"));
	#endif
		_protoline(0, 0, COLORCLKS_PER_LINE, BLANK_LEVEL);
		_protoline(0, BLANKEND, FRPORCH, BLACK_LEVEL);	// Set the color level to black
		_protoline(0, 0, SYNC, SYNC_LEVEL);				// Set HSYNC
		_protoline(0, BURST, BURSTDUR, BURST_LEVEL);	// Set color burst

		// Mitigate left edge artifact comes
		// http://www.vsdsp-forum.com/phpbb/viewtopic.php?f=14&t=2206#p11698
		SPIWriteWord(PROTOLINE_WORD_ADDRESS(0) + 0x38, 0x000e, false);
		
		// Construct protoline 1. This is a short+short VSYNC line
	#ifdef SERIAL_DEBUG
		Serial.println(F("Construct protoline 1"));
	#endif
		_protoline(1, 0, COLORCLKS_PER_LINE, BLANK_LEVEL);
		_protoline(1, 0, SHORTSYNC, SYNC_LEVEL);					// Short sync at the beginning of line
		_protoline(1, COLORCLKS_LINE_HALF, SHORTSYNCM, SYNC_LEVEL);	// Short sync at the middle of line
		
		// Construct protoline 2. This is a long+long VSYNC line
	#ifdef SERIAL_DEBUG
		Serial.println(F("Construct protoline 2"));
	#endif
		_protoline(2, 0, COLORCLKS_PER_LINE, BLANK_LEVEL);
		_protoline(2, 0, LONGSYNC, SYNC_LEVEL);						// Long sync at the beginning of line
		_protoline(2, COLORCLKS_LINE_HALF, LONGSYNCM, SYNC_LEVEL);	// Long sync at the middle of line
		// Now set first lines of frame to point to prototype lines
	#ifdef SERIAL_DEBUG
		Serial.println(F("1st lines point to protolines"));
	#endif
		for ( i=1; i <=9; i++) {
			if (i>=4 and i<=6) {
				SPIWriteByte (INDEX_START_BYTES + i*3, 0x00, false);
				SPIWriteByte (INDEX_START_BYTES + i*3 + 1, PROTOLINE_WORD_ADDRESS(2), false);
				SPIWriteByte (INDEX_START_BYTES + i*3 + 2, PROTOLINE_WORD_ADDRESS(2)>>8, false);
			}
			else {
				SPIWriteByte (INDEX_START_BYTES + i*3, 0x00, false);
				SPIWriteByte (INDEX_START_BYTES + i*3 + 1, PROTOLINE_WORD_ADDRESS(1), false);
				SPIWriteByte (INDEX_START_BYTES + i*3 + 2, PROTOLINE_WORD_ADDRESS(1)>>8, false);
			};
		}
		// Set pic line indexes to point to protoline 0 and their individual picture line.
	#ifdef SERIAL_DEBUG
		Serial.println(F("Pic line index to protoline 0"));
	#endif
		for (i=0; i<ENDLINE-STARTLINE; i++) { 
			SPIWriteByte (INDEX_START_BYTES + (i + STARTLINE)*3,     (PICLINE_BYTE_ADDRESS(i) << 7) & 0x80, false);
			SPIWriteByte (INDEX_START_BYTES + (i + STARTLINE)*3 + 1, (PICLINE_BYTE_ADDRESS(i) >> 1), false);
			SPIWriteByte (INDEX_START_BYTES + (i + STARTLINE)*3 + 2, (PICLINE_BYTE_ADDRESS(i) >> 9), false);
		}
		// Enable Video Display Controller, set video mode to NTSC, set program length and linecount.
		SPIWriteRegister16 (WriteVideoDisplayControl2, Enable_Video | NTSC | Program_Length | LineCount, false );
#endif

#ifdef PAL300x240
	word w = 0;

		// Enable PAL Y lowpass filter
		SPIWriteRegister40 (WriteBlockMoveControl1, 0x0000, 0x0000, BMVC_PYF, false );
//		SPIWriteRegister40 (WriteBlockMoveControl1, 0x0000, 0x0000, BMVC_PYF | BMVC_DACC, false );
		// set all line indexes to point to protoline 0
	#ifdef SERIAL_DEBUG
		Serial.println(F("set line index to protoline 0"));
	#endif
		for ( i=0; i < TOTAL_LINES; i++) {
			SPIWriteByte (INDEX_START_BYTES + i*3    , 0x00, false);
			SPIWriteByte (INDEX_START_BYTES + i*3 + 1, PROTOLINE_WORD_ADDRESS(0), false);
			SPIWriteByte (INDEX_START_BYTES + i*3 + 2, PROTOLINE_WORD_ADDRESS(0)>>8, false);
		}
		// Construct protoline 0
	#ifdef SERIAL_DEBUG
		Serial.println(F("Construct protoline 0"));
	#endif
		w = PROTOLINE_WORD_ADDRESS(0); // Could be w=0 because proto 0 always starts at address 0
		for (i=0; i<=COLORCLKS_PER_LINE; i++) {
			SPIWriteWord((u_int16)w++, BLANK_LEVEL, false);
		}
		// Set HSYNC
		w = PROTOLINE_WORD_ADDRESS(0);
		for (i=0; i<SYNC; i++) SPIWriteWord((u_int16)w++,SYNC_LEVEL, false);
		// Set color burst
		w = PROTOLINE_WORD_ADDRESS(0)+BURST;
		for (i=0; i<BURSTDUR; i++) SPIWriteWord((u_int16)w++,BURST_LEVEL, false);
		// Mitigate left edge artifact comes
		// http://www.vsdsp-forum.com/phpbb/viewtopic.php?f=14&t=2206#p11698
		SPIWriteWord(PROTOLINE_WORD_ADDRESS(0) + 0x38, 0x000c, false);

		// Makes a black&white picture
		//for (i=0; i<BURSTDUR; i++) SPIWriteWord(w++,BLANK_LEVEL, false);

// For testing purposes, make some interesting pattern to proto 0
#if PAL_PROTOLINE_DEBUG	
	w = PROTOLINE_WORD_ADDRESS(0)+BLANKEND;
	SPIWriteWord(w++, 0x7F, false);
	for (i=1; i<=50; i++) {
		SPIWriteWord((u_int16)w++, 0x797F+i*0x1300, false); //"Proto-maximum" green level + color carrier wave
	}
	// To make red+blue strip
	#define RED_BIT 0x0400
	#define BLUE_BIT 0x0800
	SPIWriteWord(PROTOLINE_WORD_ADDRESS(0) + 126, 0x7380 + RED_BIT, false);
	SPIWriteWord(PROTOLINE_WORD_ADDRESS(0) + 130, 0x7380 + RED_BIT + BLUE_BIT, false);
	SPIWriteWord(PROTOLINE_WORD_ADDRESS(0) + 134, 0x7380 + BLUE_BIT, false);
	SPIWriteWord(PROTOLINE_WORD_ADDRESS(0) + 138, 0x7380, false);
	// Max V and min U levels
	SPIWriteWord(PROTOLINE_WORD_ADDRESS(0) + 146, 0x79c1, false);
	SPIWriteWord(PROTOLINE_WORD_ADDRESS(0) + 147, 0x79c1, false);
	SPIWriteWord(PROTOLINE_WORD_ADDRESS(0) + 148, 0x79c1, false);

	SPIWriteWord(PROTOLINE_WORD_ADDRESS(0) + 156, 0xf54f+BLANK_LEVEL, false);
	SPIWriteWord(PROTOLINE_WORD_ADDRESS(0) + 157, 0xf54f+BLANK_LEVEL, false);
	SPIWriteWord(PROTOLINE_WORD_ADDRESS(0) + 158, 0xf54f+BLANK_LEVEL, false);
	// Orangish color to the end of proto line
	w = PROTOLINE_WORD_ADDRESS(0)+FRPORCH-1; 
	for (i=0; i<=90; i++) {
		SPIWriteWord((u_int16)w--, (WHITE_LEVEL-0x30)|0xc400, false);
	}
#endif	

		// Construct protoline 1. This is a short+short VSYNC line
	#ifdef SERIAL_DEBUG
		Serial.println(F("Construct protoline 1"));
	#endif
		w = PROTOLINE_WORD_ADDRESS(1);
		for (i=0; i<=COLORCLKS_PER_LINE; i++) {
			SPIWriteWord((u_int16)w++, BLANK_LEVEL, false);
		}
		w = PROTOLINE_WORD_ADDRESS(1);
		for (i=0; i<SHORTSYNC; i++) SPIWriteWord((u_int16)w++,SYNC_LEVEL, false); // Short sync at the beginning of line
		w = PROTOLINE_WORD_ADDRESS(1)+COLORCLKS_LINE_HALF;
		for (i=0; i<SHORTSYNCM; i++) SPIWriteWord((u_int16)w++,SYNC_LEVEL,false); // Short sync at the middle of line

		// Construct protoline 2. This is a long+long VSYNC line
	#ifdef SERIAL_DEBUG
		Serial.println(F("Construct protoline 2"));
	#endif
		w = PROTOLINE_WORD_ADDRESS(2);
		for (i=0; i<=COLORCLKS_PER_LINE; i++) {
			SPIWriteWord((u_int16)w++, BLANK_LEVEL, false);
		}
		w = PROTOLINE_WORD_ADDRESS(2);
		for (i=0; i<LONGSYNC; i++) SPIWriteWord((u_int16)w++,SYNC_LEVEL, false); // Long sync at the beginning of line
		w = PROTOLINE_WORD_ADDRESS(2)+COLORCLKS_LINE_HALF;
		for (i=0; i<LONGSYNCM; i++) SPIWriteWord((u_int16)w++,SYNC_LEVEL, false); // Long sync at the middle of line

		// Construct protoline 3. This is a long+short VSYNC line
	#ifdef SERIAL_DEBUG
		Serial.println(F("Construct protoline 3"));
	#endif
		w = PROTOLINE_WORD_ADDRESS(3);
		for (i=0; i<=COLORCLKS_PER_LINE; i++) {
			SPIWriteWord((u_int16)w++, BLANK_LEVEL, false);
		}
		w = PROTOLINE_WORD_ADDRESS(3);
		for (i=0; i<LONGSYNC; i++) SPIWriteWord((u_int16)w++,SYNC_LEVEL, false); // Short sync at the beginning of line
		w = PROTOLINE_WORD_ADDRESS(3)+COLORCLKS_LINE_HALF;
		for (i=0; i<SHORTSYNCM; i++) SPIWriteWord((u_int16)w++,SYNC_LEVEL, false); // Long sync at the middle of line	

		// Set first lines of frame to point to PAL sync lines
		// Here the frame starts, lines 1 and 2
	#ifdef SERIAL_DEBUG
		Serial.println(F("1st lines point to PAL sync lines"));
	#endif
		// frame starts, lines 1 and 2
		for (i=0; i<2; i++) {
			SPIWriteByte (INDEX_START_BYTES + i*3, 0x00, false);
			SPIWriteByte (INDEX_START_BYTES + i*3 + 1, PROTOLINE_WORD_ADDRESS(2), false);
			SPIWriteByte (INDEX_START_BYTES + i*3 + 2, PROTOLINE_WORD_ADDRESS(2)>>8, false);
		}
		// Line 3
		SPIWriteByte (INDEX_START_BYTES + 2*3, 0x00, false);
		SPIWriteByte (INDEX_START_BYTES + 2*3 + 1, PROTOLINE_WORD_ADDRESS(3), false);
		SPIWriteByte (INDEX_START_BYTES + 2*3 + 2, PROTOLINE_WORD_ADDRESS(3)>>8, false);
		// Lines 4 and 5
		for (i=3; i<5; i++) {
			SPIWriteByte (INDEX_START_BYTES + i*3, 0x00, false);
			SPIWriteByte (INDEX_START_BYTES + i*3 + 1, PROTOLINE_WORD_ADDRESS(1), false);
			SPIWriteByte (INDEX_START_BYTES + i*3 + 2, PROTOLINE_WORD_ADDRESS(1)>>8, false);
		}
		// These are three last lines of the frame, lines 310-312
		for (i=TOTAL_LINES-3; i<TOTAL_LINES; i++) {
			SPIWriteByte (INDEX_START_BYTES + i*3, 0x00, false);
			SPIWriteByte (INDEX_START_BYTES + i*3 + 1, PROTOLINE_WORD_ADDRESS(1), false);
			SPIWriteByte (INDEX_START_BYTES + i*3 + 2, PROTOLINE_WORD_ADDRESS(1)>>8, false);
		}
	

		// Set pic line indexes to point to protoline 0 and their individual picture line.
	#ifdef SERIAL_DEBUG
		Serial.println(F("Pic line index to protoline 0"));
	#endif
		for (i=0; i<ENDLINE-STARTLINE; i++) { 
			SPIWriteByte (INDEX_START_BYTES + (i + STARTLINE)*3,     ((PICLINE_BYTE_ADDRESS(i) << 7) & 0x80) |(0 &0xf), false);
			SPIWriteByte (INDEX_START_BYTES + (i + STARTLINE)*3 + 1,  (PICLINE_BYTE_ADDRESS(i) >> 1), false);
			SPIWriteByte (INDEX_START_BYTES + (i + STARTLINE)*3 + 2,  (PICLINE_BYTE_ADDRESS(i) >> 9), false);
		}
		// Enable Video Display Controller, set video mode to PAL, set program length and linecount.
		SPIWriteRegister16 (WriteVideoDisplayControl2, Enable_Video | PAL | Program_Length | LineCount, false );

		// Fixes the picture to proto area border artifacts when BEXTRA > 0.
		if (BEXTRA>0) {
			for (i=0; i<ENDLINE-STARTLINE; i++) {
				if (i%4==PICX%4) {
					for (j=0; j<BEXTRA;j++) {
						if (j%2==1){
							SPIWriteByte(PICLINE_BYTE_ADDRESS(i)+PICLINE_LENGTH_BYTES+j,0xc4, false); // V and U of the first proto pixel after picture.
						} else {
							SPIWriteByte(PICLINE_BYTE_ADDRESS(i)+PICLINE_LENGTH_BYTES+j,0xcf, false); // Y of the first proto pixel after picture.
						}
					}
				}
			}
		}
#endif
	}
	else {
	#ifdef SERIAL_DEBUG
		Serial.print(F("No VS23S010D-L module found: 0x"));
		Serial.println(result,HEX);
	#endif
	}
		
	return result;
}

// ----------------------------------------------
// Read 8bit byte from SRAM memory
byte P42Display::SPIReadByte (u_int32 address)
{
	byte result = 0;
	byte result_hi = 0;
	byte result_lo = 0;
	
	// take the SS pin low to select the chip:
	digitalWrite(slaveSelectPin,LOW);
	// send in the address and return value via SPI:
	SPI.transfer( ReadSRAM );
	SPI.transfer( byte ((address >> 16) & 0xFF ));
	SPI.transfer( byte ((address >>  8) & 0xFF ));
	SPI.transfer( byte( address         & 0xFF ));
	result = SPI.transfer( byte(0x00) );
	// take the SS pin high to de-select the chip:
	digitalWrite(slaveSelectPin,HIGH); 

	return result;
}
// ----------------------------------------------
// Read 16bit word from SRAM memory
word P42Display::SPIReadWord (u_int32 address)
{
	word result = 0;
	byte result_hi = 0;
	byte result_lo = 0;
	
	address = address << 1;
 	// take the SS pin low to select the chip:
	digitalWrite(slaveSelectPin,LOW);
	//  send in the address and return value via SPI:
	SPI.transfer( ReadSRAM );
	SPI.transfer( byte ((address >> 16) & 0xFF ));
	SPI.transfer( byte ((address >>  8) & 0xFF ));
	SPI.transfer( byte( address         & 0xFF ));
	result_hi = SPI.transfer(0x00);
	result_lo = SPI.transfer(0x00);

	// take the SS pin high to de-select the chip:
	digitalWrite(slaveSelectPin,HIGH); 

//	Serial.print(F("SPIReadWord value hi: 0x"));
//	Serial.print(result_hi,HEX);
//	Serial.print(F(" lo: 0x"));
//	Serial.println(result_lo,HEX);
//	result = (result_hi	<< 8) | (result_lo) ;

	return result;
}

// ----------------------------------------------
// Write 8bit byte into SRAM memory
void P42Display::SPIWriteByte (u_int32 address, byte value, boolean debug = false)
{
	// take the SS pin low to select the chip:
	digitalWrite(slaveSelectPin,LOW);
	// send in the address and return value via SPI:
	SPI.transfer( WriteSRAM );
	SPI.transfer( byte ((address >> 16) & 0xFF ));
	SPI.transfer( byte ((address >>  8) & 0xFF ));
	SPI.transfer( byte ( address        & 0xFF ));
	SPI.transfer( byte ( value          & 0xFF ));
	// take the SS pin high to de-select the chip:
	digitalWrite(slaveSelectPin,HIGH); 

	if (debug ){
		Serial.print(F("SPI opcode: 0x"));
		Serial.print(WriteSRAM,HEX);
		Serial.print(F(" adress: 0x"));
		Serial.print(address,HEX);
		Serial.print(F(" : 0x"));
		Serial.println(value,HEX);
	};
	
}

// ----------------------------------------------
// Write 16bit word into SRAM memory
void P42Display::SPIWriteWord (u_int32 address, word value, boolean debug = false)
{
	byte high = value >> 8;
	byte low = value & 0xFF;
// print 1st because address gets shifted
	if (debug ){
		Serial.print(F("SPI opcode: 0x"));
		Serial.print(WriteSRAM,HEX);
		Serial.print(F(" adress: 0x"));
		Serial.print(address,HEX);
		Serial.print(F(" : 0x"));
		Serial.println(value,HEX);
//		Serial.print(F("SPIWriteWord value hi: 0x"));
//		Serial.print(byte ((value >> 8) & 0xFF),HEX);
//		Serial.print(F(" lo: 0x"));
//		Serial.println(byte (value) & 0xFF,HEX);
	};

	// take the SS pin low to select the chip:
	digitalWrite(slaveSelectPin,LOW);
	// send in the address and return value via SPI:
	address = address << 1;
	SPI.transfer( WriteSRAM );
	SPI.transfer( byte ((address >> 16) & 0xFF ));
	SPI.transfer( byte ((address >>  8) & 0xFF ));
	SPI.transfer( byte ( address        & 0xFF ));
	SPI.transfer( high);
	SPI.transfer( low );
	// take the SS pin high to de-select the chip:
	digitalWrite(slaveSelectPin,HIGH); 

	
}

// ----------------------------------------------
// Draws a filled rectangle, with the specified color, from coordinates (x1,y1) to (x2,y2)
// used from VLSI Arduino demo
void P42Display::FilledRectangle (u_int16 x1, u_int16 y1, u_int16 x2, u_int16 y2, u_int16 color) {
	u_int16 width, i;
	u_int32 address;

	if (y1 >= PICY) return; // If starting y-coordinate is past the picture borders, nothing is done
	if (y2 >= PICY) y2=PICY-1; // The rectangle stops at the bottom border of the image

	width = (x2-x1)+1;
	if (width>XPIXELS) width=XPIXELS;

	// Loops through one horizontal line at a time.
	while (y1 <= y2) {
		address = PICLINE_BYTE_ADDRESS(y1) + x1;
		// Color values of each x coordinate of the horizontal line
		for (i=0; i<width; i++) {
			SPIWriteByte (address+i, color, false);
		}  
		y1++;
	}
}

// -----------------------------------------------
// Fill memory locations of display data with colour, 0x00 would equal black 
void P42Display::ClearScreen (byte colour) {
	u_int32 address;
	u_int32 length, i;

	// fast version
	// memory start and size
	address = ((u_int32)PICLINE_BYTE_ADDRESS(0) + 0);
	//	length = (u_int32)XPIXELS * (u_int32)YPIXELS;
	length = ((u_int32)PICLINE_BYTE_ADDRESS(YPIXELS-1) + XPIXELS-1)-address;

	// take the SS pin low to select the chip:
	digitalWrite(slaveSelectPin,LOW);
	// send in the address and return value via SPI:
	SPI.transfer( WriteSRAM );
	SPI.transfer( byte ((address >> 16) & 0xFF ));
	SPI.transfer( byte ((address >>  8) & 0xFF ));
	SPI.transfer( byte ( address        & 0xFF ));
	for (i = 0; i < length; i++)
		SPI.transfer( colour );
	// take the SS pin high to de-select the chip:
	digitalWrite(slaveSelectPin,HIGH); 

}

// -----------------------------------------------
// Convert a RGB color value into VUY and set the pixel. 
// This only works with the video high color palette
// color is 0x00rrggbb
void P42Display::SetRGBPixel (word x, word y, u_int32 colour) {
	
	u_int32 address;
	u_int16 red, green, blue;
	int Y, U, V;
	byte pixdata;
	
	if ((x <= XPIXELS-1) && (y <= YPIXELS-1)) {
		address = PICLINE_BYTE_ADDRESS(y) + x;
		red = (colour >>16) & 0xFF;
		green = (colour >>8) & 0xFF;
		blue = colour & 0xFF;

		Y = (76 * red + 150 * green + 29 * blue) >> 8;
		U = (red << 7 - 107 * green - 20 * blue) >> 8;
		V = (-43 * red - 84 * green + blue << 7) >> 8;

		pixdata = 	((U>>(8-UBITS))<<USHIFT) |
					((V>>(8-VBITS))<<VSHIFT) |
					((Y>>(8-YBITS)));

		SPIWriteByte (address, pixdata, false);
	}
}

// -----------------------------------------------
// Set a pixel with the given value interpreted as VUY
// color value is a word lower bits are used for lo colour palettes
void P42Display::SetYUVPixel (word x, word y, byte colour) {
	
	u_int32 address;
	
	if ((x <= XPIXELS-1) && (y <= YPIXELS-1)) {
		address = PICLINE_BYTE_ADDRESS(y) + x;
	
		SPIWriteByte (address, colour, false);
//		SPIWriteByte (address+1, colour, false);
	}
}


// -----------------------------------------------
// Print Char from FLASH
void P42Display::PrintChar (char Letter, word x, word y, byte colour) {

byte bitmap[8];
u_int16 i,j = 0;
u_int32 address, char_address;
	
	address = PICLINE_BYTE_ADDRESS(y) + x;
	char_address = (Letter - '!') * 8;

//		Serial.print(F(" 0x"));
//		Serial.println(char_address, HEX);

	digitalWrite(MemSelectPin,LOW);
	SPI.transfer( 0x03 );								// read data
	SPI.transfer( byte((char_address >>16) & 0xFF) );	// start address
	SPI.transfer( byte((char_address >> 8) & 0xFF) );
	SPI.transfer( byte( char_address       & 0xFF) );
	for (i = 0; i <= 7; i++)
		bitmap[i] = SPI.transfer( 0x00 );
	digitalWrite(MemSelectPin,HIGH); 

	for (i = 0; i <= 7; i++)
	{
//		Serial.print(F(" 0x"));
//		Serial.print(bitmap[i], HEX);
		
		for (j = 8; j > 0; j--)
		{
//			Serial.print(j);
			if ((bitmap[i] >> (j-1) ) & 0x01 ) {
				SetYUVPixel ( x+8-j, y+i, colour);
//				Serial.print(F("."));
			}
		}	
	}
//	Serial.println();

}



// -----------------------------------------------
// Print String
void P42Display::PrintString (char* Text, word x, word y, byte colour) {
char Letter;
u_int16 i,j = 0;
u_int16 x_loc = 0;

	Letter = Text[i];
	x_loc = x;
//	Serial.print (Letter);
//	Serial.print (Text[i]);
	
	while (Text[i] != 0x00) {
//		Serial.print (F('<'));
		if ((Text[i] >= '!') && (Text[i] <= '~')){			// char between 0x21 and 0x7F
//			Serial.print (x_loc, DEC);
//			Serial.print (F('<'));
			PrintChar (Text[i], x_loc, y, colour);
		}
		x_loc = x_loc + 8;
		i++;
	}
//	Serial.println();
}

// -----------------------------------------------
// receive number of bytes on UART and write to Flash memory address
byte P42Display::UARTDataToFlash ( u_int32 length, u_int32 mem_location) {
	u_int32	i,j = 0;
	byte return_value = 0;
	
	Serial.print (F("Expecting"));
	Serial.print (length, DEC);
	Serial.print (F("= 0x"));
	Serial.print (length, HEX);
	Serial.print (F(" Bytes."));
	
	
	return return_value;
};   





// --------------------------------------------------------------------------------------
// Hexdump Flash memory to UART
// --------------------------------------------------------------------------------------
void SPImemdump (unsigned long address, unsigned int bytes) {

	unsigned long i = 0;
	byte result =0;
	
	digitalWrite(MemSelectPin,LOW);
	SPI.transfer( 0x03 );					// read data
	SPI.transfer( (address >>16) &0xFF );	// start address
	SPI.transfer( (address >> 8) &0xFF );
	SPI.transfer( (address     ) &0xFF );
	for (i = 0; i < bytes; i++) {
		if ((i % 8) == 0) {
			if (i != 0)
				Serial.println (";");
			Serial.print(" 0x");
			Serial.print(address + i,HEX);
			Serial.print(": ");
		}
		result = SPI.transfer( 0x00 );
		Serial.print(" 0x");
		if (result <0x10)
			Serial.print ("0");
		Serial.print(result,HEX);
	}
    digitalWrite(MemSelectPin,HIGH); 
	Serial.println("<");
  

}



// -----------------------------------------------
// Read Data from Flash, interpret as an BMP image and display at coordinates x,y
// Supported BMP: 24bit colour depth
// Non colour mapping yet
void P42Display::DisplayBMPFromFlash ( u_int32 mem_location, u_int16 x, u_int16 y ) {
	
	unsigned long counter = 0;
	byte inbyte = 0;
	byte pixelvalue = 0;
	byte red,green,blue = 0;
	
	unsigned long FileSize = 54;		// set to header size
	unsigned long PixelOffset = 54;		// Offset from beginning of file to the beginning of the bitmap data
	unsigned long Width = 0;			// 
	unsigned long Height = 0;			// 
	byte BitsPerPixel = 0;
	byte Compression = 0;
	unsigned long ulongtemp = 0;		// 


	SPImemdump (0, 16);
	SPImemdump (0x1000, 64);
	
	SPImemdump (mem_location + counter, 16);

	while ( counter < FileSize ) {
		
		digitalWrite(MemSelectPin,LOW);
		SPI.transfer( 0x03 );									// read data
		SPI.transfer( byte(((mem_location + counter) >>16) & 0xFF) );	// Flash start address
		SPI.transfer( byte(((mem_location + counter) >> 8) & 0xFF) );
		SPI.transfer( byte( (mem_location + counter)       & 0xFF) );
		inbyte = SPI.transfer( 0x00 );
		digitalWrite(MemSelectPin,HIGH); 
		
		
//Serial.print("(0x");
//Serial.print(mem_location + counter,HEX);
//Serial.print(")");
//Serial.print(inbyte,HEX);
//Serial.print(".");
		switch ( counter ) {
			case 0: {
				if (inbyte != 'B') {
					Serial.println(F("B0: Not a BMP"));
					counter = FileSize;						// to stop while loop
				}
				break;
			}
			case 1: {
				if (inbyte != 'M') {
					Serial.println(F("B1: Not a BMP"));
					counter = FileSize;						// to stop while loop
				}
				break;
			}
			case 2:{							// 2-5 file size in bytes
				ulongtemp = inbyte;
				break;
			}
			case 3:{
				ulongtemp += (inbyte<<8);
				break;
			}
			case 4:{
				ulongtemp += (inbyte<<16);
				break;
			}
			case 5:{
				FileSize = ulongtemp + (inbyte<<24);
				Serial.print(F("Filesize: "));
				Serial.println(FileSize);
				break;
			}
			case 0xA:{							// 10-13 pixel offset
				PixelOffset = inbyte;
				break;
			}
			case 0xB:{
				PixelOffset += (inbyte<<8);
				break;
			}
			case 0xC:{
				PixelOffset += (inbyte<<16);
				break;
			}
			case 0xD:{
				PixelOffset = PixelOffset + (inbyte<<24);
				Serial.print(F("PixelOffset: 0x"));
				Serial.println(PixelOffset,HEX);
				break;
			}
			case 0x12:{							// 0x12-0x15 width
				Width = inbyte;
				break;
			}
			case 0x13:{
				Width += (inbyte<<8);
				break;
			}
			case 0x14:{
				Width += (inbyte<<16);
				break;
			}
			case 0x15:{
				Width = Width + (inbyte<<24);
				break;
			}
			case 0x16:{							// 0x12-0x15 Height
				Height = inbyte;
				break;
			}
			case 0x17:{
				Height += (inbyte<<8);
				break;
			}
			case 0x18:{
				Height += (inbyte<<16);
				break;
			}
			case 0x19:{
				Height = Height + (inbyte<<24);
				break;
			}
			case 0x1C:{
				BitsPerPixel = inbyte;
				Serial.print(BitsPerPixel);
				Serial.println(F("bit image"));
				break;
			}
			case 0x1E:{
				Compression = inbyte;
				break;
			}
			default: {
				break;
			}
		}	// switch
		if ( counter >= PixelOffset ) {
			switch ( BitsPerPixel ) {
				case 8: {
					//Serial.println(F("8bit image"));
					digitalWrite(MemSelectPin,LOW);
					SPI.transfer( 0x03 );									// read data
					SPI.transfer( byte(((mem_location + 0x36 +inbyte*4) >>16) & 0xFF) );	// Flash start address of BMP colour table
					SPI.transfer( byte(((mem_location + 0x36 +inbyte*4) >> 8) & 0xFF) );
					SPI.transfer( byte( (mem_location + 0x36 +inbyte*4)       & 0xFF) );
					red = SPI.transfer( 0x00 );
					green = SPI.transfer( 0x00 );
					blue = SPI.transfer( 0x00 );
					digitalWrite(MemSelectPin,HIGH); 
					
					pixelvalue = (red>>6) + (green>>6) + (blue>>6);		// translate RGB into 12 value grey scale
					SetYUVPixel ( x + (counter-PixelOffset)%Width , y+Height - (counter-PixelOffset)/Width, pixelvalue+2); // shift 2 values up
					
					break;
				}
				case 24: {
					//Serial.println(F("24bit image"));
					switch ( (counter-PixelOffset)%3 ) {
						// scale 24bit into 4 lower bit of pixelvalue for a 12-step grey scale image.
						case 0:{
							// blue sub-pixel
							pixelvalue = inbyte>>6;
							break;
						}
						case 1:{
							// green sub-pixel
							pixelvalue = pixelvalue + (inbyte>>6);
							break;
						}
						case 2:{
							// red sub-pixel
							pixelvalue = pixelvalue + (inbyte>>6);
							SetYUVPixel ( x + (counter-PixelOffset)%Width , y+Height - (counter-PixelOffset)/Width, pixelvalue+2); // shift 2 values up
							break;
						}
						default: {
							break;
						}
					}
				}
				default: {
					break;
				}
			}
		}
		counter ++; // this is the last instruction in the file read while loop
	}		// while
	Serial.println (F("Image done."));




}
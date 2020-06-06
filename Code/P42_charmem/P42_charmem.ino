// Demo Sketch for P42 Display Shield
// written by
// Wolfgang Friedrich
// Last change: 2020/Jun/03
// https://hackaday.io/project/21097-ntscpal-video-display-shield
// https://github.com/wolfgangfriedrich/P42Display

// The program takes a 8x8 font bitmap and copies the data into the unused
// memory for the VS23S010. 
// Character memory start address is the 1st byte after the frame buffer. 
// The character and string print routine accesses this memory instead of the SPI Flash. 


// supported resolutions:
//  NTSC 320x200x8bit 
//  PAL  300x240x8bit

#include <Arduino.h>
#include <SPI.h>

// *** Set Video mode and Uno/Mega platform in this header file ***
#include "VS23S010D-L.h"
#include "hack8x8.h"

P42Display P42Display; 

const u_int32 START_FREE_VIDEO_MEM	= (PICLINE_BYTE_ADDRESS(YPIXELS-1) + XPIXELS-1) +1;
const u_int16 CHARMAP_BYTES		=  sizeof (hack_8ptBitmaps);

// Grab character data from video memory and print on screen
// printing characters between 0x21 and 0x7F, others get a "X"
void PrintVChar (char Letter, u_int16 x, u_int16 y, byte colour) {

byte bitmap;
u_int16 i,j = 0;
u_int32 address, char_address;
	
	address = PICLINE_BYTE_ADDRESS(y) + x;
	if ((Letter >= '!') && (Letter <= '~')){			// char between 0x21 and 0x7F
		char_address = START_FREE_VIDEO_MEM + (Letter - '!') * 8;		// The '!' is the 1st character from the std ASCII table that is implemented
	}
	else {
		char_address = START_FREE_VIDEO_MEM + ('X' - '!') * 8;
	}
	
//	for (i = 0; i <= 7; i++)
//		bitmap[i] = P42Display.SPIReadByte( char_address + i );

	for (i = 0; i <= 7; i++)
	{
		bitmap = P42Display.SPIReadByte( char_address + i );
		for (j = 8; j > 0; j--)
		{
			if ((bitmap >> (j-1) ) & 0x01 ) {
				P42Display.SetYUVPixel ( x+8-j, y+i, colour);
			}
		}	
	}

}

// Process string of characters using character data from video memory
void PrintVString (char* Text, u_int16 x, u_int16 y, byte colour) {
char Letter;
u_int16 i,j = 0;
u_int16 x_loc = 0;

	Letter = Text[i];
	x_loc = x;
	
	while (Text[i] != 0x00) {
		if ((Text[i] >= '!') && (Text[i] <= '~')){			// char between 0x21 and 0x7F
			PrintVChar (Text[i], x_loc, y, colour);
		}
		x_loc = x_loc + 8;
		i++;
	}

}

void setup() {
	u_int16 P42_Display_ID = 0;

	while (!Serial) ;

	Serial.begin(115200);
	Serial.println("");
	Serial.println(F("P42 Display Characters in Video Memory Test"));

	// Config pins
	pinMode(nWPPin, OUTPUT);
	digitalWrite(nWPPin, HIGH);
	pinMode(nHOLDPin, OUTPUT);
	digitalWrite(nHOLDPin, HIGH);

	// Disable pins that are used for SPI on Uno.
#ifdef MEGA
	pinMode(11, INPUT);
	pinMode(12, INPUT);
	pinMode(13, INPUT);
#endif	

	// Config SPI interface
	pinMode(slaveSelectPin, OUTPUT);
	digitalWrite(slaveSelectPin, HIGH);
	pinMode(MemSelectPin, OUTPUT);
	digitalWrite(MemSelectPin, HIGH);
	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.setDataMode(SPI_MODE0);
	SPI.setBitOrder(MSBFIRST) ; 
	
	P42_Display_ID = P42Display.Config();

}


void loop() {

	word adc_value = 0;
	byte incomingByte = 0;   // for incoming serial data
	byte GPIOControlReg = 0;
	u_int32 i,j,x,y;		// Used for counters
	u_int32 address;
	byte value;
	
	Serial.println(F("Image address map "));
	
	address = PICLINE_BYTE_ADDRESS(0) + 0;
	Serial.print(F("Address pixel (0,0): 0x"));
	Serial.println(address, HEX );
	address = PICLINE_BYTE_ADDRESS(0) + 1;
	Serial.print(F("Address pixel (1,0): 0x"));
	Serial.println(address, HEX );
	address = PICLINE_BYTE_ADDRESS(0) + 2;
	Serial.print(F("Address pixel (2,0): 0x"));
	Serial.println(address, HEX );
	address = PICLINE_BYTE_ADDRESS(1) + 0;
	Serial.print(F("Address pixel (0,1): 0x"));
	Serial.println(address, HEX );
	address = PICLINE_BYTE_ADDRESS(10) + 10;
	Serial.print(F("Address pixel (10,10): 0x"));
	Serial.println(address, HEX );
	address = PICLINE_BYTE_ADDRESS(YPIXELS-1) + XPIXELS-1;
	Serial.print(F("Address pixel ("));
	Serial.print(XPIXELS);
	Serial.print(F(","));
	Serial.print(YPIXELS);
	Serial.print(F("): 0x"));
	Serial.println(address, HEX );

	P42Display.ClearScreen ( 0x00 );

	// Write Char data into video memory

	Serial.print(F("Start Character Map 0x"));
	Serial.println(START_FREE_VIDEO_MEM, HEX );
	Serial.print(F("Size of Character Map 0x"));
	Serial.println(CHARMAP_BYTES, HEX );

	for (i = 0; i < CHARMAP_BYTES; i++) {
		P42Display.SPIWriteByte (START_FREE_VIDEO_MEM + i , hack_8ptBitmaps[i], false);
	}


	// test character and string print function
	PrintVChar ('4', 0, 40, 0xEE);
	PrintVChar ('2', 8, 40, 0x05);

	PrintVString ("!\"#$%&\'()*+,-./0123456789:;<=>?@", 0, 0, 0x05);
	PrintVString ("ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`", 0, 16, 0x05);
	PrintVString ("abcdefghijklmnopqrstuvwxyz{|}~", 0, 32, 0x05);

//	Serial.println(F("Clear Screen [press key]"));
//	while (Serial.available() == 0) {};
//	incomingByte = Serial.read();
//
//	P42Display.ClearScreen ( 0x00 );

	while (Serial.available() == 0) {};
	incomingByte = Serial.read();
	Serial.println(F("End of test! [Restart press key]"));
	delay(1);
	while (Serial.available() == 0) {};
	incomingByte = Serial.read();
}

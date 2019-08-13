// Demo Sketch for P42 Display Shield
// written by
// Wolfgang Friedrich
// Last change: 2018/Mar/30
// https://hackaday.io/project/21097-ntscpal-video-display-shield
// https://github.com/wolfgangfriedrich/P42Display


// supported resolutions:
//  NTSC 320x200x8bit 
//  PAL  300x240x8bit

#include <Arduino.h>
#include <SPI.h>

// *** Set Video mode and Uno/Mega platform in this header file ***
#include "VS23S010D-L.h"

P42Display P42Display; 

void setup() {
	u_int16 P42_Display_ID = 0;

	while (!Serial) ;

	Serial.begin(115200);
	Serial.println("");
	Serial.println(F("P42 Display Shield Test"));

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
	u_int32 i,j,x,y; // Used for counters
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
	
	// Test GPIO 
	Serial.println(F("Test GPIO#4 - set high during clear screen operation") );
	P42Display.SPIWriteRegister( WriteGPIOControl, PIO4Dir | PIO4High, false  );
	P42Display.ClearScreen ( 0x00 );
	P42Display.SPIWriteRegister( WriteGPIOControl, 0x00, false );


	// Draw  colour map test image
	Serial.println("Draw colour map test image");

	#define XSIZEREC XPIXELS/16
	#define YSIZEREC YPIXELS/16
	for (i=0; i<16; i++) for (j=0; j<16; j++) {
		P42Display.FilledRectangle((i*XSIZEREC),              (j*YSIZEREC),              (i*XSIZEREC)+(XSIZEREC-1), (j*YSIZEREC)+(YSIZEREC-1), (j*16)+i);	// Draw colored rectangles
		P42Display.FilledRectangle((i*XSIZEREC),              (j*YSIZEREC)+(YSIZEREC-1), (i*XSIZEREC)+(XSIZEREC-1), (j*YSIZEREC)+(YSIZEREC-1), 0);			// Draw black horizontal line
		P42Display.FilledRectangle((i*XSIZEREC)+(XSIZEREC-1), (j*YSIZEREC),              (i*XSIZEREC)+(XSIZEREC-1), (j*YSIZEREC)+(YSIZEREC-1), 0);			// Draw black vertical line
	}

	Serial.println(F("Display Image [press key]") );
	while (Serial.available() == 0) {};
	incomingByte = Serial.read();
	
	P42Display.DisplayBMPFromFlash ( 0x3000, 10, 10 );

	Serial.println(F("Clear Screen [press key]"));
	while (Serial.available() == 0) {};
	incomingByte = Serial.read();

	P42Display.ClearScreen ( 0x00 );

	// colour bars
	Serial.println(F("4 RGB Colour Bars (experimental) [press key]"));
	while (Serial.available() == 0) {};
	incomingByte = Serial.read();

	x = 0;
	y = 0;
	for (i = 0; i < YPIXELS; i++)
		for (j = x; j < XPIXELS/8; j++)
			P42Display.SetRGBPixel (  j,   i, 0x00ff0000);
	x = XPIXELS/8;
	y = 0;
	for (i = 0; i < YPIXELS; i++)
		for (j = x; j < 2*XPIXELS/8; j++)
			P42Display.SetRGBPixel (  j,   i, 0x0000ff00);
	x = 2*XPIXELS/8;
	y = 0;
	for (i = 0; i < YPIXELS; i++)
		for (j = x; j < 3*XPIXELS/8; j++)
			P42Display.SetRGBPixel (  j,   i, 0x000000ff);
	x = 3*XPIXELS/8;
	y = 0;
	for (i = 0; i < YPIXELS; i++)
		for (j = x; j < 4*XPIXELS/8; j++)
			P42Display.SetRGBPixel (  j,   i, 0x00ffffff);
	
	// more colour bars
	Serial.println(F("4 YUV Pixel [press key]") );
	while (Serial.available() == 0) {};
	incomingByte = Serial.read();

	x = 4*XPIXELS/8;
	y = 0;
	for (i = y; i < YPIXELS; i++)
		for (j = x; j < 5*XPIXELS/8; j++)
			P42Display.SetYUVPixel (  j,   i, 0x24);
	x = 5*XPIXELS/8;
	y = 0;
	for (i = y; i < YPIXELS; i++)
		for (j = x; j < 6*XPIXELS/8; j++)
			P42Display.SetYUVPixel (  j,   i, 0x94);
	x = 6*XPIXELS/8;
	y = 0;
	for (i = y; i < YPIXELS; i++)
		for (j = x; j < 7*XPIXELS/8; j++)
			P42Display.SetYUVPixel (  j,   i, 0x54);
	x = 7*XPIXELS/8;
	y = 0;
	for (i = y; i < YPIXELS; i++)
		for (j = x; j < 8*XPIXELS/8; j++)
			P42Display.SetYUVPixel (  j,   i, 0xbf);
	
	// single character print

	Serial.println(F("Character [press key]"));
	while (Serial.available() == 0) {};
	incomingByte = Serial.read();

	P42Display.PrintChar ('4', 0, 40, 0xEE);
	P42Display.PrintChar ('2', 8, 40, 0xEE);
	
	// String output all characters
	Serial.println(F("All characters [press key]"));
	while (Serial.available() == 0) {};
	incomingByte = Serial.read();

	P42Display.ClearScreen ( 0x02 );
	P42Display.PrintString ("!\"#$%&\'()*+,-./0123456789:;<=>?@", 0, 0, 0x05);
	P42Display.PrintString ("ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`", 0, 16, 0x05);
	P42Display.PrintString ("abcdefghijklmnopqrstuvwxyz{|}~", 0, 32, 0x05);

	// String output C-64 inspired start screen :-)
	Serial.println("String [press key]");
	while (Serial.available() == 0) {};
	incomingByte = Serial.read();

	P42Display.ClearScreen ( 0x5c );
	P42Display.PrintString ("**** ARDUINO P42 DISPLAY SHIELD ****", 8, 0, 0x55);
	P42Display.PrintString ("2M FLASH SYSTEM 261368 BYTES FREE.", 16, 16, 0x54);
	P42Display.PrintString ("READY.", 0, 32, 0x53);

	Serial.println(F("2 colour select [press key]") );
	while (Serial.available() == 0) {
		P42Display.PrintChar ('_', 0, 40, 0x53);
		delay (500);
		P42Display.PrintChar ('_', 0, 40, 0x5c);
		delay (500);
	};
	// purge buffer
	incomingByte = Serial.read();

	// Print boxes with colour 2 brightness steps difference
	P42Display.ClearScreen ( 0x02 );
	for (y=0; y < YPIXELS-8; y = y+8) {
		for (x=0; x < XPIXELS-16; x = x+16 ) {
			P42Display.FilledRectangle ( x,    y,   x+15, y+7, y/8*16 + x/16  + 1 );
			P42Display.FilledRectangle ( x+ 2, y+2, x+ 6, y+5, y/8*16 + x/16  + 1 + 2 );
			P42Display.FilledRectangle ( x+10, y+2, x+13, y+5, y/8*16 + x/16  + 1 - 2 );
		}
	}

	Serial.println(F("BoingBall [press key]") );
	while (Serial.available() == 0) {};
	incomingByte = Serial.read();

	
	// BoingBall animation 
	P42Display.ClearScreen ( 0x00 );
	
	const int     BOINGBALL_ADDR		= 0x1000;
	const u_int32 START_FREE_VIDEO_MEM	= (PICLINE_BYTE_ADDRESS(YPIXELS-1) + XPIXELS-1) +1;
	const byte    BALLX 				= 32;
	const byte    BALLY 				= 32;
	const int     BOINGBALL_BYTES		= BALLX * BALLY *8;
	const u_int32 MEMBYTES_LINE 		= (PICLINE_BYTE_ADDRESS(1) + 0) - (PICLINE_BYTE_ADDRESS(0) + 0);
	
	u_int32 skip = 0;

	// copy ball data into video memory after picture data.
	// quick and dirty inefficient memory copy routine
	for (i = 0; i <= BOINGBALL_BYTES; i++) {
		digitalWrite(MemSelectPin,LOW);
		SPI.transfer( 0x03 );									// read data
		SPI.transfer( byte((BOINGBALL_ADDR+i >>16) & 0xFF) );	// Flash start address
		SPI.transfer( byte((BOINGBALL_ADDR+i >> 8) & 0xFF) );
		SPI.transfer( byte( BOINGBALL_ADDR+i       & 0xFF) );
			value = SPI.transfer( 0x00 );
		digitalWrite(MemSelectPin,HIGH); 
		
		// scramble lines in video memory
		// the block transfer function needs to have the skip area for source and target memory. Really VLSI?
		if (i% (BALLX*BALLY) == 0)
			skip = BALLX * (i/(BALLX*BALLY));
		else if (( i % BALLX) == 0)
			skip = skip + (MEMBYTES_LINE-BALLX)+1;
		else
			skip++;
				
		P42Display.SPIWriteByte (START_FREE_VIDEO_MEM + skip , value, false);
	}

	x = 11;
	y = 11;

	byte frame_no = 0;
	int x_dir = 1;
	int y_dir = 1;
	int frame_dir = 1;
	u_int32 moveblock2 = 0;
	u_int32 source, target, control;

	while (Serial.available() == 0) {
	
		// wait for line counter to be outside of visible area
		while ( P42Display.SPIReadRegister16 ( ReadCurrentLinePLL, false) & 0x01ff < ENDLINE ) {}
		
		// move block
		P42Display.SPIWriteRegister40 (WriteBlockMoveControl1, 
			((u_int32)(START_FREE_VIDEO_MEM + frame_no*BALLX) >>1 & 0xFFFF), 
			((PICLINE_BYTE_ADDRESS(y-1) + x-1) >>1 & 0xFFFF), 
			BMVC_PYF | ((u_int32)(START_FREE_VIDEO_MEM + frame_no*BALLX) & 0x00001) <<2 | ((PICLINE_BYTE_ADDRESS(y-1) + x-1) & 0x00001) <<1 , false);
		moveblock2 = (u_int32)((u_int32)(MEMBYTES_LINE-BALLX)<<16) | (BALLX<<8) | BALLY;
		P42Display.SPIWriteRegister32 (WriteBlockMoveControl2, moveblock2, false);
		P42Display.SPIWriteRegister (StartBlockMove, 0x00 , false);

		x=x+x_dir;
		y=y+y_dir;
		
		// set new ball direction and rotation at the borders
		randomSeed(analogRead(0));
		if ( x <= 1 ) {
			x_dir  = 1;
			y_dir = random(-1, 2);
			frame_dir = random(1, 3);
		}
		else if ( x >= XPIXELS - BALLX ) {
			x_dir  = -1;
			y_dir = random(-1, 2);
			frame_dir = random(1, 3);
		}

		if ( y <= 1 ) { 
			y_dir  = 1;
			x_dir = random(-1, 2);
			frame_dir = random(1, 3);
		}
		else if ( y >= YPIXELS - BALLY ) {
			y_dir  = -1;
			x_dir = random(-1, 2);
			frame_dir = random(1, 3);
		}
		
		//rotation
		if (frame_dir == 1) {
			frame_no++;
			if (frame_no > 7) 
				frame_no = 0;
		}
		else if (frame_dir == 2) {
			if (frame_no == 0) 
				frame_no = 7;
			else
				frame_no--;
		}
		else {
			frame_no++;
			if (frame_no > 7) 
				frame_no = 0;
		}		
		// wait for line counter to be at beginning of visible area
		//while ( P42Display.SPIReadRegister16 ( ReadCurrentLinePLL, false) & 0x01ff > ENDLINE ) {}
		delay (20);

		};
	incomingByte = Serial.read();
	Serial.println(F("End of test! [Restart press key]"));
	delay(1);
	while (Serial.available() == 0) {};
	incomingByte = Serial.read();
}

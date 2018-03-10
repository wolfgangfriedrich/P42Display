// Demo Sketch for P42 Display Shield
// written by
// 2018/Feb/11
// Wolfgang Friedrich
// https://hackaday.io/project/21097-arduino-video-display-shield
// https://github.com/wolfgangfriedrich/P42Display


// supported resolutions:
//  NTSC 320x200x8bit 
//  PAL  300x240x8bit

#include "Arduino.h"
#include <SPI.h>

// *** Set Video mode and Uno/Mega platform in this header file ***
#include "VS23S010D-L.h"

P42Display P42Display; 

void setup() {
	unsigned int P42_Display_ID = 0;

	Serial.begin(115200);
	Serial.println("");
	Serial.println(F("P42 Display Shield Test"));

	P42_Display_ID = P42Display.Config();

}


void loop() {

	word adc_value = 0;
	byte incomingByte = 0;   // for incoming serial data
	byte GPIOControlReg = 0;
	u_int32 i,j,x,y; // Used for counters
	u_int32 address;

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
//	P42Display.SPIReadRegister (ReadGPIOControl, true);
	P42Display.ClearScreen ( 0x00 );
	P42Display.SPIWriteRegister( WriteGPIOControl, 0x00, false );
//	Serial.println(GPIOControlReg, HEX );


// Draw test image
	Serial.println("Draw test image");
//	Serial.println("Draw test image [press key] ");
//	while (Serial.available() == 0) {};
//	incomingByte = Serial.read();

	#define XSIZEREC XPIXELS/16
	#define YSIZEREC YPIXELS/16
	for (i=0; i<16; i++) for (j=0; j<16; j++) {
		P42Display.FilledRectangle((i*XSIZEREC),              (j*YSIZEREC),              (i*XSIZEREC)+(XSIZEREC-1), (j*YSIZEREC)+(YSIZEREC-1), (j*16)+i);	// Draw colored rectangles
		P42Display.FilledRectangle((i*XSIZEREC),              (j*YSIZEREC)+(YSIZEREC-1), (i*XSIZEREC)+(XSIZEREC-1), (j*YSIZEREC)+(YSIZEREC-1), 0);			// Draw black horizontal line
		P42Display.FilledRectangle((i*XSIZEREC)+(XSIZEREC-1), (j*YSIZEREC),              (i*XSIZEREC)+(XSIZEREC-1), (j*YSIZEREC)+(YSIZEREC-1), 0);			// Draw black vertical line
	}

	Serial.println(F("Clear Screen [press key]"));
	while (Serial.available() == 0) {};
	incomingByte = Serial.read();

	P42Display.ClearScreen ( 0x00 );

	Serial.println(F("4 RGB Pixel [press key]"));
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
	
	Serial.println("4 YUV Pixel [press key]");
	while (Serial.available() == 0) {};
	incomingByte = Serial.read();

//	P42Display.ClearScreen ( 0x00 );
	x = 4*XPIXELS/8;
	y = 0;
	for (i = y; i < YPIXELS; i++)
		for (j = x; j < 5*XPIXELS/8; j++)
			P42Display.SetYUVPixel (  j,   i, 0x22);
	x = 5*XPIXELS/8;
	y = 0;
	for (i = y; i < YPIXELS; i++)
		for (j = x; j < 6*XPIXELS/8; j++)
			P42Display.SetYUVPixel (  j,   i, 0x92);
	x = 6*XPIXELS/8;
	y = 0;
	for (i = y; i < YPIXELS; i++)
		for (j = x; j < 7*XPIXELS/8; j++)
			P42Display.SetYUVPixel (  j,   i, 0x52);
	x = 7*XPIXELS/8;
	y = 0;
	for (i = y; i < YPIXELS; i++)
		for (j = x; j < 8*XPIXELS/8; j++)
			P42Display.SetYUVPixel (  j,   i, 0xbf);
	
	Serial.println(F("Character [press key]"));
	while (Serial.available() == 0) {};
	incomingByte = Serial.read();

	P42Display.PrintChar ('4', 0, 40, 0x15);
	P42Display.PrintChar ('2', 8, 40, 0x15);
	
	Serial.println(F("All characters [press key]"));
	while (Serial.available() == 0) {};
	incomingByte = Serial.read();

	P42Display.ClearScreen ( 0x02 );
	P42Display.PrintString ("!\"#$%&\'()*+,-./0123456789:;<=>?@", 0, 0, 0x05);
	P42Display.PrintString ("ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`", 0, 16, 0x05);
	P42Display.PrintString ("abcdefghijklmnopqrstuvwxyz{|}~", 0, 32, 0x05);

	Serial.println("String [press key]");
	while (Serial.available() == 0) {};
	incomingByte = Serial.read();

	P42Display.ClearScreen ( 0x5c );
	P42Display.PrintString ("**** ARDUINO P42 DISPLAY SHIELD ****", 8, 0, 0x55);
	P42Display.PrintString ("2M FLASH SYSTEM 261368 BYTES FREE.", 16, 16, 0x54);
	P42Display.PrintString ("READY.", 0, 32, 0x53);
	while (Serial.available() == 0) {
		P42Display.PrintChar ('_', 0, 40, 0x53);
		delay (500);
		P42Display.PrintChar ('_', 0, 40, 0x5c);
		delay (500);
	};
	incomingByte = Serial.read();

	Serial.println(F("End of test! [Restart press key]"));
	delay(1);
	while (Serial.available() == 0) {};
	incomingByte = Serial.read();
}

// Sketch for P42 Display Shield
// Memory test and FLASH write with character set and BoingBall Demo
// written by
// Wolfgang Friedrich
// Last change: 2018/Mar/30
// https://hackaday.io/project/21097-ntscpal-video-display-shield
// https://github.com/wolfgangfriedrich/P42Display

// *** Comment out this line if target system is not an Arduino MEGA ***
//#define MEGA

// inslude the SPI library:
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "hack8x8.h"

#include <avr/pgmspace.h>
#include "ball_frames.h"

const int slaveSelectPin  = 10;
const int MemSelectPin    = 9;
const int nHOLDPin        = 8;
const int nWPPin          = 7;

const int CHAR_MAP_ADDR		= 0x0000;
const int BOINGBALL_ADDR	= 0x1000;
const int IMAGE1_ADDR		= 0x3000;
const int IMAGE2_ADDR		= 0x5000;

void setup() {
	unsigned int P42_Display_ID = 0;

//	P42Display::Init()

	Serial.begin(19200);
	Serial.println("");
	Serial.println(F("P42 Display Shield Memory Test"));

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

	
}


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

// --------------------------------------------------------------------------------------
// As the function says
// --------------------------------------------------------------------------------------
void SPImemSectorErase ( unsigned long mem_addr ) {

  byte StatusReg = 0x01;
	
	digitalWrite(MemSelectPin,LOW);
	SPI.transfer( 0x06 );               // WEN
	digitalWrite(MemSelectPin,HIGH); 
	//delay(1);
	digitalWrite(MemSelectPin,LOW);
	SPI.transfer( 0x20 );               // Sector erase
	SPI.transfer( (mem_addr >>16) & 0xFF );        // sector address (16 blocks are reserved for font)
	SPI.transfer( (mem_addr >> 8) & 0xFF );
	SPI.transfer( 0x00 );
	digitalWrite(MemSelectPin,HIGH); 

//	Serial.print( mem_addr + 0x10, HEX );
//	Serial.print( " " );

	//delay(1000);
    StatusReg = 1;
	while (StatusReg & 0x01 == 1) {
		Serial.print(StatusReg,HEX);
		digitalWrite(MemSelectPin,LOW);
		SPI.transfer( 0x05 );
		StatusReg = SPI.transfer(0x00);
		digitalWrite(MemSelectPin,HIGH); 
		delay(1);
	};
	Serial.print(StatusReg, HEX);
//	Serial.println();

}

// --------------------------------------------------------------------------------------
// write a single Byte to flash
// --------------------------------------------------------------------------------------
void SPIwritebyte ( unsigned long mem_addr, byte data ) {
	
	byte StatusReg = 0x01;

	digitalWrite(MemSelectPin,LOW);
	SPI.transfer( 0x06 );               // WEN
	digitalWrite(MemSelectPin,HIGH); 
//	delay(1);
	digitalWrite(MemSelectPin,LOW);
	SPI.transfer( 0x02 );               // Page program
    SPI.transfer( (mem_addr >>16) & 0xFF );		// start address
    SPI.transfer( (mem_addr >> 8) & 0xFF );		// 
    SPI.transfer( (mem_addr     ) & 0xFF );		// 
	SPI.transfer( data);
	digitalWrite(MemSelectPin,HIGH); 
	
	StatusReg = 1;
	while (StatusReg & 0x01 == 1) {
//		Serial.print("*");
		digitalWrite(MemSelectPin,LOW);
		SPI.transfer( 0x05 );
		StatusReg = SPI.transfer(0x00);
		digitalWrite(MemSelectPin,HIGH); 
	};
//	Serial.println(StatusReg, HEX);

}


void loop() {

  byte MfrID = 0;
  byte DeviceID = 0;
  byte Capacity = 0;
  byte address = 0x90;
  byte incomingByte = 0;   // for incoming serial data
  byte StatusReg = 0x01;
  unsigned long i,j = 0;
  byte blockwrite_len = 0;
//*

  Serial.println(F("!!!5 seconds wait... !!!"));
  delay(5000);

  Serial.println(F("Press Key to Start!"));		// This is to stop the program when a new one is compiling and overwrite the content
  while (Serial.available() == 0) {};
  incomingByte = Serial.read();
  
  
  // SPI Flash test
  digitalWrite(MemSelectPin,LOW);
  SPI.transfer( address );
  SPI.transfer( 0x00 );
  SPI.transfer( 0x00 );
  SPI.transfer( 0x00 );
  MfrID = SPI.transfer(0x00);
  DeviceID = SPI.transfer(0x00);
  digitalWrite(MemSelectPin,HIGH); 

  Serial.print(F("SPIMem MfrID: 0x"));
  Serial.print(MfrID,HEX);
  Serial.print(F(" DeviceID: 0x"));
  Serial.println(DeviceID,HEX);

  address = 0x9f; // Jedec ID
  digitalWrite(MemSelectPin,LOW);
  SPI.transfer( address );
  MfrID = SPI.transfer(0x00);
  DeviceID = SPI.transfer(0x00);
  Capacity = SPI.transfer(0x00);
  digitalWrite(MemSelectPin,HIGH); 

  Serial.print(F("Jedec MfrID: 0x"));
  Serial.print(MfrID,HEX);
  Serial.print(F(" DeviceID: 0x"));
  Serial.print(DeviceID,HEX);
  Serial.print(F(" Capacity: 0x"));
  Serial.println(Capacity,HEX);

  Serial.println("-");
  while (Serial.available() == 0) {};
  incomingByte = Serial.read();

  address = 0x20; // Sector erase
  digitalWrite(MemSelectPin,LOW);
  SPI.transfer( 0x06 );               // WEN
  digitalWrite(MemSelectPin,HIGH); 
  delay(1);
  digitalWrite(MemSelectPin,LOW);
  SPI.transfer( address );               // Sector erase
  SPI.transfer( 0x00 );               // sector address
  SPI.transfer( 0x00 );
  SPI.transfer( 0x00 );
  digitalWrite(MemSelectPin,HIGH); 

  //delay(1000);
  StatusReg = 1;
  while (StatusReg & 0x01 == 1) {
    Serial.print(StatusReg,HEX);
    digitalWrite(MemSelectPin,LOW);
    SPI.transfer( 0x05 );
    StatusReg = SPI.transfer(0x00);
    digitalWrite(MemSelectPin,HIGH); 
    delay(1);
  };
  Serial.println(StatusReg, HEX);

  digitalWrite(MemSelectPin,LOW);
  SPI.transfer( 0x06 );               // WEN
  digitalWrite(MemSelectPin,HIGH); 
  delay(1);
  digitalWrite(MemSelectPin,LOW);
  SPI.transfer( 0x02 );               // Page program
  SPI.transfer( 0x00 );               // start address
  SPI.transfer( 0x00 );
  SPI.transfer( 0x00 );
  SPI.transfer( 0xAF );
  SPI.transfer( 0xFE );
  digitalWrite(MemSelectPin,HIGH); 

  StatusReg = 1;
  while (StatusReg & 0x01 == 1) {
    Serial.print("*");
    digitalWrite(MemSelectPin,LOW);
    SPI.transfer( 0x05 );
    StatusReg = SPI.transfer(0x00);
    digitalWrite(MemSelectPin,HIGH); 
  };
  Serial.println(StatusReg, HEX);

  digitalWrite(MemSelectPin,LOW);
  SPI.transfer( 0x03 );               // read data
  SPI.transfer( 0x00 );               // start address
  SPI.transfer( 0x00 );
  SPI.transfer( 0x00 );
  MfrID = SPI.transfer( 0x00 );
  DeviceID = SPI.transfer( 0x00 );
  digitalWrite(MemSelectPin,HIGH); 

  Serial.print(MfrID, HEX);
  Serial.println(DeviceID, HEX);

  Serial.println("done Flash.");
  while (Serial.available() == 0) {};
  incomingByte = Serial.read();

  // I2C EEPROM memory test
  Wire.begin(); // wake up I2C bus
  Wire.beginTransmission(0x50);
  Wire.write(0x00); // memory address 0
  Wire.write(0x00);
  Wire.write(0xC0); 
  Wire.write(0xFF); 
  Wire.write(0xEE); 
  Wire.endTransmission();
  
  delay(250);
   
  Wire.begin(); // wake up I2C bus
  Wire.beginTransmission(0x50);
  Wire.write(0x00); // memory address 0
  Wire.write(0x00);
  Wire.endTransmission();

  incomingByte = 0x00;
  Wire.requestFrom(0x50, 3); // 7-bit addres without R/W bit
  incomingByte = Wire.read(); // first received byte stored 
  Serial.print(incomingByte, HEX);
  incomingByte = Wire.read(); // 2nd received byte stored 
  Serial.print(incomingByte, HEX);
  incomingByte = Wire.read(); // 3nd received byte stored 
  Serial.println(incomingByte, HEX);

  Serial.println("done EEPROM.");
  
  while (Serial.available() == 0) {};
  incomingByte = Serial.read();

  
// No sector erase because first 2 bytes of char map are 0x00 0x40, which overwrites the test 0xAFFE.  
// Write Character map to Flash at address 0x00
  for (i=0; i<=(sizeof(hack_8ptBitmaps))>>8; i++)	// # of 256byte blocks loops
  {
    digitalWrite(MemSelectPin,LOW);
    SPI.transfer( 0x06 );               // WEN
    digitalWrite(MemSelectPin,HIGH); 
    delay(1);
    digitalWrite(MemSelectPin,LOW);
    SPI.transfer( 0x02 );               // Page program
    SPI.transfer( 0x00 );               // start address
    SPI.transfer(    i );				// this works  only up to 255 blocks = 16Kbyte of font data
    SPI.transfer( 0x00 );
    
	if (i < ( (sizeof(hack_8ptBitmaps)) >> 8 )) 
	{
		blockwrite_len = 255;			// write full block
	}
	else
	{
		blockwrite_len = ((sizeof(hack_8ptBitmaps)) & 0xFF);			// write the tail end
	}
	
    for (j = 0; j <= blockwrite_len; j++)	// write 256 byte
    {
      SPI.transfer( hack_8ptBitmaps[j + i*32*8] );
	}
    digitalWrite(MemSelectPin,HIGH); 

    StatusReg = 1;
    while (StatusReg & 0x01 == 1) {
      Serial.print("o");
      digitalWrite(MemSelectPin,LOW);
      SPI.transfer( 0x05 );
      StatusReg = SPI.transfer(0x00);
      digitalWrite(MemSelectPin,HIGH); 
    };
	Serial.println(StatusReg, HEX);
  }

  SPImemdump ( 0, 16 );
  
  Serial.println(F("done char bitmap to FLASH."));
  
  while (Serial.available() == 0) {};
  incomingByte = Serial.read();

  
// --------------------------------------------------------------------------------------
// BOINGBALL data copy
// --------------------------------------------------------------------------------------

unsigned long ballSize = 0;
static unsigned long _ballSize = 0;

ballSize = sizeof(boingball);
_ballSize = ballSize;


 // Sector erase for Boingball mem
  for (i=0; i<(sizeof(boingball)>>8); i++)	// # of 256byte blocks loops
  {

	Serial.print(BOINGBALL_ADDR+(i*256),HEX);
	SPImemSectorErase ( BOINGBALL_ADDR+(i*256) );
	Serial.println();

//	digitalWrite(MemSelectPin,LOW);
//	SPI.transfer( 0x06 );               // WEN
//	digitalWrite(MemSelectPin,HIGH); 
//	delay(1);
//	digitalWrite(MemSelectPin,LOW);
//	SPI.transfer( 0x20 );               // Sector erase
//	SPI.transfer( 0x00 + (i/240) );        // sector address (16 blocks are reserved for font)
//	SPI.transfer( 0x10 + (i%240) );
//	SPI.transfer( 0x00 );
//	digitalWrite(MemSelectPin,HIGH); 
//
//	Serial.print( i + 0x10, HEX );
//	Serial.print( " " );
//
//	//delay(1000);
//    StatusReg = 1;
//	while (StatusReg & 0x01 == 1) {
//	Serial.print(StatusReg,HEX);
//	digitalWrite(MemSelectPin,LOW);
//	SPI.transfer( 0x05 );
//	StatusReg = SPI.transfer(0x00);
//	digitalWrite(MemSelectPin,HIGH); 
//	delay(1);
//	};
//	Serial.println(StatusReg, HEX);
	  
//	  SPImemdump ( 0x1000 +i*256, 16 );
	  
  }
  

unsigned long ball_addr = 0;
byte ballbyte = 0;

// debug
//	Serial.println ("Print boingball data");
//	for (i=0; i<= 100; i++) {
//	  	Serial.print( pgm_read_byte_near(boingball + i), HEX);
//		Serial.print( " " );
//		if (i%16 == 0)
//			Serial.println ("<");
//
//	}

// Write BoingBall map to Flash at address 0x1000
  for (i=0; i<(sizeof(boingball))>>8; i++)	// # of 256byte blocks loops
  {
	ball_addr = BOINGBALL_ADDR + i*256;

    digitalWrite(MemSelectPin,LOW);
    SPI.transfer( 0x06 );               // WEN
    digitalWrite(MemSelectPin,HIGH); 
    delay(1);
    digitalWrite(MemSelectPin,LOW);
    SPI.transfer( 0x02 );				// Page program
    SPI.transfer( (ball_addr >>16) & 0xFF );		// start address
    SPI.transfer( (ball_addr >> 8) & 0xFF );		// 
    SPI.transfer( (ball_addr     ) & 0xFF );		// 
    
//	Serial.print("Block Addr :0x");
//	Serial.print(ball_addr, HEX);
//	Serial.print(" -> ");
	
	if (i < ( (sizeof(boingball)) >> 8 )) 
	{
		blockwrite_len = 255;			// write full block
	}
	else
	{
		blockwrite_len = ((sizeof(boingball)) & 0xFF);			// write the tail end
	}
	
    for (j = 0; j <= blockwrite_len; j++)	// write 256 byte
    {
      SPI.transfer( pgm_read_byte_near(boingball +(j + i*32*8)) );
//	  if (j<16) {
//		  	Serial.print(pgm_read_byte_near(boingball +(j + i*32*8)), HEX);
//			Serial.print( " " );
//	  }
	}
    digitalWrite(MemSelectPin,HIGH); 

    StatusReg = 1;
    while (StatusReg & 0x01 == 1) {
      Serial.print("w");
      digitalWrite(MemSelectPin,LOW);
      SPI.transfer( 0x05 );
      StatusReg = SPI.transfer(0x00);
      digitalWrite(MemSelectPin,HIGH); 
    };
	Serial.println(StatusReg, HEX);
	
//	SPImemdump ( 0x1000 +i*256, 16 );

  }

	SPImemdump ( 0x1000, 256 );
	SPImemdump ( 0x2FF8, 16 );
	SPImemdump ( 0, 16 );
  
	Serial.println(F("done Boingball bitmap to FLASH."));


//*/  End comment when everything above is disabled (line 178 after var declaration in loop{} )

//  Serial.println(F("!!!10 seconds wait... !!!"));
//  delay(10000);
  Serial.println(F("!!!Use a proper terminal SW (e.g. Teraterm) and binary transfer!!!"));
  Serial.println(F("Any key to start Image mem erase!"));
  
  while (Serial.available() == 0) {};
  incomingByte = Serial.read();


// --------------------------------------------------------------------------------------
// Receive image data over UART and write to Flash
// Currently suppoerted 24bit BMP
// --------------------------------------------------------------------------------------

	{
	
	unsigned long FileSize = 54;		// set to header size
	byte inbyte = 0;
	unsigned long counter = 0;
	unsigned long ulongtemp = 0;		// 

	SPImemdump ( IMAGE1_ADDR, 32 );

	for (i=IMAGE1_ADDR; i<(IMAGE1_ADDR+16384); i=i+256)	// # of 256byte blocks loops for an image max 16k+255 SIZE
	{
		Serial.print(i,HEX);
		SPImemSectorErase ( i );
		Serial.println();
	}
	Serial.println(F(" SectorErase for image done!"));
	SPImemdump ( IMAGE1_ADDR, 32 );

	Serial.setTimeout(10000);
	Serial.println(F("Please send BMP image now. (10sec timeout)"));
	// file read while loop
	while ( counter < FileSize ) {
		
		while ( !Serial.available() ) {}	// wait for next byte; timeout default is 1000msec
		inbyte = Serial.read();
//Serial.print("(");
//Serial.print(counter,HEX);
//Serial.print(")");

if (counter < 54) {
  Serial.print(inbyte,HEX);
  Serial.print(".");
}
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
				Serial.println(FileSize,HEX);
				break;
			}
			default: {
				break;
			}
		}	// switch
		SPIwritebyte ( IMAGE1_ADDR+counter, inbyte );
		counter ++; // this is the last instruction in the file read while loop
	}		// while
	Serial.println (F("Image done."));

	SPImemdump ( IMAGE1_ADDR, 256 );

	delay(1);
	}
	
	SPImemdump ( 0, 16 );
	SPImemdump ( 0x1000, 16 );

	
	Serial.println(F("Press key to restart"));
	while ( !Serial.available() ) {}	// wait for next byte;
	incomingByte = Serial.read();

}




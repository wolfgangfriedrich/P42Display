// Demo Sketch for P42 Display Shield
// Memory test
// written by
// Wolfgang Friedrich
// 3.Nov.2017


// inslude the SPI library:
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
//#include "VS23S010D-L.h"
//#include "font8x8.h"
#include "hack8x8.h"
//#include "Consolas.h"

// *** Comment out this line if target system is not an Arduino MEGA ***
#define MEGA

const int slaveSelectPin  = 10;
const int MemSelectPin    = 9;
const int nHOLDPin        = 8;
const int nWPPin          = 7;



void setup() {
	unsigned int P42_Display_ID = 0;

//	P42Display::Init()

	Serial.begin(115200);
	Serial.println("");
	Serial.println("P42 Display Shield Memory Test");

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


void loop() {

  byte MfrID = 0;
  byte DeviceID = 0;
  byte Capacity = 0;
  byte address = 0x90;
  byte incomingByte = 0;   // for incoming serial data
  byte StatusReg = 0x01;
  unsigned long i,j = 0;
  byte blockwrite_len = 0;

	Serial.println("Press Key to Start!");		// This is to stop the program when a new one is compiling and overwrite the content
  while (Serial.available() == 0) {};
  incomingByte = Serial.read();
  
  // SPI transfer
  digitalWrite(MemSelectPin,LOW);
  SPI.transfer( address );
  SPI.transfer( 0x00 );
  SPI.transfer( 0x00 );
  SPI.transfer( 0x00 );
  MfrID = SPI.transfer(0x00);
  DeviceID = SPI.transfer(0x00);
  digitalWrite(MemSelectPin,HIGH); 

  Serial.print("SPIMem MfrID: 0x");
  Serial.print(MfrID,HEX);
  Serial.print(" DeviceID: 0x");
  Serial.println(DeviceID,HEX);

  address = 0x9f; // Jedec ID
  digitalWrite(MemSelectPin,LOW);
  SPI.transfer( address );
  MfrID = SPI.transfer(0x00);
  DeviceID = SPI.transfer(0x00);
  Capacity = SPI.transfer(0x00);
  digitalWrite(MemSelectPin,HIGH); 

  Serial.print("Jedec MfrID: 0x");
  Serial.print(MfrID,HEX);
  Serial.print(" DeviceID: 0x");
  Serial.print(DeviceID,HEX);
  Serial.print(" Capacity: 0x");
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

  // debug
  	digitalWrite(MemSelectPin,LOW);
	SPI.transfer( 0x03 );               // read data
	SPI.transfer( 0x00 );               // start address
	SPI.transfer( 0x00 );
	SPI.transfer( 0x00 );
	for (i = 0; i <= 7; i++) {
		StatusReg = SPI.transfer( 0x00 );
		Serial.print(" 0x");
		Serial.print(StatusReg,HEX);
	}
    digitalWrite(MemSelectPin,HIGH); 
	Serial.println("<");
  
  
  Serial.println("done char bitmap to FLASH.");

    while (Serial.available() == 0) {};
	incomingByte = Serial.read();

  
	delay(1);
}




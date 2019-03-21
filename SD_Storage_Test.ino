// Ported to SdFat from the native Arduino SD library example by Bill Greiman
// On the Ethernet Shield, CS is pin 4. SdFat handles setting SS
const int CHIP_SELECT = 10;
const char FILE_NAME[] = "data5.txt";
/*
 SD card read/write

 This example shows how to read and write data to and from an SD card file
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4

 created   Nov 2010
 by David A. Mellis
 updated 2 Dec 2010
 by Tom Igoe
 modified by Bill Greiman 11 Apr 2011
 This example code is in the public domain.

 */
#include <SPI.h>
#include <SdFat.h>
SdFat sd;
SdFile myFile;

int counter;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }  // wait for Leonardo
  Serial.println("Type any character to start");
  while (Serial.read() <= 0) {
  }
  delay(400);  // catch Due reset problem

  counter = 0;


  // Initialize SdFat or print a detailed error message and halt
  // Use half speed like the native library.
  // change to SPI_FULL_SPEED for more performance.
  if (!sd.begin(CHIP_SELECT, SPI_HALF_SPEED)) {
    sd.initErrorHalt();
  }
}

void loop() {
  
    // open the file for write at end like the Native SD library
  if (!myFile.open(FILE_NAME, O_RDWR | O_CREAT | O_AT_END)) {
    sd.errorHalt("opening the file for write failed");
  }
  
  // if the file opened okay, write to it:
  Serial.print("Writing to " + String(FILE_NAME) + "... ");
  myFile.print("Testing ");
  myFile.println(counter);
  

  // close the file:
  myFile.close();
  Serial.println("done.");

  // re-open the file for reading:
  if (!myFile.open(FILE_NAME, O_READ)) {
    sd.errorHalt("opening the file for read failed");
  }
  Serial.println(FILE_NAME);

  // read from the file until there's nothing else in it:
  int data;
  while ((data = myFile.read()) >= 0) {
    Serial.write(data);
  }
  // close the file:
  myFile.close();
  counter++;
  delay(1);
  // nothing happens after setup
}



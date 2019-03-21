#include <Wire.h>

#define SENSOR_ADDR_OFF  (0x1D)
#define SENSOR_ADDR_ON   (0x53)

// Set the sensor address here

const uint8_t sensorAddr = SENSOR_ADDR_OFF;

// Sensor register addresses (gotten from datasheet)
#define REG_DEVID_ADDR        (0x00)
#define REG_THRESH_TAP_ADDR   (0x1d)
#define REG_TAP_DUR_ADDR      (0x21)
#define REG_TAP_LATENCY_ADDR  (0x22)
#define REG_TAP_WINDOW_ADDR   (0x23)      
#define REG_BW_RATE_ADDR      (0x2c)
#define REG_PWR_CTL_ADDR      (0x2d)
#define REG_INT_ENABLE_ADDR   (0x2e)
#define REG_DATA_FORMAT_ADDR  (0x31)
#define REG_DATAX0_ADDR       (0x32)
#define REG_DATAX1_ADDR       (0x33)
#define REG_DATAY0_ADDR       (0x34)
#define REG_DATAY1_ADDR       (0x35)
#define REG_DATAZ0_ADDR       (0x36)
#define REG_DATAZ1_ADDR       (0x37)
#define REG_FIFO_CTL_ADDR     (0x38)

const int DELAY = 500; //(miliseconds)

void setup(){
  Serial.begin(9600);
  Serial.println("Serial Connection Started \n");
  WriteByte(sensorAddr, REG_BW_RATE_ADDR, 0x08);
   // Disable auto sleep
  WriteByte(sensorAddr, REG_PWR_CTL_ADDR, 0x08);
   // Disable interrupts (the pins are not brought out anyway)
  WriteByte(sensorAddr, REG_INT_ENABLE_ADDR, 0x0);
}

void loop(){
  uint8_t devId;
  uint8_t x_msb;   // X-axis most significant byte
  uint8_t x_lsb;   // X-axis least significant byte
  uint8_t y_msb;   // Y-axis most significant byte
  uint8_t y_lsb;   // Y-axis least significant byte
  uint8_t z_msb;   // Z-axis most significant byte
  uint8_t z_lsb;   // Z-axis least significant byte
  uint16_t x;
  uint16_t y;
  uint16_t z;
  
  if (ReadByte(sensorAddr, 0x0, &devId) != 0) {
      Serial.println("Cannot read device ID from sensor");
  } else if (devId != 0xE5) {
      Serial.print("Wrong/invalid device ID ");
      Serial.print(devId);
      Serial.println(" (expected 0xE5)");
  } else {
// Read the output
    if ((ReadByte(sensorAddr, REG_DATAX1_ADDR, &x_msb) == 0) &&
    (ReadByte(sensorAddr, REG_DATAX0_ADDR, &x_lsb) == 0) &&
    (ReadByte(sensorAddr, REG_DATAY1_ADDR, &y_msb) == 0) &&
    (ReadByte(sensorAddr, REG_DATAY0_ADDR, &y_lsb) == 0) &&
    (ReadByte(sensorAddr, REG_DATAZ1_ADDR, &z_msb) == 0) &&
    (ReadByte(sensorAddr, REG_DATAZ0_ADDR, &z_lsb) == 0))
    {
      x = (x_msb << 8) | x_lsb;
      y = (y_msb << 8) | y_lsb;
      z = (z_msb << 8) | z_lsb;
      
      // Perform 2's complement
      int16_t real_x = ~(x - 1);
      int16_t real_y = ~(y - 1);
      int16_t real_z = ~(z - 1);
      
      Serial.print(" X: ");
      Serial.print(real_x);
      Serial.print(", "); 
      Serial.print(" Y: ");
      Serial.print(real_y);
      Serial.print(", ");
      Serial.print(" Z: ");
      Serial.print(real_z);
      Serial.print(", ");
      Serial.println();
      
      //String dataString1 = " X: " + String(real_x) + " ,  " + " Y: " + String(real_y) + " ,  " + " Z: " + String(real_z) ;
      String dataString1 = " X: " + String(real_x);
      //Serial.println(dataString1);
      String dataString2 = dataString1 + " ,  ";
      //Serial.println(dataString2);
      String dataString3 = dataString2 + " Y: ";
      //Serial.println(dataString3);
      String dataString4 = dataString3 + String(real_y);
      //Serial.println(dataString4);
      String dataString5 = dataString4 + " ,  ";
      //Serial.println(dataString5);
      String dataString6 = dataString5 + " Z: ";
      //Serial.println(dataString6);
      String dataString = dataString6 + String(real_z);
      //Serial.println(dataString);
    }
  }
  delay(DELAY);
}

// Read a byte on the i2c interface
int ReadByte (uint8_t addr, uint8_t reg, uint8_t *data) {
   // Do an i2c write to set the register that we want to read from
   Wire.beginTransmission(addr);
   Wire.write(reg);
   Wire.endTransmission();
   // Read a byte from the device
   Wire.requestFrom(addr, (uint8_t)1);
   if (Wire.available()) {
      *data = Wire.read();
   }
   else {
      // Read nothing back
      return -1;
   }
   return 0;
}

// Write a byte on the i2c interface
void WriteByte (uint8_t addr, uint8_t reg, byte data) {
   // Begin the write sequence
   Wire.beginTransmission(addr);
   // First byte is to set the register pointer
   Wire.write(reg);
   // Write the data byte
   Wire.write(data);
   // End the write sequence; bytes are actually transmitted now
   Wire.endTransmission();
}

  
  
  
  





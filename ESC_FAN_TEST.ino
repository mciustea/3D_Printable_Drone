/* Mihai Ciustea
 * 26-June-2015
 * Proiect Avion
 * 
 * This is the program on the Arduino Mega, controlling the aircraft.
 * It takes care of radio communication with the ground station
 * , receiving information from the Processing sketch on that station.
 * This also takes care of controll of the aircraft's controll surfaces and
 * ESCs (connected to the engines).
 */

#include <Servo.h>

// Pin setup 
const int ESC_PIN1 = 7;
const int ESC_PIN2 = 5;
const int LEFT_AILERON = 2;
const int RIGHT_AILERON = 4;
const int FRONT_WHEEL = 9;
const int ELEVATOR = 10;
const int RUDDER = 11;

// ESC boot signals
const int MAX_SIGNAL = 2000; // may not need these
const int MIN_SIGNAL = 700;

boolean bootedEngines; // disables throttles, 
// so no signals are sent to the engines

// ESCs
Servo ESC2;
Servo ESC1;

// Experimental delay to read from buffer, its fine without on this end
//since its only receiving, not transmitting

// Declare control variables, & trim
int joystickX, joystickY, joystickZ, joystickZWheel, joystickT, trimFlag, trimX, trimY, trimZ;
const int TRIM_RANGE = 20;
// Throttles?
int throttle = 0;
int thrust = 0;

// Declare Servos 
Servo frontWheel;
Servo aileronLeft;
Servo aileronRight;
Servo elevator;
Servo rudder;

// Transmission constants
const char HEADER   = 'H';
const char A_TAG    = 'M';
const char B_TAG    = 'X'; // Not sure why 2 tags
const int  TOTAL_BYTES  = 10  ; // the total bytes in a message
const char STARTER = '$'; // not sure where this goes


void setup(){
  
  Serial.begin(9600); //console
  Serial1.begin(57600); //radio
  
  // No trim in the beginning
  // Save trim settings in the future?
  trimX = 0;
  trimY = 0;
  trimZ = 0;
  
  // Initialize servos & ESCs
  ESC1.attach(ESC_PIN1);
  ESC2.attach(ESC_PIN2);
  aileronLeft.attach(LEFT_AILERON);
  aileronRight.attach(RIGHT_AILERON);
  elevator.attach(ELEVATOR);
  rudder.attach(RUDDER);
  frontWheel.attach(FRONT_WHEEL);
  
  // Boot up ESCs, currently not working as intended
  Serial.println("Boot up the engines? 1 for Yes, 2 for No");
  // Wait for response
  while(!Serial.available()){
    Serial.read();
  }  
  int response = Serial.parseInt();
  bootedEngines = (response == 1);
  if (bootedEngines) {
    //change messages!
    
    Serial.println("Booting up ESCs !");
    Serial.println("=== Ensure throttle is turned off ! ! ! ===");
    Serial.println("==={ Push throttle to maximum forward position ! }===");
    //delay(2000);
    
    Serial.println("=== Warning! Engines may start! ===");
    Serial.println("Turn on main power !");
    Serial.println("10s till start ");
    
   // ESC1.writeMicroseconds(MAX_SIGNAL);
   // ESC2.writeMicroseconds(MAX_SIGNAL);
    delay(10000);
    //Serial.println("Writing min");
    //Serial.println("ESC Booted");
    //ESC1.writeMicroseconds(MIN_SIGNAL);
    //ESC2.writeMicroseconds(MIN_SIGNAL);
    Serial.println("=== ESCs booted! ===");
    Serial.println("=== Throttle && Engines ENABLED! ===");
    Serial.println("=== Servos && Radio Communication ENABLED! ===");
    Serial.println("=== Warning! Engines may start! ===");
    Serial.println("=== Aircraft Operational! ===");
  } else {
    Serial.println("=== Turn on main power first! ===");
    Serial.println("=== Throttle && Engines DISABLED! ===");
    Serial.println("=== Servos && Radio Communication ENABLED! ===");
    Serial.println("=== Aircraft Operational! ===");
  }
  // Start radio communication
  Serial1.write(STARTER); // not sure if necessary    
}  

void loop() { 
  //Serial.println("Now in loop");
  //throttle= Serial1.read();
  if (Serial1.peek() != -1) {
    readSerial();
    setTrim();
    writeControls();
  }
    //writeControls();
}   
  
  

  //delay(10);


void writeControls(){
  if ( (thrust >= 700) && (thrust <= 2000) && (joystickX != 0) 
                          && (joystickY != 0) && (joystickZ != 0) ) {
    //String output = "Thrust is " + String(thrust);
    //Serial.print("Thrust is ");
    //Serial.println(thrust);
    if(bootedEngines){
      ESC1.writeMicroseconds(thrust);
      ESC2.writeMicroseconds(thrust);
    }
    aileronLeft.write(joystickX + trimX);
    aileronRight.write(joystickX + trimX);
    elevator.write(joystickY + trimY);
    rudder.write(joystickZ + trimZ);
    int wheelZ = map(joystickZ + trimZ, 20, 160, 140, 40);
    frontWheel.write(wheelZ);
  }  
}
void setTrim(){
  if (trimFlag == 5) {
    //trimX = joystickX - 90;
    trimX = checkTrimRange(joystickX - 90);
    //trimY = joystickY - 90;
    trimY = checkTrimRange(joystickY - 90);
    //trimZ = joystickZ - 90;
    trimZ = checkTrimRange(joystickZ - 90);
  } else if (trimFlag == 10) {
    trimX = 0;
    trimY = 0;
    trimZ = 0;
  }
}

int checkTrimRange(int trimValue){
  if (trimValue > TRIM_RANGE) {
    return TRIM_RANGE;
  } else if (trimValue < -TRIM_RANGE) {
    return -TRIM_RANGE;
  } else {
    return trimValue;
  }
}

// Takes care of radio communication, reads 10 byte packets
// from the 3DR radio link sent from the ground station's
// Processing sketch.
void readSerial(){
  if (Serial1.available() >= TOTAL_BYTES) {      
     if (Serial1.read() == HEADER) { 
      char tag = Serial1.read(); 
      if (tag == A_TAG) {    
        //Collect integers         
        int a = Serial1.read(); 
        joystickX = a;  
        int b = Serial1.read();
        joystickY = b; 
        int c = Serial1.read(); 
        joystickZ = c;
        int d = Serial1.read() * 256;
        d = d + Serial1.read();
        thrust = d; 
        int e = Serial1.read();
        trimFlag = e;
        Serial1.read();
        Serial1.read();

        //Print radio input
        Serial.print("Received integers | a:");
        Serial.print(a + trimX);
        Serial.print(", b:");
        Serial.print(b + trimY);
        Serial.print(", c:");
        Serial.print(c + trimZ);
        Serial.print(", d:");       
        Serial.print(d);
        Serial.print(", e:");
        Serial.println(e);
      }
      else {
        Serial.print("got message with unknown tag ");
        Serial.write(tag);
      }
    }
  }
}



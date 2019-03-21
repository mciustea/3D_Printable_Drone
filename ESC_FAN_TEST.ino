#include <Servo.h>


int MOTOR_PIN = 9;
int MAX_SIGNAL = 2000;
int MIN_SIGNAL = 700;
Servo motor;
int delayT = 0;

const char HEADER       = 'H';
const char A_TAG    = 'M';
const char B_TAG    = 'X';
const int  TOTAL_BYTES  = 10  ; // the total bytes in a message

const char STARTER = '$';

void setup() {
  
  Serial.begin(9600);
  Serial1.begin(57600); //radio
  motor.attach(MOTOR_PIN);
  motor.attach(MOTOR_PIN);
  
  Serial.println("Boot up the ESC? 1 for Yes, 2 for No");
  while(!Serial.available()){
    Serial.read();
  }  
  int response = Serial.parseInt();
  if(response == 1){
    Serial.println("Booting up ESC !");
    Serial.println("Writing max");
    Serial.println("2s till start ");
    motor.writeMicroseconds(MAX_SIGNAL);
    delay(2000);
    Serial.println("Writing min");
    Serial.println("ESC Booted");
    motor.writeMicroseconds(MIN_SIGNAL);
  } else {
    Serial.println("No ESC Booted");
  }
  Serial1.write(STARTER);
    
}  

int throttle = 0;
int thrust = 0;

void loop() { 
  //Serial.println("Now in loop");
  //throttle= Serial1.read();
  
  if(Serial1.peek() != -1){
    readSerial();
    if((thrust >= 700) && (thrust <= 2000)){
      //String output = "Thrust is " + String(thrust);
      //Serial.print("Thrust is ");
      //Serial.println(thrust);
      motor.writeMicroseconds(thrust);
    }  
  }
  //delay(10);
}

void readSerial(){
if ( Serial1.available() >= TOTAL_BYTES)
  {      
     if( Serial1.read() == HEADER)
    { 
      delay(delayT); 
      char tag = Serial1.read();
      delay(delayT); 
      if(tag == A_TAG)
      {
        //Collect integers
         
        int a = Serial1.read() * 256; 
        a = a + Serial1.read();
        delay(delayT);  
        int b = Serial1.read() * 256;
        delay(delayT); 
        b = b + Serial1.read();
        delay(delayT);  
        int c = Serial1.read() * 256;
        delay(delayT); 
        c = c + Serial1.read();
        delay(delayT);  
        int d = Serial1.read() * 256;
        delay(delayT); 
        d = d + Serial1.read();
        thrust = d; 
        /*
        String rxStr = "Received integers | a:" + String(a) +
           ", b:" + String(b) + ", c:" + String(c) + ", d:" + 
           String(d);  
        */   
        Serial.print("Received integers | a:");
        Serial.print(a);
        Serial.print(", b:");
        Serial.print(b);
        Serial.print(", c:");
        Serial.print(c);
        Serial.print(", d:");       
        Serial.println(d);
      }
      else {
        Serial.print("got message with unknown tag ");
        Serial.write(tag);
      }
    }
  }
}



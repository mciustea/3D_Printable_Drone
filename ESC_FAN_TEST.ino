#include <Servo.h>


int ESC_PIN1 = 7;
int ESC_PIN2 = 5;
int LEFT_AILERON = 2;
int RIGHT_AILERON = 4;
int FRONT_WHEEL = 9;
int ELEVATOR = 10;
int RUDDER = 11;

int MAX_SIGNAL = 2000;
int MIN_SIGNAL = 700;
Servo ESC2;
Servo ESC1;
int delayT = 0;
int X = 0;
int Zw = 0;
int Y = 0;
int Z = 0;
int T = 0;

Servo frontWheel;
Servo aileronL;
Servo aileronR;
Servo Elevator;
Servo Rudder;

const char HEADER       = 'H';
const char A_TAG    = 'M';
const char B_TAG    = 'X';
const int  TOTAL_BYTES  = 10  ; // the total bytes in a message

const char STARTER = '$';

void setup() {
  
  Serial.begin(9600);
  Serial1.begin(57600); //radio
  frontWheel.attach(FRONT_WHEEL);
  ESC1.attach(ESC_PIN1);
  ESC2.attach(ESC_PIN2);
  aileronL.attach(LEFT_AILERON);
  aileronR.attach(RIGHT_AILERON);
  Elevator.attach(ELEVATOR);
  Rudder.attach(RUDDER);
  //ESC1.attach(ESC_PIN1);
  
  Serial.println("Boot up the ESC? 1 for Yes, 2 for No");
  while(!Serial.available()){
    Serial.read();
  }  
  int response = Serial.parseInt();
  if(response == 1){
    Serial.println("Booting up ESC !");
    Serial.println("Writing max");
    Serial.println("2s till start ");
    ESC1.writeMicroseconds(MAX_SIGNAL);
    ESC2.writeMicroseconds(MAX_SIGNAL);
    delay(2000);
    Serial.println("Writing min");
    Serial.println("ESC Booted");
    ESC1.writeMicroseconds(MIN_SIGNAL);
    ESC2.writeMicroseconds(MIN_SIGNAL);
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
    if((thrust >= 700) && (thrust <= 2000) && (X != 0) &&(Y != 0) && (Z != 0)){
      //String output = "Thrust is " + String(thrust);
      //Serial.print("Thrust is ");
      //Serial.println(thrust);
      ESC1.writeMicroseconds(thrust);
      ESC2.writeMicroseconds(thrust);
      aileronL.write(X);
      aileronR.write(X);
      Elevator.write(Y);
      Rudder.write(Z);
      frontWheel.write(Zw);
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
        X = a;
        delay(delayT);  
        int b = Serial1.read() * 256;
        delay(delayT); 
        b = b + Serial1.read();
        Y = b;
        delay(delayT);  
        int c = Serial1.read() * 256;
        delay(delayT); 
        c = c + Serial1.read();
        Z = c;
        delay(delayT);  
        int d = Serial1.read() * 256;
        delay(delayT); 
        d = d + Serial1.read();
        thrust = d; 
        Zw = map(Z, 20, 160, 140, 40);
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



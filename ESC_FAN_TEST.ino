#include <Servo.h>


int MOTOR_PIN = 9;
int MAX_SIGNAL = 2000;
int MIN_SIGNAL = 700;
Servo motor;

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
    Serial.println("Sending min");
    motor.writeMicroseconds(MIN_SIGNAL);
    Serial.println("Enter Thrust from 700 to 2000");
  } else {
    Serial.println("No ESC Booted");
  }
    
}  

int throttle = 0;
int thrust = 0;

void loop() { 
  //Serial.println("Now in loop");
  throttle= Serial1.read();
  if(throttle != -1){
    thrust = map(throttle, 0, 255, 700, 2000);
    Serial.print("Thrust is ");
    Serial.println(thrust);
    motor.writeMicroseconds(thrust);  
  }
  Serial1.flush();
  delay(200);
  
}



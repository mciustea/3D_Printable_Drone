int i = 0;


void setup() {
  Serial.begin(9600);
  Serial1.begin(57600); //legatura radio
}

void loop() {
  
  Serial.println(i);
  delay(500);    
  Serial1.println(i);
  delay(500);
  i++;

}

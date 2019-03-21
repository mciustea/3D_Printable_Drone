// BinaryDataFromProcessing
// These defines must mirror the sending program:
const char HEADER       = 'H';
const char A_TAG    = 'M';
const char B_TAG    = 'X';
const int  TOTAL_BYTES  = 10  ; // the total bytes in a message

void setup()
{
  Serial.begin(9600);
  Serial1.begin(57600);
}

void loop(){
  delay(0);
  if ( Serial1.available() >= TOTAL_BYTES)
  {
     if( Serial1.read() == HEADER)
    {
      char tag = Serial1.read();
      if(tag == A_TAG)
      {
        //Collect integers
        int a = Serial1.read() * 256; 
        a = a + Serial1.read();
        int b = Serial1.read() * 256;
        b = b + Serial1.read();
        int c = Serial1.read() * 256;
        c = c + Serial1.read();
        int d = Serial1.read() * 256;
        d = d + Serial1.read();
        
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


import org.gamecontrolplus.gui.*;
import org.gamecontrolplus.*;
import net.java.games.input.*;
import processing.serial.*;

int delayT = 3;

Serial port;

ControlIO control;
ControlDevice stick;

float pX, pY, pZ, pT;

public static final char HEADER    = 'H';
public static final char A_TAG = 'M';
public static final char B_TAG = 'X';

public void setup(){
  
  port = new Serial(this, "COM9", 57600);
  size(500, 500);
  background(255, 255, 250);
  control = ControlIO.getInstance(this);
  stick = control.getMatchedDevice("test"); 
  if(stick == null){
    println("No joystick found!");
    System.exit(-1);
  }
}

public void draw(){
  //port.write(byte(pT));
  //delay(200);
  getInput();
  background(255, 255, 250);
  ellipse(pX, pY, 20, 20);
  textSize(32);
  fill(0);  
  text("X:", 50, 250);
  text(pX, 75, 250);
  text("Y:", 275, 250);
  text(pY, 300, 250);
  text("Z:", 50, 350);
  text(pZ, 85, 350);
  text("Throttle", 200, 350);
  text(pT, 325, 350);
  fill(150, 150, 150);
  text("Coordinate test: ", 100, 100); 
  sendMessage(A_TAG, int(pX), int(pY), int(pZ), int(pT));
 delay(30); 
}



public void getInput(){
  pX = map(stick.getSlider("X").getValue(), -1, 1, 20, 160);
  pY = map(stick.getSlider("Y").getValue(), -1, 1, 20, 160 );
  pZ = map(stick.getSlider("Z").getValue(), -1, 1, 160, 20);
  pT = map(stick.getSlider("T").getValue(), -1, 1, 700, 2000);
}

void sendMessage(char tag, int a, int b, int c, int d){
  // send the given index and value to the serial port
  port.write(HEADER);
  delay(delayT);
  port.write(tag);
  delay(delayT);
  port.write((char)(a / 256)); // msb
  delay(delayT); 
  port.write(a & 0xff);  //lsb
  text("a & 0xff : ", 150, 300);
  text(280 & 0xff, 300, 300); 
  delay(delayT);
  port.write((char)(b / 256)); // msb
  delay(delayT); 
  port.write(b & 0xff);  //lsb
  delay(delayT);
  port.write((char)(c / 256)); // msb
  delay(delayT); 
  port.write(c & 0xff);  //lsb
  delay(delayT);
  port.write((char)(d / 256)); // msb
  delay(delayT); 
  port.write(d & 0xff);  //lsb
}


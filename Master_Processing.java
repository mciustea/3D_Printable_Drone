import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import org.gamecontrolplus.gui.*; 
import org.gamecontrolplus.*; 
import net.java.games.input.*; 
import processing.serial.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class Master_Processing extends PApplet {

/* Mihai Ciustea
 * 26-June-2015
 * Proiect Avion
 *
 * This program enables the ground station to transmit 
 * controll information (using the gamecontrolplus library)
 * from the joystick to the aircraft.
 * The current link uses a 3DR 100mW transceiver, currently just 
 * transmitting. It uses 10 Byte packets. 
 *
 */








final int DELAY = 3;

Serial port;

ControlIO control;
ControlDevice stick;

float deltaX, deltaY, deltaZ, deltaT, deltaC;
boolean buttonS, buttonR;
int trim;

public static final char HEADER    = 'H';
public static final char A_TAG = 'M';
public static final char B_TAG = 'X';

public void setup(){
  
  //initialize port as the radio serial line
  port = new Serial(this, "COM9", 57600);
  size(750, 750);
  background(255, 255, 250);
  //initialize the joystick controls
  control = ControlIO.getInstance(this);
  stick = control.getMatchedDevice("Joystick"); 
  if(stick == null){
    println("No joystick found!");
    System.exit(-1);
  }
}

// Draw is a loop
public void draw(){
  //port.write(byte(deltaT));
  //delay(200);
  getInput(); // Get joystick input
  //Then set background and print the control info
  background(255, 255, 250);
  ellipse(deltaX, deltaY, 20, 20);
  textSize(32);
  fill(0);  
  text("X:", 50, 250);
  text(deltaX, 75, 250);
  text("Y:", 275, 250);
  text(deltaY, 300, 250);
  text("Z:", 50, 350);
  text(deltaZ, 75, 350);
  text("Throttle:", 225, 350);
  text(deltaT, 350, 350);
  fill(150, 150, 150);
  //text("Coordinate test: ", 100, 100); 
  text("HAT: ", 50, 300);
  text(deltaC, 125, 300); 
  text("S: ", 275, 300); 
  text(str(buttonS), 325, 300); 
  text("R: ", 450, 300);
  text(str(buttonR), 500, 300); 
  text("Trim: ", 600, 300);
  text(this.trim, 690, 300);
  
  //Then send the control info to the serial line
  sendMessage(A_TAG, PApplet.parseInt(deltaX), PApplet.parseInt(deltaY), PApplet.parseInt(deltaZ), PApplet.parseInt(deltaT), this.trim);
 delay(30); 
}


 
  //Gets joystick input from the stick object, 
  //and sets the global variables accordingly 
public void getInput(){
  deltaX = map(stick.getSlider("X").getValue(), -1, 1, 20, 160);
  deltaY = map(stick.getSlider("Y").getValue(), -1, 1, 20, 160 );
  deltaZ = map(stick.getSlider("Z").getValue(), -1, 1, 160, 20); //reversed due to physical setup
  deltaT = map(stick.getSlider("T").getValue(), -1, 1, 700, 2000);
  deltaC = stick.getButton("C").getValue();
  buttonS = stick.getButton("S").pressed();
  buttonR = stick.getButton("R").pressed();
  if (buttonS) {
    this.trim = 5;
  } else if (buttonR) {
    this.trim = 10;
  } else {
    this.trim = 0;
  }
}

public void sendMessage(char tag, int a, int b, int c, int d, int e){
  // send the given index and value to the serial port
  port.write(HEADER);
  delay(DELAY);
  port.write(tag);
  delay(DELAY);
  port.write((char) a); // 1 byte
  delay(DELAY); 
  port.write((char) b);  // 1 byte
  delay(DELAY);
  port.write((char) c); // 1 byte
  delay(DELAY); 
  port.write((char) (d / 256));  //most significant byte
  delay(DELAY);                //(2 bytes)
  port.write(d & 0xff); // least significant byte
  delay(DELAY); 
  port.write(e);  // 1 byte
  delay(DELAY);
  port.write(0); // free byte
  delay(DELAY); 
  port.write(0);  //free byte
}

  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "Master_Processing" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}

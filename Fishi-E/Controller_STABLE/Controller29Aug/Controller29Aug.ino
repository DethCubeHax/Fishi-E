/* 
 *  The controller code from June 1, 2022
 *  Tim: idk how the previous code works
 *  so we make a new one
 * 
 */

//For the screen setting
#include <SPI.h>
#include <Wire.h>
#include <ezButton.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

////////////////////////////
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
////////////////////////////


#define KILL_SW 5
#define HOME_SW 7
#define HOME_LED 10
#define KILL_LED 8

ezButton button(KILL_SW);

bool killed = true;    // keeping my fingers alive


// Timers
long int timer1 = 0;       //store the time when transmiting


// The byte array to store the message to be sent
byte msg[6];


//Variables for transmission
//signal1:R;     signal2:U;     signal3:T;      signal4:P
String sum_str;
char message[6];

String angle;


//Button default values
int xMap, yMap;
int tMap = 5;         //default value of turnVal
int pMap = 0;         //default value of power

//declare Aunduino joystick
int sensorValueX = 0;    
int sensorValueY = 0;




void setup() {
   // Setup Serial
  Serial.begin(19200,SERIAL_8O1); // Hello, you might need to change it later

  Serial.println("y999");


  
//  Serial1.begin(38400,SERIAL_8E1);
//  Serial2.begin(38400,SERIAL_8E1);
//  Serial3.begin(38400,SERIAL_8E1);
//  only enable when mega is used
  
  
  button.setDebounceTime(90); 

  ///////////////////////////////////////////////////////////////

   display.clearDisplay();

  // Draw BREED welcome message
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(SCREEN_WIDTH, 0);
  display.println("Welcome to");
  display.setTextSize(3); // Draw 2X-scale text
  display.println("BREED");
  display.display();

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(1000);
  ////////////////////////////////////////////////////////////

  //Setup Arduino pins
//  pinMode(A0, INPUT);
//  pinMode(A1, INPUT);
//  pinMode(A2, INPUT);
//  pinMode(A3, INPUT);
//  pinMode(A4, INPUT_PULLUP);
//  pinMode(A5, INPUT);
  pinMode(HOME_SW, INPUT_PULLUP);
  pinMode(KILL_SW,INPUT_PULLUP);
  pinMode(HOME_LED,OUTPUT);
  pinMode(KILL_LED,OUTPUT);
}

void loop() {

    button.loop();
    int homing = digitalRead(HOME_SW);

    if (homing == LOW){
      Serial.write("h");
      Serial.flush();
      digitalWrite(HOME_LED, HIGH);
      ////////////////////////////////////////////////////
      display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("HOME NOW");
     display.display();
     ///////////////////////////////////////////////////
      return;
    } else{
      digitalWrite(HOME_LED, LOW);
    }

    if (button.isPressed()){
      killed = !killed;
    }

  
//  time_now = millis();

  //Update control variables from buttons
  //Reading Joystick Value
  sensorValueX = analogRead(A0);
  delay(1);

  sensorValueY = analogRead(A1);
  delay(1);

  //pMap =   map(analogRead(A4), 50, 1010 , 0, 9); 
  /////////////////////////////////////////// using A3 ///////////////////////////////////
  pMap =   map(analogRead(A3), 50, 1010 , 0, 9); 
  ///////////////////////////////////////////////////////////////////////////////////////
  //pMap = 9;
  delay(1);
  pMap = constrain(pMap, 0,9);
 
  tMap =   map(analogRead(A2), 50, 900 , 1, 9);
  delay(1);
  tMap = constrain(tMap, 0,9);

  if (sensorValueX > 400 && sensorValueX < 600){
    xMap = 50;
  } else if (sensorValueX <400 && sensorValueX > 0){
    xMap = map(sensorValueX, 50, 400 , 90, 60);
  } else if (sensorValueX > 600 && sensorValueX < 1000){
    xMap = map(sensorValueX, 600, 1000 , 40, 10);;
  }

  
  if (sensorValueY > 400 && sensorValueY < 600){
    yMap = 45;
  } else if (sensorValueY <400 && sensorValueY > 0){
    yMap = map(sensorValueY, 50, 400 , 10, 40);
  } else if (sensorValueY > 600 && sensorValueY < 1000){
    yMap = map(sensorValueY, 600, 1000 , 60, 90);;
  }

  /////////////////////////////////////////////////////////////////////////////////
  display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    if (killed == false){
      display.print("PWM: ");
      display.println(pMap);
    } else{
      display.print("PWM: ");
      display.print("KILLED !!!");
      display.print("(");
      display.print(pMap);
      display.print(")");
    }
    
    // Turning control Left
    display.print("Turning:");
    int turnVal=tMap;
    if ((turnVal>=1) and (turnVal<=4)){
      display.print("RIGHT => ");
      }
      // Going Straight
      else if ((turnVal>=5) and (turnVal<=5)){
        display.print("STRAIGHT => ");
      }
      // Turning control Right
      else if ((turnVal>=6) and (turnVal<=9)){
        display.print("LEFT => ");
      }
    display.println(tMap);
    display.println("Servo:  ");
    display.print("X= ");
    display.print(xMap);
    display.print(", Y= ");
    display.print(yMap);
    display.println();
    display.print(" angle: ");
    display.println(angle);
    display.display();
    //////////////////////////////////////////////////////////////////
    
    // Send signal after every 50 ms
   // send command
    if (millis() - timer1 > 100) {
      timer1 = millis(); //update timer1
  
      // Prepare string and send to receiver
  
      //speed, pitch, roll, yaw, in that order
      if (yMap < 10){
        yMap = 10;
      }
      if (xMap < 10){
        xMap = 10;
      }
      if (xMap > 90){
        xMap = 90;
      }
      if (yMap > 90){
        yMap = 90;
      }
      
      if (tMap <= 0){
        tMap = 1;
      }
      if (pMap <= 0){
        pMap = 0;
      }
      if (tMap > 9){
        tMap = 9;
      }
      if (pMap > 9){
        pMap = 9;
      }

      int newT = map(tMap, 1,9,9,1);
      
      if (killed == true){
        digitalWrite(KILL_LED, HIGH);
        pMap = 0;
      } else {
        digitalWrite(KILL_LED, LOW);
      }
      sum_str = "c" + String(pMap) + String( (char)yMap) + String(newT) + String( (char) xMap) + "e"; //payload + checksum
      strcpy(message,sum_str.c_str());
      //Serial.println((String) xMap + " " +yMap);
      String(message).getBytes(msg,7);
      //Serial.println((String) yMap + " " + xMap);
      for (int i = 0; i < 6; i++){ 
//        Serial.write(msg[i]);
//        Serial.flush();
  
         //enable if using mega
        
//        Serial1.write(msg[i]);
//        Serial1.flush();
//  
//        Serial2.write(msg[i]);
//        Serial2.flush();
//        
//        Serial3.write(msg[i]);
//        Serial3.flush();
        
      }
      //Serial.println();
     }

  
  
    
    /*
    Serial1.print(message);
    Serial1.flush();
    
    Serial2.print(message);
    Serial2.flush();
    
    Serial3.print(message);
    Serial3.flush();
    */

   
   //This is the get sensor function, not tested yet
//    getSensor();
//    display.print("Water Level: ");
//    display.println(waterLevel);
//    display.print("Tempature ");
//    display.print(tempature);

    

}


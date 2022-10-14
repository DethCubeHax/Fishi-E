/* 
 *  The controller code from June 1, 2022
 *  Tim: idk how the previous code works
 *  so we make a new one
 * 
 */

//For the screen setting
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// Timers
long int timer1 = 0;       //store the time when transmiting

long int bounceDelay = 90;  // delay to avoid bounce on switch, or you call the "ping"

// The byte array to store the message to be sent
byte msg[6];


//Variables for transmission
//signal1:R;     signal2:U;     signal3:T;      signal4:P
String sum_str;
char message[6];


//declare Aunduino buttons
int leftbutton = 5;           
int rightbutton = 3;
int upbutton = 2;
int downbutton = 4;
int keybutton = 8;

//sensor data
int waterLevel = -1;
int tempature = -1;

//Button default values
int xMap, yMap;
int tMap = 5;         //default value of turnVal
int pMap = 0;         //default value of power

//declare Aunduino joystick
int sensorValueX = analogRead(A0);    
int sensorValueY = analogRead(A1);

enum buttons { 
  // The enum representing each control button
   UP, RIGHT, DOWN, LEFT , KEY 
   // up: 0, down: 2, right: 1, left: 3, key : 5
};

typedef struct  { // the struct "btn", to store the info of each button
  
  int idx; //the button type
  int lastState = LOW; // the last button state
  long int debounce ; // time to check the debounce
  int state = HIGH; // current state of the button
  int pin; // the pin number of the button
  
} btn;

btn btns[5]; // the array storing each btn button

void checkButton(int idx){
  
  int reading = digitalRead(btns[idx].pin);

  if ( reading != btns[idx].lastState){
    btns[idx].debounce = millis();
  } 
  if ( (millis() - btns[idx].debounce) > bounceDelay){

    switch(idx){
      
      case LEFT:

        if(btns[idx].state == HIGH && tMap > 1){
          tMap--;
          btns[idx].debounce = millis();
          btns[RIGHT].debounce = btns[idx].debounce;
          //Serial.println("left");
        }
        return;
         
          
      case RIGHT:
      
         if(btns[idx].state == HIGH && tMap < 9){
          tMap++;
          btns[idx].debounce = millis();
          btns[LEFT].debounce = btns[RIGHT].debounce;
          //Serial.println("right");
        }
        return;

      case UP:

        if(btns[idx].state == HIGH && pMap < 9){
            pMap++;
            btns[idx].debounce = millis();
            btns[DOWN].debounce = btns[idx].debounce;
            //Serial.println("up");
          }
         return;

      case DOWN:

        if(btns[idx].state == HIGH && pMap > 0){
          pMap--;
          btns[idx].debounce = millis();
          btns[UP].debounce = btns[idx].debounce;
          //Serial.println("down");
        }
        return;

      case KEY:
      
        if(btns[idx].state == HIGH){
          
          btns[idx].debounce = millis();
          pMap = 0;
          tMap = 5;
          //Serial.println("key");
        }
        return;
       
      default:
        Serial.println("default");
        return;

      
    }

    btns[idx].lastState = reading;
    
  }

  
}



void setup() {
   // Setup Serial
  Serial.begin(19200,SERIAL_8O1); // Hello, you might need to change it later
  //Serial.begin(19200);

  
//  Serial1.begin(19200,SERIAL_8O1);
//  Serial2.begin(19200,SERIAL_8O1);
//  Serial3.begin(19200,SERIAL_8O1);
//  only enable when mega is used
  
  


  for (int i = 0; i < 5; i++){
    btns[i].idx = i;
    btns[i].pin = i + 2; 
  }

  btns[KEY].pin = 8;

  // Setup Screen
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { //Ive changed the address //already chill
    //Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  //  display.display();
  //  delay(2000); // Pause for 2 seconds
  //  // Clear the buffer
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
  delay(100);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...



  

  //Setup Arduino pins
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(leftbutton,INPUT_PULLUP);
  pinMode(rightbutton,INPUT_PULLUP);
  pinMode(upbutton,INPUT_PULLUP);
  pinMode(downbutton,INPUT_PULLUP);
  pinMode(keybutton,INPUT_PULLUP);

}

void loop() {
  
//  time_now = millis();

  //Update control variables from buttons
  //Reading Joystick Value
  sensorValueX = analogRead(A0);
  sensorValueY = analogRead(A1);
  xMap = map(sensorValueX, 50, 900 , 10, 90);
  //yMap = map(sensorValueY, 30, 1000, 1, 9);

  //yMap = map(sensorValueY, 0, 1024, 0, 9);
  yMap = map(sensorValueY, 50, 920, 10, 90);

  // reading the state of each button
  int currentLeft = digitalRead(leftbutton);
  int currentRight = digitalRead(rightbutton);
  int currentUp = digitalRead(upbutton);
  int currentDown = digitalRead(downbutton);
  
  

  if (currentLeft == LOW && currentRight == LOW ){ //home the encoder
      Serial.write("h");
      Serial.flush();
     
     //enable only when mega is used
      //timer1 = millis();
//      Serial1.write("h");
//      Serial1.flush();
//
//      Serial2.write("h");
//      Serial2.flush();
//
//      Serial3.write("h");
//      Serial3.flush();

      
      display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("HOME NOW");
     display.display();

     return;

      
  }

  checkButton(LEFT);
  checkButton(RIGHT);

  if (currentUp == LOW && currentDown == LOW ){ //go full speed

      if (millis() - timer1 > 50) {
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

      int newT = map(tMap, 1,9,9,1);
      sum_str = "c" + String(9) + String( (char)yMap) + String(newT) + String( (char) xMap) + "e"; //payload + checksum
      strcpy(message,sum_str.c_str());
      //Serial.println((String) xMap + " " +yMap);
      String(message).getBytes(msg,7);
      //Serial.println((String) yMap + " " + xMap);
      for (int i = 0; i < 6; i++){ 
        Serial.write(msg[i]);
        Serial.flush();
  
        
//        Serial1.write(msg[i]);
//        Serial1.flush();
//  
//        Serial2.write(msg[i]);
//        Serial2.flush();
//        
//        Serial3.write(msg[i]);
//        Serial3.flush();
        
      }
     }
      
      display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("MAX SPEED");
     display.display();

     return;

      
  }

  //check if the button is pressed
  
  checkButton(UP);
  checkButton(DOWN);
  checkButton(KEY);

  //Update Screen
    display.clearDisplay();
    display.setTextSize(1); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("PWM: ");
    display.println(pMap);
    // Turning control Left
    display.print("Turning:");
    int turnVal=tMap;
    if ((turnVal>=1) and (turnVal<=4)){
      display.print("LEFT => ");
      }
      // Going Straight
      else if ((turnVal>=5) and (turnVal<=5)){
        display.print("STRAIGHT => ");
      }
      // Turning control Right
      else if ((turnVal>=6) and (turnVal<=9)){
        display.print("RIGHT => ");
      }
    display.println(tMap);
    display.println("Servo:  ");
    display.print("X= ");
    display.print(xMap);
    display.print(", Y= ");
    display.print(yMap);
    display.println();
    //display.display();
    
    
    // Send signal after every 50 ms
   // send command
    if (millis() - timer1 > 50) {
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

      int newT = map(tMap, 1,9,9,1);
      sum_str = "c" + String(pMap) + String( (char)yMap) + String(newT) + String( (char) xMap) + "e"; //payload + checksum
      strcpy(message,sum_str.c_str());
      //Serial.println((String) xMap + " " +yMap);
      String(message).getBytes(msg,7);
      //Serial.println((String) yMap + " " + xMap);
      for (int i = 0; i < 6; i++){ 
        Serial.write(msg[i]);
        Serial.flush();
  
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
    display.display();
    

}

void getSensor (){
   // get sensor function
  if (Serial.available() > 0){
    int last_water = waterLevel;
    int last_temp = tempature;
    for (int i = 0; i < 1; i ++){
      waterLevel = Serial.read();
      tempature = Serial.read();
    }
    if (waterLevel < 0)
    {
      waterLevel = last_water;
    }
    if (tempature < 0){
      tempature = last_temp;
    }
    
  }
}

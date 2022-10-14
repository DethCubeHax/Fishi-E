# SNAPP The Robotic Fish

## Overview
Now this is the controller code. This is to tell you what is going on in the controller code.

## <br> What is the command ?
First, the controller will send the command to the fish. The format of the command is below:
```Arduino
c1234e
```
Now let me talk about what each element means...

```c``` : The start of the command

```1``` : The power level, ranges from 0 - 9 (int)

```2``` : The pitch value, ranges from 10 - 90 (ascii value), will be displayed as a ```char``` in the code

```3``` : The yaw value, ranges from 1 - 9, 1 - 4 means turn left, 5 means go straight, 6 - 9 means turn right

```4``` : The roll value, ranges from 10 - 90 (ascii value), will be displayed as a ```char``` in the code

```e``` : The end of the command

### <br> Some Special Command
In the code, you will see a part of ```Serial.write("home")```. This is the so-called "home" command, what it does is that, it will send the command to the fish, such that the fish will reset the encoder position back to 0. The command is:

```
home
```


## <br>Code Breakdown
As the title says, this section will break down every part of the code down the smallest detail.

### <br>**The Variables**
---

The variables are as follows:

- The ```timer1``` variable is to store the last time a command is sent, when it is larger than 30, the command will be sent again. In other words, it let the system to send a command every 30ms. 

  Data Type: long int
  ```c
  long int timer1 = 0;
  ```

- The ```bounceDelay``` variable. This variable means the "break" between the press of each button, such as one press will only change the values by 1, instead of keep increasing, here the value is 90, meaning each button can be pressed every 90ms. 

  Data type: long int.
  ```c
  long int bounceDelay = 90;
  ```
- The ```msg[6]``` array. This store the command ```c1234e``` in an array. Later in the code, the element will be "written" one by one.

  Data type: byte array

  ```c
  byte msg[6];
  ```
- The ```sum_str``` and ```message[6]```. They are the command again, but in a String and in a char array, the command will be first stored as a String, then be converted to an char array, eventually to coverted to a byte array (the ```mes[6]``` above).

  Data type: String (sum_str) and char array (message[6])

  ```c
  String sum_str;
  char message[6];
  ```

- The ```button```s. These are the pin numbers of each button on the control pad, I don't think I need to explain which button they are, right ?
Just one to mark is that the ```keybutton``` is the joystick (YES! You can press the joystick!). 

  Data type: int
  ```c
  //declare Aunduino buttons
  int leftbutton = 5;           
  int rightbutton = 3;
  int upbutton = 2;
  int downbutton = 4;
  int keybutton = 8;
  ```
- ```waterLevel``` and  ```tempature```. Yea... I made a spelling mistake, but you know, they refer to water level and the temperature respectively.

- The ```Map```s. We have 4 Maps, the ```xMap```, ```yMap```, ```tMap``` and ```pMap```. They are the 4 control values, the meaning of each Map is below:

  ```xMap```: The roll value.

  ```yMap```: The pitch value.

  ```tMap```: The yaw value.

  ```pMap```: The power level.

- ```sensorValue```. They are the ```analogRead()``` raw value of the joystick. ```sensorValueX``` will be mapped to xMap, while ```yMap``` will be mapped to yMap.

  Data type: int

    ```c
    //Button default values
    int xMap, yMap;
    int tMap = 5;         //default value of turnVal
    int pMap = 0;         //default value of power

    //declare Aunduino joystick
    int sensorValueX = analogRead(A0);    
    int sensorValueY = analogRead(A1);
    ```

- **```enum buttons{}```**
  
  Since each button has different pin number, it will be a good idea to store they in the same array, and to have a easier understanding of which button we using, we use ```enum``` to make the button to represent a number, such that we can call the button without actually typing the int number.

  ```c
  enum buttons { 
  // The enum representing each control button
   UP, RIGHT, DOWN, LEFT , KEY 
   // up: 0, down: 2, right: 1, left: 3, key : 5
  };
  ```

- **```typedef struct {} btn;```**

  Each button has the same attributes, so it is more convenient to make a ```struct``` and store all the attribute in it. Moreover, make a struct array of buttons. Then we can check every button by simply calling the ```btn[]``` array.

  These are the attributes:

  ```idx```: The position of the button in the array (like it is btns[**1**] ? btns[**2**], etc). 
  
  Date type: int

  ```lastState```: The previous state of the button, if it is pressed or not.

  Data type: int

  ```debounce```: The last time the button is pressed.

  Data type: long int

  ```state```: The current state of the button, whether it is being pressed or not.

  Data type: int

  ```pin```: The pin number of the button on the Arduino board.

  Data type: int

  Then, we create an array to store them all, which is ```btn btns[5]```.

  ```c
  typedef struct  { // the struct "btn", to store the info of each button
  
  int idx; //the button type
  int lastState = LOW; // the last button state
  long int debounce ; // time to check the debounce
  int state = HIGH; // current state of the button
  int pin; // the pin number of the button
  
  } btn;

  btn btns[5]; // the array storing each btn button

  ```

  Now, we can go into the functions...


## <br> **Functions**
---
The following are the functions in the controller code.

### <br> **The ```checkButton(int idx)``` Function**


---

By its name, it of course checks button. You will see it requires a parameter ```ìdx```. This is the button number (The ```idx``` in the ```struct``` above). Eachvalue has different action, to handle different cases, we use the ```switch ... case``` method.

But before we go into, let's have a look the first few lines. 

First, we ```digitalRead()``` the button to see if it is being pressed or not.

```c
int reading = digitalRead(btns[idx].pin);
```
After getting the button state, we check if the press is effective by checking if the last state and the current state is the same, if so, we do nothing. Else we update the press time (The ```debounce```).

```c
if ( reading != btns[idx].lastState){
    btns[idx].debounce = millis();
  }
```
Then, we move on to the ```switch...case``` part. First, we make sure the button is pressed at least 90ms after the previoud press, else we do nothing.

```c
if ( (millis() - btns[idx].debounce) > bounceDelay)
```

If the button press is effective, we do different action base on the button pin.

First, if it is the ```leftbutton```. If the ````state``` if HIGH, meaning it is being pressed. Then we update the tMap value by subtracting 1 (of course only if tMap is larger than 1), then update other attribute of the button (```debounce``` of left button and right button, becuase you cannot press both together or the time interval between the press of the 2 button cannot be too short.)

```c
case LEFT:

        if(btns[idx].state == HIGH && tMap > 1){
          tMap--;
          btns[idx].debounce = millis();
          btns[RIGHT].debounce = btns[idx].debounce;
          //Serial.println("left");
        }
        return;
```

Then, in the case of ```right button```, we do the same thing as ```leftbutton```, but the direction is reversed.

```c
case RIGHT:
      
         if(btns[idx].state == HIGH && tMap < 9){
          tMap++;
          btns[idx].debounce = millis();
          btns[LEFT].debounce = btns[RIGHT].debounce;
          //Serial.println("right");
        }
        return;
```

Then, for ```upbutton```, we check the ```state``` of the button. Next, we add the ```pMap``` by 1 (of course if pMap is smaller than 9), then we update the attributes of ```UP``` and ```DOWN``` button too, the concept is same as the ```LEFT``` and ```RIGHT``` button.

```c
case UP:

        if(btns[idx].state == HIGH && pMap < 9){
            pMap++;
            btns[idx].debounce = millis();
            btns[DOWN].debounce = btns[idx].debounce;
            //Serial.println("up");
          }
         return;
```

Next, for the ```downbutton```. It is same as ```UP``` button, again with reversed direction.

```c
case DOWN:

        if(btns[idx].state == HIGH && pMap > 0){
          pMap--;
          btns[idx].debounce = millis();
          btns[UP].debounce = btns[idx].debounce;
          //Serial.println("down");
        }
        return;
```

Lastly, the ````KEY``` button, which is the joystick. When it is pressed (effective press of course). It "reset" the command, such that the power level is 0 again and the turn value is 5, going straight.

```c
case KEY:
      
        if(btns[idx].state == HIGH){
          
          btns[idx].debounce = millis();
          pMap = 0;
          tMap = 5;
          //Serial.println("key");
        }
        return;
```

The ```default```. It is.. just default. Do nothing but send a message on the serial monitor. Simply for debugging.

```c
default:
        Serial.println("default");
        return;
```
Finally, we update the ```lastState``` of the button.

```c
btns[idx].lastState = reading;
```
### <br> **The ```setup()``` Function**
---

With no doubt, we start with setting up to serial channle baud rate.

FYI, we might use UNO or MEGA for the controller, based on the board type, we might need to set up other serial channel as well.

```c
  Serial.begin(19200,SERIAL_8O1); // Hello, you might need to change it later
  //Serial.begin(19200);

  
//  Serial1.begin(19200,SERIAL_8O1);
//  Serial2.begin(19200,SERIAL_8O1);
//  Serial3.begin(19200,SERIAL_8O1);
//  only enable when mega is used
```

Next, we use a for-loop to set the ```idx``` and ```pin``` in ```btns[]```. One to remark is that the pin number of ```KEY``` is 8, so we have one more line to correct that.

```c
 for (int i = 0; i < 5; i++){
    btns[i].idx = i;
    btns[i].pin = i + 2; 
  }

  btns[KEY].pin = 8;
```

Then, we set up the screen of the controller.

```c
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
```

Finally, we set the mode of each button pin, they are ``` INPUT_PULLUP``` for button digital pins, and ```INPUT``` for analog pins.

```c
 //Setup Arduino pins
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(leftbutton,INPUT_PULLUP);
  pinMode(rightbutton,INPUT_PULLUP);
  pinMode(upbutton,INPUT_PULLUP);
  pinMode(downbutton,INPUT_PULLUP);
  pinMode(keybutton,INPUT_PULLUP);
```

Cool, we have set up the system.

Now let's go to the loop.

### **The ```loop()``` Functions**
---

First, we get the value of ```sensorValueX``` and ```sensorValueY```, then map them to ```xMap``` and ```yMap``` respectively. FYI, ```analogRead()``` return value between 0 and 1023, but the mapping range is smaller than that, becuase the ```analogRead()``` would be too sensitive.

```c
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
```

Then we read the ```state``` of each button.

```c
int currentLeft = digitalRead(leftbutton);
  int currentRight = digitalRead(rightbutton);
  int currentUp = digitalRead(upbutton);
  int currentDown = digitalRead(downbutton);
```

#### **NOW, we have some "functions" in this function**

Do you recall the "home" command we have mentioned ? (No? Go back and read again.)

So we can "home" the encoder by pressing the ```leftbutton``` and the ```rightbutton``` at the same time.

If they are pressed at the same time, we ```Serial.write()``` the ```"h"``` to the fish. Also, display the ```HOME NOW``` message on the screen, and return.

```c
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
```
Then, we move on to ```checkButton``` of ```LEFT``` and ```RIGHT```.

```c
  checkButton(LEFT);
  checkButton(RIGHT);
```

<br>
After that, we have another function, which allows the fish to go full speed regardless of the actual ```pMap``` value. It is the ```max.speed``` mode. It will be triggered by pressing the ```UP``` and ```DOWN``` button at the same time. And display the ```MAX SPEED``` message on the screen.

```c
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
```

<br>

You might be a bit confused. What is the code doing ? What is happening to the ```message``` variable ?. Let me explain it.

First, we gather the ```Map```s together, and form a string ```sum_str```.

```c
sum_str = "c" + String(9) + String( (char)yMap) + String(newT) + String( (char) xMap) + "e"; 
```

Then, we convert the string to the char array ```message[6]```.

```c
strcpy(message,sum_str.c_str());
```

After that, we convert the char array to byte array ```msg[6]```.

```c
String(message).getBytes(msg,7);
```

Finally, we ```Serial.write``` the byte array ```msg[6]``` element by element using a for-loop.

```c
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
```

_Alright, explanation over, let's go back to the code._

<br>

After the ```max.speed``` mode, we check the remaining button and update the screen

```c
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
```

Next, we ```Serial.write()``` the command, IF AND ONLY IF timer1 is larger than 50ms (check what timer1 means in the "variable" part). Basically, the if-statement makes the system to send the commandd evrey 50ms.

Moreover, in case things go wrong, we will do a final check of ```xMap``` and ```yMap``` to make sure they are within the proper range (10 - 90).

```c
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
```

The send command code:

```c
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
```

Finally, we ```display``` the information on the screen.

```c
display.display();
```

That's it for the ```loop()``` function.

### <br> **The ```getSensor()``` Function**
---

*Remarks: This function is to receive the sensor data from the fish, which is still under development stage and not tested yet (at least before 18 July 2022). I would explain the code, but it is possible that the code will be different in the future.*

So, to get sensor value, we receive the message for the Serial channel using ```Serial.read()```, but first, we have to make sure that there is incoming data using ```Serial.available()```, if it is greater than 0, then there is incoming data, meaning we can start reading them.

First, we store the current value of ```waterLevel``` and ```tempature``` in ```last_water``` and ```last_temp``` respectively.

```c
int last_water = waterLevel;
int last_temp = tempature;
```

Next, we read the data and store them to the 2 variables.

```c
for (int i = 0; i < 1; i ++){
  waterLevel = Serial.read();
  tempature = Serial.read();
}
```

Lastly, we check if the received data is in a proper range (like it is not negative). If so, we do nothing, else we will use the previous value, which is stored in ```last_water``` and ```last_temp```.

```c
if (waterLevel < 0)
{
  waterLevel = last_water;
}
    
if (tempature < 0){
  tempature = last_temp;
}
```

That's all for the ```getSensor()``` function.

<br>

---

<br>

Great, you have read the entire README of the controller code, now you could master the code of the controller, if you have any quesions about the code. Please read through the entire README again. If that doesn't help, witch hunt Oscar.

<br>

## Date: 18 July 2022
## Author: Oscar (that's why you should witch hunt him)

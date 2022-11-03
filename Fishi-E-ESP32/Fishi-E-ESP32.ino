///////////////////////////////////////////////// Libraries //////////////////////////////////////////////////////////
#include "Fins.h"
#include "Motor.h"

#define debug 0

Fins fins;
Motor motor;


///////////////////////////////////////////////// VARIABLES /////////////////////////////////////////////////////////
int speedVal = 0;
int pitchVal = 50;
int rollVal = 50;
int yawVal = 5;

int incomingByte[4] = {0};
float motor_Pwm = 0;
int turnVal = 5;
long int killTimer;

int counter = 0;


/////////////////////////////////////////// END VARIABLES ///////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////    DRIVER CODE    ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void getData();

/////////////////////////////////////////// Begin Setup() Function /////////////////////////////////////////////////

void setup() 
{
  //Initialize //Serial
  Serial.begin(19200,SERIAL_8O1);

  #if debug
  Serial.println("Arming........");   // just some display message 
  #endif
        
  fins.initFin();
  motor.initMotor();

  //Initialize kill timer
  killTimer = millis();
  
  // Slight delay to allow components to come to their normal state.
  delay(100);

  #if debug
  Serial.println("Setup is complete!");
  #endif
}


///////////////////////////////////////// End Setup() Function //////////////////////////////////////////////////////////



//////////////////////////////////////// Begin Loop() Function //////////////////////////////////////////////////////////

void loop() 
{
  killswitch();
  getData();

  #if debug
  //Serial.print(Serial2.read());
  #endif

  enc_pos = motor.encoderPosition();
  //Serial.println(enc_pos);
  //Serial.println(pos_Main);

  //Serial.println(enc_pos);
  turnVal = yawVal;

  //Send PWM signal to motor
  motor_Pwm = (speedVal*255)/9;
  
  //Servo control expression. Linear combination of X and Y component of JoyStick // This part need to ask Tim
  fins.finControl(rollVal, pitchVal);

  int throttle = motor.yaw_turn(motor_Pwm, turnVal, speedVal);
  //if (turnVal > 5) Serial.println((String) "Turning RIGHT: " + enc_pos);
  //if (turnVal < 5) Serial.println((String) "Turning LEFT: " + enc_pos);
  motor.driveMotor(throttle);

  //delay(1500);

 Serial.println(enc_pos);
}

//////////////////////////////////////// End Loop() Function /////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////   END DRIVER CODE    ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////    COMMUNICATION AND CRITICAL FUNCTIONALITY    ////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void killswitch()
{
  if (millis() - killTimer > 3000) 
  {
    speedVal=0;
    motor_Pwm = 0;
    pusherESC.write(1500);
  }
}



// data update from controller
void getData()
{                  
// updates the speed, pitch, roll and yaw command 
// cmd signal would be : c speed(1) pitch(1) roll(1) yaw (1) e 
// the buffer would be of length 6.
  if(Serial.available()>=6)
  {
    int x = Serial.read();

    if (x == 'h')   // Home command
    {
      motor.resetMotor();   // Reset the encoder to 0
    }
    
    else if(x == 'c')    // Elif, just in case some wild signal pops in
    {
      for (int i = 0; i < 4; i++)
      {
        incomingByte[i] = Serial.read();
      }
        
        speedVal  = incomingByte[0] -'0';

        pitchVal  = incomingByte[1];

        yawVal  = incomingByte[2] -'0';

        rollVal  = incomingByte[3];
      killTimer = millis();  
    }
  }
    //Serial.print(speedVal);
    //Serial.print((String) + speedVal + " " + pitchVal + " " + yawVal + " " + rollVal);
    //Serial.println("");
    //#endif
}

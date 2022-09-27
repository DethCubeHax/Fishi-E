////////////////////////////////////
///////////// INCLUDES /////////////
////////////////////////////////////

#include "Fins.h"
#include "Motor.h"
#include "Communications.h"

////////////////////////////////////
//////////////  END  ///////////////
////////////////////////////////////



//--------------------------------//
//--------------------------------//



////////////////////////////////////
//////////// VARIABLES /////////////
////////////////////////////////////


// Communications.h
int speedVal = 0;
int pitchVal = 50;
int rollVal = 50;
int yawVal = 0;

int incomingByte[5] = {0};
float motorPWM = 0;
long int killTimer;
bool setHome = false;
bool yawOnFastStroke = false;
// End


// Motor.h
int PIN_PUSHERESC = 5;
int THROTTLE_MIN = 1500;
int THROTTLE_MAX = 2000;

const int encRes = 4;
const int gearRatio = 49;
const int countsPerTick = 125;
float encPos = 0;
float diff = 0.2;
long int countsPerRevolution;
//End


// Fins.h
int servoPin1 = 9;
int servoPin2 = 11;

float s1, s2;
float maxAttackAngle = 50;

int servoOneCalibration = 0;
int servoTwoCalibration = 0;
// End

////////////////////////////////////
/////////////// END ////////////////
////////////////////////////////////



//--------------------------------//
//--------------------------------//



////////////////////////////////////
///////// HEADER CONSTRUCTS ////////
////////////////////////////////////

Communications comms;
Motor motor;
Fins fins;

////////////////////////////////////
/////////////// END ////////////////
////////////////////////////////////



//--------------------------------//
//--------------------------------//




////////////////////////////////////
/////////// DRIVER CODE ////////////
////////////////////////////////////

void setup()
{
    comms.initComms();
    fins.initFins();
    motor.initMotor();
    delay(100);
}

void loop()
{
    comms.getData();
    fins.finControl();
    motor.driveMotor();
}

////////////////////////////////////
/////////////// END ////////////////
////////////////////////////////////




//--------------------------------//
//--------------------------------//

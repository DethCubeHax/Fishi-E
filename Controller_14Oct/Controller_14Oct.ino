#include "Inputs.h"
#include "Communications.h"


int KILL_SW = 5;
int HOME_SW = 7;
int HOME_LED = 10;
int KILL_LED = 8;

bool killed = true;    // keeping my fingers alive

// Timers
long int timer1 = 0;       //store the time when transmiting

//Button default values
int xMap = 50;
int yMap = 50;
int tMap = 5;         //default value of turnVal
int pMap = 0;         //default value of power

//declare Aunduino joystick
int sensorValueX = 0;    
int sensorValueY = 0;


Communications comms;
Inputs inputs;


void setup() 
{ 
    comms.initComms();
    inputs.initInput();
}

void loop() 
{
    inputs.getInput();
    comms.transmit();
}

#include <Servo.h>
#include "HardwareQDEC.h"

extern int PIN_PUSHERESC, THROTTLE_MAX, THROTTLE_MIN;

Servo pusherESC;
QDEC qdec;

extern const int encRes, gearRatio, countsPerTick;
extern float encPos, diff;
extern long int countsPerRevolution;
extern int speedVal, pitchVal, rollVal, yawVal;
extern float motorPWM;
extern bool setHome, yawOnFastStroke;

class Motor
{
    public:
    void initMotor()
    {
        pusherESC.attach(PIN_PUSHERESC);
        pusherESC.writeMicroseconds(THROTTLE_MIN);
        delayMicroseconds(3000);
        countsPerRevolution = static_cast<long>(countsPerTick) * static_cast<long>(gearRatio*encRes);
        qdec.initQDEC();    // initialise the QDEC
    }
    
    void driveMotor()
    {
        encPos = encoderPosition();
        int throttle = 0;
        
        if (yawOnFastStroke == true)
        {
          if ((yawVal > 0) && (encPos > 44.0) && (encPos < 226.0))
          {
            yawOnFastStroke = false;
          }
          if ((yawVal < 0) && (( encPos > 225.0 && encPos < 361.0) || (encPos >= 0.0 && encPos < 45.0)))
          {
            yawOnFastStroke = false;
          }
          throttle = yaw_turn(motorPWM, 0, speedVal);
        }
        else
        {
          throttle = yaw_turn(motorPWM, yawVal, speedVal);
        }
        throttle = map(throttle, 0, 255, THROTTLE_MIN, THROTTLE_MAX);
        pusherESC.writeMicroseconds(throttle);
    }


    private:
    int yaw_turn(int pwm, int turn, int speedVal)
    {
        //Accepts a pwm signal and outputs a pwm signal from 0-255
        if(turn == 0)
        {
            #if debug
            Serial.print("Normal PWM: ");
            Serial.println(pwm);
            Serial.flush();
            #endif
            return (pwm);
        }
  
        if( (encPos > 44.0) && (encPos < 226.0))
        {
            float x = (1 + turn*diff)*pwm;
            if (x > 255) x = 255;
            if (x < 30 and speedVal != 0) x = 30;
            if (speedVal == 0) x = 0;
    
            #if debug
            Serial.print(" First half PWM: ");
            Serial.println(x);
            #endif
            return round(x);
        }
  
        if ( ( encPos > 225.0 && encPos < 361.0) || (encPos >= 0.0 && encPos < 45.0))
        {
            float x = (1 - turn*diff)*pwm; 
            if (x > 255) x = 255;
            if (x < 30 and speedVal != 0) x = 30;
            if (speedVal == 0) x = 0;
    
            #if debug
            Serial.print("Second Half PWM: ");
            Serial.println(x);
            #endif
            return round(x);
        }
    }

    float encoderPosition()
    {
        if (setHome = true)
        {
            qdec.encReset();
            setHome = false;
        }

        // takes the current count and updates it to an angle from 0 degrees - 360 degress
        long qdecCount = qdec.encRead();
        long x = qdecCount - floor(static_cast<float>(qdecCount) / static_cast<float>(countsPerRevolution)) * countsPerRevolution;   // new_fin%count_per_revolution 
  
        if (x == 0)
        {
            return qdecCount*1.0/countsPerRevolution*360;
        }
        return x*1.0/countsPerRevolution*360;
    }
};

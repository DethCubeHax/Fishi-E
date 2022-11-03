#include <ESP32Servo.h>
//#include "HardwareQDEC.h"

#define PIN_PUSHERESC   5

#define THROTTLE_MIN        1450
#define THROTTLE_MAX        2000

Servo pusherESC;
//QDEC qdec;                              // Initialise Due's Hardware QDEC

const int encoder_resolution = 4;
const int gearbox = 49;
const int cpt = 125;
float enc_pos = 0;
float diff = 0.2;
long int count_per_revolution;

class Motor
{
    public:
    float encoderPosition()
    {
        // takes the current count and updates it to an angle from 0 degrees - 360 degress
        // long new_fin = 0；
        // long x = new_fin - floor(static_cast<float>(new_fin) / static_cast<float>(count_per_revolution)) * count_per_revolution;   // new_fin%count_per_revolution 
        // //Serial.println(x*1.0/count_per_revolution*360);
        // if (x == 0)
        // {
        //     return new_fin*1.0/count_per_revolution*360;
        // }
        // return x*1.0/count_per_revolution*360;
        return 0;
    }
    int yaw_turn(int pwm, int turn, int speedVal)
    {
        //Map the turn value to +/- 4, which is easier to understand
        turn -= 5;
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
  
        if( (enc_pos > 44.0) && (enc_pos < 226.0))
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
  
        if ( ( enc_pos > 225.0 && enc_pos < 361.0) || (enc_pos >= 0.0 && enc_pos < 45.0))
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
    void resetMotor()
    {
        //qdec.encReset();
    }
    void initMotor()
    {
        pusherESC.attach(PIN_PUSHERESC);
        pusherESC.writeMicroseconds(THROTTLE_MIN);
        delayMicroseconds(3000);
        count_per_revolution = static_cast<long>(cpt) * static_cast<long>(gearbox*encoder_resolution);
        //qdec.initQDEC();    // initialise the QDEC
    }
    
    void driveMotor(int throttle)
    {
        throttle = map(throttle, 0, 255, THROTTLE_MIN, THROTTLE_MAX);
        pusherESC.writeMicroseconds(throttle);
    }
};

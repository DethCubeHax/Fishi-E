#include <ESP32Servo.h>

Servo servo1;
Servo servo2;

int servoPin1 = 9;
int servoPin2 = 11;

float s1, s2;
float maxAttackAngle = 50;

//int servo_one_tune = 11;
//int servo_two_tune = -26;

int servo_one_tune = 0;
int servo_two_tune = 4;

class Fins
{
    public:

    void initFin()
    {
        pinMode(servoPin1, OUTPUT);
        pinMode(servoPin2, OUTPUT);
        servo1.attach(servoPin1);
        servo2.attach(servoPin2);
        servo1.write(90);
        servo2.write(90);
    }

    void finControl(int rollVal, int pitchVal)
    {
//        s1 = (((43-rollVal)*maxAttackAngle/40 +(42-pitchVal)*maxAttackAngle/40) + 90.0);
//        s2 = (((43-rollVal)*maxAttackAngle/40 -(42-pitchVal)*maxAttackAngle/40) + 90.0);
//        s1 = constrain(s1, 0, 180);
//        s2 = constrain(s2, 0, 180);
//        servo1.write(s1);
//        servo2.write(s2);
//        #if debug
//        Serial.println((String) "S1:" + s1);
//        Serial.println((String) "S2:" + s2);
//        #endif

        //For servo used, the range of writeMicroseconds are different
        s1 = (((50-rollVal) +(50-pitchVal)*maxAttackAngle/40) + 90.0) + servo_one_tune;
        s2 = (((50-rollVal) -(50-pitchVal)*maxAttackAngle/40) + 90.0) + servo_two_tune;
//        s1 = constrain(s1, 25, 155);
//        s2 = constrain(s2, 25, 155);
        // The range of DS3120: 500 - 2500 (red 20kg servo)
        s1 = map(s1, 0, 180, 500, 2500); 
        s2 = map(s2, 0, 180, 500, 2500);
        s1 = constrain(s1, 500, 2500);
        s2 = constrain(s2, 500, 2500);

//         servo1.write(s1);
//         servo2.write(s2);
        
//        int test = 0;
//        servo1.write(test);
//        servo2.write(90);
        servo1.writeMicroseconds(s1);
        servo2.writeMicroseconds(s2);
        
        //Serial.println((String) "S1: " + s1 + " S2: " + s2 + " Roll: " + rollVal + " pitch: " + pitchVal);
    }
};

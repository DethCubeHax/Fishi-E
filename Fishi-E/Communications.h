extern int yawVal, pitchVal, rollVal, speedVal;
extern float motorPWM;
extern long int killTimer;
extern bool setHome, yawOnFastStroke;
extern int incomingByte[5];

class Communications
{
    public:
    void initComms()
    {
        Serial.begin(115200, SERIAL_8O1);
        Serial2.begin(19200, SERIAL_8O1);
    }

    void getData()
    {
        if (Serial2.available()>=6)
        {
            int x = Serial2.read();

            if (x == 'h')
            {
                setHome = true;
            }

            else if (x == 'c')
            {
                for (int i = 0; i < 5; i++)
                {
                    incomingByte[i] = Serial2.read();
                }

                if (incomingByte[4] == 'e')
                {
                    speedVal = incomingByte[0] - '0';
                    pitchVal = incomingByte[1];
                    int t = incomingByte[2] - '0';
                    if (t != yawVal)
                    {
                        yawVal = t;
                        yawOnFastStroke = true;
                    }
                    
                    rollVal = incomingByte[3];
                    
                    killTimer = millis();
                    standardizeData();
                }
            }
        }

        killSwitch();
    }

    private:
    void killSwitch()
    {
        if (millis() - killTimer > 3000) 
        {
            speedVal = 0;
            motorPWM = 0;
            pusherESC.write(1500);
        }
    }

    void standardizeData()
    {
        pitchVal = constrain(pitchVal, 10, 90);
        speedVal = constrain(speedVal, 0, 9);
        yawVal = constrain(yawVal, 1, 9);
        rollVal = constrain(rollVal, 10, 90);
        
        yawVal -= 5;
    }
};

extern int yawVal, pitchVal, rollVal, speedVal;
extern float motorPWM;
extern long int killTimer;
extern bool setHome;

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
                    yawVal = incomingByte[2] - '0';
                    rollVal = incomingByte[3];
                    
                    killTimer = millis();
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
};
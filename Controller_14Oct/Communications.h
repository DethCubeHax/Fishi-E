extern int xMap, yMap, tMap, pMap;
// The byte array to store the message to be sent
byte msg[6];
//Variables for transmission
//signal1:R;     signal2:U;     signal3:T;      signal4:P
String sum_str;
char message[6];

class Communications
{
public:
    void initComms()
    {
        Serial.begin(19200,SERIAL_8O1); //Setup the Serial channel for RC communication

        //  Serial1.begin(38400,SERIAL_8E1);
        //  Serial2.begin(38400,SERIAL_8E1);
        //  Serial3.begin(38400,SERIAL_8E1);
        //  only enable when mega is used
    }

    void transmit()
    {
        createTransmissionString();
        for (int i = 0; i < 6; i++)
        {
            Serial.write(msg[i]);
        }
    }

private:
    void createTransmissionString()
    {
        sum_str = "c" + String(pMap) + String( (char)yMap) + String(tMap) + String( (char) xMap) + "e"; //payload + checksum
        strcpy(message,sum_str.c_str());
        //Serial.println((String) xMap + " " +yMap);
        String(message).getBytes(msg,7);
    }
};

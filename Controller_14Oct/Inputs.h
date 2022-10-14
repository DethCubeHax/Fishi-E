#include <ezButton.h>
extern int KILL_SW, HOME_SW, KILL_LED, HOME_LED;
extern int sensorValueX, sensorValueY, pMap, tMap, xMap, yMap;
extern long int timer1;
extern bool killed;


ezButton button(KILL_SW);

class Inputs
{
public:
    void initInput()
    {
        button.setDebounceTime(90); 
        pinMode(HOME_SW, INPUT_PULLUP);
        pinMode(KILL_SW,INPUT_PULLUP);
        pinMode(HOME_LED,OUTPUT);
        pinMode(KILL_LED,OUTPUT);
    }
    
    void getInput()
    {
        button.loop();
        int homing = digitalRead(HOME_SW);

        if (homing == LOW){
        Serial.write("h");
        Serial.flush();
        digitalWrite(HOME_LED, HIGH);
        return;
        } 
        else
        {
            digitalWrite(HOME_LED, LOW);
        }

        if (button.isPressed())
        {
            killed = !killed;
        }

        sensorValueX = analogRead(A0);
        delay(1);
        sensorValueY = analogRead(A1);
        delay(1);
        pMap = analogRead(A3);
        delay(1); 
        tMap = analogRead(A2);
        delay(1);

        if (killed == true)
        {
            digitalWrite(KILL_LED, HIGH);
            pMap = 0;
        } 
        else 
        {
            digitalWrite(KILL_LED, LOW);
        }
    }


private:
    void standardizeData()
    {
        pMap = map(pMap, 0, 1023, 0, 9);
        pMap = constrain(pMap, 0,9);

        if (sensorValueX > 400 && sensorValueX < 600)
        {
            xMap = 50;
        } 
        else if (sensorValueX <400 && sensorValueX > 0)
        {
            xMap = map(sensorValueX, 50, 400 , 90, 60);
        } 
        else if (sensorValueX > 600 && sensorValueX < 1000)
        {
            xMap = map(sensorValueX, 600, 1000 , 40, 10);
        }
        xMap = constrain(xMap, 10, 90);

        
        if (sensorValueY > 400 && sensorValueY < 600)
        {
            yMap = 50;
        } 
        else if (sensorValueY <400 && sensorValueY > 0)
        {
            yMap = map(sensorValueY, 50, 400 , 10, 40);
        } 
        else if (sensorValueY > 600 && sensorValueY < 1000)
        {
            yMap = map(sensorValueY, 600, 1000 , 60, 90);
        }
        yMap = constrain(yMap, 10, 90);

        if (millis() - timer1 > 100) 
        {
            timer1 = millis(); //update timer1
    
            tMap = map(tMap, 0, 1023, 9, 1);
            tMap = constrain(tMap, 1, 9);
        }
    }
};

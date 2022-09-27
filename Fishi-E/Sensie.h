// Libraries
#include <DHT.h>

// Variables and definitions
int level = -1;
int temp = -1;

#define DHTPIN 7
#define DHTTYPE DHT22
#define waterLevelPin A2


// Construct the DHT
DHT dht(DHTPIN, DHTTYPE);

// Create a class called Sensie to easily call upon sensor stuff
class Sensie
{
    
    // Create publicly accessible functions to return the waterLevel and stuff
    public:

    int waterLevel()
    {
        level = analogRead(waterLevelPin);
        level = map(level, 0, 1024, 0, 255);
        return int(level);
    }

    int temperature()
    {
        temp = dht.readTemperature();
        return int(temp);
    }
};

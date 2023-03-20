#include <Arduino.h>
#include "MAX30105.h"

#define ARRAY_RATE_SIZE 4

class SensorService
{
    MAX30105 particleSensor;

    byte rates[ARRAY_RATE_SIZE];
    byte rateSpot;
    long lastBeat;
    int beat;

    bool isFirstGetBeat;

public:
    SensorService();
    
    bool IsHeartBeatDetected();
    bool Start();
    int GetHeartBeat();

    void Clear();
};

#include <Arduino.h>
#include "MAX30105.h"

#define BUFFER_SATURATION_SIZE 100

class SensorService
{
    MAX30105 particleSensor;

    long lastBeat;
    int beat;

    uint32_t irBuffer[BUFFER_SATURATION_SIZE];
    uint32_t redBuffer[BUFFER_SATURATION_SIZE];
    int32_t spo2;
    int8_t validSPO2;
    int32_t heartRate;
    int8_t validHeartRate;

public:
    SensorService();

    bool Start();

    bool CanGetLastBeat();
    int GetLastBeat();

    bool CanGetSaturation();
    bool IsSaturationBufferFull();
    int GetSaturation();

    void Clear();
};

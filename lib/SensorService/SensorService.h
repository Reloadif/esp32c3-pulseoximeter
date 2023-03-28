#include <Arduino.h>
#include "MAX30105.h"

#define ARRAY_HEARTRATE_SIZE 4
#define BUFFER_SATURATION_SIZE 100

class SensorService
{
    MAX30105 particleSensor;

    byte rates[ARRAY_HEARTRATE_SIZE];
    byte rateSpot;
    long lastBeat;
    int32_t beat;

    uint32_t irBuffer[BUFFER_SATURATION_SIZE];
    uint32_t redBuffer[BUFFER_SATURATION_SIZE];
    int32_t spo2;
    int8_t validSPO2;
    int32_t heartRate;
    int8_t validHeartRate;

public:
    SensorService();

    bool Start();

    bool CanGetHeartBeat();
    bool IsHeartRateArrayFull();
    int32_t GetHeartBeat();

    bool CanGetSaturation();
    bool IsSaturationBufferFull();
    int32_t GetSaturation();

    void Clear();
};

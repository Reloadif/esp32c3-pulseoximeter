#include <SensorService.h>

#include <Wire.h>
#include "heartRate.h"
#include "spo2_algorithm.h"

SensorService::SensorService() : rateSpot(0), lastBeat(0), beat(0), spo2(0), validSPO2(0)
{
    for (byte i = 0; i < ARRAY_HEARTRATE_SIZE; ++i)
    {
        rates[i] = 0;
    }
    for (byte i = 0; i < BUFFER_SATURATION_SIZE; ++i)
    {
        irBuffer[i] = 0;
        redBuffer[i] = 0;
    }
}

bool SensorService::Start()
{
    if (!particleSensor.begin(Wire, I2C_SPEED_FAST))
    {
        return false;
    }

    particleSensor.setup();
    particleSensor.setPulseAmplitudeRed(0x0A);
    particleSensor.setPulseAmplitudeGreen(0);
    return true;
}

bool SensorService::CanGetHeartBeat()
{
    if (!checkForBeat(particleSensor.getIR()))
        return false;

    long delta = millis() - lastBeat;
    lastBeat = millis();

    long beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute > 20 && beatsPerMinute < 255)
    {
        rates[rateSpot++] = (byte)beatsPerMinute;
        rateSpot %= ARRAY_HEARTRATE_SIZE;

        beat = 0;
        for (int i = 0; i < ARRAY_HEARTRATE_SIZE; ++i)
        {
            beat += rates[i];
        }
        beat /= ARRAY_HEARTRATE_SIZE;
    }

    return true;
}

bool SensorService::IsHeartRateArrayFull()
{
    return rates[ARRAY_HEARTRATE_SIZE - 1] != 0;
}

int32_t SensorService::GetHeartBeat()
{
    return beat;
}

bool SensorService::CanGetSaturation()
{
    for (byte i = 25; i < BUFFER_SATURATION_SIZE; ++i)
    {
        redBuffer[i - 25] = redBuffer[i];
        irBuffer[i - 25] = irBuffer[i];
    }

    for (byte i = 75; i < BUFFER_SATURATION_SIZE; ++i)
    {
        while (!particleSensor.available())
        {
            particleSensor.check();
        }

        redBuffer[i] = particleSensor.getRed();
        irBuffer[i] = particleSensor.getIR();
        particleSensor.nextSample();
    }

    maxim_heart_rate_and_oxygen_saturation(irBuffer, BUFFER_SATURATION_SIZE, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

    return validSPO2;
}

bool SensorService::IsSaturationBufferFull()
{
    return redBuffer[BUFFER_SATURATION_SIZE - 1] != 0 && irBuffer[BUFFER_SATURATION_SIZE - 1] != 0;
}

int32_t SensorService::GetSaturation()
{
    return spo2;
}

void SensorService::Clear()
{
    rateSpot = 0;
    lastBeat = 0;
    beat = 0;
    spo2 = 0;
    validSPO2 = 0;

    for (byte i = 0; i < ARRAY_HEARTRATE_SIZE; ++i)
    {
        rates[i] = 0;
    }
    for (byte i = 0; i < BUFFER_SATURATION_SIZE; ++i)
    {
        irBuffer[i] = 0;
        redBuffer[i] = 0;
    }
}

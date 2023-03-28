#include <Arduino.h>

#include <BLEDevice.h>
#include <POServer.h>
#include <SensorService.h>

#define I2C_SDA 4
#define I2C_SCL 5

SensorService *pSensorService = nullptr;
POServer *pPOServer = nullptr;

bool IsDeviceConnected = false;
bool IsNeedRestart = false;

int MeasurementSelection = 0;

void setup()
{
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);

  pSensorService = new SensorService();
  if (!pSensorService->Start())
  {
    Serial.println("MAX3010X was not found. Please check wiring/power.");
    while (1)
      ;
  }

  pPOServer = new POServer();
  pPOServer->Start();
}

void loop()
{
  if (IsDeviceConnected)
  {
    if (MeasurementSelection == 1)
    {
      if (pSensorService->CanGetHeartBeat() && pSensorService->IsHeartRateArrayFull())
      {
        int hb = pSensorService->GetHeartBeat();
        Serial.println(hb);
        pPOServer->NotifyHeartBeat(hb);
      }
    }
    else if (MeasurementSelection == 2)
    {
      
      if (pSensorService->CanGetSaturation() && pSensorService->IsSaturationBufferFull())
      {
        int ox = pSensorService->GetSaturation();
        Serial.println(ox);
        pPOServer->NotifyOxygenSaturation(ox);
      }
    }
  }
  else
  {
    if (IsNeedRestart)
    {
      pSensorService->Clear();
      pPOServer->Start();
      IsNeedRestart = false;
    }
  }
}

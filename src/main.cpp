#include <Arduino.h>

#include <BLEDevice.h>
#include <POServer.h>
#include <SensorService.h>

#define I2C_SDA 4
#define I2C_SCL 5

SensorService *pSensorService;

POServer *pPOServer = nullptr;
bool IsDeviceConnected;
bool IsNeedRestart;

void setup()
{
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);

  pSensorService = new SensorService();
  if (!pSensorService->Start())
  {
    Serial.println("MAX30102 was not found. Please check wiring/power.");
    while (1)
    {
    }
  }

  pPOServer = new POServer();
  pPOServer->Start();
}

void loop()
{
  if (IsDeviceConnected)
  {
    if (pSensorService->CanGetHeartBeat() && pSensorService->IsBufferFull())
    {
      int hb = pSensorService->GetHeartBeat();
      pPOServer->NotifyHeartBeat(hb);
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

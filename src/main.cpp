#include <Arduino.h>

#include <BLEDevice.h>
#include <POServer.h>
#include <SensorService.h>

SensorService *pSensorService;

POServer *pPOServer = nullptr;
bool IsDeviceConnected;
bool IsNeedRestart;

void setup()
{
  Serial.begin(115200);
  Serial.println("Test message");

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
    if (pSensorService->IsHeartBeatDetected())
    {
      int hb = pSensorService->GetHeartBeat();
      pPOServer->NotifyHeartBeat(hb);
    }
  }
  else
  {
    if (IsNeedRestart)
    {
      pPOServer->Start();
      IsNeedRestart = false;
    }
  }

  delay(1000);
}

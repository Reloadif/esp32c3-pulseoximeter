#include <BLEServer.h>

#define BLE_SERVER_NAME "ESP32-C3-PULSE-OXIMETER"

#define HEART_BEAT_SERVICE "5fafc201-1fb5-459e-8fcc-000000000000"

#define HEART_BEAT_DETECTION_CHARACTERISTIC "5fafc201-1fb5-459e-8fcc-000000000001"
#define HEART_BEAT_CHARACTERISTIC "5fafc201-1fb5-459e-8fcc-000000000002"

extern bool IsDeviceConnected;
extern bool IsNeedRestart;

class POServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        IsDeviceConnected = true;
    };

    void onDisconnect(BLEServer *pServer)
    {
        IsDeviceConnected = false;
        IsNeedRestart = true;
    }
};

class POServer
{
    BLEServer *pBLEServer;

    BLEService *pHBService;
    BLECharacteristic *pHbDetectedCharacteristic;
    BLECharacteristic *pHbCharacteristic;

public:
    POServer();

    void Start();
    void NotifyHeartBeat(int &value);
};

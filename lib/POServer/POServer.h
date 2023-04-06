#include <Arduino.h>
#include <BLEServer.h>

#define BLE_SERVER_NAME "ESP32-C3-PULSE-OXIMETER"

#define PULSEOXIMETER_SERVICE "5fafc201-1fb5-459e-8fcc-000000000000"

#define MEASUREMENT_SELECTION_CHARACTERISTIC "5fafc201-1fb5-459e-8fcc-000000000001"
#define LAST_BEAT_CHARACTERISTIC "5fafc201-1fb5-459e-8fcc-000000000002"
#define OXYGEN_SATUARATION_CHARACTERISTIC "5fafc201-1fb5-459e-8fcc-000000000003"

extern bool IsDeviceConnected;
extern bool IsNeedRestart;

extern int MeasurementSelection;

template <typename IntegerType>
IntegerType bitsToInt(const uint8_t *bits, bool little_endian = true)
{
    IntegerType result = 0;
    if (little_endian)
        for (int n = sizeof(result); n >= 0; n--)
            result = (result << 8) + bits[n];
    else
        for (unsigned n = 0; n < sizeof(result); n++)
            result = (result << 8) + bits[n];
    return result;
}

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

class MeasurementSelectionCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        MeasurementSelection = bitsToInt<int>(pCharacteristic->getData());
    }
};

class POServer
{
    BLEServer *pBLEServer;

    BLEService *pPOService;
    BLECharacteristic *pMeasurementSelectionCharacteristic;
    BLECharacteristic *pLastBeatCharacteristic;
    BLECharacteristic *pOxygenSaturationCharacteristic;

public:
    POServer();
    void Start();

    void NotifyLastBeat(int &value);
    void NotifyOxygenSaturation(int &value);
};

#include <POServer.h>

#include <BLEServer.h>
#include <BLEDevice.h>
#include <BLE2902.h>

POServer::POServer()
{
    BLEDevice::init(BLE_SERVER_NAME);

    pBLEServer = BLEDevice::createServer();
    pBLEServer->setCallbacks(new POServerCallbacks());

    pPOService = pBLEServer->createService(PULSEOXIMETER_SERVICE);

    pMeasurementSelectionCharacteristic = pPOService->createCharacteristic(
        MEASUREMENT_SELECTION_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    pMeasurementSelectionCharacteristic->setCallbacks(new MeasurementSelectionCallbacks());

    pHeartBeatCharacteristic = pPOService->createCharacteristic(
        HEART_BEAT_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    pHeartBeatCharacteristic->addDescriptor(new BLE2902());

    pOxygenSaturationCharacteristic = pPOService->createCharacteristic(
        OXYGEN_SATUARATION_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    pOxygenSaturationCharacteristic->addDescriptor(new BLE2902());

    pPOService->addCharacteristic(pMeasurementSelectionCharacteristic);
    pPOService->addCharacteristic(pHeartBeatCharacteristic);
    pPOService->addCharacteristic(pOxygenSaturationCharacteristic);

    pPOService->start();
}

void POServer::Start()
{
    pBLEServer->getAdvertising()->start();
}

void POServer::NotifyHeartBeat(int &value)
{
    pHeartBeatCharacteristic->setValue(value);
    pHeartBeatCharacteristic->notify();
}

void POServer::NotifyOxygenSaturation(int &value)
{
    pOxygenSaturationCharacteristic->setValue(value);
    pOxygenSaturationCharacteristic->notify();
}

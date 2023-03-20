#include <POServer.h>

#include <BLEServer.h>
#include <BLEDevice.h>
#include <BLE2902.h>

POServer::POServer()
{
    BLEDevice::init(BLE_SERVER_NAME);

    pBLEServer = BLEDevice::createServer();
    pBLEServer->setCallbacks(new POServerCallbacks());

    pHBService = pBLEServer->createService(HEART_BEAT_SERVICE);

    pHbDetectedCharacteristic = pHBService->createCharacteristic(
        HEART_BEAT_DETECTION_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    pHbDetectedCharacteristic->addDescriptor(new BLE2902());

    pHbCharacteristic = pHBService->createCharacteristic(
        HEART_BEAT_CHARACTERISTIC,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
    pHbCharacteristic->addDescriptor(new BLE2902());

    pHBService->addCharacteristic(pHbDetectedCharacteristic);
    pHBService->addCharacteristic(pHbCharacteristic);

    pHBService->start();
}

void POServer::Start()
{
    pBLEServer->getAdvertising()->start();
}

void POServer::NotifyHeartBeat(int &value)
{
    pHbCharacteristic->setValue(value);
    pHbCharacteristic->notify();
}

#pragma once

#include <Arduino.h>
#include "BLEDevice.h"
#include "string.h"

//Tag data packaging
typedef struct tagData{
  float x;
  float y;
  float z;
  float temperature;
  float pressure;
  float alt;
} tagData_t;
//Tag Class: To get info from (BLE)
class myTag{
    public:
        myTag();
        myTag(String tagName, int tagID, String serviceID, String characteristic_UUID);
        String tagName;
        int tagID;
        BLEUUID serviceUUID;
        BLEUUID characteristic_UUID;

        tagData_t tagData;
        bool isConnected;

        String makeDataPacket(void);
        void connect();
        void get();
        void post();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Arduino.h>
#include "BLEDevice.h"
#include "string.h"

//Tag data packaging
typedef struct tagData{
  float id;
  float x;
  float y;
  float z;
  float temperature;
  float pressure;
  float lat;
  float lng;
} tagData_t;
//Tag Class: To get info from (BLE)
class myTag{
    public:
        myTag();
        myTag(BLEUUID serviceID, BLEUUID characteristic_UUID);
        BLEUUID serviceUUID;
        BLEUUID characteristic_UUID;

        tagData_t tagData;
        bool isConnected;

        String makeDataPacket(void);
        void connect(int tagID);
        void get();
        void post();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////

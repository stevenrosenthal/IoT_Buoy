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
        myTag(int tagID, std::string tagName, String serviceID, String characteristic_UUID);
        int tagID;
        std::string tagName;
        BLEUUID serviceUUID;
        BLEUUID characteristic_UUID;
    

        tagData_t tagData;
        int buoyRSSI;
        bool isConnected;

        void connect(std::string tagName);
        void get();
        void post(std::string x);
    private:
      bool connectToServer();
      BLEClient* my_pClient; 
};

//////////////////////////////////////////////////////////////////////////////////////////////////////

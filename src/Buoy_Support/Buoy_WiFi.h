#pragma once

#include <Arduino.h>
#include "WiFi.h"
#include "HTTPClient.h"
#include "string.h"
#include "BLEDevice.h"

//BAKER BLOCK

//Tag data packaging
typedef struct tagData{
  int16_t id;
  float x;
  float y;
  float z;
  float temperature;
  float pressure;
  double lat;
  double lng;
} tagData_t;
//Tag Class: To get info from (BLE)
class myTag{
    public:
        myTag();
        myTag(String serviceID, String characteristic_UUID);
        String serviceID;
        String characteristic_UUID;

        tagData_t tagData;
        bool isConnected;

        String makeDataPacket(void);
        void connect(int tagID);
        void disconnect(int tagID);
        void get();
        void post();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////

//Server Class: To connect to(WiFi)
class myServer{
    public:
        myServer();
        myServer(String ssid, String password, String serverPort, String serverIP);

        String ssid;
        String password;

        String port;
        String IP;
        bool isConnected;

        IPAddress buoyIP;       //Buoy's IP Address

        //WIFI
        void connect(void);
        void post(String serverPath, String postBody); // serverPath format = /directories
        String get(String serverPath);
};
//Buoy Class
/*
You may add to this/edit. The add to the constrcutor for a buoy if you wish and modify functions.
you may also add private support functions too. Keep it clean 
*/
class myBuoy{
public:
    //Constructors
    myBuoy(myServer* server,myTag* tag);

    //Vars
    myServer* server;    //server Info
    myTag* tag;       //current tag Info
};
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
  float preassure;
  double lat;
  double lng;
} tagData_t;
//Tag Class: To get info from (BLE)
class myTag{
    public:
        myTag();
        String serviceID;
        String characteristic_UUID;
        myTag(String ssid, String password, String serverPort, String serverIP);

        tagData_t tagData;
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
};
//Buoy Class
/*
You may add to this/edit. The add to the constrcutor for a buoy if you wish and modify functions.
you may also add private support functions too. Keep it clean 
*/
class Buoy{
public:
    //Constructors
    Buoy(String ssid, String password, String serverPort, String serverIP);

    //Vars
    myServer server;
    HTTPClient connection;
    IPAddress IP;

    myTag curTag;
    String msgBuffer;
    bool isConnected;

    //Fxs
    //TAG (BLE)
    void connectToTag(int tagID);
    void disconnectFromTag(int tagID);
    void getFromTag();
    void postToTag();

    //WIFI
    void connectToServer(void);
    void postToServer(String serverPath, String postBody); // serverPath format = /directories
    String getFromServer(String serverPath);

private:
    void createPostData(void);
};
#pragma once

#include <Arduino.h>
#include "WiFi.h"
#include "HTTPClient.h"
#include "string.h"

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
        int comm(String serverPath, String postBody); // serverPath format = /directories
};

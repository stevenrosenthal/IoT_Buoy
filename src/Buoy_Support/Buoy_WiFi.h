#include <Arduino.h>
#include "WiFi.h"
#include "HTTPClient.h"
#include "string.h"
//Network to connect to
class myServer{
    public:
        myServer();
        myServer(String ssid, String password, String serverPort, String serverIP);

        String ssid;
        String password;

        String port;
        String IP;
};

class Buoy{
public:
    //Constructors
    Buoy(String ssid, String password, String serverPort, String serverIP);

    //Vars
    myServer server;
    HTTPClient connection;
    IPAddress IP;
    bool isConnected;

    //Fxs
    void connectToServer(void);
    void postToServer(String serverPath, String postBody); // serverPath format = /directories
    String getFromServer(String serverPath);
};
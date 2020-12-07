#include "Buoy_WiFi.h"
#include "Buoy_BLE.h"

//Buoy Class
class myBuoy{
public:
    //Constructors
    myBuoy(myServer* server,myTag* tag);

    //Vars
    myServer* server;    //server Info
    myTag* tag;       //current tag Info
};

//Buoy
myBuoy::myBuoy(myServer* server, myTag* tag){
  //Server
    this->server = server;
  
  //Tag
    this->tag = tag;
}
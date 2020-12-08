#include "Buoy_WiFi.h"
#include "Buoy_BLE.h"

//Buoy Class
class myBuoy{
public:
    //Constructors
    myBuoy(int ID, myServer* server,myTag* tag);

    //Vars
    int buoyID;
    myServer* server;    //server Info
    myTag* tag;       //current tag Info

  String makeDataPacket(void){
  char buffer[200];
  sprintf(buffer,"{\"buoy_id\": %d, \"tag_id\": %d, \"accel\": [%f,%f,%f], \"temperature\": %f, \"pressure\": %f, \"altitude\": %f}",
          buoyID,
          tag->tagID,
          tag->tagData.x,
          tag->tagData.y,
          tag->tagData.z,
          tag->tagData.temperature,
          tag->tagData.pressure,
          tag->tagData.alt
        );
  tag->tagData = {0};            //resets tagData packet
  String msgBuffer(buffer);
  return msgBuffer;
}
};

//Buoy
myBuoy::myBuoy(int ID, myServer* server, myTag* tag){
  this->buoyID = ID;
  //Server
    this->server = server;
  
  //Tag
    this->tag = tag;
}
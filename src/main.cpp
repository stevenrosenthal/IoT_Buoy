#include <Arduino.h>
#include <stdlib.h>
#include "Buoy_Support/Buoy.h"

myServer server("uhouse","","3000","104.62.174.178");
myTag tag(37,"4fafc201-1fb5-459e-8fcc-c5c9c331914b","beb5483e-36e1-4688-b7f5-ea07361b26a8");
myBuoy buoy(37,&server,&tag);

void setup() {
  buoy.tag->connect("tag1"); // FIXME:
  buoy.server->connect();
}

void loop() {
  /* BLE Get Info */
  buoy.tag->get();

  /* WIFI Post to Server */
  if((bool)(buoy.server->comm("/api/data",buoy.makeDataPacket())) == false){
    Serial.println("Tag in regular mode");
  }
  else{
    Serial.println("Buoy->tag LP mode");
    std::string shutdown = "true";
    buoy.tag->post(shutdown);
    sys_delay_ms(4000);
  }
  sys_delay_ms(1000);
}
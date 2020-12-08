#include <Arduino.h>
#include <stdlib.h>
#include "Buoy_Support/Buoy.h"

myServer server("John Carr's iPhone","johncarr","3000","104.62.174.178");
myTag tag(37,"4fafc201-1fb5-459e-8fcc-c5c9c331914b","beb5483e-36e1-4688-b7f5-ea07361b26a8");

myBuoy buoy(37,&server,&tag);

void setup() {
  buoy.tag->connect();
  buoy.server->connect();
}

void loop() {
  /* BLE Get Info */
  buoy.tag->get();
  /*
  buoy.tag->tagData.x = 1;
  buoy.tag->tagData.y = 1;
  buoy.tag->tagData.z = 1;
  buoy.tag->tagData.pressure = 10000;
  buoy.tag->tagData.temperature = 27;
  buoy.tag->tagData.alt = 1300;
*/
  /* WIFI Post to Server */
  if((bool)(buoy.server->comm("/api/data",buoy.makeDataPacket())) == false){
    Serial.println("Tag in regular mode");
  }
  else{
    Serial.println("Buoy->tag LP mode");
    // Code here for turning off LED
    sys_delay_ms(4000);
  }
  sys_delay_ms(1000);
}
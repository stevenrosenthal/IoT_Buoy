#include <Arduino.h>
#include "Buoy_Support/Buoy_WiFi.h"

myServer server("BetterNEB","mattsmullet","3000","192.168.1.136");
myTag tag("123","123");
myBuoy buoy(&server,&tag);

void setup() {
  buoy.server->connect();

  //make up some fake tag data
  buoy.tag->tagData.id = 37;
  buoy.tag->tagData.lat = 99.123;
  buoy.tag->tagData.lng = 100.432;
  buoy.tag->tagData.pressure = 10232.23;
  buoy.tag->tagData.temperature = 25.43;
  buoy.tag->tagData.x = 4;
  buoy.tag->tagData.y = 5;
  buoy.tag->tagData.z = 6;
}

void loop() {
  buoy.server->post("/api/data",buoy.tag->makeDataPacket());
  sys_delay_ms(1000);
}
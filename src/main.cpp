#include <Arduino.h>
#include <stdlib.h>
#include "Buoy_Support/Buoy.h"

myServer server("uhouse","","3000","10.35.32.1");
myTag tag("testTag", 37,"4fafc201-1fb5-459e-8fcc-c5c9c331914b","beb5483e-36e1-4688-b7f5-ea07361b26a8");

myBuoy buoy(&server,&tag);

void setup() {
  buoy.tag->connect();
  buoy.server->connect();
}

void loop() {
  /* BLE Get Info */
  buoy.tag->get();
  /* WIFI Post to Server */
  buoy.server->post("/api/data",buoy.tag->makeDataPacket());
  sys_delay_ms(1000);
}
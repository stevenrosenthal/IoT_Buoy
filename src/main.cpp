#include <Arduino.h>
#include "Buoy_Support/Buoy_WiFi.h"

Buoy myBuoy("BetterNEB","mattsmullet","5000","192.168.1.136");

void setup() {
  myBuoy.connectToServer();
}

void loop() {
  myBuoy.postToServer("/api/data","{\"msg\": \"test\"}");
  sys_delay_ms(1000);
}
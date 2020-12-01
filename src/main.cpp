#include <Arduino.h>
#include "Buoy_Support/Buoy_WiFi.h"

Buoy myBuoy("uhouse","","80","10.35.32.73");

void setup() {
  myBuoy.connectToServer();
}

void loop() {
  myBuoy.postToServer("/api/data","THIS IS A TEST");
}
#include <Arduino.h>
#include "Buoy_Support/Buoy_WiFi.h"

Buoy myBuoy("NerdsWhoAlsoHaveFun","skatergators","5000","192.168.1.64");

typedef struct {
  int16_t id;
  float x;
  float y;
  float z;
  float temperature;
  float preassure;
  double lat;
  double lng;
} trackerData_t;

void createPostData(trackerData_t, char*);

void setup() {
  myBuoy.connectToServer();
}

void loop() {
  myBuoy.postToServer("/api/data","{\"msg\": \"test\"}");
  sys_delay_ms(1000);
}

void createPostData(trackerData_t data, char* buff)
{
  sprintf("{tag_id: %d, accel: [%f,%f,%f]}, temperature: %f, preassure: %f, location: [%f, %f]", \
          buff, data.id,data.x,data.y,data.z,data.temperature,data.preassure,data.lng,data.lat);
}
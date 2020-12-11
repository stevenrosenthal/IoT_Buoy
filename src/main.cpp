#include <Arduino.h>
#include <stdlib.h>
#include "Buoy_Support/Buoy.h"

#define NUM_TAGS 2

int it = 0;

myTag tag0(37,"tag0","4fafc201-1fb5-459e-8fcc-c5c9c331914b","beb5483e-36e1-4688-b7f5-ea07361b26a8");
myTag tag1(1, "tag1","4fafc201-1fb5-459e-8fcc-c5c9c331914b","beb5483e-36e1-4688-b7f5-ea07361b26a8");
myTag* tags[2] = {&tag0, &tag1};
myServer server("uhouse","","3000","10.35.32.95");
myBuoy buoy(37,&server,tags[it]);

///////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  delay(100);
  buoy.server->connect();
}

void loop() {
  buoy.tag = tags[it];
  /* BLE Get Info */
  Serial.print("Tag: ");
  Serial.println(tags[it]->tagName.c_str());

  buoy.tag->connect(tags[it]->tagName);
    Serial.print("HERE");
  buoy.tag->get();

  /* WIFI Post to Server */
  if((bool)(buoy.server->comm("/api/data",buoy.makeDataPacket())) == false){
    Serial.println("Tag in regular mode");
  }
  else{
    buoy.tag->connect(tags[it]->tagName);
    Serial.println("Buoy->tag LP mode");
    std::string shutdown = "true";
    buoy.tag->post(shutdown);
  }

  Serial.println("Moving to next tag ...");

  sys_delay_ms(500);

  /*Move to next tag*/
  it++;
  if(it >= NUM_TAGS){
    it = 0;
  }
}
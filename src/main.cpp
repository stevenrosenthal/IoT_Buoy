#include <Arduino.h>
#include <stdlib.h>
#include "Buoy_Support/Buoy_WiFi.h"
#include "BLEDevice.h"

// WIFI
myServer server("uhouse","","3000","10.35.32.1");
myTag tag("123","123");
myBuoy buoy(&server,&tag);

// BLE
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
static BLEUUID characteristic_UUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

// UUID of the "Power Down" Service on the Tag
//BLEUUID SHUTDOWN_SERVICE_UUID("39533dce-4e87-4fa9-a338-f4316436dd19");
//BLEUUID SHUTDOWN_CHARACTERISTIC_UUID("eb439398-88f1-4d1d-b107-19a4dc6c53ce");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
//static BLERemoteCharacteristic* shutdownRemoteCharacteristic; // new one
static BLEAdvertisedDevice* myDevice;

std::string myDataString; // Where our data is recieved to from the tracker

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};
// do I need to notify callback for the second one?
static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("Notify callback for characteristic ");
    Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    Serial.print(" of data length ");
    Serial.println(length);
    Serial.print("data: ");
    Serial.println((char*)pData);
}
// need to modify connect to server?
bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str()); //address of the tag is printed
    
    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server/Tag.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");


   pRemoteCharacteristic = pRemoteService->getCharacteristic(characteristic_UUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID 0: ");
      Serial.println(characteristic_UUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");

    if(pRemoteCharacteristic->canNotify())
      pRemoteCharacteristic->registerForNotify(notifyCallback);

    connected = true;
    return true;
}

/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

void setup() {
  // BLE Setup code
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);

  buoy.server->connect();
}

void loop() {
  /* BLE Connection */
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }
if (connected) {
  
  /* Reading and converting the data from the tracker */
     if(pRemoteCharacteristic->canRead()) {
       myDataString = (pRemoteCharacteristic->readValue());
      /* Converting the string to a char array for parsing */
       char c[myDataString.size() + 1];
       strcpy(c, myDataString.c_str());
      /* Parsing the char array */
       char *token = strtok(c, " ");
       int i = 0; 
       char *temp[6] = {};
        while (token != NULL) 
        { 
            printf("%s\n", token);
            temp[i++] = token;
            token = strtok(NULL, " ");
        }
        i = 0; // Reseting index of parsing data
        /* Setting the data equal to its converted value */
        buoy.tag->tagData.pressure = atof(temp[0]); // Other method?
        buoy.tag->tagData.temperature = atof(temp[1]);
        buoy.tag->tagData.lat = atof(temp[2]); // actually altitude
        buoy.tag->tagData.x = atof(temp[3]);
        buoy.tag->tagData.y = atof(temp[4]);
        buoy.tag->tagData.z = atof(temp[5]);
        Serial.println("We made in here but skip the if");

        /* Sending the command to turn off the LED */
    if (pRemoteCharacteristic->canWrite()){
      std::string shutdown = "true"; // FIXME: make this a global variable that is filled in from John's Server
      pRemoteCharacteristic->writeValue(shutdown);
      Serial.println("sent the request to turn off");
  }

    }
  }else if(doScan){
    BLEDevice::getScan()->start(0);
  }
  /* WIFI Post */
  buoy.server->post("/api/data",buoy.tag->makeDataPacket());
  sys_delay_ms(1000);
}
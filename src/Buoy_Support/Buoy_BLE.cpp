#include "Buoy_BLE.h"

static BLEUUID _serviceUUID;
static BLEUUID _characteristic_UUID;

myTag::myTag(){}
myTag::myTag(BLEUUID serviceID, BLEUUID characteristic_UUID){
  this->serviceUUID = serviceID;
  this->characteristic_UUID = characteristic_UUID;

  _serviceUUID = serviceID;
  _characteristic_UUID = characteristic_UUID;

  this->isConnected = false;
  tagData = {0};                //initializes tagData packet to zero
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*#region Support*/
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

std::string myDataString;

class MyClientCallback : public BLEClientCallbacks {
void onConnect(BLEClient* pclient) {
}

void onDisconnect(BLEClient* pclient) {
connected = false;
Serial.println("onDisconnect");
}
};

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
BLERemoteService* pRemoteService = pClient->getService(_serviceUUID);
if (pRemoteService == nullptr) {
Serial.print("Failed to find our service UUID: ");
Serial.println(_serviceUUID.toString().c_str());
pClient->disconnect();
return false;
}
Serial.println(" - Found our service");


pRemoteCharacteristic = pRemoteService->getCharacteristic(_characteristic_UUID);
if (pRemoteCharacteristic == nullptr) {
Serial.print("Failed to find our characteristic UUID 0: ");
Serial.println(_characteristic_UUID.toString().c_str());
pClient->disconnect();
return false;
}
Serial.println(" - Found our characteristic");

if(pRemoteCharacteristic->canNotify())
pRemoteCharacteristic->registerForNotify(notifyCallback);

connected = true;
return true;
}

/*
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
if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(_serviceUUID)) {

BLEDevice::getScan()->stop();
myDevice = new BLEAdvertisedDevice(advertisedDevice);
doConnect = true;
doScan = true;

} // Found our server
} // onResult
}; // MyAdvertisedDeviceCallbacks
/*#endregion*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////

String myTag::makeDataPacket(void){
  char buffer[200];
  sprintf(buffer,"{\"tag_id\": %f, \"accel\": [%f,%f,%f], \"temperature\": %f, \"pressure\": %f, \"location\": [%f, %f]}",
          tagData.id,
          tagData.x,
          tagData.y,
          tagData.z,
          tagData.temperature,
          tagData.pressure,
          tagData.lng,
          tagData.lat
        );
  tagData = {0};            //resets tagData packet
  String msgBuffer(buffer);
  return msgBuffer;
}
void myTag::connect(int tagID){
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

    if (doConnect == true) {
        if (connectToServer()) {
        Serial.println("We are now connected to the BLE Server.");
        }
        else {
            Serial.println("We have failed to connect to the server; there is nothin more we will do.");
        }
        doConnect = false;
    }
    isConnected = true;
}
void myTag::get(){
    if (connected) {
        isConnected = true;
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
            tagData.pressure = atof(temp[0]); // Other method?
            tagData.temperature = atof(temp[1]);
            tagData.lat = atof(temp[2]); // actually altitude
            tagData.x = atof(temp[3]);
            tagData.y = atof(temp[4]);
            tagData.z = atof(temp[5]);
        }
        }else if(doScan){
        isConnected = false;
        BLEDevice::getScan()->start(0);
    }
}


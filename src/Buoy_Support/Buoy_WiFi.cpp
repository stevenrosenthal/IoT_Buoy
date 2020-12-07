#include "Buoy_WiFi.h"

//CONSTRCUTORS

//BLE (BAKER BLOCK)
myTag::myTag(){}
myTag::myTag(String serviceID, String characteristic_UUID){
  this->serviceID = serviceID;
  this->characteristic_UUID = characteristic_UUID;
  this->isConnected = false;
  tagData = {0};                //initializes tagData packet to zero
}


//WiFi
myServer::myServer(){}
myServer::myServer(String ssid, String password, String serverPort, String serverIP){
    this->ssid = ssid;
    this->password = password;
    this->port = serverPort;
    this->IP = serverIP;
    this->isConnected = false;
}

//Buoy
myBuoy::myBuoy(myServer* server, myTag* tag){
  //Server
    this->server = server;
  
  //Tag
    this->tag = tag;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////

//FUNCTIONS

//Tag (BAKER BLOCK )
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
  /*POPULATE ME*/
}
void myTag::disconnect(int tagID){
/*POPULATE ME*/
}
void myTag::get(){
/*POPULATE ME*/
}
void myTag::post(){
/*POPULATE ME*/
}


//Server
void myServer::connect(void){
    //Turn on serial monitor
  Serial.begin(115200);
  delay(100);

  //Header
  Serial.println("~~~~~~~~~~~~~");
  Serial.println("    Buoy     ");
  Serial.println("~~~~~~~~~~~~~\n");

  //Scan network
  SCAN_NETS:
  Serial.println("Scanning For Networks...");
  int n = WiFi.scanNetworks();
  bool netFound = false;
  if(n){    //If networks were found
    Serial.print(n);
    Serial.println(" Networks Found:");
    Serial.println("-----------------");
    for (int i = 0; i < n; i++) {
            //SID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")\n");
            if(ssid.equals(WiFi.SSID(i))){
              netFound = true;  
            }
    }
    Serial.println("-----------------\n");
  }
  else{
    Serial.println("No Networks Found...");
  }

  if(netFound){
    //Setup WiFi
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
  
    //Attemppt to connect
    Serial.print("Attempting to connect to ");
    Serial.print(ssid);
    Serial.println("...");
    
    WiFi.begin(ssid.c_str(), password.c_str());

   //Try to connect
   int attemptI = 1;
   do{
      Serial.print("Attempt ");
      Serial.println(attemptI++);
      delay(1000);
   }
   while(WiFi.status() != WL_CONNECTED);
    
    Serial.println("Connected!");
    Serial.print("IP Address: ");
    buoyIP = WiFi.localIP();
    Serial.println(WiFi.localIP());
  }
  else{
    Serial.print(ssid);
    Serial.println(" was not found... Rescanning...");
    goto SCAN_NETS;
  }

  isConnected = true;
}
void myServer::post(String serverPath, String postBody){
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
    HTTPClient connection;
    String serverName;
    if(postBody.charAt(0) == '/'){
      serverName = "http://" + IP + ":" + port + serverPath + "/";
    }
    else{
      serverName = "http://" + IP + ":" + port + serverPath;
    }

   Serial.println("Posting to server: " + serverName);
   Serial.println("body: " + postBody);
   
   connection.begin(serverName);  //Specify destination for HTTP request
   connection.addHeader("Content-Type", "application/json");

   int httpResponseCode = connection.POST(postBody);   //Send the actual POST request
  
   if(httpResponseCode>0){
    String response = connection.getString();                      //Get the response to the request
  
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
   }
   else{
    Serial.print("Error on sending POST: ");
    Serial.println(connection.errorToString(httpResponseCode).c_str());
   }
  
   connection.end();  //Free resources
  }
  else{
    Serial.println("Could not post... WiFi disconnected");   
  }
}


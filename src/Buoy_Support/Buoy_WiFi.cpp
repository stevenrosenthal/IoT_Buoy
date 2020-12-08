#include "Buoy_WiFi.h"

//CONSTRCUTORS
myServer::myServer(){}
myServer::myServer(String ssid, String password, String serverPort, String serverIP){
    this->ssid = ssid;
    this->password = password;
    this->port = serverPort;
    this->IP = serverIP;
    this->isConnected = false;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////

//FUNCTIONS
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
int myServer::comm(String serverPath, String postBody){
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
    HTTPClient connection;
    String serverName;
    if(postBody.charAt(0) == '/'){
      serverName = "http://" + IP + ":" + port + serverPath + "/";
    }
    else{
      serverName = "http://" + IP + ":" + port + serverPath;
    }

   //Serial.println("Posting to server: " + serverName);
   //Serial.println("body: " + postBody);
   
   connection.begin(serverName);  //Specify destination for HTTP request
   connection.addHeader("Content-Type", "application/json");

   int httpResponseCode = connection.POST(postBody);   //Send the actual POST request
  
  String response;
   if(httpResponseCode>0){
    response = connection.getString();                      //Get the response to the request
  
    //Serial.println(httpResponseCode);   //Print return code
    //Serial.println(response);           //Print request answer
   }
   else{
    //Serial.print("Error on sending POST: ");
    //Serial.println(connection.errorToString(httpResponseCode).c_str());
   }

   connection.end();  //Free resources
    return response.equals("{\"success\":true,\"msg\":\"LP\"}");
  }
  else{
    Serial.println("Could not post... WiFi disconnected");   
  }
  return -1;
}


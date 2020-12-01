#include "Buoy_WiFi.h"
//WiFi
myServer::myServer(){}

myServer::myServer(String ssid, String password, String serverPort, String serverIP){
    this->ssid = ssid;
    this->password = password;
    this->port = serverPort;
    this->IP = serverIP;
}

//Buoy
Buoy::Buoy(String ssid, String password, String serverPort, String serverIP){
    this->server.ssid = ssid;
    this->server.password = password;
    this->server.port = serverPort;
    this->server.IP = serverIP;

    this->isConnected = false;
}

//Buoy Functions
void Buoy::connectToServer(void){
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
            if(server.ssid.equals(WiFi.SSID(i))){
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
    Serial.print("");
    Serial.println("...");
    
    WiFi.begin(server.ssid.c_str(), server.password.c_str());

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
    this->IP = WiFi.localIP();
    Serial.println(WiFi.localIP());
  }
  else{
    Serial.print(server.ssid);
    Serial.println(" was not found... Rescanning...");
    goto SCAN_NETS;
  }
}
void Buoy::postToServer(String serverPath, String postBody){
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
    String serverName;
    if(postBody.charAt(0) == '/'){
      serverName = "http://" + server.IP + ":" + server.port + serverPath + "/";
    }
    else{
      serverName = "http://" + server.IP + ":" + server.port + serverPath;
    }

   Serial.println("Posting to server: " + serverName);
   Serial.println("Body: " + postBody);
   
   connection.begin(serverName);  //Specify destination for HTTP request
   connection.addHeader("Content-Type", "text/plain");

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

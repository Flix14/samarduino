#include <ArduinoJson.h>

#define DEBUG true

char WIFI_SSID[255] = "Nivel653";
char WIFI_PASS[255] = "444benxhouse";
String response;
String responseGPS;

void setup()
{  
  delay(500);
  
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  delay(100);

  //SETUP GPS
  sendDataToGPSModule("AT+CGPSPWR=1\r\n", 500, DEBUG);
  delay(500);
  
  //SETUP ESP8266
  char WIFI_CONN[600];
  sendDataToWiFiModule("AT+CWMODE=3\r\n", 1000, DEBUG);
  
  strcpy(WIFI_CONN, "AT+CWJAP=\"");
  strcat(WIFI_CONN, WIFI_SSID);
  strcat(WIFI_CONN, "\",\"");
  strcat(WIFI_CONN, WIFI_PASS);
  strcat(WIFI_CONN, "\"\r\n");
  
  sendDataToWiFiModule(WIFI_CONN, 1000, DEBUG);
  delay(10000);
  sendDataToWiFiModule("AT+CIPMUX=1\r\n", 1000, DEBUG);
  sendDataToWiFiModule("AT+CIPSERVER=1,80\r\n", 1000, DEBUG);
  delay(10000);
  sendDataToWiFiModule("AT+CIFSR\r\n", 1000, DEBUG);
  delay(10000);
}
void loop()
{
  responseGPS = sendDataToGPSModule("AT+CGPSINF=0\r\n", 1000, DEBUG);
  if (Serial1.available()){
    if(Serial1.find("+IPD,")) {
      response = "Null";
      delay(1000);
      int connId = Serial1.peek()-48;
      Serial1.find("/?");
      int option = Serial1.peek()-48;
      
      if(option == 1) {
        response = "Success";
        delay(500);
        sendDataToWiFiModule("AT+CIPSEND=" + String(connId) + "," + response.length() + "\r\n", 500, DEBUG);
        sendDataToWiFiModule(response, 100, DEBUG);
        sendDataToWiFiModule("AT+CIPCLOSE=" + String(connId) + "\r\n", 1000, DEBUG);
      }
      if(option == 2) {
        sendDataToWiFiModule("AT+CIPSEND=" + String(connId) + "," + responseGPS.length() + "\r\n", 500, DEBUG);
        sendDataToWiFiModule(responseGPS, 100, DEBUG);
        sendDataToWiFiModule("AT+CIPCLOSE=" + String(connId) + "\r\n", 1000, DEBUG);
      }
    }
  }
}

String sendDataToWiFiModule(String command, const int timeout, boolean debug) {
  String response = "";

  Serial1.print(command);
  long int timeX = millis();
  while( (timeX+timeout) > millis() ){
    while(Serial1.available()){
      char c = Serial1.read();
      response+=c;
    }
  }
  if(debug){
    Serial.print(response);
  }
  return response;
}

String sendDataToGPSModule(String command, const int timeout, boolean debug) {
  String response = "";

  Serial2.print(command);
  long int timeX = millis();
  while( (timeX+timeout) > millis() ){
    while(Serial2.available()){
      char c = Serial2.read();
      response+=c;
    }
  }
  if(debug){
    Serial.print(response);
  }
  return response;
}

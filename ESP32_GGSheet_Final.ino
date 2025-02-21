#include <Arduino.h>
#include <HardwareSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>
//ENTER_GOOGLE_DEPLOYMENT_ID
const char * ssid = "emthaygitrongdoimatkeseetinh";
const char * password = "12345678";
String GOOGLE_SCRIPT_ID = "AKfycbx08de4vuP7AHvcukYyJeAmDaEUlqp3EAWBFEGXWQasEFsBKOZGOPUU9S43H89zwpsrpw";
// https://docs.google.com/spreadsheets/d/1aTuQF0Tzic5rfxqfFyF6-pyEuicZPtc9wzVk6G_bUEc/edit?gid=0#gid=0
// https://script.google.com/macros/s/AKfycbx08de4vuP7AHvcukYyJeAmDaEUlqp3EAWBFEGXWQasEFsBKOZGOPUU9S43H89zwpsrpw/exec
const int rxPin = 16;
const int txPin = 17;
char data;
unsigned long Time;
String param = "";

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("CONNECTED !!!");
  Serial2.begin(115200, SERIAL_8N1, rxPin, txPin);
}


// ======================================================
void loop()
{
  if (Serial2.available()) {
    data = Serial2.read();
    print_speed();
  }
}

void print_speed()
{
  if ( data != 0)
  {
    param  = "DATA=" + String(data);

    Serial.println(param);
    write_to_google_sheet(param);
  }
  else
  {
    Serial.println("No data !!!");
  }

}
/******/
void write_to_google_sheet(String params)
{
  HTTPClient http;
  String url = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?" + params;
  //Serial.print(url);
  Serial.println("Data sent to Google Sheets");
  //---------------------------------------------------------------------
  //starts posting data to google sheet
  http.begin(url.c_str());
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  int httpCode = http.GET();
  Serial.print("HTTP Status Code: ");
  Serial.println(httpCode);
  //---------------------------------------------------------------------
  //getting response from google sheet
  String payload;
  if (httpCode > 0) {
    payload = http.getString();
    Serial.println("Payload: " + payload);
  }
  //---------------------------------------------------------------------
  http.end();
}

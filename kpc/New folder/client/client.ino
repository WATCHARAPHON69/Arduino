#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
char*       ssid = "FILM69";             
char*       password = "00000000";        
const String  Devicename = "Device_1";
IPAddress APlocal_IP(192, 168, 4, 1);
//IPAddress    apIP(10, 10, 10, 1);
IPAddress APgateway(192, 168, 4, 1);
IPAddress APsubnet(255, 255, 255, 0);
unsigned int UDPPort = 2390;     
WiFiUDP Udp;
char result[16];             
char packetBuffer[255];      
int setdatain = 0;


int set = 3;
String data[] = {"", "", ""};


void setup() {
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();
  lcd.print("Hello, world!");
  lcd.setCursor(0, 1); //ตัวที่ , บรรทัดที่
  lcd.print("Hello, world!");
  Check_WiFi_and_Connect();
}
void loop() {
  Send_Data_To_Server();
}
void Send_Data_To_Server() {
  unsigned long tNow;
  tNow = millis();                                           
  dtostrf(tNow, 8, 0, result);                               
  Udp.beginPacket(APlocal_IP, UDPPort);                        
  Udp.write("A\nB\nC\n");
  Udp.endPacket();                                             
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    for (int i = 0; i <= len; i++) {
      if (packetBuffer[i] == '\n') {
        setdatain++;
        if (setdatain == set) {
          setdatain = 0;
        }
      }
      else {
        data[0, setdatain] += packetBuffer[i];
        //Serial.println(packetBuffer[i]);
      }
    }
    Serial.print(data[0, 0]);
    Serial.print(" # ");
    Serial.print(data[0, 1]);
    Serial.print(" # ");
    Serial.println(data[0, 2]);
    data[0, 0] = ""; data[0, 1] = ""; data[0, 2] = "";
  }
}

//====================================================================================

void Check_WiFi_and_Connect() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();                                          // probably not necessary due to WiFi.status() != WL_CONNECTED
    WiFi.begin("DataTransfer");                                 // reconnect to the Network
    Serial.println();
    Serial.print("Wait for WiFi");

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: " + WiFi.localIP().toString());

    Udp.begin(UDPPort);
  }
}

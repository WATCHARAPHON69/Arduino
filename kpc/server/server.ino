#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
char*       ssid = "FILM69";              // SERVER WIFI NAME
char*       password = "00000000";          // SERVER PASSWORD
#define     MAXSC     6           // MAXIMUM NUMBER OF CLIENTS
IPAddress APlocal_IP(192, 168, 4, 1);
IPAddress APgateway(192, 168, 4, 1);
IPAddress APsubnet(255, 255, 255, 0);
unsigned int UDPPort = 2390;      // local port to listen on
WiFiUDP Udp;
char packetBuffer[255]; //buffer to hold incoming packet
char result[10];
int setdatain = 0;
#include <Ultrasonic.h>
Ultrasonic ultrasonic(12, 13);
int set = 3;
String data[] = {"", "", ""};
#include "DHT.h"

#define DHTPIN D4     // Digital pin connected to the DHT sensor
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
float h;
float t;
float f;
String LED1;
const int pingPin = D1;
int inPin = D2;
int cm;
int len;
int packetSize;
String N;
String DATAOUT;

void setup() {
  Serial.begin(115200);
  pinMode(D8, INPUT);
  pinMode(D5, OUTPUT);

  dht.begin();
  SetWifi("DataTransfer", "");
}

void loop() {
  HandleClients();
}

void HandleClients() {
  unsigned long tNow;
  packetSize = Udp.parsePacket();

  cm =  ultrasonic.read();

  if (digitalRead(D8) == HIGH) {
    LED1 = "ON";
  } else {
    LED1 = "OFF";
  }
  if (packetSize) {
    len = Udp.read(packetBuffer, 255);
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
      }
    }
    if (data[0, 2] == "ON") {
      digitalWrite(D5, HIGH);
      Serial.println("ON");
    } else {
      digitalWrite(D5, LOW);
    }
    Serial.print(data[0, 0]);
    Serial.print(" # ");
    Serial.print(data[0, 1]);
    Serial.print(" # ");
    Serial.println(data[0, 2]);
    data[0, 0] = ""; data[0, 1] = ""; data[0, 2] = "";

    h = dht.readHumidity();
    t = dht.readTemperature();
    f = 3535;
    N = "\n";
    DATAOUT = String(h) + N + String(t) + N + String(LED1) + N;
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.print(DATAOUT);
    Udp.endPacket();
  }
}

void SetWifi(char* Name, char* Password) {
  WiFi.disconnect();
  WiFi.mode(WIFI_AP_STA);
  Serial.println("WIFI Mode : AccessPoint Station");
  ssid      = Name;
  password  = Password;
  WiFi.softAPConfig(APlocal_IP, APgateway, APsubnet);
  WiFi.softAP(ssid, password, 1 , 0, MAXSC);
  Serial.println("WIFI < " + String(ssid) + " > ... Started");
  delay(50);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AccessPoint IP : ");
  Serial.println(IP);
  Udp.begin(UDPPort);
  Serial.println("Server Started");
}

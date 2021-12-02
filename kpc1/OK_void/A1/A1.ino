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
Ultrasonic ultrasonic(D1, D2);
int set = 3;
String data[] = {"", "", ""};
#include "DHT.h"

#define DHTPIN D7     // Digital pin connected to the DHT sensor
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
float h;
float t;
float f;
int BT[] = {0, 0, 0, 0, 0, 0, 0, 0};
bool A[] = {false, false};

const int pingPin = D1;
int inPin = D2;
int cm;
int len;
int packetSize;
String N;
String DATAOUT;

void setup() {
  Serial.begin(115200);
  pinMode(D6, INPUT_PULLUP);
  pinMode(D5, INPUT_PULLUP);
  pinMode(D4, OUTPUT);
  pinMode(D3, OUTPUT);

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

    //Serial.print(cm);
    //Serial.print("cm # ");
    Serial.print(data[0, 0]);
    Serial.print(" # ");
    Serial.print(data[0, 1]);
    Serial.print(" # ");
    Serial.print(data[0, 2]);
    Serial.print(" # BT1 = ");
    Serial.print(SW(D5, 0));
    Serial.print(" # BT2 = ");
    Serial.println(SW(D6, 1));


    h = dht.readHumidity();
    t = dht.readTemperature();
    //f = 3535;
    if (SW(D5, 0) == 1) {
      if (t >= 28) {
        digitalWrite(D3, HIGH);
      } else {
        digitalWrite(D3, LOW);
      }
    } else {
      if (data[0, 1] == "1") {
        digitalWrite(D3, HIGH);
      } else {
        digitalWrite(D3, LOW);
      }
    }
    data[0, 0] = ""; data[0, 1] = ""; data[0, 2] = "";

    N = "\n";
    DATAOUT = "S" + N + String(t) + N + String(h) + N + String(SW(D5, 0)) + N + String(SW(D6, 1)) + N;
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

int SW(int pin, int BT) {
  if (digitalRead(pin) == LOW) {
    A[BT] = !A[BT];
  }
  while (digitalRead(pin) == LOW);
  return A[BT];
}

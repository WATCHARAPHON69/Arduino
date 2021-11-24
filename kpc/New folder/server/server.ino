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

int set = 3;
String data[] = {"", "", ""};

float T;
float H;
float x;

void setup() {
  Serial.begin(115200);
  SetWifi("DataTransfer", "");
}
void loop() {
  HandleClients();
}

void HandleClients() {
  unsigned long tNow;
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
      }
    }
    Serial.print(data[0, 0]);
    Serial.print(" # ");
    Serial.print(data[0, 1]);
    Serial.print(" # ");
    Serial.println(data[0, 2]);
    data[0, 0] = ""; data[0, 1] = ""; data[0, 2] = "";



    T = T + 1;
    H = H + 2;
    x = x + 3;
    String N = "\n";
    String DATAOUT = String(T) + N + String(H) + N + String(x) + N;
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

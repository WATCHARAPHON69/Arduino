#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
char*       ssid = "FILM69";
char*       password = "00000000";
const String  Devicename = "Device_1";

unsigned long period = 100; //ระยะเวลาที่ต้องการรอ
unsigned long last_time = 0; //ประกาศตัวแปรเป็น global เพื่อเก็บค่าไว้ไม่ให้ reset จากการวนloop
unsigned long period1 = 1000; //ระยะเวลาที่ต้องการรอ
unsigned long last_time1 = 0; //ประกาศตัวแปรเป็น global เพื่อเก็บค่าไว้ไม่ให้ reset จากการวนloop
IPAddress APlocal_IP(192, 168, 4, 1);
//IPAddress    apIP(10, 10, 10, 1);
IPAddress APgateway(192, 168, 4, 1);
IPAddress APsubnet(255, 255, 255, 0);
unsigned int UDPPort = 2390;
WiFiUDP Udp;
char result[16];
char packetBuffer[255];
int setdatain = 0;
String data1;
int save[] = {0, 0, 0};
int savedata;
int set = 5;
String data[] = {"", "", "", "", ""};
int BT1;
int A1;
int A3;
int BT3;
bool A[] = {false, false, false, false, false};
void setup() {
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();
  Check_WiFi_and_Connect();
}
void loop() {
  Send_Data_To_Server();
}


void Send_Data_To_Server() {



  String N = "\n";
  String DATAOUT =  "NAN" + N + BT3 + N + "NAN" + N;
  Udp.beginPacket(APlocal_IP, UDPPort);
  Udp.print(DATAOUT);
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
    //########################################################################################################
    float TEMP = data[0, 1].toFloat();
    float HUM = data[0, 2].toFloat();

    if (TEMP >= 35) {
      if ( millis() - last_time > period) {
        last_time = millis(); //เซฟเวลาปัจจุบันไว้เพื่อรอจนกว่า millis() จะมากกว่าตัวมันเท่า period
        IO(D7, 1);
        IO(D8, 1);
      } else {
       
        IO(D8, 0);
      }
    }
    else if (TEMP >= 30) {
     if ( millis() - last_time1 > period1) {
        last_time1 = millis(); //เซฟเวลาปัจจุบันไว้เพื่อรอจนกว่า millis() จะมากกว่าตัวมันเท่า period
        IO(D7, 1);
        IO(D8, 0);
      } else {
        IO(D7, 0);
        IO(D8, 0);
      }

    } else {
      IO(D7, 0);
      IO(D8, 0);
    }





    //Serial.println(g);
    Serial.print(data[0, 1]);
    Serial.print(" # ");
    Serial.print(data[0, 2]);
    Serial.print(" # ");
    Serial.print(data[0, 3]);
    Serial.print(" # ");
    Serial.println(data[0, 4]);



    //########################################################################################################
    data[0, 0] = ""; data[0, 1] = ""; data[0, 2] = ""; data[0, 3] = ""; data[0, 4] = "";
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

int SW(int pin, int BT) {
  pinMode(pin, INPUT_PULLUP);
  if (digitalRead(pin) == LOW) {
    A[BT] = !A[BT];
  }
  while (digitalRead(pin) == LOW);
  return A[BT];
}
void IO(int pin, int DIGI) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, DIGI);
}
int II(int pin) {
  pinMode(pin, INPUT_PULLUP);
  return digitalRead(pin);
}
void IA(int pin, int ANALOG) {
  pinMode(pin, OUTPUT);
  analogWrite(pin, ANALOG);
}
void LCD_SET(int set1, int set2) {
  lcd.setCursor(set1, set2);
}

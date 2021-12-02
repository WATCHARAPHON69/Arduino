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
String data1;
int save[] = {1, 2, 3};
int savedata;
int set = 4;
String data[] = {"", "", "", ""};
int BT1;
int A1;
void setup() {
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, INPUT_PULLUP);
  Check_WiFi_and_Connect();
}
void loop() {
  Send_Data_To_Server();
}


void Send_Data_To_Server() {
  if (digitalRead(D5) == HIGH) {
    data1 = "OFF";
  } else {
    data1 = "ON";
  }
  String N = "\n";
  String DATAOUT =  data1 + N + "NAN" + N + "NAN" + N;
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

    int i = data[0, 0].length();
    int g = data[0, 1].length();




    if (data[0, 1] == "0") {
      digitalWrite(D3, HIGH);
      digitalWrite(D4, LOW);
      lcd.setCursor(0, 0);
      lcd.print("Monitor  ");
      lcd.print(data[0, 3]);
      lcd.print(" CM    ");
      lcd.setCursor(0, 1);
      lcd.print("CM");

      //lcd.setCursor(i-1, 1);
      //lcd.print(" ");

      if (data[0, 2] == "O") {
        A1++;
        if (A1 >= 2) {
          A1 = 2;
        }
        if (A1 == 1) {
          if (savedata >= 2) {
            savedata = -1;
          }
          savedata++;
        }
      } else {
        A1 = 0;
      }
      if (savedata == 0) {
        lcd.setCursor(0, 1);
        lcd.print("Save 1 = ");
        lcd.print(save[0]);
        lcd.print(" CM  ");
      } else if (savedata == 1) {
        lcd.setCursor(0, 1);
        lcd.print("Save 2 = ");
        lcd.print(save[1]);
        lcd.print(" CM  ");
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Save 3 = ");
        lcd.print(save[2]);
        lcd.print(" CM  ");
      }
    }


    else if (data[0, 1] == "1") {
      digitalWrite(D3, LOW);
      digitalWrite(D4, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("Save  ");
      lcd.print(data[0, 3]);
      lcd.print(" CM      ");


      if (data[0, 2] == "O") {
        A1++;
        if (A1 >= 2) {
          A1 = 2;
        }
        if (A1 == 1) {
          save[savedata] = data[0, 3].toInt();
          if (savedata >= 2) {
            savedata = -1;
          }

          savedata++;


        }
      } else {
        A1 = 0;
      }
      if (savedata == 0) {
        lcd.setCursor(0, 1);
        lcd.print("Save 1                 ");
        //lcd.print(save[0]);
        //lcd.print(" CM");
      } else if (savedata == 1) {
        lcd.setCursor(0, 1);
        lcd.print("Save 2               ");
        //lcd.print(save[1]);
        //lcd.print(" CM");
      } else {
        lcd.setCursor(0, 1);
        lcd.print("Save 3              ");
        //lcd.print(save[2]);
        //lcd.print(" CM");
      }
    }







    Serial.print(save[0]);
    Serial.print(" # ");
    Serial.print(save[1]);
    Serial.print(" # ");
    Serial.print(save[2]);
    Serial.print(" # ");



    Serial.print(savedata);
    Serial.print(" # ");
    //Serial.println(g);
    Serial.print(data[0, 1]);
    Serial.print(" # ");
    Serial.print(data[0, 2]);
    Serial.print(" # ");
    Serial.println(data[0, 3]);
    data[0, 0] = ""; data[0, 1] = ""; data[0, 2] = ""; data[0, 3] = "";
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

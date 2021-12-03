
#define BLYNK_AUTH_TOKEN            "2fli26MIRgLmGxhmGyU2sdys8k1YfgNI"
#include "DHT.h"
#define DHTPIN 23
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
BlynkTimer timer;
unsigned long period = 1000; //ระยะเวลาที่ต้องการรอ
unsigned long last_time = 0; //ประกาศตัวแปรเป็น global เพื่อเก็บค่าไว้ไม่ให้ reset จากการวนloop
char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "FILM69";
char pass[] = "00000000";
int Relay1, Relay2, Relay3, Relay4, tempset, analogset;
int analogin, Mode, humset, i, j, TIME_CHECK, TIME_RUN, TIME;
int analogpin = 15;
bool Status;
float HUM, TEMP;
void setup()
{
  // Debug console
  Serial.begin(115200);
  dht.begin();
  pinMode(analogpin, INPUT);
  //Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  Blynk.begin(auth, ssid, pass, "oasiskit.com", 8080);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
  timer.run();

  if ( millis() - last_time > period) {
    last_time = millis(); //เซฟเวลาปัจจุบันไว้เพื่อรอจนกว่า millis() จะมากกว่าตัวมันเท่า period
    TIME++;
  }
  if (Mode == 1) {
    if (j == 0) {
      if (i >= 1) {
        i = -1;
      }
      i++;
    }
    j++;
  }
  else {
    j = 0;
  }

  RUN();

  analogin = analogRead(analogpin);
  HUM = dht.readHumidity();
  TEMP = dht.readTemperature();


\*
  Serial.print("  Relay1 : ");
  Serial.print(Relay1);
  Serial.print("  Relay2 : ");
  Serial.print(Relay2);
  Serial.print("  Relay3 : ");
  Serial.print(Relay3);
  Serial.print("  Relay4 : ");
  Serial.print(Relay4);
  Serial.print(" SET_TEMP : ");
  Serial.print(tempset);
  Serial.print(" SET_ANALOG : ");
  Serial.print(analogset);
  Serial.print(" TEMP : ");
  Serial.print(TEMP);
  Serial.print(" *C ");
  Serial.print(" HUM : ");
  Serial.print(HUM);
  Serial.print(" % ");
  Serial.print(" ANALOG_IN : ");
  Serial.print(analogin);
  Serial.print(" SET_HUM : ");
  Serial.print(humset);
  Serial.print(" MODE : ");
  Serial.print(Mode);
  Serial.print(" i : ");
  Serial.print(i);
  Serial.print(" TIME_CHECK : ");
  Serial.print(TIME_CHECK);
  Serial.print(" TIME_RUN : ");
  Serial.print(TIME_RUN);
  Serial.print(" Status : ");
  Serial.print(Status);
  Serial.print(" TIME_CHECK : ");
  Serial.print(TIME_CHECK);
  Serial.print(" TIME_RUN : ");
  Serial.print(TIME_RUN);
  Serial.print(" TIME : ");
  Serial.println(TIME);
*/
  
  Blynk.virtualWrite(V7, analogin);
  Blynk.virtualWrite(V8, TEMP);
  Blynk.virtualWrite(V9, HUM);
  Blynk.virtualWrite(V10, i);
}
void RUN() {
  if (TIME >= TIME_CHECK + TIME_RUN) {
    TIME = 0;
  }
  //#################### MODE MUNAUL ##############################################
  if (i == 0) {
    IO(26, Relay1);
    IO(25, Relay2);
    IO(33, Relay3);
    IO(32, Relay4);
  }
  //#################### MODE AUTO ################################################
  else {
    if (TIME_CHECK == 0 || TIME > TIME_CHECK && TIME < TIME_CHECK + TIME_RUN) {
      if (HUM >= humset) {
        IO(26, 1);
        BLYNK_V_WRITE(V1, 1);
      } else {
        IO(26, 0);
        BLYNK_V_WRITE(V1, 0);
      }
      if (analogin >= analogset) {
        IO(25, 1);
        BLYNK_V_WRITE(V2, 1);
      } else {
        IO(25, 0);
        BLYNK_V_WRITE(V2, 0);
      }
      //digitalWrite(33, Relay3);
      //digitalWrite(32, Relay4);
      //Serial.println("#########################################################################################################################");
    }
  }
}


void BLYNK_V_WRITE(int v, int io) {
  Blynk.virtualWrite(v, io);
}
void IO(int pin , int DIGI) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, DIGI);
}

BLYNK_WRITE(V1) {
  Relay1 = param.asInt();
}
BLYNK_WRITE(V2) {
  Relay2 = param.asInt();
}
BLYNK_WRITE(V3) {
  Relay3 = param.asInt();
}
BLYNK_WRITE(V4) {
  Relay4 = param.asInt();
}
BLYNK_WRITE(V5) {
  tempset = param.asInt();
}
BLYNK_WRITE(V6) {
  analogset = param.asInt();
}
BLYNK_WRITE(V11) {
  Mode = param.asInt();
}
BLYNK_WRITE(V0) {
  humset = param.asInt();
}
BLYNK_WRITE(V12) {
  TIME_CHECK = param.asInt();
}
BLYNK_WRITE(V13) {
  TIME_RUN = param.asInt();
}

#include "FILM69.h"
FILM69 F;
#define CH1 2
#define CH2 3
#define CH3 4
#define CH4 5
int ch1;
int ch2;
int ch3;
int ch4;
int PWM_FR, PWM_FL, PWM_BR, PWM_BL,PWM;
int PWM_1 = 60;
int MOTOR_FL1 = 26;
int MOTOR_FL2 = 27;
int MOTOR_FR1 = 29;
int MOTOR_FR2 = 28;
int MOTOR_BL1 = 24;
int MOTOR_BL2 = 25;
int MOTOR_BR1 = 23;
int MOTOR_BR2 = 22;
String con;
void setup() {
  Serial.begin(115200);
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);

}


void loop() {
  ch1 = readChannel(CH1, -100, 100, 0);
  ch2 = readChannel(CH2, -100, 100, 0);
  ch3 = readChannel(CH3, 295, -90, 0);
  ch4 = readChannel(CH4, -100, 100, 0);

  PWM = ch3;
  if (PWM >= 254) PWM = 255;
  if (PWM <= 10 ) PWM = 0;


  if (ch4 >= 50) CW();                              //หมุนซ้าย
  else if (ch4 <= -50) CCW();                       //หมุนขวา
  /*
    else if (ch1 <= -50 && ch2 <= -50) con = "FL";  //เดินหน้าเบี่ยงซ้าย
    else if (ch1 >= 50 && ch2 <= -50) con = "FR";   //เดินหน้าเบี่ยงขวา
    else if (ch1 <= -50 && ch2 >= 50) con = "BL";   //ถอยหลังเบี่ยงซ้าย
    else if (ch1 >= 50 && ch2 >= 50) con = "BR";    //ถอยหลังเบี่ยงขวา
  */
  else if (ch2 <= -50) FF();                        //เดินหน้า
  else if (ch2 >= 50) BB();                         //ถอยหลัง
  else if (ch1 >= 50) RR();                         //ไปทางขวา
  else if (ch1 <= -50) LL();                        //ไปทางซ้าย
  else Stop();                                      //หยุด

  Serial.print("Ch1:\t");
  Serial.print(ch1);
  Serial.print("\tCh2:\t");
  Serial.print(ch2);
  Serial.print("\tCh3:\t");
  Serial.print(PWM);
  Serial.print("\tCh4:\t");
  Serial.println(ch4);


}

int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue) {
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
  //return ch;
}
void CW() {
  PWM = PWM_1;
  F.DW(MOTOR_FL1, 0);        F.DW(MOTOR_FR1, 1);
  F.DW(MOTOR_FL2, 1);        F.DW(MOTOR_FR2, 0);
  F.AW(9, PWM);              F.AW(8, PWM);

  F.DW(MOTOR_BL1, 0);        F.DW(MOTOR_BR1, 1);
  F.DW(MOTOR_BL2, 1);        F.DW(MOTOR_BR2, 0);
  F.AW(7, PWM);              F.AW(6, PWM);


}
void CCW() {
  PWM = PWM_1;
  F.DW(MOTOR_FL1, 1);        F.DW(MOTOR_FR1, 0);
  F.DW(MOTOR_FL2, 0);        F.DW(MOTOR_FR2, 1);
  F.AW(9, PWM);              F.AW(8, PWM);

  F.DW(MOTOR_BL1, 1);        F.DW(MOTOR_BR1, 0);
  F.DW(MOTOR_BL2, 0);        F.DW(MOTOR_BR2, 1);
  F.AW(7, PWM);              F.AW(6, PWM);
}
void FF() {
  F.DW(MOTOR_FL1, 1);        F.DW(MOTOR_FR1, 1);
  F.DW(MOTOR_FL2, 0);        F.DW(MOTOR_FR2, 0);
  F.AW(9, PWM);              F.AW(8, PWM);

  F.DW(MOTOR_BL1, 1);        F.DW(MOTOR_BR1, 1);
  F.DW(MOTOR_BL2, 0);        F.DW(MOTOR_BR2, 0);
  F.AW(7, PWM);              F.AW(6, PWM);
}
void BB() {
  F.DW(MOTOR_FL1, 0);        F.DW(MOTOR_FR1, 0);
  F.DW(MOTOR_FL2, 1);        F.DW(MOTOR_FR2, 1);
  F.AW(9, PWM);              F.AW(8, PWM);

  F.DW(MOTOR_BL1, 0);        F.DW(MOTOR_BR1, 0);
  F.DW(MOTOR_BL2, 1);        F.DW(MOTOR_BR2, 1);
  F.AW(7, PWM);              F.AW(6, PWM);
}
void RR() {
  F.DW(MOTOR_FL1, 0);        F.DW(MOTOR_FR1, 1);
  F.DW(MOTOR_FL2, 1);        F.DW(MOTOR_FR2, 0);
  F.AW(9, PWM);              F.AW(8, PWM);

  F.DW(MOTOR_BL1, 1);        F.DW(MOTOR_BR1, 0);
  F.DW(MOTOR_BL2, 0);        F.DW(MOTOR_BR2, 1);
  F.AW(7, PWM);              F.AW(6, PWM);
}
void LL() {
  F.DW(MOTOR_FL1, 1);        F.DW(MOTOR_FR1, 0);
  F.DW(MOTOR_FL2, 0);        F.DW(MOTOR_FR2, 1);
  F.AW(9, PWM);              F.AW(8, PWM);

  F.DW(MOTOR_BL1, 0);        F.DW(MOTOR_BR1, 1);
  F.DW(MOTOR_BL2, 1);        F.DW(MOTOR_BR2, 0);
  F.AW(7, PWM);              F.AW(6, PWM);
}
void Stop() {
  F.DW(MOTOR_FL1, 0);        F.DW(MOTOR_FR1, 0);
  F.DW(MOTOR_FL2, 0);        F.DW(MOTOR_FR1, 0);

  F.DW(MOTOR_BL1, 0);        F.DW(MOTOR_BR1, 0);
  F.DW(MOTOR_BL2, 0);        F.DW(MOTOR_BR1, 0);
  F.AW(9, 0);                F.AW(8, 0);
  F.AW(7, 0);                F.AW(6, 0);
}

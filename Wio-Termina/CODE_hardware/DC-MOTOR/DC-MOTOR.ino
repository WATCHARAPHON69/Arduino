int PWM = 90;
int PWM1 = 255;
void setup() {
  // put your setup code here, to run once:
pinMode(BCM2,OUTPUT);
pinMode(BCM3,OUTPUT);
pinMode(BCM4,OUTPUT);
pinMode(BCM5,OUTPUT);
pinMode(BCM26,OUTPUT);
pinMode(BCM27,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(BCM2,HIGH);
digitalWrite(BCM3,LOW);
digitalWrite(BCM4,HIGH);
digitalWrite(BCM5,LOW);
analogWrite(BCM26,PWM);
analogWrite(BCM27,PWM1);
delay(1000);
digitalWrite(BCM2,LOW);
digitalWrite(BCM3,HIGH);
digitalWrite(BCM4,LOW);
digitalWrite(BCM5,HIGH);
analogWrite(BCM26,PWM);
analogWrite(BCM27,PWM1);
delay(1000);
}

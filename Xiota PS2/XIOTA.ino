#include <Blynk.h>
#define BLYNK_PRINT Serial*/
#define BLYNK_TEMPLATE_ID "TMPLXF-1u5tg"
#define BLYNK_TEMPLATE_NAME "IOT"
#define BLYNK_AUTH_TOKEN "v3DYvxClfiZLjlw3Jcw2BQ6rmiEaTf1g"
#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <BlynkSimpleStream.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
int HBSensor = 4;
int HBCount = 0;
int HBCheck = 0;
int TimeinSec = 0;
int HBperMin = 0;
int HBStart = 2;
int HBStartCheck = 0;
char auth[]="v3DYvxClfiZLjlw3Jcw2BQ6rmiEaTf1g";
#define kpa2atm 0.00986923267
int pressurePin = 0;
int val;
float pkPa; // pressure in kPa
float pAtm; // pressure in Atm

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Blynk.begin(Serial,auth);
  lcd.begin(20, 4);
  pinMode(HBSensor, INPUT);
  pinMode(HBStart, INPUT_PULLUP);
  pinMode(2,OUTPUT);
  pinMode (6,OUTPUT);
  Timer1.initialize(800000); 
  Timer1.attachInterrupt( timerIsr );
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Current BPM  : ");
  lcd.setCursor(0,1);
  lcd.print("Time in Sec : ");
  digitalWrite(6,LOW);
  pinMode(A0,INPUT);
  pinMode(A1,OUTPUT);
}

void loop() {
 Blynk.run();
  
  
  if(digitalRead(HBStart) == LOW){HBStartCheck = 1;}
  
  if(HBStartCheck == 1)
  {
      if((digitalRead(HBSensor) == HIGH) && (HBCheck == 0))
      {
        HBCount = HBCount + 1;
        HBCheck = 1;
      }
      if((digitalRead(HBSensor) == LOW) && (HBCheck == 1))
      {
        HBCheck = 0;   
      }
      if(TimeinSec == 10)
      {
          
          val = analogRead(A0);
          pkPa = ((float)val/(float)1023+0.095)/0.009;
          Serial.println(pkPa);
          HBperMin = HBCount * 6;
          lcd.setCursor(16,0);
          lcd.print(HBperMin);
          
          if(HBperMin<50 || pkPa>115)
          {
            digitalWrite(6,HIGH);
          }
          HBStartCheck = 0;
          lcd.setCursor(14,2);
          lcd.print(HBperMin);
          lcd.print(" ");
          lcd.setCursor(0,3);
          
          HBCount = 0;
          TimeinSec = 0;      
          digitalWrite(2,LOW);
      }
  }
}

void timerIsr()
{
  if(HBStartCheck == 1)
  {
    TimeinSec = TimeinSec + 1;
    lcd.setCursor(14,1);
    lcd.print(TimeinSec);
    lcd.print("_");
}
}

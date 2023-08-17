
#include <LiquidCrystal.h>
#define green 10
#define red 9
#define white 8
#define buzzer 11
#define gasSensor A1

int baselineTemp = 0;

int celsius = 0;

int sensval;//gas sensor

int d=200;

LiquidCrystal lcd(2,3,4,5,6,7);

/*===============================================================*/

void setup() {
  
  lcd.begin(16, 2);
  pinMode(A0,INPUT);
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(white,OUTPUT);
  pinMode(buzzer,OUTPUT);
  Serial.begin(9600);

}

/*==============================================================*/


void loop() {
  //base temperature that the sensor will start at
    baselineTemp = 26;
  
  //reading the temp in celsius from analog pin A0
  celsius = map(((analogRead(A0) - 20) * 3.04), 0, 1023, -40, 125);
  
  Serial.println(celsius);
  
  //display temp on lcd
  lcd.setCursor(0, 1);
  lcd.print(celsius);
  lcd.setCursor(4, 1);
  lcd.print("c");
  
  
  //reading gas sensor value from analog pin A1
  sensval=analogRead(gasSensor);
 
  lcd.setCursor(0,0);
  lcd.print(sensval);
  if (celsius >= baselineTemp + 20 && celsius < baselineTemp+30){  	digitalWrite(red,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(white,LOW);
    
    tone(buzzer,1000,200);
    tone(buzzer,1000,200);
    lcd.setCursor(6,1);
    lcd.print("Hot   ");
    
  }
  
if (celsius >= baselineTemp + 10 && celsius < baselineTemp+20){
  	digitalWrite(green,HIGH);
    digitalWrite(white,LOW);
    digitalWrite(red,LOW);
    lcd.setCursor(6,1);
    lcd.print("Normal");
  }
  
if (celsius < baselineTemp) {
    digitalWrite(white,HIGH);
    digitalWrite(green,LOW);
    digitalWrite(red,LOW);
    lcd.setCursor(6,1);
    lcd.print("Low   ");
  }
  
  
  
  //measuring how dense the weight of air is around the sensor
  if(sensval>700){
  	lcd.setCursor(4,0);
    lcd.print("High");
    tone(buzzer,1000,200);
    delay(d);
  }
  
  else{
    lcd.setCursor(4,0);
    lcd.print("Low ");
    noTone(buzzer);
  }
  
}
 
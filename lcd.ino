#include <Wire.h>
#include <RTClib.h>
#include"DHT.h"
#include <LiquidCrystal.h>
#define DHTTYPE DHT11
#define DHTPIN 8
#define button 7
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
RTC_DS1307 rtc;
DHT dht(DHTPIN, DHTTYPE);
float temp, humid;
int state = 0;
int Laststate = 0;
int count = 0,count1=0;
char daysOfTheWeek[7][12] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
void setup() {
  lcd.begin(16, 2);
  dht.begin();
  pinMode(button, INPUT);
  while (!Serial);
  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  DisplayTime();
}

void loop() {
  DisplayTime();
  state = digitalRead(button);
  if (state != Laststate) {
    if (state == HIGH) {
      count++;
    }
    Laststate = state;
    lcd.clear();
    if (count % 2 == 0)
    {
      lcd.clear();
      DisplayTime();
    }
    else {
      lcd.clear();
      DisplayTemp();
    }
  }
}
void DisplayTime() {
  DateTime now = rtc.now();
  char buf[] = "DD/MM/YYYY";
  lcd.setCursor(0, 0);
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
  lcd.print("   ");
  lcd.print(now.toString(buf));
  lcd.setCursor(6, 1);
  lcd.print(String(now.timestamp(DateTime::TIMESTAMP_TIME)));
}
void DisplayTemp() {
  temp = dht.readTemperature();
  humid = dht.readHumidity();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.setCursor(9, 0);
  lcd.print(temp);
  lcd.write(B11011111);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humid);
  lcd.print("%");
}

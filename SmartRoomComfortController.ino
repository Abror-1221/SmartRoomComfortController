#include <HCSR04.h>             
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN A0      //Pin Sensor Suhu & Kelembaban
#define DHTTYPE DHT11  //Tipe Sensor Suhu & Kelembaban

LiquidCrystal_I2C lcd(0x27, 16, 2); 
DHT dht(DHTPIN, DHTTYPE);

float t;        //Variabel Suhu
float h;        //Variabel Kelembaban
float d;        //Variabel Jarak
int   r = 7;    //Variabel Pin Relay
int Led1 = 6;   //Variabel Pin Lampu Tidur
int Led2 = 5;   //Variabel Pin Lampu Kamar
int stat = 0;   //Variabel nilai kondisi lampu kamar (2 = Led1 ON, Led2 OFF; 1 = Led1 OFF, Led2 ON; 0 = Led1 OFF, Led2 OFF)
HCSR04 hc(4,3); //4 Pin Trig, 3 Pin Echo pada Ultrasonic

void setup() {
  dht.begin();
  lcd.init();
  lcd.backlight();
  pinMode(r,    OUTPUT);
  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  t = dht.readTemperature(); //Membaca & menyimpan nilai suhu hasil pembacaan sensor
  h = dht.readHumidity();    //Membaca & menyimpan nilai kelembaban hasil pembacaan sensor
  d = hc.dist();             //Membaca & menyimpan nilai jarak hasil pembacaan sensor
  //digitalWrite(r, HIGH);
  kipas();

  if (d <= 4) {
    if (stat == 2) {
      stat = 1;
    } else {
      stat = 2;
    }
    if (digitalRead(Led2) == LOW || stat == 1) {
      digitalWrite(Led2, HIGH);
      digitalWrite(Led1, LOW);
      stat = 1;
    } else if (digitalRead(Led1) == LOW || stat == 2) {
      digitalWrite(Led2, LOW);
      digitalWrite(Led1, HIGH);
      stat = 2;
    }
  } else if (d <= 10 && d > 4 && (stat == 1 || stat == 2)) {
    digitalWrite(Led2, LOW);
    digitalWrite(Led1, LOW);
    stat = 0;
  }

  lcd.setCursor(0, 0);
  lcd.print("Suhu: ");
  lcd.print(t);

  lcd.setCursor(0, 1);
  lcd.print("Kelembaban: ");
  lcd.print(h);
  

  Serial.print("Jarak : ");
  Serial.println(d);
  Serial.print("Status : ");
  Serial.println(stat);
  //Serial.print(digitalRead(Led2));
  delay(500);
  lcd.clear();

}

void kipas() {
  if (t > 34) {
    digitalWrite(r, LOW);
  } else {
    digitalWrite(r, HIGH);
  }
}

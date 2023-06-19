#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <RtcDS1302.h>

#define DHTTYPE DHT11
#define DHTPIN 2
#define LEDR 11 // led rojo
#define LEDG 10 // led verde
#define LEDB 9 // led azul
float hum;
float temp;
long tAnt;
char buff[17];

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
ThreeWire myWire(4, 5, 3); // configuro IO, CLOCK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

void setup() {
    Serial.begin(9600);
    lcd.begin(); //inicializa el lcd
    lcd.backlight(); //prende las luces del display
    dht.begin();
    Rtc.Begin();
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    Rtc.SetDateTime(compiled);
}

void loop() {
    setVariables();
    printHum();
    setColorsLeds();
    printDateTime();
}

void setVariables() {
    temp = dht.readTemperature();
    hum = dht.readHumidity();
}

void printHum() {
    if (millis() - tAnt > 500) {
        tAnt = millis();
        sprintf(buff, "HUM: %d TEMP: %d", (int)hum, (int)temp);
        lcd.setCursor(0, 1);
        lcd.print(buff);
    }
}

void printDateTime() {
    RtcDateTime now = Rtc.GetDateTime();
    int dia = now.Day();
    int mes = now.Month();
    int anio = now.Year();
    int hora = now.Hour();
    int minuto = now.Minute();
    int segundo = now.Second();
    sprintf( buff, "%02d/%02d/%04d %02d:%02d", dia, mes, anio, hora, minuto);
    Serial.println(buff);
    lcd.setCursor(0, 0);
    lcd.print(buff);
}

void setColorsLeds() {
    if (temp < 20) {
        analogWrite(LEDR, 0);
        analogWrite(LEDG, 0);
        analogWrite(LEDB, 255);
    }

    if (temp < 30 && temp >= 20) {
        analogWrite(LEDR, 0);
        analogWrite(LEDG, 255);
        analogWrite(LEDB, 0);
    }

    if (temp < 30 && temp >= 40) {
        analogWrite(LEDR, 255);
        analogWrite(LEDG, 102);
        analogWrite(LEDB, 0);
    }

    if (temp < 30 && temp >= 40) {
        analogWrite(LEDR, 255);
        analogWrite(LEDG, 0);
        analogWrite(LEDB, 0);
    }
}

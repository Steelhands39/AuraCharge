#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 

int sensorPin = A0;

float maxInputVoltage = 25.0;  
float analogMax = 1023.0;

void setup() {
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Voltage Sensor");
  delay(1500);
}

void loop() {
  int sensorValue = analogRead(sensorPin);

  float voltage = (sensorValue / analogMax) * maxInputVoltage;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Voltage Sensor");

  lcd.setCursor(0, 1);
  lcd.print("Voltage: ");
  lcd.print(voltage, 3);
  lcd.print(" V");

  delay(500);
}

#include <TM1637Display.h>
const int voltageSensorPin = A3;          
float vIn;   
float vOut;
float voltageSensorVal;                   
const float factor = 512.8;               
const float vCC = 5.00;                  
TM1637Display display(2, 3);
void setup() {
  display.setBrightness(0x0f);
}

void loop() {

  voltageSensorVal = analogRead(voltageSensorPin);    
  vOut = (voltageSensorVal / 1024) * vCC;             
  vIn =  vOut * factor;
  display.showNumberDecEx(vIn, 0x40, true);

  delay(1000);
}

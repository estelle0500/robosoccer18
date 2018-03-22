#include <Wire.h>
#include <lcd.h>

LCD display;

void setup() {
  // put your setup code here, to run once:
  display.begin(0x3C);
  display.clearDisplay();
}

void loop() {
  // put your main code here, to run repeatedly:
  display.clearDisplay();
  display.println("Hello World!");
  display.display();
  delay(1000);
  display.clearDisplay();
  display.println("Bling!");
  display.display();
  delay(1000);
}

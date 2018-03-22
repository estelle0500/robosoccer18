#include "lcd.h"

void LCD::begin(uint8_t i2caddr) {
  this->Adafruit_SSD1306::begin(SSD1306_SWITCHCAPVCC, i2caddr);
  this->setTextSize(2);
  this->setTextColor(WHITE);
};

void LCD::clearDisplay() {
  this->Adafruit_SSD1306::clearDisplay();
  this->setCursor(0,0);
}

void LCD::println(char buf[]) {

}

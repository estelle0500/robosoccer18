#ifndef LCD_H
#define LCD_H

#include "Adafruit_SSD1306.h"

class LCD : public Adafruit_SSD1306 {
  /* There are MANY graphics functions for the LCD
      The only functions that are really needed are:
        - LCD() (constructor)
        - begin() (initialize with I2C address of LCD)
        - println() (works the same way as Serial.println(), but the words won't
                     display until display() is called)
        - display() (sends the data over to LCD)
        - clearDisplay() (clears the display)
  */
  public:
    LCD() : Adafruit_SSD1306(-1) {};
    void begin(uint8_t i2caddr = 0x3C);
    void clearDisplay();
};
#endif

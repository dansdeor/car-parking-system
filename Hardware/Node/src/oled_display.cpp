#include "oled_display.h"
#include "header.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define I2C_FREQUENCY 100000

TwoWire I2Cbus = TwoWire(0);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2Cbus, OLED_RESET);

void oled_display_init()
{
    I2Cbus.begin(I2C_SDA_PIN, I2C_SCL_PIN, I2C_FREQUENCY);
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println("Oled display allocation failed");
    }
}

void oled_display_show(String message)
{
    display.setTextSize(OLED_TEXT_SIZE);
    display.setTextColor(WHITE);
    display.setCursor(OLED_CURSOR_X, OLED_CURSOR_Y);
    display.print(message);
    display.display();
}

void oled_display_clear()
{
    display.clearDisplay();
    display.display();
}

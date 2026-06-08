/*
Calendar control by joystick
TFT LCD ST7735S
Analog Joystick

Wire Connection TFT + UNO
ST7735 Pin	Arduino Pin
VCC	        5V or 3.3V
GND	        GND
CS	        D10
RST	        D9
DC (A0)	D8
SDA (MOSI)	D11
SCK	        D13

Wiring for Analog Joystick + Arduino Uno
Joystick Pin	Arduino Uno Pin	Function
VCC	            5V	            Power supply
GND	            GND	            Ground
VRx	            A0	            Horizontal analog input
VRy	            A1	            Vertical analog input
SW	            D2 (optional)	Button (digital read)
*/

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// TFT and Joystick pins
#define TFT_CS   10
#define TFT_RST   9
#define TFT_DC    8
#define JOY_X     A0
#define JOY_Y     A1
#define JOY_BTN   2

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

const char* months[] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

const int daysInMonth[] = {
  31, 28, 31, 30, 31, 30,
  31, 31, 30, 31, 30, 31
};

int currentMonth = 0; // Jan = 0
int debounce = 0;

void drawCalendar(int monthIndex) {
  tft.fillScreen(ST77XX_BLACK);
  
  tft.setTextColor(ST77XX_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(20, 0);
  tft.print(months[monthIndex]);

  int days = daysInMonth[monthIndex];
  int col = 0, row = 0;
  int cellW = 20, cellH = 20;

  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);

  for (int day = 1; day <= days; day++) {
    int x = col * cellW + 10;
    int y = row * cellH + 30;

    tft.drawRect(x, y, cellW, cellH, ST77XX_WHITE);
    tft.setCursor(x + 5, y + 5);
    tft.print(day);

    col++;
    if (col == 7) {
      col = 0;
      row++;
    }
  }
}

void setup() {
  pinMode(JOY_BTN, INPUT_PULLUP);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);
  drawCalendar(currentMonth);
}

void loop() {
  int xVal = analogRead(JOY_X);

  // Debounce so month doesn't change too fast
  if (debounce == 0) {
    if (xVal < 400) {
      currentMonth = (currentMonth + 11) % 12; // Prev month
      drawCalendar(currentMonth);
      debounce = 10;
    } else if (xVal > 600) {
      currentMonth = (currentMonth + 1) % 12;  // Next month
      drawCalendar(currentMonth);
      debounce = 10;
    }
  }

  // Reset to January if button is pressed
  if (digitalRead(JOY_BTN) == LOW) {
    currentMonth = 0;
    drawCalendar(currentMonth);
    delay(300);
  }

  if (debounce > 0) debounce--;
  delay(100);
}

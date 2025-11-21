/*
()@@@@@   SS       II
           SS      II
()          SS     II
()@@@@@    SS      II
()          SS     II
()           SS    II
*/
// Editor       : Dmitriy Shumkin
// Update       : 11/12/2025
// Project name : FSI (Full Sensor Irrigation)
// File name    : FSI.ino

/*
* This code has been modified, and will change within time to provide updates, and other improvements.
* If need be published, an encryption key will be posted, in order to compare.
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include "DHT20.h"

// OLED Display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DHT20 sensor
DHT20 DHT;

//globals
uint8_t count = 0;

void setup(){
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("DHT20 LIBRARY VERSION: ");
  Serial.println(DHT20_LIB_VERSION);
  Serial.println();
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  Wire.begin();
  DHT.begin();    //  ESP32 default pins 21 22

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  delay(1000);
}

void loop(){
  DHT.read();
  ssdLCD();
  delay(500);
}

void ssdLCD(){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Temperature");
  display.println(DHT.getTemperature(), 1);
  display.println("Humidity");
  display.println(DHT.getHumidity(), 1);
  display.display();
  display.clearDisplay(); // Clear display and go onto display the soil sensor status
}

// DHT20 sensor function. Might get rid of this later, since I only want bare info to print on the LCD.
void dhtsensor(){
if (millis() - DHT.lastRead() >= 2000)
  {
    //  READ DATA
    uint32_t start = micros();
    int status = DHT.read();
    uint32_t stop = micros();

    if ((count % 10) == 0)
    {
      count = 0;
      Serial.println();
      Serial.println("Type\tHumidity (%)\tTemp (°C)\tTime (µs)\tStatus");
    }
    count++;

    Serial.print("DHT20 \t");
    //  DISPLAY DATA, sensor has only one decimal.
    Serial.print(DHT.getHumidity(), 1);
    Serial.print("\t\t");
    Serial.print(DHT.getTemperature(), 1);
    Serial.print("\t\t");
    Serial.print(stop - start);
    Serial.print("\t\t");
    switch (status)
    {
      case DHT20_OK:
        Serial.print("OK");
        break;
      case DHT20_ERROR_CHECKSUM:
        Serial.print("Checksum error");
        break;
      case DHT20_ERROR_CONNECT:
        Serial.print("Connect error");
        break;
      case DHT20_MISSING_BYTES:
        Serial.print("Missing bytes");
        break;
      case DHT20_ERROR_BYTES_ALL_ZERO:
        Serial.print("All bytes read zero");
        break;
      case DHT20_ERROR_READ_TIMEOUT:
        Serial.print("Read time out");
        break;
      case DHT20_ERROR_LASTREAD:
        Serial.print("Error read too fast");
        break;
      default:
        Serial.print("Unknown error");
        break;
    }
    Serial.print("\n");
  }
}
//Edited with <3, tears and blood.
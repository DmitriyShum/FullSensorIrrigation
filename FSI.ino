/*
FFFFFFF  SSS       II
FF      SS SS      II
FF          SS     II
FFFFFFF    SS      II
FF          SS SS  II
FF           SSS   II
*/

// Editor       : Dmitriy Shumkin
// Update       : 01/14/2026
// Project name : FSI (Full Sensor Irrigation)
// File name    : FSI.ino
// Description: This program is to be run with the combination of the Arduino UNO, sensors, LCD and relay provided in the README.md. 

/* Important to read:
* =========================
* Error cases: The code will not execute if the board isn't connected to the computer and the correct port.
* If the BAUD rate is incorrect, the code will return sets of symbols, such as "@.a$" and so on. To fix this, check to make sure the Serial.begin is set to Serial.begin(9600)
* Always check the connections if a component(s) isn't turning on or is on, but faint.
* A bad soldering job can cause many problems, including health risks, burnt out components, components connected to the wrong pins, too much solder causing wrong connections. Read the README.md, and ALWAYS follow safety procedures 
* =========================
* This code has been modified, and will change over time to provide updates, and other improvements. An SHA key is provided in the README.md file to compare to the current update.
===========================
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include "DHT20.h"

// Soil moisturizer assignments 
#define RELAY_PIN 2
#define MOISTURE_PIN A0
#define THRESHOLD 560
int value = analogRead(MOISTURE_PIN);

// OLED Display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3D // < See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DHT20 sensor
DHT20 DHT;

//global assignment for 8 bit int
uint8_t count = 0;

void setup(){
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  Serial.println(__FILE__);
  Serial.print("DHT20 LIBRARY VERSION: ");
  Serial.println(DHT20_LIB_VERSION);
  Serial.println();
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  Wire.begin();
  DHT.begin();  // ESP32 default pins 21 22

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE); // Default color is WHITE
  delay(1000);
}

void loop(){
  DHT.read();
  ssdLCD();
  delay(500);
}

// LCD Screen
void ssdLCD(){
  int value = analogRead(MOISTURE_PIN); // read the analog value from sensor
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Temperature :");
  display.println(((DHT.getTemperature()*1.8)+32), 1); // prints temperature in F
  display.println("Humidity    : "); // prints humidity index
  display.println(DHT.getHumidity(), 1);
  display.println("Soil Moisture: ");
  display.println(value);
  if(value > THRESHOLD){
    display.println("Soil sensor:\n Dry, pump=on");
    digitalWrite(RELAY_PIN, HIGH);
  }
  else{
    display.println("Soil sensor: \n Moist, pump=off");
    digitalWrite(RELAY_PIN, LOW);
  }
   delay(500);
  
  display.display();
  display.clearDisplay(); // Clear display and go onto display the soil sensor status
}

// DHT20 Sensor
void dhtsensor(){
if (millis() - DHT.lastRead() >= 2000){
    //  READ DATA
    uint32_t start = micros();
    int status = DHT.read();
    uint32_t stop = micros();

    switch (status){
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
//Edited with <3

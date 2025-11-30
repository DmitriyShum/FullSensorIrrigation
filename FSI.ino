/*
()@@@@@  SSS       II
()      SS SS      II
()          SS     II
()@@@@@    SS      II
()          SS SS  II
()           SSS   II
*/
// Editor       : Dmitriy Shumkin
// Update       : 11/30/2025
// Project name : FSI (Full Sensor Irrigation)
// File name    : FSI.ino

/*
* This code has been modified, and will change within time to provide updates, and other improvements.
* If need be published, an encryption key will be posted, in order to compare.
*/


/*GOALS: 
* print soil sensor data to the screen
* make arduino automatically reset on power. Or figure out why LCD doesnn't turn on right away.
* Try to connect the RGB LED
*  Will I have to solder relay wires to the board, or how else shall I do it?
* Make a sprinkler for widespread watering
* Connect a flyback diode for the relay?
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include "DHT20.h"

// Soil moisturizer
#define RELAY_PIN 2
#define MOISTURE_PIN A0
#define THRESHOLD 560
int value = analogRead(MOISTURE_PIN);

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
  DHT.begin();    //  ESP32 default pins 21 22

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  delay(1000);
}

void loop(){
  DHT.read();
  soil();
  ssdLCD();
  delay(500);
}

void ssdLCD(){
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Temperature :");
  display.println(((DHT.getTemperature()*1.8)+32), 1); // prints temperature in F
  display.println("Humidity    : "); // prints humidity index
  display.println(DHT.getHumidity(), 1);


   delay(500);
  
  display.display();
  display.clearDisplay(); // Clear display and go onto display the soil sensor status
}

// DHT20 sensor function. Might get rid of this later, since I only want bare info to print on the LCD.
void dhtsensor(){
if (millis() - DHT.lastRead() >= 2000){
    //  READ DATA
    uint32_t start = micros();
    int status = DHT.read();
    uint32_t stop = micros();

    /*if ((count % 10) == 0){
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
    Serial.print("\t\t");*/
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
  void soil(){
    int value = analogRead(MOISTURE_PIN); // read the analog value from sensor

  if (value > THRESHOLD) {
    Serial.print("The soil is DRY => turn pump ON");
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    Serial.print("The soil is WET => turn pump OFF");
    digitalWrite(RELAY_PIN, LOW);
  }

  Serial.print(" (");
  Serial.print(value);
  Serial.println(")");

  delay(2000);

   }
//Edited with <3, tears and blood.

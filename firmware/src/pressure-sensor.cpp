#include <Adafruit_LPS2X.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define LPS_CS 10
#define LPS_SCK 12
#define LPS_MISO 13
#define LPS_MOSI 11

Adafruit_LPS25 lps;

void pressureSetup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Adafruit LPS25 test (SPI mode)!");

  if (!lps.begin_SPI(LPS_CS, LPS_SCK, LPS_MISO, LPS_MOSI)) {
    Serial.println("Failed to find LPS25 chip");
    while (1)
      delay(10);
  };
  Serial.println("LPS25 Found!");
};

void pressureLoop() {
  sensors_event_t temp, pressure;
  lps.getEvent(&pressure, &temp);

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" °C");

  Serial.print("Pressure: ");
  Serial.print(pressure.pressure);
  Serial.println(" hPa\n");

  delay(500);
};

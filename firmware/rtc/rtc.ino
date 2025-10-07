#include <Wire.h>
#include "RTClib.h"

#define SDA_0 8
#define SCL_0 9

RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);
  delay(500);

  Wire.begin(SCL_0, SDA_0);

  Serial.println("\n[BOOT] Starting RTC test...");

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC. Check wiring!");
    while (4) delay(200);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting time to compile time.");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.println("RTC initialized successfully.");
}

void loop() {
  DateTime now = rtc.now();

  Serial.printf("%04d-%02d-%02d %02d:%02d:%02d\n",
                now.year(), now.month(), now.day(),
                now.hour(), now.minute(), now.second());

  Serial.print("Temperature: ");
  Serial.print(rtc.getTemperature());
  Serial.println(" °C\n");

  delay(1000);
}

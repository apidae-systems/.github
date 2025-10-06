/* learn.adafruit.com/adafruit-tmp117-high-accuracy-i2c-temperature-monitor/arduino */
/* dronebotworkshop.com/tmp117/ */
#include <Wire.h>
#include <Adafruit_TMP117.h>
#include <Adafruit_Sensor.h>

static const int  I2C_SDA_PIN = 8;
static const int  I2C_SCL_PIN = 9;
static const int  TMP117_INT_PIN = 10; // set to -1 if not wired
static const uint8_t CANDIDATE_ADDRS[] = { 0x48, 0x49, 0x4A, 0x4B };

Adafruit_TMP117 tmp117;
uint8_t active_addr = 0x48;

volatile bool tmp117_alert = false;
void IRAM_ATTR onTmp117Alert() { tmp117_alert = true; }

void setup() {
  Serial.begin(115200);
  delay(50);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  bool ok = false;
  for (uint8_t addr : CANDIDATE_ADDRS) {
    if (tmp117.begin(addr, &Wire)) {
      active_addr = addr;
      ok = true;
      break;
    }
  }
  if (!ok) {
    Serial.println("TMP117 not found at 0x48–0x4B on this I2C bus.");
    while (true) delay(10);
  }

  Serial.print("TMP117 detected at I2C address 0x");
  Serial.println(active_addr, HEX);

  if (TMP117_INT_PIN >= 0) {
    pinMode(TMP117_INT_PIN, INPUT_PULLUP);   // INT is open-drain, normally HIGH
    attachInterrupt(digitalPinToInterrupt(TMP117_INT_PIN),
                    onTmp117Alert, FALLING);
    Serial.println("INT pin armed (FALLING).");
  }
}

void loop() {
  sensors_event_t temp;
  tmp117.getEvent(&temp);

  const float c = temp.temperature;
  const float f = c * 9.0f / 5.0f + 32.0f;

  Serial.print("Temperature: ");
  Serial.print(c, 2); Serial.print(" °C  |  ");
  Serial.print(f, 2); Serial.println(" °F");

  if (tmp117_alert) {
    Serial.println("TMP117 ALERT interrupt detected!");
    tmp117_alert = false;
  }

  delay(1000);
}

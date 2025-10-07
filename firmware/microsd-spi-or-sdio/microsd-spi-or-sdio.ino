#include "SdFat.h"
#include "sdios.h"
#include <Arduino.h>

#define SD_CS_PIN 10
#define SD_MOSI_PIN 11
#define SD_MISO_PIN 13
#define SD_SCK_PIN 12
#define SPI_SPEED_MHZ 25

SdFat sd;
SdFile file;

SdCardFactory cardFactory;
SdCard *m_card = nullptr;
uint8_t sectorBuffer[512];

void die(const char *msg) {
  Serial.print("Error: ");
  Serial.println(msg);
  if (sd.sdErrorCode()) {
    Serial.print("SD Error Code: 0x");
    Serial.print(sd.sdErrorCode(), HEX);
    Serial.print(", Data: 0x");
    Serial.println(sd.sdErrorData(), HEX);
  }
  while (true)
    delay(1000);
}

bool quickFormat() {
  SdSpiConfig cfg(SD_CS_PIN, SHARED_SPI, SD_SCK_MHZ(SPI_SPEED_MHZ));
  m_card = cardFactory.newCard(cfg);
  if (!m_card || m_card->errorCode())
    return false;
  uint32_t sectors = m_card->sectorCount();
  ExFatFormatter exFatFormatter;
  FatFormatter fatFormatter;
  bool ok = sectors > 67108864
                ? exFatFormatter.format(m_card, sectorBuffer, &Serial)
                : fatFormatter.format(m_card, sectorBuffer, &Serial);
  return ok;
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.println("\n--- ESP32-S3 SdFat SPI Test ---");
  SPI.begin(SD_SCK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);
  Serial.println("Initializing SD card...");
  if (!sd.begin(SD_CS_PIN, SD_SCK_MHZ(SPI_SPEED_MHZ))) {
    Serial.println("Formatting SD card...");
    if (!quickFormat())
      die("Format failed");
    if (!sd.begin(SD_CS_PIN, SD_SCK_MHZ(SPI_SPEED_MHZ)))
      die("SD initialization failed after format");
  }
  Serial.println("SD card initialized successfully.");
  if (!file.open("/test.csv", O_WRONLY | O_CREAT | O_APPEND))
    die("Failed to open file for writing.");
  file.println("2025-10-06T12:00:00Z,12.34,56.78");
  file.close();
  Serial.println("Data written to /test.csv");
  if (!file.open("/test.csv", O_RDONLY))
    die("Failed to open file for reading.");
  Serial.println("\n--- Reading File Content ---");
  int c;
  while ((c = file.read()) >= 0)
    Serial.write(c);
  Serial.println("\n--- End of File ---");
  file.close();
}

void loop() {}

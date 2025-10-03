#include <Arduino.h>
#include <pas-co2-ino.hpp>
/* https://github.com/Infineon/arduino-pas-co2-sensor/tree/master */

/*
 * sensor supports 100KHz and 400KHz
 * hardware setup & pull-ups value will * also influence the i2c operation.
 * change this value to 100000 in case of communication issues
 */

#define I2C_FREQ_HZ 400000
/* demo-mode value; not recommended for long-term measurements */
/* #define PERIODIC_MEAS_INTERVAL_IN_SECONDS 10 */
/* specification value for stable operation (uncomment for
 * long-time-measurements) */
#define PERIODIC_MEAS_INTERVAL_IN_SECONDS 60L
#define PRESSURE_REFERENCE 900

/*
 * Calibration flag: set to 1 when exposing sensor to 400 ppm reference
 */
#define CALIBRATION_MODE 1
#define CALIBRATION_TARGET_PPM 400

/*
 * Create CO2 object. Unless otherwise specified,
 * using the Wire interface
 */
PASCO2Ino cotwo;

int16_t co2ppm;
Error_t err;

void setup() {
  Serial.begin(115200);
  delay(800);
  Serial.println("serial initialized");

  /* initialize i2c interface used by sensor */
  Wire.begin();
  Wire.setClock(I2C_FREQ_HZ);

  /* initialize sensor */
  err = cotwo.begin();

  if (XENSIV_PASCO2_OK != err) {
    Serial.print("initialization error: ");
    Serial.println(err);
  }
  /* set reference pressure before starting measurement */
  err = cotwo.setPressRef(PRESSURE_REFERENCE);
  if (XENSIV_PASCO2_OK != err) {
    Serial.print("pressure reference error: ");
    Serial.println(err);
  }

#if CALIBRATION_MODE
  /* perform forced recalibration at known reference concentration */
  err = cotwo.performForcedRecalibration(CALIBRATION_TARGET_PPM);
  if (XENSIV_PASCO2_OK != err) {
    Serial.print("calibration error: ");
    Serial.println(err);
  } else {
    Serial.print("calibrated to ");
    Serial.print(CALIBRATION_TARGET_PPM);
    Serial.println(" ppm");
  }
#endif

  /* measure every 60 seconds */
  err = cotwo.startMeasure(PERIODIC_MEAS_INTERVAL_IN_SECONDS);
  if (XENSIV_PASCO2_OK != err) {
    Serial.print("start measure error: ");
    Serial.println(err);
  }

  delay(1000);
}

void loop() {
  /* Wait for value to be ready. */
  delay(PERIODIC_MEAS_INTERVAL_IN_SECONDS * 1000);

  err = cotwo.getCO2(co2ppm);
  if (XENSIV_PASCO2_OK != err) {
    /* Retry in case of timing synch mismatch */
    if (XENSIV_PASCO2_ERR_COMM == err) {
      delay(600);
      err = cotwo.getCO2(co2ppm);
      if (XENSIV_PASCO2_OK != err) {
        Serial.print("get co2 error: ");
        Serial.println(err);
      }
    }
  }

  Serial.print("co2 ppm value : ");
  Serial.println(co2ppm);

  /*
   * Assuming we have some mechanism to obtain a
   * pressure reference (i.e. a pressure sensor),
   * we could compensate again by setting the new reference.
   * Here we just keep the initial value.
   */
  err = cotwo.setPressRef(PRESSURE_REFERENCE);
  if (XENSIV_PASCO2_OK != err) {
    Serial.print("pressure reference error: ");
    Serial.println(err);
  }
}

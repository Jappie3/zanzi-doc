# Sensor code
This section explains how the sensor code functions found in this [repository](https://github.com/JokerIsMyBae/FLWSB-sketches).
The repository contains various sketches with different parts of the code present in the final versions. These final versions are the `sensor_sketch_final` sketches, `noSleep` and `noSleep_GPS`. 

## Usage
To use any of the sensors, make sure you import the correct library. For the SCD, BME and SDS, head to the repository linked below, download the library as .zip and import into Arduino IDE using `Sketch > Include Library > Add .ZIP Library`. For the TinyGPS library you can use the Arduino IDE library manager, and search for `tinygps`.

## Data formatting
The data formatting is pretty well explained in the following table, which is also present in the final sketches.

| Byte nr | Name        | Sensor range         | On Node MCU | Reformat | Sensor |
  | ------- | ----------- | -------------------- | ----------- | -------- | - |
  | 0       | Error byte  | n/a                  | n/a         | n/a      | n/a |
  | 1-2     | Temperature | -40 tot 85°C         | +40 \*10     | /100 -40 | BME280 |
| 3-5     | Pressure    | 300 tot 1100 hPa     | \*100        | /100     | BME280 |
  | 6-7     | Humidity    | 0 tot 100%           | \*100        | /100     | BME280 |
  | 8-9     | Temperature | -10 tot 60°C         | +10 \*100    | /100 -10 | SCD41 |
  | 10-11   | co2         | 400 tot 5000 ppm     | \*100        | /100     | SCD41 |
 | 12-13   | Humidity    | 0 tot 95 %           | \*100        | /100     | SCD41 |
  | 14-15   | PM2.5       | 0 tot 999 μg/m       | \*10         | /10      | SDS011 |
  | 16-17   | PM10        | 0 tot 999 μg/m       | \*10         | /10      | SDS011 |
  | 18-19   | Battery V   | 0 tot 3.3V           | \*100        | /100     | n/a |
  | 20-27   | Latitude    | -90 tot 90 (float)   |             |          | GY-NEO6MV2 |
  | 28-35   | Longitude   | -180 tot 180 (float) |             |          | GY-NEO6MV2 |
  
```
If battery level equals 3.3V, this means it is between 3.3V and 4.2V

error byte
bit 4 = 1 -> GY-NEO not responding 
bit 3 = 1 -> battery lvl under 3.3V
bit 2 = 1 -> SDS not responding
bit 1 = 1 -> SCD not responding
bit 0 = 1 -> BME not responding 
```

The values get compensated on the MCU and reverse compensated in the data parser on The Things Stack. This data formatting is based on the formatting used [here](https://github.com/DaanDekoningKrekels/ArduinoCore-samd/blob/master/libraries/SAMDaaNo21/examples/).

## SCD41
The SCD41 measures CO2, temperature and humidity.

For the SCD41 we are using a forked version of the [Sensirion i²C SCD4x library](https://github.com/JokerIsMyBae/arduino-i2c-scd4x)
The original version isn't made to be used with single shot measurement, which was needed in this application. The original, and the original BME280 library, also have the delays built into the methods. This was changed to reduce the total time that is needed to complete all measurements.

The sensor code looks like this:
```c++
// Before void setup()
#include <SensirionI2CScd4x.h>
SensirionI2CScd4x scd4x;
bool scd_status = true;
uint16_t serialnr0, serialnr1, serialnr2, error, co2_scd = 0;
float temp_scd = 0.0f, hum_scd = 0.0f;

void setup() {
	Wire.begin();
	scd4x.begin(Wire);
}

void loop() {
	// Wake up SCD
	error = scd4x.wakeUp();
	if (error)
		scd_status = false;
	
	// wake-up time for scd41
	if (scd_status)
		delay(20);
	
	// check if sensor woke up
	if (scd_status)
		error = scd4x.getSerialNumber(serialnr0, serialnr1, serialnr2);
	if (error)
		scd_status = false;
	
	// Send out start measurement commands
	if (scd_status)
		error = scd4x.measureSingleShot();
	if (error)
		scd_status = false;
	
	// measureSingleShot needs delay of 5000ms to wait for measurements
	delay(5000);
	
	// check if data ready
	if (scd_status)
		error = scd4x.getDataReadyFlag(scd_status);
	if (error)
		scd_status = false;
	
	// read data
	if (scd_status)
		error = scd4x.readMeasurement(co2_scd, temp_scd, hum_scd);
	if (error)
	    scd_status = false;
	
	// turn off sensor
	scd4x.powerDown();
}
```
The error handling is to make sure none of the methods are executed in case the sensor doesn't respond in a previous step.
`getSerialNumber`, `getDataReadyFlag` and `readMeasurement` have a few variables as parameters that get passed as (non-constant) references. This way the variables are directly edited instead of using a `return`. The library only uses `return` for the error codes.

In the sketch the `delay()` after `scd4x.measureSingleShot()` looks different, to compensate for the amount of time passed in the timeout function of the BME280 sensor. 

## BME280
For the BME280 we use the [Adafruit BME280](https://github.com/JokerIsMyBae/Adafruit_BME280_Library) library, again forked (mainly for removing the delays in the methods). To be able to check how long the delay needs to be, the library uses a timeout function. That function was removed from the library and put into the sketch.
```C++
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

Adafruit_BME280 bme;
bool bme_status = true;
float temp_bme = 0.0f, pres_bme = 0.0f, hum_bme = 0.0f;
uint32_t timeout_start;

bool bmeTimeout(uint32_t& timeout_start) {
  timeout_start = millis();
  
  while (bme.isMeasuring()) {
    if ((millis() - timeout_start) > 2000)
      return false;
      
    delay(1);
  }
  return true;
}

void setup() {
	bme_status = bme.begin();
	
	if (bme_status) {
	    delay(10);  // bme startup time
	    while (bme.isReadingCalibration()) {};
	    bme.readCoefficients();
	    bme.setSampling(Adafruit_BME280::sensor_mode::MODE_FORCED);
	}
}

void loop() {
	if (bme_status) {
	    bme_status = bme.takeForcedMeasurement();
	
	    // if forced measurement has begun, time out until data is ready.
		if (bme_status)
		    bme_status = bmeTimeout(timeout_start);
	}
	
	// if bme is in forced mode and data is ready, read data registers.
	if (bme_status) {
	    temp_bme = bme.readTemperature();
	    pres_bme = bme.readPressure() / 100.0F;
	    hum_bme = bme.readHumidity();
	
	    // if the data wasn't correct it returns NaN
	    if (isnan(temp_bme) || isnan(pres_bme) || isnan(hum_bme))
			bme_status = false;
	    
	}
}
```
Compared to the SCD41 and SDS011, there is no need for a `wakeup` method, this is integrated in the `takeForcedMeasurement` method. 

## SDS011
The SDS sensor measures particulate matter, more specifically PM2.5 and PM10

The library used is the [SdsDustSensor](https://github.com/lewapek/sds-dust-sensors-arduino-library) library.
The sensor starts measuring automatically after being woken up, and for accurate measurements a delay of 30 seconds is needed. In the sketch, the delay is 25s because there is already a delay of 5s (for the SCD41) in between waking up and querying the measurements of the SDS.
```C++
#include "SdsDustSensor.h"
SdsDustSensor sds(Serial1);
bool sds_status = true;
uint16_t pm25_sds = 0, pm10_sds = 0;

void setup() {
	sds.begin();
	sds.setQueryReportingMode();
}

void loop() {
	// Wake-up sds
	sds.wakeup();

	// Delay mcu for 30s
	delay(30000);

	// request measurement results from sds
	PmResult pm = sds.queryPm();
	if (!pm.isOk()) {
		sds_status = false;
	} 
	else {
		pm25_sds = pm.pm25;
	    pm10_sds = pm.pm10;
	}
	
	// turn off sensors
	sds.sleep();
}
```
QueryReportingMode is analog to the single shot measurement of the other sensors in the sense that the sensor is put to sleep and woken up when needed and only measures when awake, and only reports measurements when queried.

## GY-NEO6MV2
For the GPS sensor we use the [TinyGPS](https://github.com/neosarchizo/TinyGPS) library.
```C++
#include <TinyGPS++.h>
TinyGPSPlus gps;
double lat, lon;
bool gps_status = true;

void gpsSetup() {
  // Construct UBX-CFG-RXM message payload
  byte payload[] = {0x02, 0x08, 0x01};

  // the complete message
  byte message[] = {0xB5, 0x62, 0x06, 0x11, 0x02, 0x08, 0x01};

  byte pm2_message[] = {
      0xB5, 0x62,  // Header
      0x06, 0x3B,  // Class and ID
      0x2C,        // Length of message (44)

      // Payload
      0x00,                    // version
      0x00,                    // reserved1
      0x00,                    // reserved2
      0x00,                    // reserved3
      0x42, 0x40, 0x0F, 0x00,  // flags
      0x00, 0x00, 0x75, 0x30,  // updatePeriod (in ms)
      0x00, 0x00, 0x00, 0x00,  // searchPeriod (in ms)
      0x7D, 0x13, 0x00, 0x00,  // gridOffset (in ms)
      0x0A, 0x00,              // onTime (in s)
      0x05, 0x00,              // minAcqTime (in s)
      0x00, 0x00,              // reserved4
      0x00, 0x00,              // reserved5
      0x00, 0x00, 0x00, 0x00,  // reserved6
      0x00, 0x00, 0x00, 0x00,  // reserved7
      0x00,                    // reserved8
      0x00,                    // reserved9
      0x00, 0x00,              // reserved10
      0x00, 0x00, 0x00, 0x00   // reserved11
  };

  // Calculate message checksum
  byte ck_a = 0, ck_b = 0;
  for (int i = 0; i < sizeof(message); i++) {
    ck_a += message[i];
    ck_b += ck_a;
  }

  // Construct full UBX-CFG-RXM message with checksum
  message[sizeof(message)] = ck_a;
  message[sizeof(message) + 1] = ck_b;

  // Calculate pm2_message checksum
  ck_a = 0, ck_b = 0;
  for (int i = 0; i < sizeof(pm2_message); i++) {
    ck_a += pm2_message[i];
    ck_b += ck_a;
  }

  // Construct full UBX-CFG-RXM message with checksum
  pm2_message[sizeof(pm2_message)] = ck_a;
  pm2_message[sizeof(pm2_message) + 1] = ck_b;

  // Send message to GPS module via UART
  for (int i = 0; i < sizeof(message); i++) {
    Serial1.write(message[i]);
  }

  delay(1000);

  // Send pm2_message to GPS module via UART
  for (int i = 0; i < sizeof(pm2_message); i++) {
    Serial1.write(pm2_message[i]);
  }
}

void setup() {
	gpsSetup();
}

void loop() {
	while (Serial1.available()) {
		gps.encode(Serial1.read());
	}

	// Print GPS data if valid
	if (gps.location.isValid()) {
		lat = gps.location.lat(); 
		lon = gps.location.lng();
	} 
	else {
		gps_status = false;
	}
}
```
In the end, we didn't implement the GPS sensor. We only started working on this sensor in the last few days, and at some point got it working. The last day however, it didn't work anymore, and because the setup is rather complex, we didn't have the time to debug the problem.

## Battery voltage
The battery voltage is read by connecting the BAT pin of the DFRobot solar power manager. Using `analogRead()` We can determine the battery voltage level.

## formatData()
The data formatting is handled in the `formatData()` function. The first part contains the error handling and compensation, where every value is replaced with the max value (e.g. 0xFFFF for a uint16_t) if the sensor status is false. Else, the measured value gets compensated for easier transmitting over LoRa.
The second part of the function maps every part of the data to the corresponding byte in the byte array that is sent over LoRa, using bit shifts.

## Delay vs MCU sleep
The final sketches do not implement MCU sleep. The sketch `Sleep-Configuration.ino`  contains code to enable the RTC and use the overflow interrupt to wake up the MCU after putting it to sleep. However, the implementation is not complete and presented issues where the bootloader would get wiped from the SAMDaaNo21. For this reason, and because we had the power budget, we decided to remove the mcu sleep code and ended up using Arduino's built-in `delay()` function. 
Future versions should use the MCU sleep capabilities to preserve power.


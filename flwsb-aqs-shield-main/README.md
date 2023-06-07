# Air quality sensor shield
Flexible LoRaWAN Sensor Board - Air quality sensors shield

# Schematic
The schematic for the sensor shield was made using the reference schematics from the datasheets of the sensors. By adding female headers we can connect the main board to the sensorboard by using a male-female shield system. 
Four mounting holes were added to mount the board in an enclosure.

![Connectorboard](assets/sensorboard.svg 'Figuur 1: Connectorboard')

# PCB
For the project we made a seperate sensorboard wich contains all the sensors we used for the project. The purpose of this connector board is to connect all sensors to the main FLWSB. The FLWSB can collect the data drom the sensors on this board and send this data over lorawan.

The board contains following sensors to measure differnent parameters of airquality: 
  - BME280 (relative humidity and temperature 3,3V - I2C)
  - SGP41 (VOC and NOx sensor 3,3V - I2C)
  - SCD41 (CO2 sensor 3,3V - I2C)
  - JST-connector for SPS30 (particulate matter sensor 5V - I2C)

Additionally some pinheaders where added to connect a GY-NEO6MV2 GPS module and a solar power manager board to provide solar power to the board.

![Connectorboard](assets/connectorboard.png 'Figuur 2: Connectorboard')

![Connectorboard](assets/connectorboard1.jpg 'Figuur 3: Connectorboard1')

To solder the SMD sensors to the board solder paste was applied to the pads on the PCB. Then the sensors were soldered with a reflow oven. Soldering the small SMD sensors was a tricky part. To get the sensors to work, we had to try more than once with several boards.
At first we tried to solder the sensors with solder paste and hot air. This wasn't easy and with only the hot air we were not able to solder the sensors properly to the board.

A little wire had to be added to the board to correct a mistake made in the PCB design. Without the wire only the SCD41 and the SPS30 sensors would work. When the wire was added the other sensor work as well.

The mistake has already been corrected in the PCB design.

# Bill of materials
| Component  | amount  |
|---|---|
| 15 pin header female | 2 |
| 4 pin header male | 2 |
| SPS30 JST 5 pin connector | 1 |
| SDC41 | 1 |
| SGP41 | 1 |
| BME280 | 1 |
| capacitor 1µF  | 2  |
| capacitor 100nF  | 4  |
| resistor 4,7Ω  | 1  |

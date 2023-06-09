# FLWSB
Flexible LoRaWAN Sensor Board - Hardware design

this project was built upon a previous iteration. You can find the full documentation of this here: https://github.com/Dacetylan/FLWSB?fbclid=IwAR1HLporIOgVgvebDwlQVPCnp9HiQH27QhrYqrgZZ_8g4kAjzjsdf1E0I3I

# Mistakes that were corrected
- In the previous version of the FLWSB the RX an TX lines were connected incorrectly to the LoraWan module. In the new version the RX and TX lines were connected correctly.
- In the previous version there was a mistake in the silkscreen. Pin "PA09" had to be "PA07". This mistake was corrected in the new version.

# Changes made to the board
- Three leds were added to the board. One led will indicate whether the power supply is active. The other two leds were added on the RX and TX lines of the LoraWan module to indicate data transmission.
- Another type of voltage regulator was used on the new version. Instead of the LD1117S33TR voltage regulator the LDL1117S33R was used in this version. The new voltage regulator has a wider range of input voltages and has a lower quiescent current.

# New schematic
![FLWSB](assets/FLWSB-SAMDaaNo21.svg 'Figuur 1: New schematic of the FLWSB')

In the schematic, you can see the new power supply led in the voltage regulator section.

![FLWSB](assets/powersupply.jpg 'Figuur 2: New power supply led')

You can also see the two added lora transmission LEDs.

![FLWSB](assets/LoraLeds.jpg 'Figuur 3: LoRa transmission LEDs')

# PCB
This is the PCB design from the 3D-view in Kicad. You can see the added power led next to the USB-C connector. The RX and TX LEDs were added on the right side next to the antenna connector.

![FLWSB](assets/FLWSB.png 'Figuur 4: 3D view of the new FLWSB version')

Below you can see the result of the soldered board. This board was fully soldered by hand. To solder the microcontroller, the "drag soldering" technique was used.

![FLWSB](assets/FLWSB_soldered.jpg 'Figuur 5: soldered FLWSB')

# Alterations to the existing SAMDaaNo21 library

https://github.com/DaanDekoningKrekels/ArduinoCore-samd/commit/4290de5de0dbcf02ff4ebfb3d7167deb2307ba0b

In this link you can see the changes made to work around the problem of wrongly connecting the LoRa module to the Micro controller. RX was connected to RX and TX to TX so they scratched the connections and rewired them to different pins. 
The board that we worked with (Version 2) does have the LoRa-module connected correctly. So the alterations in the above link needed to be reversed. 
The original library was forked and edited.

result: https://github.com/JazzyBee01/ArduinoCore-samd

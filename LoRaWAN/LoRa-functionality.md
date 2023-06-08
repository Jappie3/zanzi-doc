## Adding LoRa functionality to the board
Our SAMDaaNo can communicate over LoRaWAN using it’s on board RN2483 LoRa-module. The CPU on the SAMDaaNo21 is connected with this module using a serial connection (UART). The CPU and the module can communicate by sending strings and bytes to each other. The RN2483 is operated by sending commands in the form of strings. These commands are predefined and can be found in RN2483 LoRa TM Technology Module Command Reference User’s guide.

## Library 
Instead of sending the “Raw commands” our selves directly from the code, we use the RN2483 library by jpmeijers. This library handles all the communication with the LoRa-module. It includes methods for initialization, authentication and transmission. You can find and download this library as zip from: 

https://github.com/jpmeijers/RN2483-Arduino-Library

After you have downloaded the ZIP you can add this library manually in the Arduino IDE with sketch > include library > add .ZIP library…

Once you have included the library you will have access to a few example sketches through File > Examples > RN2xx3 Arduino Library. You will find it under the section custom libraries.

We have made use of the ArduinoUnoNano-basic example sketch and made a few alterations. Instead of software serial we use Serial2. The Arduino nano has only one hardware serial interface which is already in use for communication with a laptop. So in the example sketch they use a software serial interface to connect to the lora module. However in our case we are using a SAMDaaNo21 which has some extra hardware serial interfaces and our onboard LoRa-module is hardwired to Serial2.

We altered the code to use OTAA (Over The Air Authentication) instead of ABP (Activated By Personalization). We tried ABP before but we had authentication problems after power cycling the board. This was likely due to an unmatching counter. When a device is registered in the TTN to use ABP, there is a counter that both the end device and the server need to know. Similar to an Remote Control car key. This is for security because otherwise the signal could be intercepted and copied to act like the end device. 

When the board is powercycled, the counter is likely reset and does not match the counter of the server. For this reason all messages are rejected by the server. 

By switching over to OTAA this issue was solved. The resulting code for LoRaWAN tests was the following:

### LoRa-send-every-minute.ino

#### 1.	Include the necessary library and set used variables. 
The appEui (which is also called JoinEui) is set to 0x0. This is the default when no other appEui is provided by the manufacturer. The appKey is specific to the application in The Thing Network. It is used by the application to decrypt messages and it is strongly recommended to make it unique for every end-device. 

![afbeelding](https://github.com/Jappie3/zanzi-doc/assets/91837988/179ce4a5-b392-4517-bc65-9949d02d9da7)

#### 2. Setup
![afbeelding](https://github.com/Jappie3/zanzi-doc/assets/91837988/6a1b3532-b81d-4e57-a2b6-332ae8dbd5b0)

#### 3.	Define initialize_radio()
#### 4.	Loop()
#### 5.	Define led_on() and led_off()

# Practical guide

## Writing the bootloader to a SAMDaaNo21
...
## Connecting the SAMDaaNo21 to the arduino IDE
...

## Adding a SAMDaaNo To The Things Network
Step 1: Go to the application where you want to add your board. If you don’t have an application linked to your account, make one.

Step 2: Click on register a device. 

![afbeelding](https://github.com/Jappie3/zanzi-doc/assets/91837988/f6fb200f-c5c3-499a-a0ab-25645f7850c3)
Step 3: Choose the following configurations and click on confirm next to the JoinEUI.

![afbeelding](https://github.com/Jappie3/zanzi-doc/assets/91837988/030bd5fb-3ab5-41c9-9bd0-b9157fd71f7f)

Step 4: Press generate next to AppKey. This key should be unique to every device.
![afbeelding](https://github.com/Jappie3/zanzi-doc/assets/91837988/796d0f0b-cc7a-4948-ba69-ba093d988037)

Step 5: Replace the example AppKey with your own generated AppKey in the Arduino sketch.
![afbeelding](https://github.com/Jappie3/zanzi-doc/assets/91837988/026b5c96-d26e-47b3-a823-c48d91192a3d)

Step 6: If you don’t know the devEUI of your RN2483 LoRa module present on the SAMDaaNo21, make sure your sketch prints the devEUI in the serial monitor. (This is done in the example code)

Step 7: Upload your sketch. And take note of your devEUI.
![afbeelding](https://github.com/Jappie3/zanzi-doc/assets/91837988/0014b9c0-b901-43b5-8bc4-5bb9732fb90a)

Step 8: Add your devEUI to your device registration in the things network. And press “Register end device”.
![afbeelding](https://github.com/Jappie3/zanzi-doc/assets/91837988/d538e42c-9a14-4b84-98de-7e8a1f18010c)

You will be taken to your device’s page where you can see the incoming data.

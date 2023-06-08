# Practical guide

## Writing the bootloader to a SAMDaaNo21
...
## Connecting the SAMDaaNo21 to the arduino IDE
...

## Adding a SAMDaaNo To The Things Network
Step 1: Go to the application where you want to add your board. If you don’t have an application linked to your account, make one.

Step 2: Click on register a device. 
![[Pasted image 20230608085416.png]]
Step 3: Choose the following configurations and click on confirm next to the JoinEUI.
![[Pasted image 20230608085459.png]]
Step 4: Press generate next to AppKey. This key should be unique to every device.
![[Pasted image 20230608085521.png]]
Step 5: Replace the example AppKey with your own generated AppKey in the Arduino sketch.
![[Pasted image 20230608085534.png]]
Step 6: If you don’t know the devEUI of your RN2483 LoRa module present on the SAMDaaNo21, make sure your sketch prints the devEUI in the serial monitor. (This is done in the example code)

Step 7: Upload your sketch. And take note of your devEUI.
![[Pasted image 20230608085600.png]]
Step 8: Add your devEUI to your device registration in the things network. And press “Register end device”.
![[Pasted image 20230608085621.png]]
You will be taken to your device’s page where you can see the incoming data.
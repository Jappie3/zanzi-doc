# Trouble shooting
## Uploading
### Device not found by Arduino IDE
![afbeelding](https://github.com/Jappie3/zanzi-doc/assets/91837988/0562e9e3-f2d1-4f21-8458-44f623f52bc5)

If there is no COM port to select it is possible you need to rewrite the bootloader on the device. Check writing the bootloader in the documentation.
[Flashing the bootloader](https://github.com/Jappie3/zanzi-doc/blob/sensor-programming/flwsb-code/programming-setup.md#setting-up-the-samdaano21)

### Exit status 1
![afbeelding](https://github.com/Jappie3/zanzi-doc/assets/91837988/554b847b-2e7b-467c-8d8c-2eaad8ebd08a)

In case of this error, power cycling the board should fix the issue. Either by pressing the reset button or disconnecting and reconnecting the board.

## The Things Network

### Unable to join + MIC mismatch
![afbeelding](https://github.com/Jappie3/zanzi-doc/assets/91837988/e7658d42-8563-4aca-882a-7f0f49fcc525)

In this case the keys are not correct. Change the AppKey in the Arduino sketch to match the AppKey in the application on The Things Network.
[editing an app key](https://github.com/Jappie3/zanzi-doc/blob/LoRaWAN/practical-guide/things-network.md#step-4-press-generate-next-to-appkey-this-key-should-be-unique-to-every-device)

# Setup process
## Setting up the SAMDaaNo21
To flash the bootloader on the SAMDaaNo21 you need an ATMEL-ICE. The SAMDaaNo21 will be connected to the programmer via SWD.

### OpenOCD
- Install OpenOCD on your PC. Use your package manager if you're on linux (see [openocd.org](https://openocd.org/pages/getting-openocd.html)). Below is a more detailed explanation for Windows
#### Windows (64-bit)
1. Go to [msys2.org](https://www.msys2.org) and follow the steps for installation. (You do not need the gcc compiler for this project.)
2. Start the program called `MSYS2 MINGW64`
3. Execute the following command: 
``` 
pacman -S mingw-w64-x86_64-openocd 
```
This will install OpenOCD inside the `MINGW64` terminal. Use this terminal every time you need to use OpenOCD

### Flashing the bootloader
1. Connect the SAMDaaNo21 as well as the ATMEL-ICE via USB to your PC. 
2. Connect the SAMDaaNo21 to the ATMEL-ICE 

| Atmel Ice (SAM port) | Atmel pin n° | SAMDaaNo21 pin |
| --------------------- | ------------- | -------------- |
| SWDCLK | 4 | SWCLK | 
| SWDIO | 2 | SWDIO |
| nSRST | 10 | RESET |
| VTG | 1 | 3V3 |
| GND | 3 | GND |

3. Clone this github [ArduinoCore-samd](https://github.com/DaanDekoningKrekels/ArduinoCore-samd/) to any convenient location on your machine
```
   git clone https://github.com/DaanDekoningKrekels/ArduinoCore-samd.git
```
4. Navigate to the cloned repository, to the `bootloaders/samdaano21` directory while you are in the `MINGW64` terminal
5. Execute the following command: 
```
openocd -f program-bootloader.cfg
```
This config file contains the necessary steps to flash the bootloader to the SAMDaaNo21. In the list of  devices it should show up as a 'Bossa program port'
A more detailed step-by-step guide of the OpenOCD config is described in the [FLWSB Project docs](https://dacetylan.github.io/FLWSB/#/embedded-programming/bare-metal?id=programmeren)

## Setting up Arduino IDE
### Adding the SAMDaaNo21
This guide references the install steps described [here](https://github.com/DaanDekoningKrekels/ArduinoCore-samd/#developing) and [here](https://github.com/mattairtech/ArduinoCore-samd#mattairtech-dlc-core-installation)
1. Clone this repo: [ArduinoCore-API](https://github.com/arduino/ArduinoCore-API) to any convenient location on your machine
```
git clone https://github.com/arduino/ArduinoCore-API.git
```
2. Create a `hardware/arduino-git` directory in the `<SKETCHBOOK>` directory of Arduino IDE and navigate to this directory
```
cd <SKETCHBOOK>/hardware/arduino-git
``` 
3. Clone this repo: [ArduinoCore-SAMD](git clone https://github.com/DaanDekoningKrekels/ArduinoCore-samd.git samd) into the `arduino-git` directory
```
git clone https://github.com/DaanDekoningKrekels/ArduinoCore-samd.git
``` 
4. Open Arduino IDE and click `File > Preferences`
5. Click the button next to `Additional Boards Manager URLs`
6. Copy and paste the following URL: [https://www.mattairtech.com/software/arduino/package_MattairTech_index.json](https://www.mattairtech.com/software/arduino/package_MattairTech_index.json)
7. Save preferences, then open the Boards Manager
8. Install the Arduino SAMD Boards package. Use version 1.6.2 or higher.
9. Install the MattairTech SAM D|L|C Core for Arduino package and close Boards Manager after install has finished.
Attention: this concludes the setup for Arduino IDE, but it might still not work. This can be caused because the wrong version of Bossac is being used. Bossac is the programming utility tool that Arduino IDE uses to upload to the SAMDaaNo21. The installed version of Bossac should be MattairTech's fork of bossac that supports the SAMD21G16A. 
If this is not the case, get the bossac executable from MattairTech and replace the one in your Arduino IDE installation. This is located under `C:\Program files (x86)\Arduino15\` on Windows and under `/home/<username>/.arduino15` on Linux. 
Then continue to `packages/arduino/tools/bossac/` and replace the bossac utility with the version from Mattairtech.

## PlatformIO
This section describes the work done to add the SAMDaaNo21 to PlatformIO. However, it is not finished. There is a bug where PlatformIO can't find the device, because the SAMDaaNo21 gets recognised as two different devices: 'Bossa Program Port' and 'SAMDaaNo21 (Native USB Port)'. During upload, the SAMDaaNo21 goes from being recognised as the latter, to being recognised as the former. Those two have different hardware IDs which makes it so PlatformIO can't recognise the device and fails to upload.

### Adding SAMDaaNo21 to PlatformIO
1. In PlatformIO, create a new project with the `Arduino Zero (USB Native Port)` board
2. Navigate to the `.platformio` directory, to `platforms > atmelsam > boards` and create a file called `samdaano21.json`
3. In `samdaano21.json` paste the following:
```
{
	"build": {
		"arduino": {
			"ldscript": "flash_with_bootloader.ld"
		},
		"core": "arduino",
		"cpu": "cortex-m0plus",
		"extra_flags": "-D__SAMD21G16A__",
		"f_cpu": "48000000L",
		"hwids": [
			[
				"0x03EB",
				"0x2402"
			]
		],
		"mcu": "samd21g16a",
		"usb_product": "SAMDaaNo21",
		"variant": "samdaano21"
	},
	"frameworks": [
		"arduino"
	],
	"name": "SAMDaaNo21 (USB Native Port)",
	"upload": {
		"disable_flushing": true,
		"maximum_ram_size": 8192,
		"maximum_size": 65536,
		"native-usb": true,
		"protocol": "sam-ba",
		"protocols": [
			"sam-ba"
		],
		"require_upload_port": true,
		"use_1200bps_touch": true,
		"wait_for_upload_port": true
	},
	"url": "https://www.unknown.com",
    "vendor": "Custom"
}
``` 

4. Clone this [repository](https://github.com/DaanDekoningKrekels/ArduinoCore-samd/) if you haven't already: 
``` 
git clone https://github.com/DaanDekoningKrekels/ArduinoCore-samd.git
``` 
5. In the `.platformio` folder, navigate to `framework-arduino-samd` and place the contents of the ArduinoCore-samd repository in the directory
6. Download the bossac tool from one of these links (whichever is applicable): 
	- [Windows (32 & 64-bit)](https://www.mattairtech.com/software/arduino/bossac-1.7.0-mattairtech-2-mingw32.tar.gz)
	- [Linux 64-bit](https://www.mattairtech.com/software/arduino/bossac-1.7.0-mattairtech-2-x86_64-linux-gnu.tar.gz)
	- [Linux 32-bit](https://www.mattairtech.com/software/arduino/bossac-1.7.0-mattairtech-2-i686-linux-gnu.tar.gz)
	- [MacOS](https://www.mattairtech.com/software/arduino/bossac-1.7.0-mattairtech-2-x86_64-apple-darwin.tar.gz)
7. Extract the executable
8. Navigate to `.platformio > packages > tool-bossac` and replace the bossac executable with the one you downloaded
9. In the PlatformIO project, open the `platformio.ini` file
10. Edit the contents so it looks like this:
```
   [env:samdaano21]	
   platform = atmelsam
   board = samdaano21
   framework = arduino
   
   board_build.variants_dir = C:\folders\github-repos\ArduinoCore-samd\variants
   board_build.variant = samdaano21
   
   platform_packages =
    framework-arduino-samd@file://C:\folders\github-repos\ArduinoCore-samd
    tool-bossac@file://C:\folders\vakken\iot-project\bossac-versions\tool-bossac
```
After this you should be able to upload until the error where it's unable to find the correct device
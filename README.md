# CFA800480Ex-050Sx EVE Demo Code

This example Seeeduino (Arduino clone) code is for the Crystalfontz line of 5" displays powered by a Bridgetek EVE chip. These displays are driven by the powerful BT817 chip and are available in capacitive touch, resistive touch, and non-touch. They all come with a sunlight readable backlight that boasts 1000 nits when no additional layer is on top of the display. Please refer to the datasheets for more information.

For more information on our full list of EVE displays, please click [here](https://www.crystalfontz.com/products/eve-accelerated-tft-displays.php)

## Display information
5" EVE Displays\
[CFA800480E3-050SW](https://www.crystalfontz.com/product/cfa800480e3050sw) - Capacitive Touch\
[CFA800480E3-050SC](https://www.crystalfontz.com/product/cfa800480e3050sc) - Capacitive Touch With Overhanging Glass\
[CFA800480E3-050SR](https://www.crystalfontz.com/product/cfa800480e3050sr) - Resistive Touch\
[CFA800480E3-050SN](https://www.crystalfontz.com/product/cfa800480e3050sn) - Non-Touch


 
Full Functional Seeeduino Demo Kits for these products can be found here:  
[CFAF800480E1-050SC-KIT](https://www.crystalfontz.com/product/cfa800480e3050sckit) - Capacitive Touch Dev Kit\
[CFAF800480E2-050SC-KIT](https://www.crystalfontz.com/product/cfa800480e3050sckit) - Capacitive Touch With Overhanging Glass Dev Kit\
[CFAF800480E1-050SR-KIT](https://www.crystalfontz.com/product/cfa800480e3050srkit) - Resistive Touch Dev Kit\
[CFAF800480E1-050SN-KIT](https://www.crystalfontz.com/product/cfa800480e3050snkit) - Non-Touch Dev Kit

## Navigating the Code

To toggle on or off different demonstrations, some defines in "CFA10100_defines.h" can be changed.

```c++
#define BMP_DEMO             (0)  
#define   BMP_SCROLL         (0)  
#define SOUND_DEMO           (0)  
#define   SOUND_VOICE        (0)  
#define   SOUND_PLAY_TIMES   (10)
#define LOGO_DEMO            (1)  
#define   LOGO_PNG_0_ARGB2_1 (1)  
#define BOUNCE_DEMO          (1)  
#define MARBLE_DEMO          (0)  
#define TOUCH_DEMO           (0)
#define VIDEO_DEMO           (0) 
```

`BMP_DEMO` - Toggled to 1 will look to the uSD card to pull the "SPLASH.RAW" file and display it \
`BMP_SCROLL` - Toggled to 1 will look to the uSD card to pull the "CLOUDS.RAW" file and display it scrolling accross the screen\
`LOGO_DEMO` - Toggled to 1 will display the Crystalfontz Logo from flash\
`BOUNCE_DEMO` - Toggled to 1 will show a ball bouncing around the screen\
`MARBLE_DEMO` - Toggled to 1 will look in the uSD card and pull "BLUEMARB.RAW" and demonstrate the earth rotating and bouncing around in screen in place of the ball\
`TOUCH_DEMO` - Toggled to 1 will enable the touch screen (only compatible on touch versions of the display)\
`VIDEO_DEMO` - Toggled to 1 will enable the video playback. The video must already be programmed into flash by using PROGRAM_FLASH_FROM_USD (Ice_400.avi)

## Flash Usage
Creating the files:
1. Open [EVE Asset Builder](https://brtchip.com/eab/)
2. In the Image Converter tool, use the "ADD" button to load the image
3. Set the output format to "COMPRESSED_RGBA_ASTC_8x8_KHR" and ASTC Preset to "thorough"
4. Click Convert and rename the extension of the file to .a8z
5. The file names are hard coded in the firmware. These names can be found starting on line 104 of demos.cpp

Configuring the demo code:
1. Load the files on an SD card and connect to the Seeeduino (see pinout)
2. Set PROGRAM_FLASH_FROM_USD to 1
3. Compile and upload the firmware
4. Watch the serial monitor, the code will export the memory locations that are needed for the demo code to work
5. Take the macros that are exported from the code and insert them on line 92 of CFA10100_defines.h
6. Set PROGRAM_FLASH_FROM_USD to 0
7. Set VIDEO_DEMO to 1 
8. Compile and upload the code

Here is an example of the macros that will exported from the code:\
#define FLASH_SECTOR_MARBLE (1UL)\
#define FLASH_LENGTH_MARBLE (14400UL) // sectors: 3\
#define FLASH_SECTOR_SPLASH (5UL)\
#define FLASH_LENGTH_SPLASH (153600UL) // sectors: 37\
#define FLASH_SECTOR_CLOUDS (43UL)\
#define FLASH_LENGTH_CLOUDS (153600UL) // sectors: 37\
#define FLASH_SECTOR_ICE_FPV_512x300 (81UL)\
#define FLASH_LENGTH_ICE_FPV_512x300 (12882892UL) // sectors: 3145

For an in-depth guide to loading custom images on our EVE lineup of displays without an SD card (using the Seeeduino's flash memory), please refer to our blog post [available here](https://www.crystalfontz.com/blog/custom-images-on-eve-displays/).

## Connection Details
#### To [CFA10098 Adapter Board](https://www.crystalfontz.com/product/cfa10098) (See kits above)
| 10098 Pin         | Seeeduino Pin| Connection Description |
|-------------------|--------------|------------------------|
| 1  (3v3)          | 3v3          | +3.3V Power            |
| 2  (GND)          | GND          | Ground                 |
| 3  (SCK)          | D13          | Serial Clock           |
| 4  (MOSI/D0)      | D11          | MOSI/D0                |
| 5  (MISO/D1)      | D12          | MISO/D1                |
| 6  (GPIO0/D2)     | DNC          | GPIO0/D2               |
| 7  (GPIO1/D3)     | DNC          | GPIO1/D2               |
| 8  (GND)          | GND or DNC   | Ground                 |
| 9  (CS)           | D9           | Chip Select            |
| 10 (INT)          | D7           | Interupt               |
| 11 (PD)           | D8           | Chip Power Down        |
| 12 (MODE/GPIO2)   | DNC          | MODE/GPIO2             |
| 13 (AUDIO)        | DNC          | Audio PWM              |
| 14 (GND)          | GND or DNC   | Ground                 |


#### To Optional [uSD Adapter Board](https://www.crystalfontz.com/product/cfa10112) 
| microSD Pin | Seeeduino Pin| Connection Description |
|-------------|--------------|------------------------|
| 1 (CS)      | D10          | SD CS                  |
| 2 (DI)      | D11          | SD MOSI                |
| 3 (DO)      | D12          | SD MISO                |
| 4 (VDD)     | 3v3          | +3.3V Power            |
| 5 (SCLK)    | D13          | SD SCLK                |
| 6 (VSS)     | GND          | Ground                 |

## Additional Accessories
Additional accessories for the products can be found at the bottom of each of the product pages. This will include 30 position FFC cables, wires, and any accessory boards that are available.

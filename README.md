# CFAF800480Ex-050Sx EVE Demo Code

This example Seeeduino (Arduino clone) code is for the Crystalfontz line of 5" displays powered by a Bridgetek EVE chip. These displays are driven by the powerful BT817 chip and are available in capacitive touch, resistive touch, and non-touch. They all come with a sunlight readable backlight that boasts 1000 nits when no additional layer is on top of the display. Please refer to the datasheets for more information.

For more information on our full list of EVE displays, please click [here](https://www.crystalfontz.com/products/eve-accelerated-tft-displays.php)

## Display information
5" EVE Displays\
[CFAF800480E1-050SC-A2](https://www.crystalfontz.com/product/cfaf800480e1050sca2) - Capacitive Touch\
[CFAF800480E2-050SC-A2](https://www.crystalfontz.com/product/cfaf800480e2050sca2) - Capacitive Touch With Overhanging Glass\
[CFAF800480E1-050SR-A2](https://www.crystalfontz.com/product/cfaf800480e1050sra2) - Resistive Touch\
[CFAF800480E1-050SN-A2](https://www.crystalfontz.com/product/cfaf800480e1050sna2) - Non-Touch


 
Full Functional Seeeduino Demo Kits for these products can be found here:  
[CFAF800480E1-050SC-A2-2](https://www.crystalfontz.com/product/cfaf800480e1050sca22) - Capacitive Touch Dev Kit\
[CFAF800480E2-050SC-A2-2](https://www.crystalfontz.com/product/cfaf800480e2050sca22) - Capacitive Touch With Overhanging Glass Dev Kit\
[CFAF800480E1-050SR-A2-2](https://www.crystalfontz.com/product/cfaf800480e1050sra22) - Resistive Touch Dev Kit\
[CFAF800480E1-050SN-A2-2](https://www.crystalfontz.com/product/cfaf800480e1050sna22) - Non-Touch Dev Kit

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

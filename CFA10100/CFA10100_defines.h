#ifndef __CFA10100_DEFINES_H__
#define __CFA10100_DEFINES_H__
//============================================================================
//
// Definitions specific to the EVE accelerated CFA10100 board.
// (cap-touch) https://www.crystalfontz.com/product/ . . .
//
// 2020-08-05 Brent A. Crosby / Crystalfontz America, Inc.
// https://www.crystalfontz.com/products/eve-accelerated-tft-displays.php
//===========================================================================
//This is free and unencumbered software released into the public domain.
//
//Anyone is free to copy, modify, publish, use, compile, sell, or
//distribute this software, either in source code form or as a compiled
//binary, for any purpose, commercial or non-commercial, and by any
//means.
//
//In jurisdictions that recognize copyright laws, the author or authors
//of this software dedicate any and all copyright interest in the
//software to the public domain. We make this dedication for the benefit
//of the public at large and to the detriment of our heirs and
//successors. We intend this dedication to be an overt act of
//relinquishment in perpetuity of all present and future rights to this
//software under copyright law.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//OTHER DEALINGS IN THE SOFTWARE.
//
//For more information, please refer to <http://unlicense.org/>
//============================================================================
// First we need to define the particular EVE chip that we have.
#define EVE_DEVICE           (BT817)
// Now that the device is set, we can include the master (monster) EVE
// definitions header:
#include "EVE_defines.h"
//============================================================================
// You can enable/disable the different demos here.
// There is not enough RAM_G space to hold all of the scrolling background,
// logo, audio, and blue marble at the same time. However with the BT817,
// we can load elements into the flash from the uSD (once) and then we
// do not need the uSD any more. Elements could also be loaded into the
// flash by using some PC host and the Eve Asset Builder tools provided by
// Bridgetek. Some elements can then be displated directly from flash, other
// elements (notably the rotating blue marble) need to be copied to the
// faster RAM_G before dispalying them.
//
// You would want to set this once, with the uSD card loaded with the 
// appropriate files to program those images into the flash:
// 
// bluemarb.a8z      12,665
// splash.a8z       135,852
// cloud.a8z        132,455
// Ice_400.avi   12,882,892
//
// Once that step is complete, the uSD card should not be needed and may be removed.
//
#define PROGRAM_FLASH_FROM_USD (0)  //uses uSD
//
//
//
#if (0 == PROGRAM_FLASH_FROM_USD)
// Some combinations of demos and debug messages may overflow the
// Seeeduino / Arduino flash. The symptom will be a programming error
// from AVRdude.
//
#define BMP_DEMO             (1)  //Images must already be programmed into
                                  //flash by using PROGRAM_FLASH_FROM_USD
#define   BMP_SCROLL         (1)  //1=scrolling background (cloud.a8z)
                                  //0=static image (splash.a8z)
#define SOUND_DEMO           (0)  //Uses uSD
#define   SOUND_VOICE        (1)  //1=VOI_8K.RAW, 0=MUS_8K.RAW
#define   SOUND_PLAY_TIMES   (1)
#define LOGO_DEMO            (1)  //Rotating logo (the PNG or ARGB image
                                  //data is stored in the Seeeduino's flash)
#define LOGO_PNG_0_ARGB2_1   (1)  //Compressed ARGB is 5408 bytes smaller
#define BOUNCE_DEMO          (0)  //Ball-and-rubber-band demo.
#define MARBLE_DEMO          (1)  //Marble must already be programmed into
                                  //flash by using PROGRAM_FLASH_FROM_USD
                                  //(bluemarb.a8z)
#define TOUCH_DEMO           (1)
#define VIDEO_DEMO           (0)  //Video must already be programmed into
                                  //flash by using PROGRAM_FLASH_FROM_USD
                                  //(Ice_400.avi)
//
// These should have been copied from the debug console when
// you built with PROGRAM_FLASH_FROM_USD set
#define FLASH_SECTOR_MARBLE (1UL)
#define FLASH_LENGTH_MARBLE (14400UL) // sectors: 3
#define FLASH_SECTOR_SPLASH (5UL)
#define FLASH_LENGTH_SPLASH (153600UL) // sectors: 37
#define FLASH_SECTOR_CLOUDS (43UL)
#define FLASH_LENGTH_CLOUDS (153600UL) // sectors: 37
#define FLASH_SECTOR_ICE_FPV_512x300 (81UL)
#define FLASH_LENGTH_ICE_FPV_512x300 (12882892UL) // sectors: 3145
//Total sectors = 4096, free sectors = 869
//Total flash = 16777216, free flash = 3559424
//
#else // (0 == PROGRAM_FLASH_FROM_USD)
//Minimal demos when programming the EVE flash so the 
//Seeeduino / Arduino flash does not overflow
#define BMP_DEMO             (0)  //Images must already be programmed into
                                  //flash by using PROGRAM_FLASH_FROM_USD
#define   BMP_SCROLL         (0)  //1=scrolling background (cloud.a8z)
                                  //0=static image (splash.a8z)
#define SOUND_DEMO           (0)  //Uses uSD
#define   SOUND_VOICE        (1)  //1=VOI_8K.RAW, 0=MUS_8K.RAW
#define   SOUND_PLAY_TIMES   (1)
#define LOGO_DEMO            (1)  //Rotating logo (the PNG or ARGB image
                                  //data is stored in the Seeeduino's flash)
#define LOGO_PNG_0_ARGB2_1   (0)  //Compressed ARGB is 5408 bytes smaller
#define BOUNCE_DEMO          (1)  //Ball-and-rubber-band demo.
#define MARBLE_DEMO          (0)  //Marble must already be programmed into
                                  //flash by using PROGRAM_FLASH_FROM_USD
                                  //(bluemarb.a8z)
#define TOUCH_DEMO           (0)
#define VIDEO_DEMO           (0)  //Video must already be programmed into
                                  //flash by using PROGRAM_FLASH_FROM_USD
                                  //(Ice_400.avi)
#endif // (0 == PROGRAM_FLASH_FROM_USD)
//============================================================================
//
// You can set DEBUG_LEVEL to:
//   DEBUG_NONE   (0K flash, serial not used)
//   DEBUG_STATUS (1.4K ~ 2.5K flash)
//   DEBUG_GEEK   (6.4K ~ 7K flash)
// to send messages to the console (Ctrl+Shift+M) in the arduino IDE.
#define DEBUG_LEVEL (DEBUG_GEEK)
//============================================================================
// Turn on uSD code if one of the demos above uses it, or we are programming
// the flash from the uSD.
#if ((0 != SOUND_DEMO) || (0 != BMP_DEMO) || (0 != MARBLE_DEMO) || (0 != PROGRAM_FLASH_FROM_USD))
  #define BUILD_SD           (1)
#else
  #define BUILD_SD           (0)
#endif
//============================================================================
// Wiring for Seeeduino v4.2 (3.3v) connected to CFA10098 breakout for testing.
//   ARD      | Port | 10098/EVE           | Color
// -----------+------+---------------------|--------
//  #3/D3     |  PD3 | DEBUG_LED           | Green W/W
//  #7/D7     |  PD7 | EVE_INT             | Purple
//  #8/D8     |  PB0 | EVE_PD_NOT          | Blue
//  #9/D9     |  PB1 | EVE_CS_NOT          | Brown
// #10/D10    |  PB2 | SD_CS_NOT           | Grey
// #11/D11    |  PB3 | MOSI (hardware SPI) | Yellow
// #12/D12    |  PB4 | MISO (hardware SPI) | Green
// #13/D13    |  PB5 | SCK  (hardware SPI) | orange

//Arduino style pin defines
// Interrupt from EVE to Arduino - input, not used in this example.
#define EVE_INT     (7)
// PD_N from Arduino to EVE - effectively EVE reset
#define EVE_PD_NOT  (8)
// SPI chip select - defined separately since it's manipulated with GPIO calls
#define EVE_CS_NOT  (9)
// Reserved for use with the SD card library
#define SD_CS       (10)
// Debug LED, or used for scope trigger or precise timing
#define DEBUG_LED   (3)

//Faster direct port access (specific to AVR)
#define CLR_EVE_PD_NOT        (PORTB &= ~(0x01))
#define SET_EVE_PD_NOT        (PORTB |=  (0x01))
#define CLR_EVE_CS_NOT        (PORTB &= ~(0x02))
#define SET_EVE_CS_NOT        (PORTB |=  (0x02))
#define CLR_SD_CS_NOT         (PORTB &= ~(0x04))
#define SET_SD_CS_NOT         (PORTB |=  (0x04))
#define CLR_MOSI              (PORTB &= ~(0x08))
#define SET_MOSI              (PORTB |=  (0x08))
#define CLR_MISO              (PORTB &= ~(0x10))
#define SET_MISO              (PORTB |=  (0x10))
#define CLR_SCK               (PORTB &= ~(0x20))
#define SET_SCK               (PORTB |=  (0x20))
#define CLR_DEBUG_LED         (PORTD &= ~(0x08))
#define SET_DEBUG_LED         (PORTD |=  (0x08))
//============================================================================
// These defines describe the circuit board and EVE accelerator. Typically
// you will want to use these directly and not mess with them.
#define EVE_CLOCK_SOURCE     (EVE_CLOCK_SOURCE_EXTERNAL)
#define EVE_CLOCK_MUL        (EVE_EXTERNAL_CLOCK_MUL_x6_72MHz)
#define EVE_CLOCK_SPEED      ((uint32_t)(EVE_CLOCK_MUL&0x1F)*(uint32_t)12000000)
#define EVE_TOUCH_TYPE       (EVE_TOUCH_CAPACITIVE)
#define EVE_TOUCH_CAP_DEVICE (EVE_CAP_DEV_FT5316)
#define EVE_PEN_UP_BUG_FIX   (0)
//Set this to force a calibration. Then you can copy that matrix from the
//console and use it to populate touch_transform[]
#define EVE_TOUCH_CAL_NEEDED (0)
// Define RGB output pins order, determined by PCB layout
#define LCD_SWIZZLE      (0x0)
// Define active edge of PCLK. Observed by scope:
//  0: Data is put out coincident with falling edge of the clock.
//     Rising edge of the clock is in the middle of the data.
//  1: Data is put out coincident with rising edge of the clock.
//     Falling edge of the clock is in the middle of the data.
#define LCD_PCLKPOL      (1)
// Pin drive settings for this particular board+display.
#if ((EVE_DEVICE == BT815) ||(EVE_DEVICE == BT816) ||(EVE_DEVICE == BT817) || (EVE_DEVICE == BT818))
// Newer EVE chips have great granularity.
// This table sets every pin, but you can delete lines
// where the default drive is acceptable
//
// Using a macro allows the pin drive to be defined here in the
// header file (which may be included multiple times across the
// project, but actually defined in the EVE_base.cpp file which
// is only compiled once.
#define PIN_DRIVE_TABLE const uint8_t Pin_Drive_Table[22] PROGMEM =\
  {\
  (EVE_PINDRIVE_GPIO_0_HR      | EVE_PINDRIVE_HR_5mA),\
  (EVE_PINDRIVE_GPIO_1_HR      | EVE_PINDRIVE_HR_5mA),\
  (EVE_PINDRIVE_GPIO_2_HR      | EVE_PINDRIVE_HR_5mA),\
  (EVE_PINDRIVE_GPIO_3_HR      | EVE_PINDRIVE_HR_5mA),\
  (EVE_PINDRIVE_DISP_LR        | EVE_PINDRIVE_LR_1p2mA),\
  (EVE_PINDRIVE_DE_LR          | EVE_PINDRIVE_LR_1p2mA),\
  (EVE_PINDRIVE_VSYNC_HSYNC_LR | EVE_PINDRIVE_LR_1p2mA),\
  (EVE_PINDRIVE_PCLK_LR        | EVE_PINDRIVE_LR_3p6mA),\
  (EVE_PINDRIVE_BACKLIGHT_LR   | EVE_PINDRIVE_LR_1p2mA),\
  (EVE_PINDRIVE_RGB_LR         | EVE_PINDRIVE_LR_1p2mA),\
  (EVE_PINDRIVE_AUDIO_L_HR     | EVE_PINDRIVE_HR_5mA),\
  (EVE_PINDRIVE_INT_N_HR       | EVE_PINDRIVE_HR_5mA),\
  (EVE_PINDRIVE_CTP_RST_N_HR   | EVE_PINDRIVE_HR_5mA),\
  (EVE_PINDRIVE_CTP_SCL_HR     | EVE_PINDRIVE_HR_5mA),\
  (EVE_PINDRIVE_CTP_SDA_HR     | EVE_PINDRIVE_HR_5mA),\
  (EVE_PINDRIVE_SPI_DATA_HR    | EVE_PINDRIVE_HR_5mA),\
  (EVE_PINDRIVE_SPIM_SCLK_HR   | EVE_PINDRIVE_HR_10mA),\
  (EVE_PINDRIVE_SPIM_SS_N_HR   | EVE_PINDRIVE_HR_5mA),\
  (EVE_PINDRIVE_SPIM_MISO_HR   | EVE_PINDRIVE_HR_5mA),\
  (EVE_PINDRIVE_SPIM_MOSI_HR   | EVE_PINDRIVE_HR_5mA),\
  (EVE_PINDRIVE_SPIM_IO2_HR    | EVE_PINDRIVE_HR_5mA),\
  (EVE_PINDRIVE_SPIM_IO3_HR    | EVE_PINDRIVE_HR_5mA)\
  }
#else  // ((EVE_DEVICE == BT815) ||(EVE_DEVICE == BT816) ||(EVE_DEVICE == BT817) || (EVE_DEVICE == BT818))
// Older parts have less granularity.
// LCD drive strength: 0=5mA, 1=10mA
#define LCD_DRIVE_10MA   (0)
#endif // ((EVE_DEVICE == BT815) ||(EVE_DEVICE == BT816) ||(EVE_DEVICE == BT817) || (EVE_DEVICE == BT818))

// Spread Spectrum on RGB signals. Probably not a good idea at higher
// PCLK frequencies.
#define LCD_PCLK_CSPREAD (0)
//This is a 24-bit display, so no need to dither.
#define LCD_DITHER       (0)
// Pixel clock divisor (based on 72MHz internal clock)
//   0 = disable
//   1 = 60MHz (?)
//   2 = 36MHz
//   3 = 24MHz
//   4 = 18MHz
//   5 = 14.4MHz
//   6 = 12MHz
//   7 = 10.3MHz
//   8 = 9MHz
//   9 = 8MHz
//   10 = 7.2MHz
//   etc
// Our target is 33MHz.
// LCD_PCLK divisor of 2 gives us 36MHz
#define LCD_PCLK         (2)
//----------------------------------------------------------------------------
// Frame_Rate = 60Hz / 16.7mS
//----------------------------------------------------------------------------
// Horizontal timing (minimum values from ILI6122_SPEC_V008.pdf page 45)
// Target 60Hz frame rate, using the largest possible line time in order to
// maximize the time that the EVE has to process each line.
#define HPX   (800)    // Horizontal Pixel Width
#define HSW   (4)      // Horizontal Sync Width (1~40)
#define HBP   (8)      // Horizontal Back Porch (must be 46, includes HSW)
#define HFP   (8)      // Horizontal Front Porch (16~210~354)
#define HPP   (178)    // Horizontal Pixel Padding (tot=863: 862~1056~1200)
                       // EVE needs at least 1 here
// Define the constants needed by the EVE based on the timing
// Active width of LCD display
#define LCD_WIDTH   (HPX)
// Start of horizontal sync pulse
#define LCD_HSYNC0  (HFP)
// End of horizontal sync pulse
#define LCD_HSYNC1  (HFP+HSW)
// Start of active line
#define LCD_HOFFSET (HFP+HSW+HBP)
// Total number of clocks per line
#define LCD_HCYCLE  (HPX+HFP+HSW+HBP+HPP)
//----------------------------------------------------------------------------
// Vertical timing (minimum values from ILI6122_SPEC_V008.pdf page 46)
#define VLH   (480)   // Vertical Line Height
#define VS    (4)     // Vertical Sync (in lines)  (1~20)
#define VBP   (8)     // Vertical Back Porch (must be 23, includes VS)
#define VFP   (8)     // Vertical Front Porch (7~22~147)
#define VLP   (1)     // Vertical Line Padding (tot=511: 510~525~650)
                      // EVE needs at least 1 here
// Define the constants needed by the EVE based on the timing
// Active height of LCD display
#define LCD_HEIGHT  (VLH)
// Start of vertical sync pulse
#define LCD_VSYNC0  (VFP)
// End of vertical sync pulse
#define LCD_VSYNC1  (VFP+VS)
// Start of active screen
#define LCD_VOFFSET (VFP+VS+VBP)
// Total number of lines per screen
#define LCD_VCYCLE  (VLH+VFP+VS+VBP+VLP)
//============================================================================
// Throw an error if touch demo is requested for a non-touch display.
#if (EVE_TOUCH_TYPE == EVE_TOUCH_NONE)
#if (0 != TOUCH_DEMO)
  #error Cannot enable touch demo for a non-touch display.
#endif // (0 != TOUCH_DEMO)
#endif // (EVE_TOUCH_TYPE != EVE_TOUCH_NONE)
//============================================================================
// Throw an error if the controller does not match the touch type.
#if ((EVE_TOUCH_TYPE == EVE_TOUCH_RESISTIVE) && \
     ((EVE_DEVICE == FT801) || \
      (EVE_DEVICE == FT811) || \
      (EVE_DEVICE == FT813) || \
      (EVE_DEVICE == FT815) || \
      (EVE_DEVICE == FT817)))
  #error Cannot specify EVE_TOUCH_RESISTIVE for an EVE_DEVICE that only supports capacitive touch.
#endif
#if ((EVE_TOUCH_TYPE == EVE_TOUCH_CAPACITIVE) && \
     ((EVE_DEVICE == FT800) || \
      (EVE_DEVICE == FT810) || \
      (EVE_DEVICE == FT812) || \
      (EVE_DEVICE == FT816) || \
      (EVE_DEVICE == FT818)))
  #error Cannot specify EVE_TOUCH_CAPACITIVE for an EVE_DEVICE that only supports resistive touch.
#endif
//============================================================================
// Manually control the backlight brightness by touching along an axis.
#if (0 != TOUCH_DEMO)
#define MANUAL_BACKLIGHT_DEBUG (0)
#endif // (0 != TOUCH_DEMO)
//============================================================================
// Remotely control the backlight (debug only).
#if (DEBUG_LEVEL != DEBUG_NONE)
#define REMOTE_BACKLIGHT_DEBUG (0)
#endif // (DEBUG_LEVEL != DEBUG_NONE)
//============================================================================
// Test code for Reset_EVE_Coprocessor() (debug only).
#if ((0 != LOGO_DEMO) && (1 == LOGO_PNG_0_ARGB2_1))
#define DEBUG_COPROCESSOR_RESET (0)
#endif // ((0 != LOGO_DEMO) &&( 1 == LOGO_PNG_0_ARGB2_1))
//============================================================================
#endif // __CFA10100_DEFINES_H__

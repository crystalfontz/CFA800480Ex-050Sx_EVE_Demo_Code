//===========================================================================
//
// Crystalfontz Seeeduino / Arduino Simple Demonstration Program
// for FTDI / BridgeTek EVE graphic accelerators.
//
// Code written for Seeeduino v4.2 set to 3.3v (important!)
// Seeeduino v4.2 is an Arduino Uno clone that can run at 3.3v.
//
//---------------------------------------------------------------------------
//
// This is a simplified / refactored version of the code in FTDI's AN_275:
//
//  http://brtchip.com/wp-content/uploads/Support/Documentation/Application_Notes/ICs/EVE/AN_275_FT800_Example_with_Arduino.pdf
//
// I have added support for the BridgeTek BT817 EVE series.
//
// In the spirit of AN_275:
//
//   An “abstraction layer” concept was explicitly avoided in this
//   example. Rather, direct use of the Arduino libraries demonstrates
//   the simplicity of sending and receiving data through the FT800
//   while producing a graphic output.
//
// My main goal here is to be transparent about what is really happening
// from the high to lowest levels, without obfuscation, while still
// at least giving a nod to good programming practices.
//
// Plus, you probably don't have RAM and flash for all those fancy
// programming layers.
//
// The FTDI write offset (FWo) into the FT813's circular write write buffer
// is passed into and back from functions (FWol = FWo local) rather than being
// a global. Keeping track of the write offset avoids having to read that
// information from the FT813 before every SPI transaction.
//
// A nod to Rudolph R and company over at
//   https://www.mikrocontroller.net/topic/395608
//   https://github.com/RudolphRiedel/FT800-FT813
// for deep insight and lots of help in increasing our understanding
// of the fiddly bits of the EVE hardware and software architecture.
//
// 2020-08-05 Brent A. Crosby / Crystalfontz America, Inc.
// https://www.crystalfontz.com/products/eve-accelerated-tft-displays.php
//---------------------------------------------------------------------------
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
// Adapted from:
// FTDIChip AN_275 FT800 with Arduino - Version 1.0
//
// Copyright (c) Future Technology Devices International
//
// THIS SOFTWARE IS PROVIDED BY FUTURE TECHNOLOGY DEVICES INTERNATIONAL
// LIMITED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL FUTURE TECHNOLOGY
// DEVICES INTERNATIONAL LIMITED BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES LOSS OF USE,
// DATA, OR PROFITS OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// This code is provided as an example only and is not guaranteed by
// FTDI/BridgeTek. FTDI/BridgeTek accept no responsibility for any issues
// resulting from its use. By using this code, the developer of the final
// application incorporating any parts of this sample project agrees to take
// full responsible for ensuring its safe and correct operation and for any
// consequences resulting from its use.
//===========================================================================
#include <Arduino.h>
#include <SPI.h>
#include <stdarg.h>
// Definitions for our circuit board and display.
#include "CFA10100_defines.h"

#if BUILD_SD
#include <SD.h>
#endif

// The very simple EVE library files
#include "EVE_base.h"
#include "EVE_draw.h"

// Our demonstrations of various EVE functions
#include "demos.h"


  
//===========================================================================
void setup()
  {
#if (DEBUG_LEVEL != DEBUG_NONE)
  // Initialize UART for debugging messages
  Serial.begin(115200);
#endif // (DEBUG_LEVEL != DEBUG_NONE)
  DBG_STAT("Begin\n");

  //Initialize GPIO port states
  // Set CS# high to start - SPI inactive
  SET_EVE_CS_NOT;
  // Set PD# high to start
  SET_EVE_PD_NOT;
  SET_SD_CS_NOT;

  //Initialize port directions
  // EVE interrupt output (not used in this example)
  pinMode(EVE_INT, INPUT_PULLUP);
  // EVE Power Down (reset) input
  pinMode(EVE_PD_NOT, OUTPUT);
  // EVE SPI bus CS# input
  pinMode(EVE_CS_NOT, OUTPUT);
  // USD card CS
  pinMode(SD_CS, OUTPUT);
  // Optional pin used for LED or oscilloscope debugging.
  pinMode(DEBUG_LED, OUTPUT);

  // Initialize SPI
  SPI.begin();
  //Bump the clock to 8MHz. Appears to be the maximum.
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  DBG_GEEK("SPI initialzed to: 8MHz\n");

#if BUILD_SD
  // The prototype hardware appears to functon fine at 8MHz which
  // also appears to be the max that the ATmega328P can do.
  if (!SD.begin(8000000,SD_CS))
    {
    DBG_STAT("uSD card failed to initialize, or not present\n");
    //Reset the SPI clock to fast. SD card library does not clean up well.
    //Bump the clock to 8MHz. Appears to be the maximum.
    SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
    }
  else
    {
    DBG_STAT("uSD card initialized.\n");
    }
#endif

  //See if we can find the FTDI/BridgeTek EVE processor
  if(0 != EVE_Initialize())
    {
    DBG_STAT("Failed to initialize %s8%02X. Stopping.\n",EVE_DEVICE<0x14?"FT":"BT",EVE_DEVICE);
    while(1);
    }
  else
    {
    DBG_STAT("%s8%02X initialized.\n",EVE_DEVICE<0x14?"FT":"BT",EVE_DEVICE);
    }
  } //  setup()
//===========================================================================
void loop()
  {
  DBG_GEEK("Loop initialization.\n");

  //Get the current write pointer from the EVE
  uint16_t
    FWo;
  FWo = EVE_REG_Read_16(EVE_REG_CMD_WRITE);
  DBG_GEEK("Initial Offset Read: 0x%04X = %u\n",FWo ,FWo);

  //Keep track of the RAM_G memory allocation
  uint32_t
    RAM_G_Unused_Start;
  RAM_G_Unused_Start=0;
  DBG_GEEK("Initial RAM_G: 0x%08lX = %lu\n",RAM_G_Unused_Start,RAM_G_Unused_Start);

  // We need to keep track of the bitmap handles and where they are used.
  //
  // By default, bitmap handles 16 to 31 are used for built-in font and 15
  // is used as scratch bitmap handle by co-processor engine commands
  // CMD_GRADIENT, CMD_BUTTON and CMD_KEYS.
  //
  // For whatever reason, I am going to allocate handles from 14 to 0.
  uint8_t
    next_bitmap_handle_available;
  next_bitmap_handle_available=14;

  DBG_GEEK("EVE_Initialize_Flash() . . . ");
  FWo=EVE_Initialize_Flash(FWo);
  DBG_GEEK("done.\n");

  uint8_t
    flash_status;
  flash_status = EVE_REG_Read_8(EVE_REG_FLASH_STATUS);
  DBG_GEEK_Decode_Flash_Status(flash_status);

#if (0 != PROGRAM_FLASH_FROM_USD)
  //Keep track of the current write pointer into flash.
  uint32_t
    Flash_Sector;
  Flash_Sector=0;

  //Load the BLOB & write our image data to the flash from
  //the uSD card. This only needs to be executed once. It
  //uses RAM_G as scratch temporary memory, but does not
  //allocate any RAM_G.
  FWo= Initialize_Flash_From_uSD(FWo,
                                 RAM_G_Unused_Start,
                                 &Flash_Sector);
#else  // (0 != PROGRAM_FLASH_FROM_USD)
  DBG_GEEK("Not programming flash.\n");
#endif // (0 != PROGRAM_FLASH_FROM_USD)

#if (0 != BOUNCE_DEMO)
  DBG_STAT("Initialize_Bounce_Demo() . . .");
  Initialize_Bounce_Demo();
  DBG_STAT(" done.\n");
#endif // (0 != BOUNCE_DEMO)

#if (0 != LOGO_DEMO)
  DBG_STAT("Initialize_Logo_Demo() . . .");
  FWo=Initialize_Logo_Demo(FWo,&RAM_G_Unused_Start,next_bitmap_handle_available);
  //Keep track that we used a bitmap handle
  next_bitmap_handle_available--;
    
  DBG_STAT("  done.\n");
  DBG_GEEK("RAM_G after logo: 0x%08lX = %lu\n",RAM_G_Unused_Start,RAM_G_Unused_Start);
#endif // (0 != LOGO_DEMO)

#if (0 != BMP_DEMO)
  DBG_STAT("Initialize_Bitmap_Demo() . . .");
  FWo=Initialize_Bitmap_Demo(FWo,next_bitmap_handle_available);
  //Keep track that we used a bitmap handle
  next_bitmap_handle_available--;
  DBG_STAT("  done.\n");
  DBG_GEEK("RAM_G after bitmap: 0x%08lX = %lu\n",RAM_G_Unused_Start,RAM_G_Unused_Start);
#endif //(0 != BMP_DEMO)

#if (0 != SOUND_DEMO)
  DBG_STAT("Initialize_Sound_Demo() . . .");
  FWo=Initialize_Sound_Demo(FWo,&RAM_G_Unused_Start);
  DBG_STAT("  done.\n");
  DBG_GEEK("RAM_G after sound: 0x%08lX = %lu\n",RAM_G_Unused_Start,RAM_G_Unused_Start);
#endif // (0 != SOUND_DEMO)

#if (0 != MARBLE_DEMO)
  DBG_STAT("Initialize_Marble_Demo() . . .");
  FWo=Initialize_Marble_Demo(FWo,&RAM_G_Unused_Start,next_bitmap_handle_available);
  //Keep track that we used a bitmap handle
  next_bitmap_handle_available--;
  DBG_STAT("  done.\n");
  DBG_GEEK("RAM_G after marble: 0x%08lX = %lu\n",RAM_G_Unused_Start,RAM_G_Unused_Start);
#endif //MARBLE_DEMO

#if (0 != TOUCH_DEMO)
  //Bitmask of valid points in the array
  uint8_t
    points_touched_mask;
#if (EVE_TOUCH_TYPE == EVE_TOUCH_RESISTIVE)
  DBG_GEEK("Resistive touch, single point.\n");
  int16_t
    x_points[1];
  int16_t
    y_points[1];
#endif // (EVE_TOUCH_TYPE == EVE_TOUCH_RESISTIVE)

#if (EVE_TOUCH_TYPE == EVE_TOUCH_CAPACITIVE)
  DBG_GEEK("Capacitive touch, multiple points.\n");
  int16_t
    x_points[5];
  int16_t
    y_points[5];
#endif // (EVE_TOUCH_TYPE == EVE_TOUCH_CAPACITIVE)
#endif // (0 != TOUCH_DEMO)

#if (0 != VIDEO_DEMO)
  FWo=Initialize_Video_Demo(FWo,
                            &RAM_G_Unused_Start,
                            next_bitmap_handle_available);
  //Keep track that we used a bitmap handle
  next_bitmap_handle_available--;
  DBG_GEEK("RAM_G after video: 0x%08lX = %lu\n",RAM_G_Unused_Start,RAM_G_Unused_Start);
#endif // (0 != VIDEO_DEMO)


  DBG_STAT("Initialization complete, entering main loop.\n");

  while(1)
    {
    //========== FRAME SYNCHRONIZING ==========
    // Wait for graphics processor to complete executing the current command
    // list. This happens when EVE_REG_CMD_READ matches EVE_REG_CMD_WRITE, indicating
    // that all commands have been executed.  We have a local copy of
    // EVE_REG_CMD_WRITE in FWo.
    //
    // This appears to only occur on frame completion, which is nice since it
    // allows us to step the animation along at a reasonable rate.
    //
    // If possible, I have tweaked the timing on the Crystalfontz displays
    // to all have ~60Hz frame rate.
    FWo=Wait_for_EVE_Execution_Complete(FWo);

#if TOUCH_DEMO
    //Read the touch screen.
    points_touched_mask=Read_Touch(x_points,y_points);
#endif // TOUCH_DEMO

#if SOUND_DEMO
    //See if we should play a sound. The sound will synchronize
    //with the the start of the logo rotation.
    //If the previous sound is still playing it will wait until the
    //next time we call it.
    Start_Sound_Demo_Playing();
#endif //SOUND_DEMO

    //========== START THE DISPLAY LIST ==========
    // Start the display list
    FWo=EVE_Cmd_Dat_0(FWo,
                      (EVE_ENC_CMD_DLSTART));
  
    // Set the default clear color to black
    FWo=EVE_Cmd_Dat_0(FWo,
                      EVE_ENC_CLEAR_COLOR_RGB(0,0,0));
    // Clear the screen - this and the previous prevent artifacts between lists
    FWo=EVE_Cmd_Dat_0(FWo,
                      EVE_ENC_CLEAR(1 /*CLR_COL*/,1 /*CLR_STN*/,1 /*CLR_TAG*/));
    //========== ADD GRAPHIC ITEMS TO THE DISPLAY LIST ==========
    //Fill background with white
    FWo=EVE_Filled_Rectangle(FWo,
                             0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
                            
#if (0 != BMP_DEMO)
    FWo=Add_Bitmap_To_Display_List(FWo);
#endif // BMP_DEMO

#if (0 != TOUCH_DEMO)
    //See if we are touched at all.
    if(0 != points_touched_mask)
      {
      //Loop through the possible touch points
      uint8_t
        mask;
      mask=0x01;
      for(uint8_t i=0;i<5;i++)
        {
        if(0 != (points_touched_mask&mask))
          {
          //This code loops through all the points touched
          static uint32_t colors[5]=
            {
            EVE_ENC_COLOR_RGB(0x00,0x00,0xFF),
            EVE_ENC_COLOR_RGB(0x00,0xFF,0x00),
            EVE_ENC_COLOR_RGB(0xFF,0x00,0x00),
            EVE_ENC_COLOR_RGB(0xFF,0x00,0xFF),
            EVE_ENC_COLOR_RGB(0xFF,0xFF,0x00)
            };
#if (0!=DEBUG_COPROCESSOR_RESET)
          //Test code to crash coprocessor ever other time it is called --
          //for testing Reset_EVE_Coprocessor()
          DBG_STAT("Initialize_Logo_Demo() . . .");
          FWo=Initialize_Logo_Demo(FWo,&RAM_G_Unused_Start);
          DBG_STAT("  done.\n");
#endif // (0!=DEBUG_COPROCESSOR_RESET)
          FWo=EVE_Cmd_Dat_0(FWo,
                            colors[i]);
          // Make it solid
          FWo=EVE_Cmd_Dat_0(FWo,
                            EVE_ENC_COLOR_A(0xFF));
          // Draw the touch dot -- a 60px point (filled circle)
          FWo=EVE_Point(FWo,
                        x_points[i]*16,
                        y_points[i]*16,
                        60*16);
          //Tag the touch point with magenta text to show off EVE_PrintF.
          FWo=EVE_Cmd_Dat_0(FWo,
                            EVE_ENC_COLOR_RGB(0xFF,0x00,0xFF));
          //Move the the text out from under the user's finger
          int16_t
            xoffset;
          int16_t
            yoffset;
          if(x_points[i] < (LCD_WIDTH/2))
             {
             xoffset=160; 
             }
          else
             {
             xoffset=-160;
             }
          if(y_points[i] < (LCD_HEIGHT/2))
             {
             yoffset=80; 
             }
          else
             {
             yoffset=-80;
             }
          //Put the text into the display list
          FWo=EVE_PrintF(FWo,
                         x_points[i]+xoffset,
                         y_points[i]+yoffset,
                         25,         //Font
                         EVE_OPT_CENTER, //Options
                         "T[%d]@(%d,%d)",
                         i+1,
                         x_points[i],
                         y_points[i]);
                      
          }
        mask<<=1;
#if (0 != MANUAL_BACKLIGHT_DEBUG)

        //Set the backlight brightness based on the first touch point.
        if(0 != (0x01 & points_touched_mask))
          {
          FWo=Set_Backlight_From_Touch(FWo,x_points[0], LCD_WIDTH);

//      DBG_GEEK("Reset after touch.\n");
//
//  //Point the video decoder to the video which is stored in flash
//  FWo=EVE_Cmd_Dat_1(FWo,
//                     EVE_ENC_CMD_FLASHSOURCE,
//                     FLASH_SECTOR_ICE_FPV_512x300<<12);
//  //Initialize the video decoder based on the flash address that
//  //we just wrote.
//  FWo=EVE_Cmd_Dat_0(FWo,
//                     EVE_ENC_CMD_VIDEOSTARTF);                     
        
          }
#endif // (0 != MANUAL_BACKLIGHT_DEBUG)
        }
      }
#endif // (0 != TOUCH_DEMO)

#if (0 != MARBLE_DEMO)
#if (0 != TOUCH_DEMO)
    //Only show the bouncing marble if no there is no touch
    if(0 == points_touched_mask)
      {
#endif //(0 != TOUCH_DEMO)
      FWo=Add_Marble_To_Display_List(FWo);
#if (0 != TOUCH_DEMO)
      }
#endif //(0 != TOUCH_DEMO)
#endif //(0 != MARBLE_DEMO)

#if (0 != BOUNCE_DEMO)
    //========== BOUNCY BALL $ RUBBER BAND ==========
    FWo=Add_Bounce_To_Display_List( FWo);
#endif //BOUNCE_DEMO

#if (0 != VIDEO_DEMO)
#if (0 != TOUCH_DEMO)
    FWo=Add_Video_To_Display_List(FWo,points_touched_mask,x_points,y_points);
#else // (0 != TOUCH_DEMO)
    FWo=Add_Video_To_Display_List(FWo);
#endif // (0 != TOUCH_DEMO)
#endif // (0 != VIDEO_DEMO)


#if (0 != LOGO_DEMO)
    FWo=Add_Logo_To_Display_List(FWo);
#endif // (0 != LOGO_DEMO)

#if (0 != REMOTE_BACKLIGHT_DEBUG)
    int
      byte_read;
    if(-1 != (byte_read=Serial.read()))
      {
      DBG_GEEK("Serial Data Read: %3d 0x%02X",byte_read,byte_read);
      if(byte_read<=128)
        {
        EVE_REG_Write_8(EVE_REG_PWM_DUTY,byte_read);
        DBG_GEEK(", backlight set.");
        }
      DBG_GEEK("\n");
      }
#endif // (0 != REMOTE_BACKLIGHT_DEBUG)

#if (0 != VIDEO_DEMO)
    //Move the video to the next 30Hz frame 
    FWo=Update_Video_Frame(FWo);
#endif // (0 != VIDEO_DEMO)

    //========== FINSH AND SHOW THE DISPLAY LIST ==========
    // Instruct the graphics processor to show the list
    FWo=EVE_Cmd_Dat_0(FWo, EVE_ENC_DISPLAY());
    // Make this list active
    FWo=EVE_Cmd_Dat_0(FWo, EVE_ENC_CMD_SWAP);
    // Update the ring buffer pointer so the graphics processor starts executing
    EVE_REG_Write_16(EVE_REG_CMD_WRITE, (FWo));



#if (0 != BOUNCE_DEMO)
    //========== MOVE THE BALL AND CYCLE COLOR AND TRANSPARENCY ==========
    Bounce_Ball();
#endif //(0 != BOUNCE_DEMO)

#if (0 != MARBLE_DEMO)
  //========== BOUNCE THE MARBLE AROUND ==========
  Move_Marble();
#endif //(0 != MARBLE_DEMO)
    }  // while(1)
  } // loop()
//===========================================================================

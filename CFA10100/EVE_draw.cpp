//============================================================================
//
// Drawing and helper routines for EVE accelerators.
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
#include <Arduino.h>
#include <SPI.h>
#include <stdarg.h>
// Definitions for our circuit board and display.
#include "CFA10100_defines.h"
// Transparent Logo
#include "Round_Logos.h"

#if BUILD_SD
#include <SD.h>
#endif

// The very simple EVE library files
#include "EVE_defines.h"
#include "EVE_base.h"
#include "EVE_draw.h"
//===========================================================================
uint16_t EVE_Point(uint16_t FWol,
                   uint16_t point_x,
                   uint16_t point_y,
                   uint16_t ball_size)
  {
  // Select the size of the dot to draw
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_POINT_SIZE(ball_size));
  // Indicate to draw a point (dot)
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_BEGIN(EVE_BEGIN_POINTS));
  // Set the point center location
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_VERTEX2F(point_x,point_y));
  // End the point
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_END());

  //Give the updated write pointer back to the caller
  return(FWol);
  }
//===========================================================================
uint16_t EVE_Line(uint16_t FWol,
                  uint16_t x0,
                  uint16_t y0,
                  uint16_t x1,
                  uint16_t y1,
                  uint16_t width)
  {
  //Set the line width
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_LINE_WIDTH(width*16));
  // Start a line
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_BEGIN(EVE_BEGIN_LINES));
  // Set the first point
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_VERTEX2F(x0*16,y0*16));
  // Set the second point
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_VERTEX2F(x1*16,y1*16));
  // End the line
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_END());
  //Give the updated write pointer back to the caller
  return(FWol);
  }
//===========================================================================
uint16_t EVE_Filled_Rectangle(uint16_t FWol,
                              uint16_t x0,
                              uint16_t y0,
                              uint16_t x1,
                              uint16_t y1)
  {
  //Set the line width (16/16 of a pixel--appears to be about as sharp as it gets)
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_LINE_WIDTH(16));
  // Start a rectangle
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_BEGIN(EVE_BEGIN_RECTS));
  // Set the first point
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_VERTEX2F(x0*16,y0*16));
  // Set the second point
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_VERTEX2F(x1*16,y1*16));
  // End the rectangle
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_END());
  //Give the updated write pointer back to the caller
  return(FWol);
  }
//===========================================================================
uint16_t EVE_Open_Rectangle(uint16_t FWol,
                            uint16_t x0,
                            uint16_t y0,
                            uint16_t x1,
                            uint16_t y1,
                            uint16_t width)
  {
  //Set the line width
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_LINE_WIDTH(width*16));
  // Start a line set
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_BEGIN(EVE_BEGIN_LINES));
  // Top
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_VERTEX2F(x0*16,y0*16));
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_VERTEX2F(x1*16,y0*16));
  //Right
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_VERTEX2F(x1*16,y0*16));
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_VERTEX2F(x1*16,y1*16));
  //Bottom
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_VERTEX2F(x1*16,y1*16));
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_VERTEX2F(x0*16,y1*16));
  //Left
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_VERTEX2F(x0*16,y1*16));
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_VERTEX2F(x0*16,y0*16));
  // End the line set
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_END());
  //Give the updated write pointer back to the caller
  return(FWol);
  }
//===========================================================================
// Print text from a RAM string.
uint16_t EVE_Text(uint16_t FWol,
                  uint16_t x,
                  uint16_t y,
                  uint16_t Font,
                  uint16_t Options,
                  char *message)
  {
//There is some interaction between the video
//and the text.
#if (0 == VIDEO_DEMO)

//void Gpu_CoCmd_Text(Gpu_Hal_Context_t *phost,int16_t x, int16_t y, int16_t font, uint16_t options, const char8_t* s)
//  {
//  Gpu_CoCmd_StartFunc(phost,CMD_SIZE*3 + strlen(s) + 1);
//  Gpu_Copro_SendCmd(phost, CMD_TEXT);
//  Gpu_Copro_SendCmd(phost, (((uint32_t)y<<16)|(x & 0xffff)));
//  Gpu_Copro_SendCmd(phost, (((uint32_t)options<<16)|(font&0xffff)));
//  Gpu_CoCmd_SendStr(phost, s);
//  Gpu_CoCmd_EndFunc(phost,(CMD_SIZE*3 + strlen(s) + 1));  
//  }
    
  //Combine Address_offset into then select the EVE
  //and send the 24-bit address and operation flag.
  _EVE_Select_and_Address(EVE_RAM_CMD|FWol,EVE_MEM_WRITE);

//  _EVE_send_32(EVE_ENC_SAVE_CONTEXT);
//  FWol=(FWol+4)&0xFFF;
  
  
  //Send the EVE_ENC_CMD_TEXT command
  _EVE_send_32(EVE_ENC_CMD_TEXT);
  //Send the parameters of the EVE_ENC_CMD_TEXT command
  //First is 32-bit combination of Y & X
  _EVE_send_32((((uint32_t)y)<<16) | (x));
  //Second is a combinations of options and the font=32
  _EVE_send_32((((uint32_t)Options)<<16) | (Font));
  //Keep track that we have written 12 bytes
  FWol=(FWol+12)&0xFFF;
  //Pipe out the string, could be 0 length.
  uint8_t
    this_character;
  //Grab the next character from the string. If it is not
  //null, send it out.
  while(0 != (this_character=*message))
    {
    SPI.transfer(this_character);
    //Point to the next character in the string
    message++;
    //Keep track that we have written a byte
    FWol=(FWol+1)&0xFFF;
    }

  //Send the mandatory null terminator
  SPI.transfer(0);
  //Keep track that we have written a byte
  FWol=(FWol+1)&0xFFF;
 
  //We need to ensure 4-byte alignment. Add nulls as necessary.
  while(0 != (FWol&0x03))
    {
    SPI.transfer(0);
    //Keep track that we have written a byte
    FWol=(FWol+1)&0xFFF;
    }

//  _EVE_send_32(EVE_ENC_RESTORE_CONTEXT);
//  FWol=(FWol+4)&0xFFF;
  
  
  //De-select the EVE
  SET_EVE_CS_NOT;

#endif // (0 == VIDEO_DEMO)
  
  //Give the updated write pointer back to the caller
  return(FWol);
  }
//===========================================================================
// The Arduino handles flash strings way differently than RAM, so we
// need a special "F" function to handle flash strings.
uint16_t EVE_TextF(uint16_t FWol,
                   uint16_t x,
                   uint16_t y,
                   uint16_t Font,
                   uint16_t Options,
                   const __FlashStringHelper *message)
  {
  //Copy the flash string to RAM temporarily
  char
    tmp[256];
  strcpy_P(tmp,(const char *)message);
  //Put that string into to the EVE Display List
  return(EVE_Text(FWol,
                  x,
                  y,
                  Font,
                  Options,
                  tmp));
  }
//============================================================================
// Don't call _EVE_PrintFF() directly, use EVE_PrintF() macro.
//
// ref http://playground.arduino.cc/Main/Printf
//
uint16_t _EVE_PrintFF(uint16_t FWol,
                      uint16_t x,
                      uint16_t y,
                      uint16_t Font,
                      uint16_t Options,
                      const __FlashStringHelper *fmt, ... )
  {
  //Use the variable argument functions to create the string
  char
    tmp[256]; // resulting string limited to 256 chars
  va_list
    args;
  va_start(args, fmt );
  vsnprintf_P(tmp, 256, (const char *)fmt, args);
  va_end (args);
  //Put that string into to the EVE Display List

  return(EVE_Text(FWol,
                  x,
                  y,
                  Font,
                  Options,
                  tmp));
  }  
//===========================================================================
uint16_t Start_Busy_Spinner_Screen(uint16_t FWol,
                                   uint32_t Clear_Color,
                                   uint32_t Text_Color,
                                   uint32_t Spinner_Color,
                                   const __FlashStringHelper *message)
  {
  //Make sure that the chip is caught up.
  FWol=Wait_for_EVE_Execution_Complete(FWol);

//DBG_GEEK("\nFWo read from BT817 at start of Start_Busy_Spinner_Screen: %u Our SW copy: %u\n",EVE_REG_Read_16(EVE_REG_CMD_WRITE),FWol);
  
  //========== START THE DISPLAY LIST ==========
  // Start the display list
  FWol=EVE_Cmd_Dat_0(FWol, (EVE_ENC_CMD_DLSTART));
  // Set the default clear color to black
  FWol=EVE_Cmd_Dat_0(FWol, Clear_Color);
  // Clear the screen - this and the previous prevent artifacts between lists
  FWol=EVE_Cmd_Dat_0(FWol,
                     EVE_ENC_CLEAR(1 /*CLR_COL*/,1 /*CLR_STN*/,1 /*CLR_TAG*/));
  //Solid color -- not transparent
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_COLOR_A(255));

  //========== ADD GRAPHIC ITEMS TO THE DISPLAY LIST ==========
  // Set the drawing for the text
  FWol=EVE_Cmd_Dat_0(FWol,
                     Text_Color);
  //Display the caller's message at the center of the screen using bitmap handle 25
  FWol=EVE_TextF(FWol,
                 LCD_WIDTH/2,
                 LCD_HEIGHT/2,
                 25,  //font
                 EVE_OPT_CENTER,
                 message);
  // Set the drawing color for the spinner
  FWol=EVE_Cmd_Dat_0(FWol, Spinner_Color);
  //Send the spinner go command

/*
// Appears to be broken, I have an e-mail in to support 

  FWol=EVE_Cmd_Dat_2(FWol,
                     EVE_ENC_CMD_SPINNER,
                     (((uint32_t)LCD_HEIGHT/2)<<16) | (LCD_WIDTH/2),
                     //scale, style
                     (((uint32_t)1)<<16) | (1));
*/

  // Instruct the graphics processor to show the list
  FWol=EVE_Cmd_Dat_0(FWol, EVE_ENC_DISPLAY());
  // Make this list active
  FWol=EVE_Cmd_Dat_0(FWol, (EVE_ENC_CMD_SWAP));
  // Update the ring buffer pointer so the graphics processor starts
  // executing.
  // Note: this is a write to register space not EVE_RAM_CMD space.
  EVE_REG_Write_16(EVE_REG_CMD_WRITE, FWol);

//DBG_GEEK("FWo read from BT817 at end of Start_Busy_Spinner_Screen: %u Our SW copy: %u\n",EVE_REG_Read_16(EVE_REG_CMD_WRITE),FWol);
  //We are done, return the updated address.
  return(FWol);
  }
//===========================================================================
uint16_t Stop_Busy_Spinner_Screen(uint16_t FWol,
                                  uint32_t Clear_Color,
                                  uint32_t Text_Color,
                                  const __FlashStringHelper *message)
  {
  //Make sure that the chip is caught up.
  FWol=Wait_for_EVE_Execution_Complete(FWol);
  //========== START THE DISPLAY LIST ==========
  // Start the display list
  FWol=EVE_Cmd_Dat_0(FWol,
                       EVE_ENC_CMD_DLSTART);
  // Set the default clear color to black
  FWol=EVE_Cmd_Dat_0(FWol,
                       Clear_Color);
  // Clear the screen - this and the previous prevent artifacts between lists
  FWol=EVE_Cmd_Dat_0(FWol,
                       EVE_ENC_CLEAR(1 /*CLR_COL*/,1 /*CLR_STN*/,1 /*CLR_TAG*/));
  //Solid color -- not transparent
  FWol=EVE_Cmd_Dat_0(FWol,
                       EVE_ENC_COLOR_A(255));

  //========== STOP THE SPINNER ==========
  FWol=EVE_Cmd_Dat_0(FWol,
                       EVE_ENC_CMD_STOP);

  //========== ADD GRAPHIC ITEMS TO THE DISPLAY LIST ==========
  // Set the drawing for the text
  FWol=EVE_Cmd_Dat_0(FWol,
                       Text_Color);

  //Display the caller's message at the center of the screen using bitmap handle 25
  FWol=EVE_TextF(FWol,
                 LCD_WIDTH/2,
                 LCD_HEIGHT/2,
                 25,  //font
                 EVE_OPT_CENTER,
                 message);

  // Instruct the graphics processor to show the list
  FWol=EVE_Cmd_Dat_0(FWol,
                     EVE_ENC_DISPLAY());
  // Make this list active
  FWol=EVE_Cmd_Dat_0(FWol,
                       EVE_ENC_CMD_SWAP);
  // Update the ring buffer pointer so the graphics processor starts
  // executing.
  // Note: this is a write to register space not EVE_RAM_CMD space.
  EVE_REG_Write_16(EVE_REG_CMD_WRITE, FWol);
  //We are done, return the updated address.
  return(FWol);
  }
//===========================================================================
uint16_t Calibrate_Touch(uint16_t FWol)
  {
  //========== CALIBRATE ==========
  //Make sure that the chip is caught up.
  FWol=Wait_for_EVE_Execution_Complete(FWol);
  //========== START THE DISPLAY LIST ==========
  // Start the display list
  FWol=EVE_Cmd_Dat_0(FWol,
                     EVE_ENC_CMD_DLSTART);
  // Set the drawing color to white
  FWol=EVE_Cmd_Dat_0(FWol,
                     EVE_ENC_COLOR_RGB(0xFF,0xFF,0xFF));
  //Solid color -- not transparent
  FWol=EVE_Cmd_Dat_0(FWol,
                     EVE_ENC_COLOR_A(255));
  // Set the default clear color to blue
  FWol=EVE_Cmd_Dat_0(FWol,
                     EVE_ENC_CLEAR_COLOR_RGB(0,0,0xFF));
  // Clear the screen - this and the previous prevent artifacts between lists
  FWol=EVE_Cmd_Dat_0(FWol,
                     EVE_ENC_CLEAR(1 /*CLR_COL*/,1 /*CLR_STN*/,1 /*CLR_TAG*/));
  FWol=EVE_PrintF(FWol,
                  LCD_WIDTH/2,
                  LCD_HEIGHT/2,
                  25,  //font
                  EVE_OPT_CENTER,
                  "Touch dot to calibrate");
  FWol=EVE_Cmd_Dat_0(FWol,
                     EVE_ENC_CMD_CALIBRATE);
  //========== FINSH AND SHOW THE DISPLAY LIST ==========
  // Instruct the graphics processor to show the list
  FWol=EVE_Cmd_Dat_0(FWol,
                     EVE_ENC_DISPLAY());
  // Make this list active
  FWol=EVE_Cmd_Dat_0(FWol,
                     EVE_ENC_CMD_SWAP);
  // Update the ring buffer pointer so the graphics processor starts executing
  EVE_REG_Write_16(EVE_REG_CMD_WRITE, FWol);
  //Wait for the user to finish calibration.
  FWol=Wait_for_EVE_Execution_Complete(FWol);
  return(FWol);
  }
//===========================================================================
#if (1==LOGO_DEMO)
#if (0==LOGO_PNG_0_ARGB2_1)
uint16_t EVE_Load_PNG_to_RAM_G(uint16_t FWol,
                               const uint8_t *PNG_data,
                               uint32_t PNG_length,
                               uint32_t *RAM_G_Address,
                               uint32_t *Image_Width,
                               uint32_t *Image_Height)
  {
  //Load+expand our PNG into RAM_G
  //Tip of the hat to: https://www.mikrocontroller.net/topic/395608

  //Before we get too carried away, let's check to see if it will fit.
  //First calculate the uncompressed size. It appears that the dimensions
  //always show up at the same offsets in the file, since the IHDR is
  //always first, and the dimensions are first in the IHDR.
  //
  //Get the width from the PNG (Look ma, no locals, no shifts.)
  ((uint8_t *)(Image_Width))[0]=pgm_read_byte(PNG_data+16+3);
  ((uint8_t *)(Image_Width))[1]=pgm_read_byte(PNG_data+16+2);
  ((uint8_t *)(Image_Width))[2]=pgm_read_byte(PNG_data+16+1);
  ((uint8_t *)(Image_Width))[3]=pgm_read_byte(PNG_data+16+0);
  //Get the height from the PNG
  ((uint8_t *)(Image_Height))[0]=pgm_read_byte(PNG_data+20+3);
  ((uint8_t *)(Image_Height))[1]=pgm_read_byte(PNG_data+20+2);
  ((uint8_t *)(Image_Height))[2]=pgm_read_byte(PNG_data+20+1);
  ((uint8_t *)(Image_Height))[3]=pgm_read_byte(PNG_data+20+0);
  //We know that the EVE will uncompress the image as a R5G6B5 image
  //(16-bits per pixel) into RAM_G, so we can calculate the amount
  //of RAMG we will use.
  uint32_t
    RAM_G_Needed;
  RAM_G_Needed=((*Image_Width)*(*Image_Height))<<1;

  //See if there is room
  if((EVE_RAM_G_SIZE - *RAM_G_Address) < RAM_G_Needed)
    {
    DBG_STAT("EVE_Load_PNG_to_RAM_G(): Image is %lu bytes long, but only %lu are available.\n",
              RAM_G_Needed,EVE_RAM_G_SIZE-(*RAM_G_Address));
    //Bail out with the address unchanged.
    return(FWol);
    }

  //Write the CMD_LOADIMAGE and parameters
  FWol=EVE_Cmd_Dat_2(FWol,
                     //The command
                     EVE_ENC_CMD_LOADIMAGE,
                     //First is 32-bit RAM_G offset.
                     *RAM_G_Address,
                     //Second is the PNG options.
                     EVE_OPT_NODL);

  //We need to ensure 4-byte alignment.
  PNG_length=(PNG_length+0x03)&0xFFFFFFFC;

  //Pipe out PNG_length of data from PNG_data. Use chunks so we
  //can handle images larger than 4K.
  while(0 != PNG_length)
    {
    //What is the maximum we can transfer in this block?
    uint32_t
      bytes_this_block;
    uint32_t
      bytes_free;

    //See how much room is available in the EVE_RAM_CMD
    bytes_free=Get_Free_CMD_Space(FWol);

    DBG_GEEK("EVE_Load_PNG_to_RAM_G(): PNG_length= %lu bytes_free = %lu ",PNG_length,bytes_free);

    if(PNG_length <= bytes_free)
      {
      //Everything will fit in the available space.
      bytes_this_block=PNG_length;
      }
    else
      {
      //It won't all fit, transfer the maximum amount.
      bytes_this_block=bytes_free;
      }
    DBG_GEEK("bytes_this_block = %lu \n",bytes_this_block);

    //Set the address in EVE_RAM_CMD for this block
    _EVE_Select_and_Address((uint32_t)EVE_RAM_CMD|(uint32_t)FWol,
                              EVE_MEM_WRITE);

    // Keep track that we are about to send bytes_this_block bytes.
    FWol=(FWol+bytes_this_block)&0xFFF;
    PNG_length-=bytes_this_block;

    while(0!=bytes_this_block)
      {
      SPI.transfer(pgm_read_byte(PNG_data));
      PNG_data++;
      bytes_this_block--;
      }
    //Now we need to end this command.
    SET_EVE_CS_NOT;
    //OK, the data is in the EVE_RAM_CMD circular buffer, ask the chip
    //to process it.
    EVE_REG_Write_16(EVE_REG_CMD_WRITE, FWol);
    //Now wait for it to catch up
    FWol=Wait_for_EVE_Execution_Complete(FWol);
    }

  //Mark this block of RAM_G used in the callers varaible.
  *RAM_G_Address+=RAM_G_Needed;

  //To be safe, force RAM_G_Address to be 8-byte aligned (maybe not
  //needed, certainly does not hurt).
  *RAM_G_Address=(*RAM_G_Address+0x07)&0xFFFFFFF8;

  //Use a read-back to verify what we already calculated
  uint32_t
    Read_Back_RAM_G_First_Available;
  uint32_t
    Read_Back_Width;
  uint32_t
    Read_Back_Height;

  //Now go figure out what we did.
  FWol=
    Get_RAM_G_Properties_After_LOADIMAGE(FWol,
                                         &Read_Back_RAM_G_First_Available,
                                         &Read_Back_Width,
                                         &Read_Back_Height);
  if((*RAM_G_Address!=Read_Back_RAM_G_First_Available)||
     (*Image_Width!=Read_Back_Width)||
     (*Image_Height!=Read_Back_Height))
    {
    DBG_STAT("EVE_Load_PNG_to_RAM_G(): Read-back Error.\n");
    DBG_STAT("  Calc RAM_G %lu Read Back RAM_G = %lu\n",
                 *RAM_G_Address,Read_Back_RAM_G_First_Available);
    DBG_STAT("  Calc Wide %lu Read Back Wide = %lu\n",
                 *Image_Width,Read_Back_Width);
    DBG_STAT("  Calc High %lu Read Back High = %lu\n",
                 *Image_Height,Read_Back_Height);
    }

  //Return the updated address
  return(FWol);
  }
#endif // (0==LOGO_PNG_0_ARGB2_1)
#endif // (1==LOGO_DEMO)
//===========================================================================
//#if (1==LOGO_PNG_0_ARGB2_1)
uint16_t EVE_Inflate_to_RAM_G(uint16_t FWol,
                              const uint8_t *Flash_Data,
                              uint32_t data_length,
                              uint32_t *RAM_G_Address)
  {
  DBG_GEEK("\n");
  //Load and INFLATE data from flash to RAM_G
  //Write the EVE_ENC_CMD_INFLATE and parameters
  FWol=EVE_Cmd_Dat_1(FWol,
                     //The command
                     EVE_ENC_CMD_INFLATE,
                     //First is 32-bit RAM_G offset.
                     *RAM_G_Address);
  //We need to ensure 4-byte alignment.
  data_length=(data_length+0x03)&0xFFFFFFFC;
  //Pipe out data_length of data from data_address. Use chunks so we
  //can handle images larger than 4K.
  while(0 != data_length)
    {
    //What is the maximum we can transfer in this block?
    uint32_t
      bytes_this_block;
    uint32_t
      bytes_free;

    //See how much room is available in the EVE_RAM_CMD
    bytes_free=Get_Free_CMD_Space(FWol);

    DBG_GEEK("  EVE_Inflate_to_RAM_G(): data_length= %lu bytes_free = %lu ",data_length,bytes_free);

    if(data_length <= bytes_free)
      {
      //Everything will fit in the available space.
      bytes_this_block=data_length;
      }
    else
      {
      //It won't all fit, transfer the maximum amount.
      bytes_this_block=bytes_free;
      }
    DBG_GEEK("bytes_this_block = %lu \n",bytes_this_block);

    //Set the address in EVE_RAM_CMD for this block
    _EVE_Select_and_Address((uint32_t)EVE_RAM_CMD|(uint32_t)FWol,
                              EVE_MEM_WRITE);

    // Keep track that we are about to send bytes_this_block bytes.
    FWol=(FWol+bytes_this_block)&0xFFF;
    data_length-=bytes_this_block;

    while(0!=bytes_this_block)
      {
      SPI.transfer(pgm_read_byte(Flash_Data));
      Flash_Data++;
      bytes_this_block--;
      }
    //Now we need to end this command.
    SET_EVE_CS_NOT;
    //OK, the data is in the EVE_RAM_CMD circular buffer, ask the chip
    //to process it.
    EVE_REG_Write_16(EVE_REG_CMD_WRITE, FWol);
    //Now wait for it to catch up
    FWol=Wait_for_EVE_Execution_Complete(FWol);
    }

  //Get the first free address in RAM_G from after the inflated data, and
  //push it into the caller's varaible.
  FWol=Get_RAM_G_Pointer_After_INFLATE(FWol,
                                       RAM_G_Address);

  //To be safe, force RAM_G_Address to be 8-byte aligned (maybe not
  //needed, certainly does not hurt).
  *RAM_G_Address=(*RAM_G_Address+0x07)&0xFFFFFFF8;

  //Return the updated address
  return(FWol);
  }
//#endif // (1==LOGO_PNG_0_ARGB2_1)
//#endif // (1==LOGO_DEMO)
//============================================================================
#if BUILD_SD
// This reads a file from the uSD card and writes it directly
// into RAM_G, not bothering with the command processor.
//---------------------------------------------------------------------------
void EVE_Load_File_To_RAM_G(uint32_t RAM_G_Address,
                            const char *File_Name,
                            uint32_t *RAM_G_Used)
  {
  DBG_GEEK("\n");
  File
    binary_file;
  binary_file = SD.open(File_Name,FILE_READ);    
  if(0 == binary_file)
    {
    DBG_STAT("  EVE_Load_File_To_RAM_G(): Can't open \"%s\".\n",File_Name);
    return;
    }

  //Here would be a good place to do sanity checks with the size
  //of the file vs remaining RAM_G space. You would have to pass
  //in the available RAM_G.
  uint32_t
    bytes_remaining;
  bytes_remaining=binary_file.size();
  DBG_GEEK("  EVE_Load_File_To_RAM_G: found %s size: %lu\n",
           binary_file.name(),binary_file.size());
  //Inform our caller of how much of their RAM_G we are soaking up.
  *RAM_G_Used=bytes_remaining;
  //Limited RAM on the Arduino, so use a reasonable block size
  #define CHUNK_SIZE (256)
  uint8_t
    this_chunk[CHUNK_SIZE];
  do
    {
    //chunk loop
    //Transfer a full chunk, or a partial chunk on the
    //last transfer.
    uint16_t
      this_chunk_size;
    if(bytes_remaining<CHUNK_SIZE)
      {
      this_chunk_size=bytes_remaining;
      }
    else
      {
      this_chunk_size=CHUNK_SIZE;
      }
    //Fill this_chunk from the uSD
    binary_file.read(this_chunk,this_chunk_size);
    //Keep track of bytes_remaining. This frees up this_chunk_size
    //so we can use it as a byte counter below.
    bytes_remaining-=this_chunk_size;

    //Now write this_chunk_size bytes of this_chunk[] to
    //the EVE RAM_G

    //Select the EVE and send the 24-bit address and operation flag.
    _EVE_Select_and_Address(RAM_G_Address,EVE_MEM_WRITE);

    //Remember that we put this_chunk_size data in RAM_G
    RAM_G_Address+=this_chunk_size;

    //Pipe out this_chunk_size of data from this_chunk[]
    //to the EVE.
    SPI.transfer(this_chunk,this_chunk_size);

    //De-select the EVE
    SET_EVE_CS_NOT;
    } // chunk loop
  while(0 != bytes_remaining);
  //Release the BMP file handle
  binary_file.close();
  }
#endif
//============================================================================

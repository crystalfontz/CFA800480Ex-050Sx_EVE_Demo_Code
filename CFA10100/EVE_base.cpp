//============================================================================
//
// Low-Level routines for EVE accelerators.
//
// This is a simplified / refactored version of the code in FTDI's AN_275:
//
//  http://brtchip.com/wp-content/uploads/Support/Documentation/Application_Notes/ICs/EVE/AN_275_FT800_Example_with_Arduino.pdf
//
// I have added support for the EVE series.
//
// The write offset into the write buffer is passed into and back from
// functions rather than being a global.
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
//============================================================================
// Don't call SerPrintFF() directly, use DBG_STAT() or DBG_GEEK() macros.
//
// ref http://playground.arduino.cc/Main/Printf
//
// Example to dump a uint32_t in hex and decimal
//   SerPrintFF(F("RAM_G_Unused_Start: 0x%08lX = %lu\n"),RAM_G_Unused_Start,RAM_G_Unused_Start);
// Example to dump a uint16_t in hex and decimal
//   SerPrintFF(F("Initial Offset Read: 0x%04X = %u\n"),FWo ,FWo);
void SerPrintFF(const __FlashStringHelper *fmt, ... )
  {
  char
    tmp[128]; // resulting string limited to 128 chars
  va_list
    args;
  va_start(args, fmt );
  vsnprintf_P(tmp, 128, (const char *)fmt, args);
  va_end (args);
  Serial.print(tmp);
  }
//============================================================================
void DBG_GEEK_Decode_Flash_Status(uint8_t EVE_flash_status)
  {
#if (DEBUG_LEVEL == DEBUG_GEEK)
  DBG_GEEK("REG_FLASH_STATUS = 0x%02X = %3d = \"FLASH_STATUS_",EVE_flash_status,EVE_flash_status);
  switch(EVE_flash_status)
    {
    case EVE_FLASH_STATUS_INIT:
      DBG_GEEK("INIT\"\n");
      break;
    case EVE_FLASH_STATUS_DETACHED:
      DBG_GEEK("DETACHED\"\n");
      break;
    case EVE_FLASH_STATUS_BASIC:
      DBG_GEEK("BASIC (slow)\"\n");
      break;
    case EVE_FLASH_STATUS_FULL:
      DBG_GEEK("FULL (fast)\"\n");
      break;
    default:
      DBG_GEEK("Error/Unknown\"\n");
      break;
    }
#endif //  (DEBUG_LEVEL == DEBUG_GEEK)
  }
//============================================================================
void DBG_GEEK_Decode_FastFlash_Status(uint32_t EVE_fast_flash_result)
  {
#if (DEBUG_LEVEL == DEBUG_GEEK)
  DBG_GEEK("CMD_FLASHFAST result: 0x%08lX = \"",EVE_fast_flash_result);
  switch(EVE_fast_flash_result)
    {
    case 0x00000000:
      DBG_GEEK("success, no error\"\n");
      break;
    case 0x0000E001:
      DBG_GEEK("flash is not supported\"\n");
      break;
    case 0x0000E002:
      DBG_GEEK("no header detected in sector 0\"\n");
      break;
    case 0x0000E003:
      DBG_GEEK("sector 0 data failed integrity check\"\n");
      break;
    case 0x0000E004:
      DBG_GEEK("device/blob mismatch\"\n");
      break;
    case 0x0000E005:
      DBG_GEEK("failed full-speed test\"\n");
      break;
    default:
      DBG_GEEK("invalid result returned\"\n");
      break;
    }
#endif //  (DEBUG_LEVEL == DEBUG_GEEK)
  }
//============================================================================
uint8_t Validate_and_Print_Chip_ID(uint32_t Chip_ID)
  {
  uint8_t
    return_value;
  //assume success
  return_value=0;
  //Chip Identification Code: 0x00011108 
  //First byte should be 0x00
  uint8_t
    this_byte;
  this_byte=(Chip_ID>>24);
  if(0x00 != this_byte)
    {
    DBG_GEEK("Chip ID first byte fail. Expected 0x00, got 0x%02X.\n",this_byte);
    //indicate failure
    return_value=0;
    }
  //Second byte should be 0x01
  this_byte=(Chip_ID>>16) & 0x000000FFL;
  if(0x01 != this_byte)
    {
    DBG_GEEK("Chip ID second byte fail. Expected 0x01, got 0x%02X.\n",this_byte);
    //indicate failure
    return_value=0;
    }
  //Fourth byte should be 0x08
  this_byte=Chip_ID & 0x000000FFL;
  if(0x08 != this_byte)
    {
    DBG_GEEK("Chip ID fourth byte fail. Expected 0x08, got 0x%02X.\n",this_byte);
    //indicate failure
    return_value=0;
    }
    
  //Third byte indicates the chip type.
  this_byte=(Chip_ID>>8) & 0x000000FFL;

  if(this_byte==EVE_DEVICE)
    {
    DBG_GEEK("Chip ID indicates %s8%02X as expected.\n",EVE_DEVICE<0x14?"FT":"BT",EVE_DEVICE);
    }
  else
    {
    DBG_STAT("Unexpected chip if of 0x%02X = %s8%02X, code compiled for %s8%02X\n",
             this_byte,
             this_byte<0x14?"FT":"BT",this_byte,
             EVE_DEVICE<0x14?"FT":"BT",EVE_DEVICE);
    return_value=0;
    }
  return(return_value);
  }
//============================================================================
void _EVE_Select_and_Address(uint32_t Address, uint8_t Operation)
  {
  //Select the EVE
  CLR_EVE_CS_NOT;
  // Send Operation plus high address byte
  SPI.transfer((uint8_t)(Address >> 16) | Operation);
  // Send middle address byte
  SPI.transfer((uint8_t)(Address >> 8));
  // Send low address byte
  SPI.transfer((uint8_t)(Address));
  }
//============================================================================
void EVE_Command_Write(uint8_t Command, uint8_t Parameter)
  {
  //Select the EVE
  CLR_EVE_CS_NOT;
  // Send 1st (command) byte
  SPI.transfer(Command);
  // Send 2nd (parameter) byte
  SPI.transfer(Parameter);
  // Send 3rd (always 0)byte
  SPI.transfer(0);
  //End the transaction.
  SET_EVE_CS_NOT;  
  }
//============================================================================
void _EVE_send_32(uint32_t Data)
  {
  // Send data low byte
  SPI.transfer((uint8_t)(Data));
  // Send data mid-low byte
  SPI.transfer((uint8_t)(Data >> 8));
  // Send data mid-high byte
  SPI.transfer((uint8_t)(Data >> 16));
  // Send data high byte
  SPI.transfer((uint8_t)(Data >> 24));
  }
//============================================================================
void EVE_REG_Write_8(uint32_t REG_Address, uint8_t ftData8)
  {
  //Select the EVE and send the 24-bit address and operation flag.
  _EVE_Select_and_Address(REG_Address, EVE_MEM_WRITE);
  // Send data byte
  SPI.transfer(ftData8);
  //De-select the EVE
  SET_EVE_CS_NOT;
  }
//============================================================================
void EVE_REG_Write_16(uint32_t REG_Address, uint16_t ftData16)
  {
  //Select the EVE and send the 24-bit address and operation flag.
  _EVE_Select_and_Address(REG_Address, EVE_MEM_WRITE);
  // Send data low byte
  SPI.transfer((uint8_t)(ftData16));
  // Send data high byte
  SPI.transfer((uint8_t)(ftData16 >> 8));
  //De-select the EVE
  SET_EVE_CS_NOT;
  }
//============================================================================
void EVE_REG_Write_32(uint32_t REG_Address, uint32_t ftData32)
  {
  //Select the EVE and send the 24-bit address and operation flag.
  _EVE_Select_and_Address(REG_Address, EVE_MEM_WRITE);
  //Send the uint32_t ftData32
  _EVE_send_32(ftData32);
  //De-select the EVE
  SET_EVE_CS_NOT;
  }
//----------------------------------------------------------------------------
uint16_t EVE_Cmd_Dat_0(uint16_t FWol,
                       uint32_t command)
  {
  //Combine Address_offset into  then select the EVE
  //and send the 24-bit address and operation flag.
  _EVE_Select_and_Address(EVE_RAM_CMD|FWol,EVE_MEM_WRITE);
  //Send the uint32_t data
  _EVE_send_32(command);
  //De-select the EVE
  SET_EVE_CS_NOT;
  //Increment address offset modulo 4096 and return it
  return((FWol+4)&0xFFF);
  }
//----------------------------------------------------------------------------
uint16_t EVE_Cmd_Dat_1(uint16_t FWol,
                       uint32_t command,uint32_t data0)
  {
  //Combine Address_offset into  then select the EVE
  //and send the 24-bit address and operation flag.
  _EVE_Select_and_Address(EVE_RAM_CMD|FWol,EVE_MEM_WRITE);
  //Send the uint32_t data
  _EVE_send_32(command);
  //Send the first uint32_t data
  _EVE_send_32(data0);
  //De-select the EVE
  SET_EVE_CS_NOT;
  //Increment address offset modulo 4096 and return it
  return((FWol+8)&0xFFF);
  }
//----------------------------------------------------------------------------
uint16_t EVE_Cmd_Dat_2(uint16_t FWol,
                       uint32_t command,uint32_t data0, uint32_t data1)
  {
  //Combine Address_offset into  then select the EVE
  //and send the 24-bit address and operation flag.
  _EVE_Select_and_Address(EVE_RAM_CMD|FWol,EVE_MEM_WRITE);
  //Send the uint32_t data
  _EVE_send_32(command);
  //Send the first uint32_t data
  _EVE_send_32(data0);
  //Send the second uint32_t data
  _EVE_send_32(data1);
  //De-select the EVE
  SET_EVE_CS_NOT;
  //Increment address offset modulo 4096 and return it
  return((FWol+12)&0xFFF);
  }
//----------------------------------------------------------------------------
uint16_t EVE_Cmd_Dat_3(uint16_t FWol,
                       uint32_t command,
                       uint32_t data0,
                       uint32_t data1,
                       uint32_t data2)
  {
  //Combine Address_offset into  then select the EVE
  //and send the 24-bit address and operation flag.
  _EVE_Select_and_Address(EVE_RAM_CMD|FWol,EVE_MEM_WRITE);
  //Send the uint32_t data
  _EVE_send_32(command);
  //Send the first uint32_t data
  _EVE_send_32(data0);
  //Send the second uint32_t data
  _EVE_send_32(data1);
  //Send the third uint32_t data
  _EVE_send_32(data2);
  //De-select the EVE
  SET_EVE_CS_NOT;
  //Increment address offset modulo 4096 and return it
  return((FWol+16)&0xFFF);
  }
//============================================================================
uint8_t EVE_REG_Read_8(uint32_t REG_Address)
  {
  uint8_t
    ftData8;
  //Select the EVE and send the 24-bit address and operation flag.
  _EVE_Select_and_Address(REG_Address, EVE_MEM_READ);
  // Send dummy byte
  SPI.transfer(0);
  // Send another dummy, but this time the EVE will reply with the goods.
  ftData8 = SPI.transfer(0);
  //De-select the EVE
  SET_EVE_CS_NOT;
  // Return uint8_t read
  return(ftData8);
  }
//============================================================================
uint16_t EVE_REG_Read_16(uint32_t REG_Address)
  {
  uint16_t
    ftData16;
  //Select the EVE and send the 24-bit address and operation flag.
  _EVE_Select_and_Address(REG_Address, EVE_MEM_READ);
  // Send dummy byte
  SPI.transfer(0);
  // Send more dummies, but now the EVE will reply with the goods.
  // Read low byte
  ftData16 = SPI.transfer(0);
  // Read high byte
  ftData16 |= (uint16_t)SPI.transfer(0) << 8;
  //De-select the EVE
  SET_EVE_CS_NOT;
  // Return uint16_t read
  return(ftData16);
  }
//============================================================================
uint32_t EVE_REG_Read_32(uint32_t REG_Address)
  {
  uint32_t
    ftData32;
  //Select the EVE and send the 24-bit address and operation flag.
  _EVE_Select_and_Address(REG_Address, EVE_MEM_READ);
  // Send dummy byte
  SPI.transfer(0);
  // Send more dummies, but now the EVE will reply with the goods.
  // Read low byte
  ftData32 = (uint32_t)SPI.transfer(0);
  // Read mid-low byte
  ftData32 |= (uint32_t)SPI.transfer(0) << 8;
  // Read mid-high byte
  ftData32 |= (uint32_t)SPI.transfer(0) << 16;
  // Read high byte
  ftData32 |= (uint32_t)SPI.transfer(0) << 24;
  //De-select the EVE
  SET_EVE_CS_NOT;
  // Return uint32_t read
  return(ftData32);
  }
//============================================================================
void EVE_Read_Array(uint32_t EVE_Address, uint16_t length, uint8_t *destination)
  {
  //Select the EVE and send the 24-bit address and operation flag.
  _EVE_Select_and_Address(EVE_Address, EVE_MEM_READ);
  // Send dummy byte
  SPI.transfer(0);
  while(0 != length)
    {
    *destination=SPI.transfer(0);
    destination++;
    length--;
    }
  //De-select the EVE
  SET_EVE_CS_NOT;
  }
//============================================================================
// ref: BRT_AN_033_BT81X_Series_Programming_Guide, page 101
//      https://brtchip.com/wp-content/uploads/Support/Documentation/Programming_Guides/ICs/EVE/BRT_AN_033_BT81X_Series_Programming_Guide.pdf
// ref: EVE Series Programmer Guide, page 157
//      http://www.ftdichip.com/Support/Documents/ProgramGuides/EVE_Series_Programmer_Guide.pdf
// ref: https://github.com/RudolphRiedel/FT800-FT813/blob/4.x/EVE_commands.c
#if (0 != ROBUST_EXECUTION_COMPLETE)
uint16_t Reset_EVE_Coprocessor(void)
  {
#if ((EVE_DEVICE==BT815)||(EVE_DEVICE==BT816)||(EVE_DEVICE==BT817)||(EVE_DEVICE==BT818))
#if (DEBUG_LEVEL != DEBUG_NONE)
  // If debug is on, and we are a BT81x, read the error string from
  //the BT81x and spool to the console
  uint8_t
    BT81x_error_string[129];
  //Make sure it is terminated.
  BT81x_error_string[128]=0;
  EVE_Read_Array(EVE_RAM_ERR_REPORT,128,BT81x_error_string);
  DBG_GEEK("  %s\n",BT81x_error_string);
#endif // (DEBUG_LEVEL != DEBUG_NONE)
  // If it is a BT81x then we need to read a pointer first.
  uint16_t
    copro_patch_pointer;
  copro_patch_pointer = EVE_REG_Read_16(EVE_REG_COPRO_PATCH_PTR);
#endif  //((EVE_DEVICE==BT815)||(EVE_DEVICE==BT816)||(EVE_DEVICE==BT817)||(EVE_DEVICE==BT818))

//NOTE: BridgeTek uses "EVE_REG_Write_32" for all
//these, RR uses 8, 16 and 32 . . .

  //FTDI Write Offset Local
  uint16_t
    FWol;
  // This part of the reset is common to all the FT8xx and BT81x chips
  // hold co-processor engine in the reset condition
  EVE_REG_Write_32(EVE_REG_CPURESET, 1);
  // set EVE_REG_CMD_READ to 0
  EVE_REG_Write_32(EVE_REG_CMD_READ, 0);
  // set EVE_REG_CMD_WRITE to 0
  EVE_REG_Write_32(EVE_REG_CMD_WRITE, 0);
  // reset EVE_REG_CMD_DL to 0 as required by the BT81x programming
  // guide. It is not specified for but should not hurt FT8xx.
  EVE_REG_Write_32(EVE_REG_CMD_DL, 0);
  // restore EVE_REG_PCLK in case it was set to zero by an error
  EVE_REG_Write_8(EVE_REG_PCLK, LCD_PCLK);
  // Keep our local Write Offset varaible in sync
  FWol = 0;
  // restart the co-processor engine
  EVE_REG_Write_8(EVE_REG_CPURESET, 0);
  // ! From Bridgetek CoprocessorFault_Recover() example
  delay(100);

//experimental
//EVE_REG_Write_16(EVE_REG_CMD_READ,FWol);
//EVE_REG_Write_16(EVE_REG_CMD_WRITE,FWol);

#if ((EVE_DEVICE==BT815)||(EVE_DEVICE==BT816)||(EVE_DEVICE==BT817)||(EVE_DEVICE==BT818))
  // Complete the BT81x steps.
  // Write the patch address read earlier back to the chip.
  EVE_REG_Write_16(EVE_REG_COPRO_PATCH_PTR, copro_patch_pointer);
  // RR: "just to be safe"
  delay(5);

#if 0
//This bit with getting the flash going again is broke.
//It is a little display list . . . not sure why it does not execute OK.
//It will probably make more sense once the flash is going.

  uint32_t
    ftAddress;
  ftAddress = EVE_RAM_CMD + FWol;
  _EVE_Select_and_Address(ftAddress, EVE_MEM_WRITE);
  _EVE_send_32(CMD_FLASHATTACH);
  _EVE_send_32(CMD_FLASHFAST);
  SET_EVE_CS_NOT;
  //Increment address offset modulo 4096
  FWol=(FWol+8)&0xFFF;
  // Update the ring buffer pointer so the graphics processor starts executing
  EVE_REG_Write_16(EVE_REG_CMD_WRITE,FWol);
  // RR: "just to be safe"
  delay(5);
#endif //  0  
  
#endif //((EVE_DEVICE==BT815)||(EVE_DEVICE==BT816)||(EVE_DEVICE==BT817)||(EVE_DEVICE==BT818))
  //Return the current FTDI Write Offset.
  return(FWol);
  }
#endif // (0 != ROBUST_EXECUTION_COMPLETE)
//============================================================================
// Wait for graphics processor to complete executing the current command
// list. This happens when EVE_REG_CMD_READ matches EVE_REG_CMD_WRITE, indicating
// that all commands have been executed.  We have a local copy of
// EVE_REG_CMD_WRITE in SW_write_offset.
//
// Additional Read / Write of a vector at idle. I would recommend not using
// this unless you are doing low-level SPI debugging with an oscilloscope.
// It seems to have some negative side effects.
#define WRITE_AND_READ_SPI_VECTOR (0)
//
#if (0 != ROBUST_EXECUTION_COMPLETE)
uint16_t Wait_for_EVE_Execution_Complete(uint16_t SW_write_offset)
  {
  uint32_t
    timeout;
  timeout=100000;

#if (0 != WRITE_AND_READ_SPI_VECTOR)
  uint32_t
    vector;
  vector=0x0FAA55F0;
#endif
  uint16_t
    read_address;
  uint16_t
    write_address;

  uint16_t
    reported_read_address;
  uint16_t
    reported_write_address;
  reported_read_address=0;
  reported_write_address=0;

  do
    {
    read_address=EVE_REG_Read_16(EVE_REG_CMD_READ);
    //Check for a coprocessor fault.
    if(0xFFF == read_address)
      {
      DBG_GEEK("Coprocessor Fault detected. Resetting coprocessor.\n");
      //Return the new offset after resetting the coprocessor.
      return(Reset_EVE_Coprocessor());
      }
    write_address=EVE_REG_Read_16(EVE_REG_CMD_WRITE);
    if((read_address&0xF003)||(write_address&0xF003)||(SW_write_offset&0xF003)||(write_address!=SW_write_offset))
      {
      if((reported_read_address!=read_address)||(reported_write_address!=write_address))
        {
        if(write_address!=SW_write_offset)
          {
          DBG_GEEK("Write Mismatch: HDW_R=(%5u,0x%04X) HDW_W=(%5u,0x%04X) != SW_W=(%5u,0x%04X)\n",
                       read_address,read_address,
                       write_address,write_address,
                       SW_write_offset,SW_write_offset);
          }
        if((read_address&0xF003)||(write_address&0xF003)||(SW_write_offset&0xF003))
          {
          DBG_GEEK("DWORD Alignment: HDW_R=(%5u,0x%04X) HDW_W=(%5u,0x%04X) SW_W=(%5u,0x%04X)\n",
                       read_address,read_address,
                       write_address,write_address,
                       SW_write_offset,SW_write_offset);
          }
        reported_read_address=read_address;
        reported_write_address=write_address;
        }
      }
#if (0 != WRITE_AND_READ_SPI_VECTOR)
    //Write and read REG_MACRO_0 to debug SPI
    //This appears to break at least the dot drawing of the touch calibrate routine.
    //Best to leave it disabled unless you are using the scope to do low-level
    //SPI hardware debugging.
    //Write and read REG_MACRO_0 to debug SPI    
    uint32_t
      readback;
    SET_DEBUG_LED;
    EVE_REG_Write_32(REG_MACRO_0, vector);
    CLR_DEBUG_LED;
    readback=EVE_REG_Read_32(REG_MACRO_0);
    if(readback!=vector)
      {
      DBG_GEEK("WRITE_AND_READ_SPI_VECTOR: 0x%08lX read: 0x%08lX\n",vector,readback);
      }
    vector^=0xFFFFFFFF;
#endif
    //Check timeout
    if(0 != timeout)
      {
      timeout--;
      }
    else
      {
      DBG_GEEK("Wait_for_EVE_Execution_Complete: 100K tries, not complete.\n");
      DBG_GEEK("  Pointers: HDW_R=(%5u,0x%04X) HDW_W=(%5u,0x%04X) != SW_W=(%5u,0x%04X)\n",
               read_address,read_address,
               write_address,write_address,
               SW_write_offset,SW_write_offset);
      //Experimental -- breaks touch calibration
      //  DBG_GEEK("Coprocessor Fault detected. Resetting coprocessor.\n");
      //  //Return the new offset after resetting the coprocessor.
      //  return(Reset_EVE_Coprocessor());
      timeout=100000;
      }
    }while(read_address!=SW_write_offset);
  //No error -- return the unmodified offset.
  return(SW_write_offset);
  }
#endif
//============================================================================
uint16_t Get_Free_CMD_Space(uint16_t FWol)
  {
  // (4K - 4) - (write-read)
  return((4096-4)-((FWol-EVE_REG_Read_16(EVE_REG_CMD_READ))&0x0FFF));
  }
//============================================================================
//from BRT_AN_014 EVE Simple PIC Library Examples:
// An optional step then follows to allow the end of the inflated data to be
// determined, which will be useful when loading data into subsequent RAM_G
// locations to make efficient use of the memory. Whilst the commands above
// define that the data will begin at 0, the end address is not known as the
// inflated data will be larger than the compressed data written to the
// EVE_ENC_CMD_INFLATE command. The EVE_ENC_CMD_GETPTR can be used to check
// the ending address. This command actually returns its value via the
// co-processor FIFO itself. The command (4 bytes) is sent with a dummy
// (4-byte) value as a parameter and so occupies two 32-bit locations in the
// command FIFO. On completion, the co-processor replaces the dummy value with
// the ending address. The application must then perform a read of this
// location (which can be obtained by checking the current EVE_REG_CMD_WRITE
// pointer and subtracting 4 taking account of any possible rollover at
// (EVE_RAM_CMD+0).
//----------------------------------------------------------------------------
uint16_t Get_RAM_G_Pointer_After_INFLATE(uint16_t FWol,
                                         uint32_t *RAM_G_First_Available)
  {
  //Make sure that the chip is caught up.
  FWol=Wait_for_EVE_Execution_Complete(FWol);
  //Tell the chip to get the first free location in RAM_G
  FWol=EVE_Cmd_Dat_1(FWol,
                       EVE_ENC_CMD_GETPTR,0);
  // Update the ring buffer pointer so the graphics processor starts executing
  EVE_REG_Write_16(EVE_REG_CMD_WRITE,FWol);
  //Wait for the chip to catch up.
  FWol=Wait_for_EVE_Execution_Complete(FWol);

  //We know that the answer is 4 addresses lower than FWol
  //Read the value and passs it back to the caller know what we found.
  *RAM_G_First_Available=EVE_REG_Read_32(EVE_RAM_CMD+((FWol-4) & 0x0FFF));

  return(FWol);
  }
//============================================================================
uint16_t Get_RAM_G_Properties_After_LOADIMAGE(uint16_t FWol,
                                              uint32_t *RAM_G_First_Available,
                                              uint32_t *Width,
                                              uint32_t *Height)
  {
  //Make sure that the chip is caught up.
  FWol=Wait_for_EVE_Execution_Complete(FWol);
  // Tell the chip to get the first free location in RAM_G
  FWol=EVE_Cmd_Dat_3(FWol,
                     EVE_ENC_CMD_GETPROPS,0,0,0);
  // Update the ring buffer pointer so the graphics processor starts executing
  EVE_REG_Write_16(EVE_REG_CMD_WRITE,FWol);
  //Wait for the chip to catch up.
  FWol=Wait_for_EVE_Execution_Complete(FWol);

  //We know that the height is 4 addresses lower than FWol
  //Read the value and passs it back to the caller know what we found.
  *Height=EVE_REG_Read_32(EVE_RAM_CMD+((FWol-4) & 0x0FFF));

  //We know that the width is 8 addresses lower than FWol
  //Read the value and passs it back to the caller know what we found.
  *Width=EVE_REG_Read_32(EVE_RAM_CMD+((FWol-8) & 0x0FFF));

  //We know that the answer is 12 addresses lower than FWol
  //Read the value and passs it back to the caller know what we found.
  *RAM_G_First_Available=EVE_REG_Read_32(EVE_RAM_CMD+((FWol-12) & 0x0FFF));
  return(FWol);
  }
//===========================================================================
uint32_t EVE_Set_Flash_to_Fast(uint16_t FWol)
  {
  //Make sure the EVE is caught up
  FWol=Wait_for_EVE_Execution_Complete(FWol);
  //Send the command to the coprocessor
  FWol=EVE_Cmd_Dat_1(FWol,
                     EVE_ENC_CMD_FLASHFAST,
                     0);
  // Update the ring buffer pointer so the coprocessor starts executing
  EVE_REG_Write_16(EVE_REG_CMD_WRITE, (FWol));
  //Now wait for the coprocessor to catch up
  FWol=Wait_for_EVE_Execution_Complete(FWol);

  //Read the result from  EVE_ENC_CMD_FLASHFAST
  uint32_t
    CMD_FastFlash_Result;
  //We know that the result is 4 addresses lower than FWol
  //Read the value and passs it back to the caller know what we found.
  CMD_FastFlash_Result=EVE_REG_Read_32(EVE_RAM_CMD+((FWol-4) & 0x0FFF));

#if (DEBUG_LEVEL == DEBUG_GEEK)
  //Print out the flash status.
  uint8_t
    status;
  status = EVE_REG_Read_8(EVE_REG_FLASH_STATUS);
  DBG_GEEK_Decode_Flash_Status(status);
  DBG_GEEK_Decode_FastFlash_Status(CMD_FastFlash_Result);
#endif //  (DEBUG_LEVEL == DEBUG_GEEK)
  return(FWol);
  }
//---------------------------------------------------------------------------
// Switches the FLASH attached to a BT815/BT816 to full-speed
// mode, returns 0 for failing to do so
uint16_t EVE_Initialize_Flash(uint16_t FWol)
  {
  // We expect EVE_REG_FLASH_STATUS to return 0x02 - FLASH_STATUS_BASIC
  // indicating power-up is done and the attached flash is detected
  uint8_t
    status;
  status = EVE_REG_Read_8(EVE_REG_FLASH_STATUS);
  DBG_GEEK("\n");
  DBG_GEEK_Decode_Flash_Status(status);
  //The goal is to get to fast . . . so let's give that a try.
  switch(status)
    {
    case EVE_FLASH_STATUS_INIT:
      DBG_STAT("EVE_init_flash(): Error - status is INIT, not BASIC.\n");
      break;
    case EVE_FLASH_STATUS_DETACHED:
      DBG_STAT("EVE_init_flash(): Error - status is DETACHED, not BASIC.\n");
      break;
    case EVE_FLASH_STATUS_BASIC:
      //This is what we expect. Switch to fast.
      FWol=EVE_Set_Flash_to_Fast(FWol);
      //Print out the flash size, since we can.
      uint32_t
        flash_size;
      flash_size = EVE_REG_Read_32(EVE_REG_FLASH_SIZE);
      DBG_STAT("EVE_init_flash(): EVE_REG_FLASH_SIZE = %lu MB\n",flash_size);      
      break;
    case EVE_FLASH_STATUS_FULL:
      DBG_STAT("EVE_init_flash(): Warning - status already FULL.\n");
      break;
    default:
      DBG_STAT("EVE_init_flash(): Error - invalid status.\n");
      break;
    }
  return(FWol);
  }
//============================================================================
uint16_t Erase_Entire_Flash_Chip(uint16_t FWol)
  {
  DBG_GEEK("Erasing flash (takes a long time) . . . ");
  //Make sure the EVE is caught up
  FWol=Wait_for_EVE_Execution_Complete(FWol);
  //Send the command to the coprocessor
  FWol=EVE_Cmd_Dat_0(FWol,
                     EVE_ENC_CMD_FLASHERASE);
  // Update the ring buffer pointer so the coprocessor starts executing
  EVE_REG_Write_16(EVE_REG_CMD_WRITE, (FWol));
  //Now wait for the coprocessor to catch up
  FWol=Wait_for_EVE_Execution_Complete(FWol);
  DBG_GEEK("done. ");
  return(FWol);
  }
//============================================================================
#if (EVE_TOUCH_TYPE==EVE_TOUCH_CAPACITIVE) && (EVE_TOUCH_CAP_DEVICE==EVE_CAP_DEV_GT911)
//Used to send Goodix GT911 cap-touch init code to EVE from progmem
//Magic binary data from "AN_336 FT8xx - Selecting an LCD Display"
#define GOODIX_GT911_INIT_DATA_LENGTH (1216)
const uint8_t Goodix_GT911_Init_Data[GOODIX_GT911_INIT_DATA_LENGTH] PROGMEM = {
  0x1A,0xFF,0xFF,0xFF,0x20,0x20,0x30,0x00,0x04,0x00,0x00,0x00,0x02,0x00,0x00,0x00,
  0x22,0xFF,0xFF,0xFF,0x00,0xB0,0x30,0x00,0x78,0xDA,0xED,0x54,0xDD,0x6F,0x54,0x45,
  0x14,0x3F,0x33,0xB3,0x5D,0xA0,0x94,0x65,0x6F,0x4C,0x05,0x2C,0x8D,0x7B,0x6F,0xA1,
  0x0B,0xDB,0x9A,0x10,0x09,0x10,0x11,0xE5,0x9C,0x4B,0x1A,0x0B,0x0D,0x15,0xE3,0x03,
  0x10,0xFC,0xB8,0xB3,0x2D,0xDB,0x8F,0x2D,0x29,0x7D,0x90,0x48,0x43,0x64,0x96,0x47,
  0xBD,0x71,0x12,0x24,0x11,0xA5,0x64,0xA5,0xC6,0x10,0x20,0x11,0x95,0xC4,0xF0,0x80,
  0xA1,0x10,0xA4,0x26,0x36,0xF0,0x00,0xD1,0x48,0x82,0x0F,0x26,0x7D,0x30,0x42,0x52,
  0x1E,0x4C,0x13,0x1F,0xAC,0x67,0x2E,0x8B,0x18,0xFF,0x04,0xE3,0x9D,0xCC,0x9C,0x33,
  0x73,0x66,0xCE,0xE7,0xEF,0xDC,0x05,0xAA,0x5E,0x81,0x89,0x4B,0xC2,0xD8,0x62,0x5E,
  0x67,0x75,0x73,0x79,0x4C,0x83,0xB1,0x7D,0x59,0x7D,0x52,0x7B,0x3C,0xF3,0x3A,0x8E,
  0xF2,0xCC,0xB9,0xF3,0xBC,0x76,0x9C,0xE3,0x9B,0xCB,0xEE,0xEE,0xC3,0xFB,0xCD,0xE5,
  0x47,0x5C,0x1C,0xA9,0xBE,0xB8,0x54,0x8F,0x71,0x89,0x35,0xF4,0x67,0xB5,0xED,0x57,
  0xFD,0x71,0x89,0xE9,0x30,0x0C,0xC6,0xA5,0xB5,0x68,0x8B,0x19,0x54,0xFD,0x9B,0x72,
  0x4A,0xBF,0x00,0x36,0x8A,0xA3,0x0C,0x3E,0x83,0xCF,0x81,0x17,0xD9,0x22,0x5B,0x1F,
  0x80,0x41,0xF6,0xA3,0xAF,0xD5,0x08,0x93,0xD5,0x6B,0x23,0xCB,0x5E,0x6C,0x03,0x6F,
  0x28,0xAB,0x53,0x18,0x0F,0xA5,0xB1,0xDE,0x74,0x61,0x17,0xBC,0x8C,0xCE,0x96,0x2A,
  0x66,0xB5,0x57,0x4E,0x56,0xB6,0xAA,0x86,0xD7,0xF1,0x79,0x1A,0xF3,0xFC,0x02,0x4C,
  0x73,0xD9,0x8B,0xDE,0xCE,0xAD,0x88,0x84,0x51,0x3D,0x23,0xB9,0x27,0x71,0x17,0x2E,
  0xC7,0x4C,0xB2,0x36,0x97,0xB7,0xE0,0x00,0x28,0xBD,0x1C,0x95,0xB6,0x3A,0x83,0x4F,
  0x98,0x1E,0x4C,0x22,0x62,0xEA,0xA2,0xD8,0x85,0x8D,0x66,0x27,0xAA,0x28,0xC0,0x65,
  0x35,0xC9,0x92,0xBF,0x25,0x4D,0x2C,0xB1,0xD1,0x4A,0xD3,0x05,0xCE,0xBB,0x05,0x06,
  0xD8,0x2F,0x35,0x60,0x7B,0x16,0x32,0x67,0xFB,0xC0,0x54,0x11,0x4A,0xE3,0xB9,0x38,
  0x6A,0x33,0x5B,0xA1,0x60,0xB6,0xA3,0x30,0xAB,0x8D,0x8B,0x41,0x98,0x42,0x42,0x0B,
  0x66,0x2B,0x9E,0x4B,0x24,0x50,0x93,0xB8,0x93,0x8B,0x70,0x11,0xEB,0xD8,0x67,0x6F,
  0xEF,0xF5,0x5C,0x0A,0xAF,0xC2,0x28,0x2C,0x3A,0x7D,0x05,0x3B,0x70,0x32,0x67,0xF5,
  0x04,0x4E,0xC0,0x05,0x9C,0xC2,0x33,0x3C,0xBF,0x86,0x4B,0x6E,0xAD,0xED,0x2E,0xC0,
  0x79,0x9C,0xC0,0x73,0xB8,0xDA,0x78,0x43,0x3F,0x73,0x2E,0x0B,0x66,0x0A,0x61,0xE8,
  0x32,0xEB,0x72,0xB6,0x94,0x76,0xB2,0x29,0xBC,0x0C,0x87,0x4D,0xCA,0x7C,0x0C,0x60,
  0xEE,0x23,0xA1,0xEA,0xBD,0x81,0x17,0xF9,0xD4,0x8B,0xE6,0x19,0x35,0x30,0xCD,0x34,
  0x5D,0xA3,0x75,0x35,0x9A,0xAA,0x51,0x55,0xA3,0xB2,0x46,0x45,0x42,0xA7,0xF1,0x0E,
  0x2E,0xF1,0x01,0xE2,0x88,0x98,0xB3,0xC5,0x3B,0xB8,0x94,0xFE,0x31,0x84,0x30,0x0F,
  0xB0,0x89,0xC0,0x4C,0x83,0xC4,0x69,0x68,0xA2,0x56,0x51,0xA0,0xA5,0xFF,0x1A,0xAD,
  0xA2,0x89,0x56,0x91,0xD2,0xB7,0xC0,0x37,0xAF,0xC2,0xD3,0x3C,0x5B,0x78,0xE6,0xB8,
  0xAE,0x1B,0x29,0x83,0x9B,0x28,0xE0,0x1D,0x57,0xB3,0xE8,0x10,0x37,0x37,0x07,0xA5,
  0x93,0x51,0x17,0xA5,0x31,0x65,0x36,0xE0,0x4B,0xB4,0x51,0x6C,0x12,0x1D,0xE2,0x45,
  0xE1,0x6E,0xAF,0xE0,0x2A,0xD4,0x19,0x2F,0x82,0xC1,0x6E,0xEA,0xC0,0xD7,0xFC,0x38,
  0x4A,0xA2,0x18,0x2E,0xFB,0xAE,0x36,0x6A,0x44,0xF5,0x0E,0x09,0x9B,0xA0,0x16,0x78,
  0xCF,0x68,0xF0,0x1D,0x5A,0xB2,0x8C,0x1C,0x18,0xDC,0x2F,0xA6,0x70,0x3D,0xFB,0xD0,
  0xC0,0x6F,0x38,0xEF,0xEE,0x5D,0xFF,0xFB,0x3E,0x63,0x20,0xC1,0x4B,0x3D,0xBE,0xEB,
  0x7B,0xE5,0x6E,0xDA,0xC2,0x55,0x4F,0xE1,0x3B,0x62,0x14,0xEE,0xE3,0xEB,0xDC,0x0B,
  0xDD,0x95,0x19,0xB4,0x74,0xC2,0x9F,0x6F,0x60,0xC0,0x18,0xD5,0x3B,0x8B,0xB3,0x9C,
  0xD7,0x45,0xE6,0x13,0x18,0x23,0x87,0x75,0xCE,0xAB,0xCE,0xA2,0x43,0x81,0xEA,0x3D,
  0xEB,0x0B,0x68,0x67,0x54,0x40,0xDF,0xA7,0xFE,0x28,0xA3,0x65,0x5C,0x54,0x2B,0x96,
  0x2E,0xF9,0xDB,0xCD,0x07,0x74,0x0B,0x5B,0x68,0x3D,0x39,0x4B,0xDF,0x08,0x30,0x19,
  0x1C,0x77,0xFC,0xDE,0x71,0x31,0x56,0xF9,0x4A,0xB4,0xD3,0x9C,0xB5,0x3D,0xD7,0xA8,
  0x9D,0x07,0xFB,0xC7,0x96,0xF2,0xFA,0x5B,0x3A,0x84,0x5E,0x79,0x07,0x35,0x97,0x8B,
  0x62,0x06,0xA5,0x99,0x45,0xD6,0x20,0x6E,0xD3,0x64,0x65,0x1F,0x59,0x2D,0x51,0x62,
  0x17,0xCD,0xCD,0xC5,0xD1,0x6D,0xBA,0xC6,0x23,0x8D,0xBF,0xF9,0x19,0x3C,0x84,0xDF,
  0x99,0xFB,0x62,0x14,0xEF,0x92,0x8B,0x14,0xD9,0xFA,0x29,0xFA,0x89,0x3A,0xB1,0x5A,
  0x39,0x4F,0x33,0x6C,0xE9,0x14,0xFD,0xC2,0xBB,0x31,0xDE,0xCD,0x72,0x8D,0x60,0x30,
  0xAF,0xDB,0x6B,0x36,0x6F,0x8A,0x16,0x9A,0x67,0x6C,0x4F,0x3A,0xFC,0xB3,0xB2,0x4F,
  0xA4,0xC3,0x02,0x99,0x24,0x27,0xAA,0xC7,0xC9,0xA7,0xC5,0x55,0x6A,0x08,0x3B,0xB1,
  0x51,0x2E,0x38,0x02,0xE6,0x4B,0x72,0x11,0x37,0x70,0xBC,0x41,0xD0,0x89,0x4D,0x72,
  0x0A,0x73,0x37,0x3A,0xD0,0xC5,0xAD,0x7A,0x57,0x06,0x8C,0x6E,0x2A,0xD0,0x7C,0xA3,
  0x46,0x6C,0xF1,0x68,0x12,0xF5,0x62,0xD6,0xBB,0x86,0x35,0x2A,0xDD,0x16,0xB6,0x85,
  0xD3,0x74,0x94,0xB1,0xC2,0xD1,0xC0,0x55,0x5A,0xC7,0x3A,0x37,0xCB,0x02,0xE5,0x13,
  0x89,0xBB,0xA1,0xE4,0x9A,0x70,0xCB,0x91,0x7D,0xF4,0xBC,0xDC,0x76,0xE4,0x29,0xC9,
  0xB5,0x29,0xC3,0x90,0xD7,0xB7,0x33,0x50,0xFA,0x15,0xD9,0x10,0xD9,0xC8,0xEB,0x6D,
  0xE3,0xBC,0x7A,0xDA,0x8E,0x3C,0xAA,0xE0,0x70,0xF0,0xB8,0x82,0xE5,0xE0,0x71,0x05,
  0xDF,0x94,0xA3,0x50,0xA5,0xB7,0x82,0xBB,0x84,0x74,0x40,0xEE,0xA1,0x55,0xDC,0x73,
  0x8B,0xCD,0x62,0xE3,0xF4,0x1D,0x66,0x7D,0x07,0x25,0xF3,0x7B,0xDF,0x0B,0x1A,0x5C,
  0x3F,0xF3,0x74,0x3D,0xBF,0x8A,0x7B,0xF4,0xA0,0x54,0xBA,0x4A,0x1F,0x05,0xAE,0xF7,
  0x77,0x87,0xC7,0xF8,0xFD,0x87,0xF2,0x61,0x66,0x91,0xBE,0x90,0x0E,0x55,0xEE,0xDD,
  0xE7,0xC1,0x9E,0x30,0xCD,0x19,0x78,0xF8,0x0F,0xDC,0x1D,0x9E,0x09,0x46,0xB9,0x1E,
  0x67,0xE5,0x21,0xFE,0x17,0xED,0xA0,0xAC,0x3E,0xC1,0x5A,0xDE,0xE0,0xE8,0x0E,0xC8,
  0x38,0x5A,0x68,0x8E,0xE3,0x78,0x6E,0x06,0x15,0xD3,0xCB,0x41,0x96,0x63,0x97,0xDC,
  0xF7,0x57,0xA4,0x32,0x9F,0x31,0xEF,0xEA,0x3A,0x8E,0x00,0x6D,0x6C,0x7B,0x12,0x4F,
  0xE3,0x24,0x64,0xF8,0xDE,0xCD,0x60,0x7F,0x78,0x1A,0xAB,0xE4,0x45,0x3F,0x24,0x11,
  0xFC,0xC8,0x11,0x74,0xF2,0xBB,0xE3,0x58,0x8F,0xF7,0x02,0x4B,0xBF,0x06,0x82,0x3B,
  0xBC,0x0B,0x37,0xF0,0x1F,0xF3,0x7A,0x98,0xE2,0xB7,0xCF,0x9A,0x49,0xBC,0x27,0xDB,
  0x2B,0x69,0xDE,0x57,0x29,0x8F,0x8D,0x8C,0xAF,0x49,0x70,0xB8,0xFC,0x3D,0xB8,0x10,
  0x5A,0xFA,0x23,0xA8,0x52,0x77,0xB0,0x39,0x74,0x5E,0xC8,0x96,0x16,0xBE,0xB3,0x2C,
  0x68,0x0C,0xEB,0x54,0x95,0x66,0xFC,0x59,0x9A,0xC1,0x63,0xE4,0x6A,0xF2,0x7D,0xF8,
  0x40,0xC2,0xFF,0xDF,0x7F,0xF2,0x53,0x0B,0xFF,0x02,0x46,0xD6,0xE2,0x80,0x00,0x00,
  0x1A,0xFF,0xFF,0xFF,0x14,0x21,0x30,0x00,0x04,0x00,0x00,0x00,0x0F,0x00,0x00,0x00,
  0x1A,0xFF,0xFF,0xFF,0x20,0x20,0x30,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//----------------------------------------------------------------------------
uint16_t EVE_Init_Goodix_GT911(uint16_t FWol)
  {
  //Make sure that the chip is caught up.
  FWol=Wait_for_EVE_Execution_Complete(FWol);

  //Combine Address_offset into  then select the EVE
  //and send the 24-bit address and operation flag.
  _EVE_Select_and_Address(EVE_RAM_CMD|FWol,EVE_MEM_WRITE);

  //Pipe out data_length of data from data_address.
  const uint8_t
    *Flash_Data;
  uint32_t
    data_length;
  Flash_Data=Goodix_GT911_Init_Data;
  data_length=GOODIX_GT911_INIT_DATA_LENGTH;
  while(0 != data_length)
    {
    SPI.transfer(pgm_read_byte(Flash_Data));
    Flash_Data++;
    data_length--;
    }

  //Remember that we sent GOODIX_GT911_INIT_DATA_LENGTH bytes
  FWol=(FWol+GOODIX_GT911_INIT_DATA_LENGTH)&0xFFF;
  //De-select the EVE
  SET_EVE_CS_NOT;

  //OK, the data is in the EVE_RAM_CMD circular buffer, ask the chip
  //to process it.
  EVE_REG_Write_16(EVE_REG_CMD_WRITE, FWol);
  //Now wait for it to catch up
  FWol=Wait_for_EVE_Execution_Complete(FWol);

  //"Straight AN_336"
  //"Hold the touch engine in reset (write EVE_REG_CPURESET=2)"
  EVE_REG_Write_8(EVE_REG_CPURESET, 0x02);
  //Rudolph's
  //EVE_REG_Write_8(EVE_REG_TOUCH_OVERSAMPLE, 0x0f);
  //"Write REG_TOUCH_CONFIG=0x05D0"
  EVE_REG_Write_16(EVE_REG_TOUCH_CONFIG, 0x05D0);
  //    0    5    D   0
  // 0000 0101 1101 0000
  // |||| |||| |||| ||||-sampler clocks
  // |||| |||| |||| ||---supress 300ms startup
  // |||| |||| |||| |----vendor: 0=FocalTech, 1 = Azoteq
  // |||| |||| ||||------I2C address=0x5D
  // |||| |              ?? GT911 supports two I2C slave addresses:
  // |||| |              0xBA/0xBB and 0x28/0x29. ??
  // |||| |              0xBA is 1011 1010, bit reversed of 0x5D
  // |||| |--------------enable low power
  // ||||----------------ignore short circuit
  // |||-----------------reserved
  // |-------------------0 = capacitive, 1 = resistive
  //"Set GPIO0 output LOW"
  // The CFA10100 FT813 uses GPIO3 to reset GT911
  // Reset-Value is 0x8000 adding 0x0008 sets GPIO3 to output
  // Default-value for EVE_REG_GPIOX is 0x8000 -> Low output on GPIO3
  EVE_REG_Write_16(EVE_REG_GPIOX_DIR,0x8008);
  // "Wait more than 100us"
  delay(1);
  // "Write EVE_REG_CPURESET=0"
  EVE_REG_Write_8(EVE_REG_CPURESET, 0x00);
  // "Wait more than 55ms"
  // 2020-08-05 RR discovered that ~108ms is needed for multitouch
  delay(110);
  // "Set GPIO0 to input (floating)"
  // The CFA10100 FT813 uses GPIO3 to reset GT911
  EVE_REG_Write_16(EVE_REG_GPIOX_DIR,0x8000);

  //Return the updated address
  return(FWol);
  }
#endif // (TOUCH_TYPE==TOUCH_CAPACITIVE) && (TOUCH_CAP_DEVICE==CAP_DEV_GT911)
//============================================================================
//Apparently there is a "pen-up bug" in the FTDI.
#if (0 != EVE_PEN_UP_BUG_FIX)
//Magic binary data from FTDI supplied PC sample code
#define PEN_UP_BUG_FIX_INIT_DATA_LENGTH (1172)
const uint8_t Pen_Up_Bug_Fix_Init_Data[PEN_UP_BUG_FIX_INIT_DATA_LENGTH] PROGMEM = {
  0x1A,0xFF,0xFF,0xFF,0x20,0x20,0x30,0x00,0x04,0x00,0x00,0x00,0x02,0x00,0x00,0x00,
  0x22,0xFF,0xFF,0xFF,0x00,0xB0,0x30,0x00,0x78,0xDA,0xED,0x54,0xFF,0x6B,0x5C,0x45,
  0x10,0x9F,0x7D,0x9B,0x6B,0x8D,0xC9,0x79,0xF7,0x6A,0x82,0x09,0xE1,0xF4,0xEE,0x25,
  0xF6,0x92,0x34,0x3F,0x35,0x62,0xAC,0x35,0x30,0xF3,0x08,0x24,0xA6,0xB6,0x3F,0x88,
  0xD8,0xF6,0x07,0xCD,0xDB,0x4B,0x7A,0x77,0xB9,0x8B,0xC4,0x80,0x22,0x21,0x88,0x7B,
  0x41,0xF0,0x07,0x1F,0x2C,0x69,0x29,0x45,0x48,0x0C,0xD2,0xA6,0x06,0x11,0x0C,0x68,
  0x40,0xA5,0x49,0x85,0x86,0x62,0x50,0x4A,0x2B,0x94,0x16,0x14,0x85,0x28,0x14,0x22,
  0xE9,0x0F,0x52,0xFA,0x83,0x0A,0x71,0xF6,0xE5,0x6A,0xC5,0xBF,0xA1,0x6F,0xD9,0x37,
  0xB3,0x3B,0x3B,0x3B,0xF3,0x99,0x2F,0xFB,0x87,0x0F,0x3A,0xCC,0x0B,0x6D,0x72,0x59,
  0x95,0x54,0xA9,0xF2,0xAC,0x02,0x6D,0x0A,0x49,0x35,0xA7,0x5C,0x9E,0x59,0x15,0x06,
  0x59,0xE6,0xEC,0x7E,0x56,0x59,0xCE,0xF2,0xA9,0xB2,0x3D,0xBB,0x73,0x3E,0x55,0xBE,
  0xC7,0x85,0x81,0x2C,0x84,0xF9,0x5A,0x0C,0xF3,0x7C,0x43,0x31,0xA9,0x4C,0x51,0x16,
  0xC3,0x3C,0xD3,0x71,0x28,0x85,0xF9,0xFD,0x68,0x72,0x71,0x94,0xC5,0x83,0x69,0xA9,
  0x9E,0x03,0x13,0x84,0x41,0x1C,0xBB,0xF0,0x19,0x70,0x03,0x93,0x63,0xEB,0xA3,0x50,
  0x62,0x3F,0x0A,0x7B,0xB5,0xD0,0x49,0xD5,0x1D,0x18,0xF6,0xE2,0x79,0x70,0xC7,0x92,
  0x4A,0x62,0x38,0x16,0xC3,0x87,0xF5,0x20,0x0E,0xC2,0x11,0xB4,0xB6,0x64,0x2E,0xA9,
  0xDC,0x72,0xF4,0x67,0xAB,0x72,0xFC,0x29,0xDE,0x8F,0x61,0x96,0x35,0x40,0xA7,0xCA,
  0x6E,0xF0,0x56,0xBA,0x2D,0x10,0x5A,0x0E,0xBF,0x9E,0x6E,0xC4,0xA3,0xD8,0x82,0xF1,
  0xE8,0x9F,0x2A,0xFB,0x58,0x04,0xA9,0x5A,0x50,0x2A,0xA3,0xE2,0xF8,0xA8,0xCE,0x61,
  0x84,0x88,0xA9,0x45,0x71,0x14,0x1B,0xF5,0xCB,0x28,0x83,0x0C,0x36,0x57,0x25,0x8F,
  0xFD,0x2B,0x69,0x61,0x89,0x09,0xAC,0x67,0xB5,0x1A,0xAC,0x4F,0xA3,0x66,0xB8,0x8E,
  0x39,0x53,0x00,0x3D,0x8B,0x90,0x9F,0x4F,0x87,0xC1,0x3E,0x3D,0x00,0x9D,0xFA,0x05,
  0x14,0xBA,0x43,0x5B,0xFF,0x85,0xEE,0x8C,0x68,0xA7,0x1E,0xC0,0xC5,0x48,0x02,0x55,
  0x89,0xDD,0x59,0x86,0x65,0x8C,0xB1,0xBF,0xEE,0x89,0x4B,0x69,0x89,0x17,0x61,0x12,
  0xE2,0x1F,0xAF,0x62,0x1F,0xAE,0xA5,0x8D,0x5A,0xC1,0x15,0x58,0xC2,0x75,0x3C,0xCF,
  0xF3,0x73,0xF8,0xCA,0xFE,0xAB,0xAB,0x25,0xF8,0x14,0x57,0x70,0x11,0x3B,0xB4,0x3B,
  0xF6,0x23,0xC7,0xB1,0x53,0xAF,0x23,0x8C,0x5D,0xE0,0xBB,0xAC,0x2D,0xA9,0xAC,0x6C,
  0x1D,0x2F,0xC0,0x3B,0xBA,0x46,0x9F,0x01,0xD0,0x9B,0x88,0x28,0x47,0xBE,0xC3,0x65,
  0xDE,0x75,0x83,0xDD,0x5A,0x8E,0x6E,0x30,0xDD,0x55,0xA5,0xB1,0x2A,0xAD,0xA9,0x52,
  0x59,0xA5,0x4E,0x95,0x8A,0x88,0x6E,0xE0,0x0D,0x6C,0xC8,0x00,0x84,0x01,0x32,0x67,
  0x72,0x37,0xB0,0x91,0xFE,0x33,0x84,0xD0,0xB7,0xB1,0x89,0x40,0x6F,0x80,0xC0,0x0D,
  0x68,0xA2,0x56,0x91,0xA5,0xC6,0xFF,0x8D,0x56,0xD1,0x44,0x4F,0x92,0x54,0x57,0x21,
  0xA3,0x5F,0x84,0x27,0x78,0xB6,0xF2,0x4C,0x73,0x4E,0x7B,0x28,0x8E,0x07,0xC8,0xE3,
  0x15,0x67,0x32,0x67,0xAB,0x6D,0x7B,0x1B,0xF2,0x73,0xC1,0x00,0xC5,0xB0,0x46,0x3F,
  0x8D,0x7D,0xD4,0x23,0x0E,0x08,0x5F,0x1C,0x14,0xF6,0x74,0x9B,0x1E,0x84,0x98,0x76,
  0x03,0x28,0x1D,0xA2,0x3E,0x3C,0x9E,0x09,0x83,0x08,0xC5,0x78,0x31,0x63,0x73,0x23,
  0x27,0xE4,0xC8,0xA8,0x30,0x51,0xC5,0x02,0xAF,0xDD,0xE0,0xCD,0x8C,0xAD,0x94,0x04,
  0x57,0x0D,0x94,0x5E,0x13,0xEB,0xD8,0xCD,0x3E,0xD4,0xB3,0x0E,0xC7,0xDD,0xEA,0x15,
  0xDF,0xCF,0x70,0xFE,0xA3,0x5A,0xA9,0xC5,0xF7,0x32,0x6E,0xF9,0x10,0xF9,0x9C,0x71,
  0x89,0x53,0x62,0x92,0xA3,0x17,0xD7,0x73,0x10,0x92,0xAD,0x66,0x8E,0x9E,0x4A,0xA0,
  0xCD,0xB5,0x1C,0xD9,0x43,0x5D,0x9C,0x79,0x28,0x9C,0xCA,0x4C,0x82,0xA9,0xB8,0xC1,
  0x0C,0x09,0x7D,0x52,0x84,0x95,0x79,0xD1,0x41,0xDB,0xC6,0x0C,0x2F,0x50,0x07,0x0F,
  0xB6,0xA3,0x61,0x34,0xAB,0xCE,0xD3,0x14,0xBA,0xE5,0xC3,0x94,0x2A,0x0F,0x89,0x2D,
  0x74,0xF4,0x1D,0xBC,0x8A,0x69,0xB1,0x4C,0x8B,0x95,0x12,0x71,0x8D,0xA0,0xC0,0x01,
  0xDA,0xDE,0x0E,0x83,0x65,0x5A,0xE0,0x11,0xC3,0xEF,0x33,0x71,0x9C,0xC2,0x6F,0xF5,
  0x15,0x31,0x89,0xAB,0x64,0x3D,0xEE,0x15,0xA6,0x32,0x43,0x5F,0x53,0x3F,0x9A,0xCA,
  0x47,0x74,0x8D,0x2D,0xCD,0xD0,0x25,0x5E,0x85,0xBC,0xBA,0xC9,0xB1,0x86,0x52,0x56,
  0x75,0x54,0x6D,0x2E,0x89,0x34,0xED,0xD6,0x66,0xF8,0x16,0xFD,0x52,0x29,0x89,0x5B,
  0x94,0x25,0x1D,0x61,0x93,0xC3,0x56,0x7E,0x59,0x9C,0xA5,0x2D,0xD6,0xBC,0x2B,0x36,
  0x2B,0xA0,0x3F,0x24,0x8B,0xA9,0x8E,0x11,0x49,0xAF,0x1F,0xFF,0xE6,0xE8,0xA4,0xAF,
  0xF4,0xA1,0xC5,0x26,0x47,0x76,0x79,0x46,0x6D,0xE2,0x2B,0xFC,0x0E,0x64,0xF9,0x8E,
  0x87,0xB4,0x9C,0x30,0xB9,0x2D,0xAC,0xB0,0xF7,0x47,0xF4,0x1D,0x68,0xF0,0x19,0x03,
  0x25,0xD8,0x4E,0xB3,0x9F,0x65,0xAB,0x09,0x3F,0xE1,0x5F,0xA6,0x06,0x7E,0x47,0x18,
  0x1D,0x9C,0xA5,0xC7,0xFD,0x7E,0x6C,0x77,0x58,0x33,0x92,0xD8,0x13,0xBF,0x89,0x66,
  0xBF,0x6B,0xBA,0x44,0x6D,0x4E,0xF7,0xF4,0x5F,0x82,0x63,0x5E,0x86,0x31,0xB7,0x80,
  0x9E,0x54,0xCF,0x3A,0xF5,0x81,0x09,0xDC,0x91,0x4E,0x10,0xDA,0x55,0x66,0xE2,0x5E,
  0x66,0x8E,0x79,0xF7,0x33,0xF3,0x92,0x77,0x3F,0x33,0x03,0x0E,0x47,0x9D,0x5E,0xF5,
  0x56,0xA9,0x97,0x86,0x9C,0x63,0xD4,0xCE,0xBD,0x94,0xD0,0x09,0x6D,0xEF,0x2B,0xF2,
  0x7D,0x39,0x87,0xF9,0x13,0x6F,0x78,0xF5,0xB6,0x4F,0x79,0xDA,0x3E,0x6E,0xE7,0xDE,
  0xCB,0x39,0xDC,0xF5,0x34,0xED,0xD9,0x7E,0xF6,0xFD,0xB7,0x59,0x5F,0x3B,0x3B,0x91,
  0xEE,0xA5,0x0F,0x1C,0x5B,0x2D,0x56,0xEF,0x8C,0xD7,0xE7,0xC7,0x38,0x22,0x3B,0xEF,
  0x9A,0xEF,0x9F,0xF4,0x26,0x39,0x3F,0xA7,0x9C,0x29,0x7E,0x5F,0x0E,0x53,0x52,0xBD,
  0xCB,0xB7,0xF4,0x33,0xBA,0x21,0x27,0x0C,0xEA,0xF4,0x69,0x9C,0x4F,0x6F,0xA1,0x64,
  0xBA,0xE8,0x25,0x19,0xBB,0xC3,0xFD,0xFC,0x89,0x23,0xF5,0x02,0xF3,0x36,0xCF,0xF3,
  0x08,0xB0,0x8F,0x6D,0xAF,0xE1,0x39,0x5C,0x83,0x47,0xF8,0xDC,0x97,0xDE,0x71,0xFF,
  0x1C,0x1A,0x72,0x83,0x8B,0x11,0x82,0x6F,0x18,0x41,0x3F,0xEB,0x9D,0xC6,0x5A,0xBC,
  0xEE,0xD5,0xFB,0x3F,0x78,0x82,0x3B,0x97,0xBB,0x82,0x5F,0xC1,0xCF,0xFC,0x1A,0xD6,
  0xDD,0xAF,0xD7,0xF0,0xBA,0xD3,0x55,0xD9,0xC5,0x6B,0x43,0x7B,0x71,0x0F,0xD9,0x2A,
  0xFC,0xC9,0x9B,0xF5,0x85,0x5E,0x83,0x7A,0xFF,0x57,0xCF,0x50,0x8F,0xD7,0xEE,0x5B,
  0x2F,0x7E,0xF7,0x6A,0xF8,0xCC,0x9F,0x99,0xBB,0x74,0xDB,0x31,0x74,0x2D,0x73,0x93,
  0xB6,0x30,0xF2,0x04,0xBE,0xF0,0x7F,0x76,0xE0,0xC1,0xF7,0xE0,0x03,0x59,0xF7,0x0F,
  0x94,0x63,0xD3,0x67,0x1A,0xFF,0xFF,0xFF,0x14,0x21,0x30,0x00,0x04,0x00,0x00,0x00,
  0x0F,0x00,0x00,0x00,0x1A,0xFF,0xFF,0xFF,0x20,0x20,0x30,0x00,0x04,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00};
//----------------------------------------------------------------------------
uint16_t EVE_Init_Pen_Up_Bug_Fix(uint16_t FWol)
  {
  //Make sure that the chip is caught up.
  FWol=Wait_for_EVE_Execution_Complete(FWol);

  //Combine Address_offset into  then select the EVE
  //and send the 24-bit address and operation flag.
  _EVE_Select_and_Address(EVE_RAM_CMD|FWol,EVE_MEM_WRITE);

  //Pipe out data_length of data from data_address.
  const uint8_t
    *Flash_Data;
  uint32_t
    data_length;
  Flash_Data=Pen_Up_Bug_Fix_Init_Data;
  data_length=PEN_UP_BUG_FIX_INIT_DATA_LENGTH;
  while(0 != data_length)
    {
    SPI.transfer(pgm_read_byte(Flash_Data));
    Flash_Data++;
    data_length--;
    }

  //Remember that we sent GOODIX_GT911_INIT_DATA_LENGTH bytes
  FWol=(FWol+PEN_UP_BUG_FIX_INIT_DATA_LENGTH)&0xFFF;
  //De-select the EVE
  SET_EVE_CS_NOT;

  //OK, the data is in the EVE_RAM_CMD circular buffer, ask the chip
  //to process it.
  EVE_REG_Write_16(EVE_REG_CMD_WRITE, FWol);
  //Now wait for it to catch up
  FWol=Wait_for_EVE_Execution_Complete(FWol);

  //Return the updated address
  return(FWol);
  }
#endif // (0 != EVE_PEN_UP_BUG_FIX)
//============================================================================
#if EVE_TOUCH_TYPE==EVE_TOUCH_CAPACITIVE
//Returns a bit-mask of the points touched.
uint8_t Read_Touch(int16_t x_points[5], int16_t y_points[5])
  {
  uint32_t
    temp;

  //Capacitive
  temp = EVE_REG_Read_32(EVE_REG_CTOUCH_TOUCH0_XY);
  x_points[0]=(uint16_t)(temp>>16);
  y_points[0]=(uint16_t)temp;
  temp = EVE_REG_Read_32(EVE_REG_CTOUCH_TOUCH1_XY);
  x_points[1]=(uint16_t)(temp>>16);
  y_points[1]=(uint16_t)temp;
  temp = EVE_REG_Read_32(EVE_REG_CTOUCH_TOUCH2_XY);
  x_points[2]=(uint16_t)(temp>>16);
  y_points[2]=(uint16_t)temp;
  temp = EVE_REG_Read_32(EVE_REG_CTOUCH_TOUCH3_XY);
  x_points[3]=(uint16_t)(temp>>16);
  y_points[3]=(uint16_t)temp;

  //4th point is a special case
  x_points[4]=EVE_REG_Read_16(EVE_REG_CTOUCH_TOUCH4_X);
  y_points[4]=EVE_REG_Read_16(EVE_REG_CTOUCH_TOUCH4_Y);
  //Count up the points. 0x8000 means no touch.
  uint8_t
    points_touched_mask;
  points_touched_mask=0;

  uint8_t
    mask;
  mask=0x01;

  for(uint8_t i=0;i<5;i++)
    {
    if((0==(x_points[i]&0x8000))&&(0==(y_points[i]&0x8000)))
      {
      points_touched_mask|=mask;
      }
    mask<<=1;
    }
  return(points_touched_mask);
  }
#endif // EVE_TOUCH_TYPE==EVE_TOUCH_CAPACITIVE
//----------------------------------------------------------------------------
#if EVE_TOUCH_TYPE==EVE_TOUCH_RESISTIVE
//Returns a bit-mask of the points touched.
uint8_t Read_Touch(int16_t x_points[1], int16_t y_points[1])
  {
  uint32_t
    temp;
  //Resistive
  temp = EVE_REG_Read_32(EVE_REG_TOUCH_SCREEN_XY);
  x_points[0]=(uint16_t)(temp>>16);
  y_points[0]=(uint16_t)temp;
  //Count up the point.
  uint8_t
    points_touched_mask;
  points_touched_mask=0x00;
  if((0==(x_points[0]&0x8000))&&(0==(y_points[0]&0x8000)))
    {
    //We have a touch
    points_touched_mask=0x01; 
    }
  return(points_touched_mask);
  }
#endif // EVE_TOUCH_TYPE==EVE_TOUCH_RESISTIVE
//============================================================================
#if (DEBUG_LEVEL==DEBUG_GEEK)
// Do not call directly, use this macro instead:
//   DBG_GEEK_READ_AND_DUMP_TOUCH_MATRIX();
// You can use this function along with 
// #define EVE_TOUCH_CAL_NEEDED (1)
// #define DEBUG_LEVEL (DEBUG_GEEK)
// in CFA10100_defines.h to create and display a set of
// touch matrix elements.
void Read_and_Dump_Touch_Matrix(const __FlashStringHelper *message)
  {
  int32_t
    touch_transform[6];

  touch_transform[0] = EVE_REG_Read_32(EVE_REG_TOUCH_TRANSFORM_A);
  touch_transform[1] = EVE_REG_Read_32(EVE_REG_TOUCH_TRANSFORM_B);
  touch_transform[2] = EVE_REG_Read_32(EVE_REG_TOUCH_TRANSFORM_C);
  touch_transform[3] = EVE_REG_Read_32(EVE_REG_TOUCH_TRANSFORM_D);
  touch_transform[4] = EVE_REG_Read_32(EVE_REG_TOUCH_TRANSFORM_E);
  touch_transform[5] = EVE_REG_Read_32(EVE_REG_TOUCH_TRANSFORM_F);

  //We have to a little Arduino (well, AVR, really) dance to use
  //a flash string for our message.
  DBG_GEEK("Touch Transform Matrix, ");
  SerPrintFF(message);
  DBG_GEEK(":\n    {\n");
  for(uint8_t i=0;i<=5;i++)
    {
    char
      float_string[12];
    dtostrf((float)touch_transform[i]/(float)0x00010000,10,4, float_string);
    DBG_GEEK("    (int32_r)0x%08lX%c // [%c] = %s\n",
              touch_transform[i],i==5?' ':',',
              i+'A',
              float_string);
    }
  DBG_GEEK("    };\n");  
  }
#endif //(DEBUG_LEVEL==DEBUG_GEEK)  
//============================================================================
// Once you have an updated matrix from above, you can paste it in below
// and set:
// #define EVE_TOUCH_CAL_NEEDED (0)
// to send the stored touch matrix to the EVE
#if ((EVE_TOUCH_TYPE!=EVE_TOUCH_NONE) && (0 == EVE_TOUCH_CAL_NEEDED))
void Force_Touch_Matrix(void)
  {
#if 0
  //Default, identity matrix
  const int32_t touch_transform[6] PROGMEM =
    {
    (int32_t)0x00010000, // [A] =     1.0000
    (int32_t)0x00000000, // [B] =     0.0000
    (int32_t)0x00000000, // [C] =     0.0000
    (int32_t)0x00000000, // [D] =     0.0000
    (int32_t)0x00010000, // [E] =     1.0000
    (int32_t)0x00000000  // [F] =     0.0000
    };
#else
  //Resistive touch should be field calibrarted once, then the 
  //stored matrix can can be stored and sent to the EVE at
  //boot time.
  //In a final application the end user should have the
  //ability to preform a tocuh calibration which should be
  //stored as the new touch_transform[] matrix.
  const int32_t touch_transform[6] PROGMEM =
    {
    (int32_t)0x0000FFBD, // [A] =     0.9990
    (int32_t)0xFFFFFE55, // [B] =    -0.0065
    (int32_t)0x00085DC6, // [C] =     8.3663
    (int32_t)0xFFFFFF00, // [D] =    -0.0039
    (int32_t)0x0000F08D, // [E] =     0.9397
    (int32_t)0x0005FF21  // [F] =     5.9966
    };   
#endif

  EVE_REG_Write_32(EVE_REG_TOUCH_TRANSFORM_A,touch_transform[0]);
  EVE_REG_Write_32(EVE_REG_TOUCH_TRANSFORM_B,touch_transform[1]);
  EVE_REG_Write_32(EVE_REG_TOUCH_TRANSFORM_C,touch_transform[2]);
  EVE_REG_Write_32(EVE_REG_TOUCH_TRANSFORM_D,touch_transform[3]);
  EVE_REG_Write_32(EVE_REG_TOUCH_TRANSFORM_E,touch_transform[4]);
  EVE_REG_Write_32(EVE_REG_TOUCH_TRANSFORM_F,touch_transform[5]);
  }
#endif // ((EVE_TOUCH_TYPE!=EVE_TOUCH_NONE) && (0 == EVE_TOUCH_CAL_NEEDED))
//============================================================================
// This is the Pin Drive Table that is defined in the display header
// file, but actually created here.
PIN_DRIVE_TABLE;
//============================================================================
uint8_t EVE_Initialize(void)
  {
  // Wake up the EVE
  delay(20);          // Wait a few MS before waking the FT800
  CLR_EVE_PD_NOT;     // 1) lower PD#
  delay(6);           // 2) hold for 20ms
  SET_EVE_PD_NOT;     // 3) raise PD#
  delay(21);          // 4) wait for another 20ms before sending any commands

  //ref: https://github.com/RudolphRiedel/FT800-FT813/blob/4.x/EVE_commands.c
  //Reset, only required for warm-start if PowerDown line is not used
  //EVE_Command_Write(EVE_CORERST,0); 

#if (EVE_CLOCK_SOURCE == EVE_CLOCK_SOURCE_EXTERNAL)
  // Set FT800 for external clock
  EVE_Command_Write(EVE_CLKEXT,0);
  //Kick it to 72MHz
  EVE_Command_Write(EVE_CLKSEL,EVE_CLOCK_MUL);
  // 0100-0110 = 0x46
  // |||| ||||--[5:0] sets the clock frequency
  // ||         0: default clock speed
  // ||         1: reserved
  // ||         2:  2x 12MHz crystal =  24MHz internal clock
  // ||         3:  3x 12MHz crystal =  36MHz internal clock
  // ||         4:  4x 12MHz crystal =  48MHz internal clock
  // ||         5:  5x 12MHz crystal =  60MHz internal clock
  // ||         6:  6x 12MHz crystal =  72MHz internal clock
  // ||         7:  7x 12MHz crystal =  84MHz internal clock
  // ||---------[7:6] sets the PLL range
  //            0: (0x00)[5:0]=0,2,3 (use for default, 24MHz and 36MHz)
  //            1: (0x40)[5:0]=4,5,6 (use for 48MHz,60MHz and 72MHz)
#endif
  DBG_GEEK("EVE speed set to: %u MHz\n",EVE_CLOCK_SPEED/1000000UL);
  
  // Start FTxx
  EVE_Command_Write(EVE_ACTIVE,0);

  // From Rudolf, ref: https://github.com/RudolphRiedel/FT800-FT813
  // BRT AN033 BT81X_Series_Programming_Guide V1.2 had a small change to
  // chapter 2.4 "Initialization Sequence during Boot Up"
  // Send Host command “ACTIVE” and wait for at least 300 milliseconds.
  // Ensure that there is no SPI access during this time.
  // I asked Bridgetek for clarification why this has been made stricter.
  // From observation with quite a few of different displays I do not
  // agree that either the 300ms are necessary or that *reading* the
  // SPI while EVE inits itself is causing any issues.
  // But since BT815 at 72MHz need 42ms anyways before they start to
  // answer, here is my compromise, a fixed 40ms delay to provide at
  // least a short moment of silence for EVE   
  delay(40);

  //Poll EVE_REG_ID until the 0x7C vector comes back
  //Typically ~ 72mS -- Timeout at 250mS
  uint8_t
    timeout;
  timeout=0;
  uint8_t
    received_register;
  while(0x7C != (received_register=EVE_REG_Read_8(EVE_REG_ID)))
    {
    timeout++;
    if(250 <= timeout)
      {
      // Send an error message on the UART
      DBG_STAT("After %d tries, have not received ID of 0x7C. Last received was 0x%02X\n",timeout,received_register);
      DBG_STAT("Is the device connected? Is the right EVE device selected?");
      timeout=0;
      }
    else
      {
      delay(1);
      }
    }
  DBG_GEEK("Polled EVE_REG_ID register %d times.\n",timeout);

  //Poll EVE_REG_CPURESET until it returns 0, meaning that the reset is complete
  timeout=0;

  //Wait for the EVE_REG_CPURESET to indicate that all initializations are complete.
  while(0x00 != (received_register=EVE_REG_Read_8(EVE_REG_CPURESET)))
    {
    timeout++;
    if(250 <= timeout)
      {
      // Send an error message on the UART
      DBG_STAT("After %d tries, have not received EVE_REG_CPURESET of 0x00. Last received was 0x%02X\n",timeout,received_register);
      DBG_STAT("Is the device connected? Is the right EVE device selected?");
      timeout=0;
      }
    else
      {
      delay(1);
      }
    }
  DBG_GEEK("Polled EVE_REG_CPURESET register %d times.\n",timeout);

  //Just for kicks, verify and print out the chip ID.
  uint32_t
    Chip_ID;
  Chip_ID=EVE_REG_Read_32(EVE_CHIP_ID_ADDRESS);
  Validate_and_Print_Chip_ID(Chip_ID);

  //Remind the chip of the speed it is running at
  EVE_REG_Write_32(EVE_REG_FREQUENCY,EVE_CLOCK_SPEED);

#if (0 != EVE_TOUCH_CAL_NEEDED) || ((EVE_TOUCH_TYPE==EVE_TOUCH_CAPACITIVE) && (EVE_TOUCH_CAP_DEVICE==EVE_CAP_DEV_GT911)) || (0 != EVE_PEN_UP_BUG_FIX)
  //Get the currrent write pointer offset from the EVE
  uint16_t
    FWo;
  FWo = EVE_REG_Read_16(EVE_REG_CMD_WRITE);
  DBG_GEEK("Initial FWo read from EVE: %u\n",FWo);
#endif // (0 != EVE_TOUCH_CAL_NEEDED) || ((EVE_TOUCH_TYPE==EVE_TOUCH_CAPACITIVE) && (EVE_TOUCH_CAP_DEVICE==EVE_CAP_DEV_GT911)) || (0 != EVE_PEN_UP_BUG_FIX)

#if (EVE_TOUCH_TYPE==EVE_TOUCH_CAPACITIVE) && (EVE_TOUCH_CAP_DEVICE==EVE_CAP_DEV_GT911)
  FWo=EVE_Init_Goodix_GT911(FWo);
  DBG_GEEK("FWo read from EVE after GT911 initialization: %u Our SW copy: %u\n",EVE_REG_Read_16(EVE_REG_CMD_WRITE),FWo);
#endif // (EVE_TOUCH_TYPE==EVE_TOUCH_CAPACITIVE) && (EVE_TOUCH_CAP_DEVICE==EVE_CAP_DEV_GT911)

#if (0 != EVE_PEN_UP_BUG_FIX)
  FWo=EVE_Init_Pen_Up_Bug_Fix(FWo);
  DBG_GEEK("FWo read from EVE after Pen Up Bug Fix: %u Our SW copy: %u\n",EVE_REG_Read_16(EVE_REG_CMD_WRITE),FWo);
#endif // (0 != EVE_PEN_UP_BUG_FIX)

  // Now that we have some confidence we can talk to the EVE, proceed with
  // configuring it for our display.
  // Set PCLK to zero - don't clock the LCD until later
  EVE_REG_Write_8(EVE_REG_PCLK, 0);
  // Turn off backlight
  EVE_REG_Write_8(EVE_REG_PWM_DUTY, 0);

  // Initialize Display
  EVE_REG_Write_16(EVE_REG_HSIZE,   LCD_WIDTH);   // active display width
  EVE_REG_Write_16(EVE_REG_HCYCLE,  LCD_HCYCLE);  // total number of clocks per line, incl front/back porch
  EVE_REG_Write_16(EVE_REG_HOFFSET, LCD_HOFFSET); // start of active line
  EVE_REG_Write_16(EVE_REG_HSYNC0,  LCD_HSYNC0);  // start of horizontal sync pulse
  EVE_REG_Write_16(EVE_REG_HSYNC1,  LCD_HSYNC1);  // end of horizontal sync pulse
  EVE_REG_Write_16(EVE_REG_VSIZE,   LCD_HEIGHT);  // active display height
  EVE_REG_Write_16(EVE_REG_VCYCLE,  LCD_VCYCLE);  // total number of lines per screen, incl pre/post
  EVE_REG_Write_16(EVE_REG_VOFFSET, LCD_VOFFSET); // start of active screen
  EVE_REG_Write_16(EVE_REG_VSYNC0,  LCD_VSYNC0);  // start of vertical sync pulse
  EVE_REG_Write_16(EVE_REG_VSYNC1,  LCD_VSYNC1);  // end of vertical sync pulse
  EVE_REG_Write_8(EVE_REG_SWIZZLE,  LCD_SWIZZLE); // FT800 output to LCD - pin order
  EVE_REG_Write_8(EVE_REG_PCLK_POL, LCD_PCLKPOL); // LCD data is clocked in on this PCLK edge
  // Don't set PCLK yet - wait for just after the first display list

#if ((EVE_DEVICE == BT815) ||(EVE_DEVICE == BT816) ||(EVE_DEVICE == BT817) || (EVE_DEVICE == BT818))
  //BT81x series
  //Set the pin drive for any pins overridden by Pin_Drive_Table
  for(uint8_t i=0;i< sizeof(Pin_Drive_Table);i++)
      {
      EVE_Command_Write(EVE_PINDRIVE,pgm_read_byte(&Pin_Drive_Table[i]));
      DBG_GEEK("Pin_Drive_Table[%2d] = 0x%02x = %3d\n",
               i,
               pgm_read_byte(&Pin_Drive_Table[i]),
               pgm_read_byte(&Pin_Drive_Table[i]));
      }

  EVE_REG_Write_16(EVE_REG_ADAPTIVE_FRAMERATE, 0);
//  EVE_REG_Write_16(EVE_REG_AH_HCYCLE_MAX, 0);
      
#else
  //FT8xx series
  //Set the LCD Drive to 10mA or 5mA 
  // EVE_REG_GPIOX
  // 1111 1100 0000 0000
  // 5432 1098 7654 3210
  // DGGP SSIr rrrr GGGG 
  // |||| |||| |||| ||||-- GPIO 0 PIN
  // |||| |||| |||| |||--- GPIO 1 PIN
  // |||| |||| |||| ||---- GPIO 2 PIN
  // |||| |||| |||| |----- GPIO 3 PIN
  // |||| |||| ||||------- (reserved)
  // |||| |||------------- INT_N: 0=Open Drain, 1 = default
  // |||| ||-------------- SPI drive: 00=5mA, 01=10mA, 10=15mA, 11=20mA
  // ||||----------------- PCLK+RGB Drive: 0=4mA, 1=10mA
  // |||------------------ GPIO drive:  00=5mA, 01=10mA, 10=15mA, 11=20mA
  // |-------------------- DISP PIN
#if (0 != LCD_DRIVE_10MA)
  //Set 10mA drive for:
  //  PCLK, DISP , VSYNC, HSYNC, DE, RGB lines & BACKLIGHT
  EVE_REG_Write_16(EVE_REG_GPIOX,EVE_REG_Read_16(EVE_REG_GPIOX) | 0x1000);
#else  
  //Set 5mA drive for:
  //  PCLK, DISP , VSYNC, HSYNC, DE, RGB lines & BACKLIGHT
  EVE_REG_Write_16(EVE_REG_GPIOX,EVE_REG_Read_16(EVE_REG_GPIOX) & ~0x1000);
#endif // (0 != LCD_DRIVE_0_1p2mA_1_2p4mA)
#endif // ((EVE_DEVICE == BT815) ||(EVE_DEVICE == BT816) ||(EVE_DEVICE == BT817) || (EVE_DEVICE == BT818))

//EVE_REG_ADAPTIVE_FRAMERATE



#if (0 != LCD_PCLK_CSPREAD)
  EVE_REG_Write_8(EVE_REG_CSPREAD,1);
#else
  EVE_REG_Write_8(EVE_REG_CSPREAD,0);
#endif
#if (0 != LCD_DITHER)
  EVE_REG_Write_8(EVE_REG_DITHER,1);
#else
  EVE_REG_Write_8(EVE_REG_DITHER,0);
#endif
  // RGB 6-6-6 (default for FT810/FT811)
  //EVE_REG_Write_16(REG_OUTBITS,0x1B6);
  // RGB 8-8-8 (default for FT812/FT813)
  //EVE_REG_Write_16(REG_OUTBITS,0x000);

#if (EVE_TOUCH_TYPE==EVE_TOUCH_NONE)
  // Disable touch
  EVE_REG_Write_8(EVE_REG_TOUCH_MODE, EVE_TOUCHMODE_OFF);
  // Eliminate any false touches
  EVE_REG_Write_16(EVE_REG_TOUCH_RZTHRESH, 0);
#endif // (EVE_TOUCH_TYPE==EVE_TOUCH_NONE)

#if (EVE_TOUCH_TYPE==EVE_TOUCH_RESISTIVE)
  //Resistive.
  //Set the threshold somewhere reasonable.
  EVE_REG_Write_16(EVE_REG_TOUCH_RZTHRESH, 1200);
  //Oversample - higher current better resolution
  EVE_REG_Write_8(EVE_REG_TOUCH_OVERSAMPLE, 6);
  //Touch on, once per frame
  EVE_REG_Write_8(EVE_REG_TOUCH_MODE, EVE_TOUCHMODE_FRAME);
#endif // (EVE_TOUCH_TYPE==EVE_TOUCH_RESISTIVE)

#if (EVE_TOUCH_TYPE==EVE_TOUCH_CAPACITIVE)
  //Capacitive.
  //Touch on, at rate of touch IC
//  EVE_REG_Write_8(EVE_REG_TOUCH_MODE, EVE_TOUCHMODE_CONTINUOUS);
  EVE_REG_Write_8(EVE_REG_TOUCH_MODE, EVE_TOUCHMODE_FRAME);
  //Set compatibility (single touch) mode until after touch cal.
  EVE_REG_Write_8(EVE_REG_CTOUCH_EXTENDED, EVE_CTOUCH_MODE_COMPATIBILITY);
#endif // (EVE_TOUCH_TYPE==EVE_TOUCH_CAPACITIVE)

  // Turn recorded audio volume down & stop
  EVE_REG_Write_8(EVE_REG_VOL_PB, 0);
  EVE_REG_Write_32(EVE_REG_PLAYBACK_PLAY,0);
  // Turn synthesizer volume down
  EVE_REG_Write_8(EVE_REG_VOL_SOUND, 0);
  // Set synthesizer to mute
  EVE_REG_Write_16(EVE_REG_SOUND, 0x0060);
  EVE_REG_Write_8(EVE_REG_PLAY,1);

  // Write the initial display list directly to EVE_RAM_DL (the
  // display list RAM), bypassing the normal path through the
  // coprocessor - which apparently is not always available
  // at this early stage of the game.
  // This stub list just shows a black screen.
  EVE_REG_Write_32(EVE_RAM_DL + 0, EVE_ENC_CLEAR_COLOR_RGB(0x00,0x00,0x00));
  EVE_REG_Write_32(EVE_RAM_DL + 4, EVE_ENC_CLEAR(1/*color*/,1/*stencil*/,1/*tags*/));
  EVE_REG_Write_32(EVE_RAM_DL + 8, EVE_ENC_DISPLAY());

  //Tell the EVE that it can swap display lists at the
  //next available frame boundary.
  EVE_REG_Write_32(EVE_REG_DLSWAP, EVE_DLSWAP_FRAME);


/*
  from
  C:\parts_vendors\16000-16099\16084 FTDI EVE FT800 BridgeTek\ref\FTDI_Demo_Code_From_Graham\BRT_AN_025_BT81x_Beta\Code\FT9xx__BRT_AN_025__BT81X\lib\eve\source
  EVE_API.c
  
  // ---------------------- Reset all bitmap properties ------------------------
  EVE_LIB_BeginCoProList();
  EVE_CMD_DLSTART();
  EVE_CLEAR_COLOR_RGB(0, 0, 0);
  EVE_CLEAR(1,1,1);
  for (i = 0; i < 16; i++)
    {
    EVE_BITMAP_HANDLE(i);
    EVE_CMD_SETBITMAP(0,0,0,0);
    }
  EVE_DISPLAY();
  EVE_CMD_SWAP();
  EVE_LIB_EndCoProList();
  EVE_LIB_AwaitCoProEmpty();
*/




  // Nothing is being displayed yet... the pixel clock is still 0x00

  // Enable the DISP line of the LCD.
  // EVE_REG_GPIOX
  // 1111 1100 0000 0000
  // 5432 1098 7654 3210
  // DGGP SSIr rrrr GGGG 
  // |||| |||| |||| ||||-- GPIO 0 PIN
  // |||| |||| |||| |||--- GPIO 1 PIN
  // |||| |||| |||| ||---- GPIO 2 PIN
  // |||| |||| |||| |----- GPIO 3 PIN
  // |||| |||| ||||------- (reserved)
  // |||| |||------------- INT_N: 0=Open Drain, 1 = default
  // |||| ||-------------- SPI drive: 00=5mA, 01=10mA, 10=15mA, 11=20mA
  // ||||----------------- PCLK+RGB Drive: 0=4mA, 1=10mA
  // |||------------------ GPIO drive:  00=5mA, 01=10mA, 10=15mA, 11=20mA
  // |-------------------- DISP PIN
  EVE_REG_Write_16(EVE_REG_GPIOX,EVE_REG_Read_16(EVE_REG_GPIOX) | 0x8000);

  // Now start clocking data to the LCD panel, enabling the display
  EVE_REG_Write_8(EVE_REG_PCLK, LCD_PCLK);

  //Backlight frequency default is 250Hz. That is fine for CFA10100
  EVE_REG_Write_16(EVE_REG_PWM_HZ,250);
  //Crystalfontz EVE displays have soft start. No need to ramp.
//  EVE_REG_Write_8(EVE_REG_PWM_DUTY,128);
 //~~
 EVE_REG_Write_8(EVE_REG_PWM_DUTY,128);

  //See if we need to calibrate the touch screen
#if (EVE_TOUCH_TYPE != EVE_TOUCH_NONE)

#if (0 != EVE_TOUCH_CAL_NEEDED)
  DBG_STAT("Touch calibration . . .");
  //Ask the user to calibrate the touch screen.
  FWo=Calibrate_Touch(FWo);
  DBG_GEEK_READ_AND_DUMP_TOUCH_MATRIX("after touch cal");
  DBG_STAT("done.\n");
#else  // (0 != EVE_TOUCH_CAL_NEEDED)
  DBG_GEEK("Display specifies no touch calibration is needed.\n");
  //Recall the previously saved touch transform matrix -- which
  //will typically be the identity matrix for capacitive touch
  //screens, or an empirical matrix for resistive touch screens.
  Force_Touch_Matrix();
  DBG_GEEK_READ_AND_DUMP_TOUCH_MATRIX("recalled from flash");
#endif // (0 != EVE_TOUCH_CAL_NEEDED)

#if (EVE_TOUCH_TYPE==EVE_TOUCH_CAPACITIVE)
  //Capacitive.
  //Set multi-touch mode. You gots to do this _after_ the calibration.
  EVE_REG_Write_8(EVE_REG_CTOUCH_EXTENDED, EVE_CTOUCH_MODE_EXTENDED);
  DBG_GEEK("Multi-touch enabled.\n");
#endif // (EVE_TOUCH_TYPE==EVE_TOUCH_CAPACITIVE)

#if ((EVE_TOUCH_TYPE==EVE_TOUCH_CAPACITIVE) && (EVE_TOUCH_CAP_DEVICE==EVE_CAP_DEV_FT5316))
  //For some reason, the combination of the BT817 and the
  //FT5316 leave the screen in a "touched" state at this
  //point. It does not matter if the calibration has
  //been called or if the default FT5316 cal is used.
  //Empirically determined, resetting the touch
  //engine seems to fix it.
  EVE_REG_Write_8(EVE_REG_CPURESET, 2); //touch stopped
  delay(1);
  EVE_REG_Write_8(EVE_REG_CPURESET, 0); //all running
  delay(1);
  DBG_GEEK("FT5316 touch device reset.\n");
#endif // ((EVE_TOUCH_TYPE==EVE_TOUCH_CAPACITIVE) && (EVE_TOUCH_CAP_DEVICE==EVE_CAP_DEV_FT5316))

  DBG_GEEK("Waiting for no touch . . .");
  //Wait for the user to get their finger off the display--except
  //the cal does not exit until the finger is up -- so why does
  //this loop get executed up to ~ 85 times (for capacitive with
  //Goodix)? Removing this loop gives an initial, invalid, "false
  //touch" when the touch is read in the main loop. Very odd.
  //It does not hurt for any other controller.
  uint8_t
    points_touched_mask;  
  int16_t
    x_points[5];
  int16_t
    y_points[5];    
#if (DEBUG_LEVEL == DEBUG_GEEK)
  uint32_t
    touch_release_polls;
  touch_release_polls=0;
#endif // (DEBUG_LEVEL != DEBUG_NONE)
  do
    {
    //Read the touch screen.
    points_touched_mask=Read_Touch(x_points,y_points);
#if (DEBUG_LEVEL == DEBUG_GEEK)
    touch_release_polls++;
    delay(1);
#endif // (DEBUG_LEVEL != DEBUG_NONE)
    } while(0 != points_touched_mask);
  DBG_GEEK(" done. Polled %ld times(mS).\n",touch_release_polls);
#endif //  (EVE_TOUCH_TYPE != EVE_TOUCH_NONE)
  //Signal success.
  return(0);
  }
//===========================================================================
#if (0 != PROGRAM_FLASH_FROM_USD)
#if ((EVE_DEVICE == BT815) ||(EVE_DEVICE == BT816))
// zlib compressed unified.blob Thursday, ‎August ‎13, ‎2020, ‏‎11:24:18 AM
// from Eve Asset Builder v2.1.0
// C:\Users\Public\Documents\EVE Asset Builder\flash_support\bt815_6\unified.blob
const uint8_t flash_blob[320] PROGMEM =
  {
  0x78,0x9C,0x2B,0xB8,0xFF,0x7B,0x92,0x1C,0x23,0x03,0xC3,0x8B,0x65,0x06,0x0C,0x12,0x8C,0x02,0x0C,0x0C,0x0C,0x3F,0x6E,0x79,0x2D,0xD7,0xFA,0x8F,0x00,0x0E,0x0D,0xBD,
  0x45,0x49,0x45,0xD1,0x66,0x19,0x0E,0x53,0x8C,0x3E,0x1A,0xFF,0x36,0x6E,0x37,0xB6,0x30,0xAB,0xB3,0x6F,0x28,0x52,0x99,0xC0,0x70,0xBB,0xE0,0x00,0xC3,0x99,0x86,0x06,
  0x75,0x87,0x05,0xFF,0x61,0x3C,0x86,0x06,0xC1,0x03,0x22,0x0D,0xCC,0x17,0x27,0x96,0x39,0x34,0x30,0x1F,0x12,0x3C,0x3E,0xED,0xBF,0x80,0xC3,0x22,0xFB,0xE3,0x0D,0xED,
  0x45,0x81,0x25,0x4D,0x46,0x19,0x0E,0x81,0x26,0x9D,0xF6,0x20,0x33,0x1A,0xCD,0x4A,0xCD,0xA4,0x80,0xB6,0x19,0xBA,0x49,0x28,0xB7,0x58,0x3D,0xCE,0xD6,0x9E,0x0B,0xB1,
  0xCD,0xAD,0xE8,0xA8,0x21,0xC2,0x26,0x6E,0x86,0xAD,0xF6,0xAF,0x8B,0x4F,0x34,0x30,0x1C,0x3F,0xF5,0x9F,0xB1,0x61,0xCF,0x1A,0xE6,0x73,0x86,0x40,0x93,0x98,0x1A,0x6E,
  0x16,0xA6,0x99,0xCD,0x05,0x8A,0x5E,0x86,0x8A,0x82,0xDC,0x98,0x66,0x76,0x08,0x28,0x72,0x0B,0x28,0xC2,0xB2,0x9F,0xF9,0x5C,0x9A,0x99,0x02,0x90,0xF7,0xEC,0x3F,0x53,
  0xD1,0xAE,0x86,0x3D,0x0D,0x06,0x0E,0xCF,0x80,0xE2,0x1E,0x60,0xF1,0xF7,0x40,0xF1,0x7F,0x40,0xF1,0xC2,0x06,0x66,0x20,0xCD,0xD8,0x90,0x07,0x14,0x8D,0x2C,0x62,0x6C,
  0x08,0x03,0xD2,0x57,0x8A,0x2F,0x14,0x27,0x15,0x6D,0x07,0xDA,0x91,0x54,0xA4,0xD0,0xF0,0xA0,0xB8,0xA9,0x48,0xDC,0x6C,0x83,0xD9,0x3E,0xFB,0x05,0xDC,0x91,0x1F,0xDB,
  0x39,0x2B,0x1D,0x3E,0xEA,0x4F,0xEC,0x61,0x79,0x99,0x64,0x6E,0xD4,0x26,0xF0,0xEB,0xFF,0x28,0x18,0x05,0xA3,0x60,0x14,0x8C,0x82,0x51,0x30,0x0A,0x46,0xC1,0x28,0x18,
  0x05,0xA3,0x60,0x14,0x8C,0x82,0x51,0x40,0x17,0xC0,0xCC,0x10,0xC1,0x18,0x85,0x07,0x94,0x97,0x85,0x18,0x09,0xFC,0xBB,0xB3,0x0B,0x9F,0x1A,0x00,0x17,0xBA,0x1E,0xE6
  };
#endif // ((EVE_DEVICE == BT815) ||(EVE_DEVICE == BT816))

#if ((EVE_DEVICE == BT817) || (EVE_DEVICE == BT818))
// zlib compressed unified.blob ‎Thursday, ‎August ‎13, ‎2020, ‏‎11:24:18 AM
// from Eve Asset Builder v2.1.0
// C:\Users\Public\Documents\EVE Asset Builder\flash_support\bt817_8\unified.blob
const uint8_t flash_blob[799] PROGMEM =
  {
  0x78,0x9C,0xED,0x51,0xDB,0x4B,0x54,0x41,0x1C,0x9E,0xB3,0x63,0xF7,0x8D,0x76,0xBA,0x80,0x8A,0x76,0x23,0x38,0x10,0x15,0xED,0xE6,0x21,0x83,0x38,0x3B,0xDD,0x0C,0x34,
  0xD8,0x20,0xA1,0x30,0x89,0x1E,0x02,0xA5,0x08,0xCF,0x71,0xAD,0xB5,0xA0,0x71,0x14,0xB3,0x1E,0x56,0x42,0xF3,0x21,0x62,0xBB,0x41,0x11,0xF9,0x50,0x41,0x37,0xB2,0xA2,
  0x19,0x6D,0x39,0xD4,0x6E,0x1B,0x65,0x51,0x54,0x28,0x51,0x84,0x11,0x84,0x2F,0x45,0x51,0x9D,0x69,0xCE,0x39,0x2B,0xBE,0xF9,0x07,0xC4,0xF9,0x1E,0x86,0x8F,0xDF,0x7C,
  0xF3,0xFD,0xBE,0xDF,0x6F,0x8C,0xE1,0xDF,0xDD,0x7D,0x10,0x80,0x2D,0x17,0x57,0x82,0x9B,0x30,0x04,0x00,0xF8,0xF9,0xB6,0xF2,0xF2,0x52,0x31,0x0E,0x60,0x21,0x56,0x66,
  0x43,0x96,0x64,0x05,0x14,0x3C,0x9E,0x45,0xE1,0xC0,0x42,0x0A,0xFA,0x43,0xBC,0xFD,0x28,0x78,0xB7,0x81,0x6D,0x60,0x2A,0x1F,0xAF,0x95,0x66,0x9C,0xCA,0x09,0x8E,0xD8,
  0x3B,0x63,0xAB,0x5D,0x28,0xF5,0x1A,0x09,0xB9,0xE7,0x54,0xF7,0xD4,0x74,0x41,0x9F,0x37,0x2A,0xB8,0x8A,0x7C,0x8C,0x20,0x36,0x56,0xF5,0x14,0x85,0x14,0x3E,0xD6,0x74,
  0x43,0xDE,0xCF,0x37,0x67,0x9A,0x54,0x7A,0xD6,0xD9,0x49,0x16,0xA0,0x2F,0x1B,0x2B,0xF5,0x3A,0xD2,0xA0,0x17,0xC5,0xD4,0x58,0x2D,0x49,0x0E,0xA9,0x3C,0x8E,0x51,0xBA,
  0xDD,0x16,0x14,0x0C,0x04,0x71,0x9B,0x8D,0x32,0xA3,0xEB,0x9A,0xC8,0x61,0x3D,0xC8,0x42,0x60,0x3B,0x51,0xE8,0xA7,0xFB,0x30,0xD7,0x4C,0xEA,0x48,0xB1,0x96,0xC0,0x2D,
  0xAB,0x6A,0xB4,0x0E,0xFD,0x4C,0x64,0x91,0x36,0x4F,0x9B,0x6D,0x56,0x85,0x7B,0x48,0x84,0xB6,0x98,0x88,0x7D,0x30,0x4A,0xC2,0x0A,0x5D,0xF6,0x00,0xE6,0x12,0x66,0x33,
  0xE9,0x21,0x87,0xC2,0x09,0x9C,0x0C,0x2F,0x58,0x95,0x92,0xDA,0x37,0x65,0xBF,0xCB,0x26,0xE5,0x53,0x28,0x32,0x45,0x10,0xF7,0xCA,0x1C,0x09,0xB3,0x5C,0xBE,0xEC,0x34,
  0x02,0x34,0x5D,0x70,0x83,0x38,0x09,0xD2,0xF9,0x04,0x03,0x76,0x2F,0x69,0x20,0x4D,0xE4,0x9E,0x1E,0xC2,0xBD,0xE4,0xBB,0xCC,0x7B,0x5B,0xFA,0x81,0xD6,0x38,0x36,0x86,
  0x50,0xB6,0x96,0x20,0xEB,0xA9,0x7D,0x82,0xAB,0xFC,0x06,0x41,0xE9,0x61,0xBB,0x81,0x94,0x66,0x32,0x44,0xE5,0xBD,0x04,0xB4,0xC2,0xFE,0x57,0xF2,0x4D,0x90,0x59,0xBA,
  0xD7,0xFD,0x6E,0xBE,0x7B,0x0F,0x71,0x7A,0x95,0x98,0x95,0xBA,0x42,0x5F,0xC8,0x8C,0xE7,0xF2,0xF7,0xDF,0xDC,0xFB,0x98,0xF6,0x79,0xE3,0x1F,0xBB,0x3E,0x9C,0x92,0xFB,
  0x37,0x18,0xC8,0xD6,0xCB,0x79,0x30,0x85,0xAF,0xBB,0xCD,0x94,0xFB,0x23,0x4E,0xAF,0x84,0x89,0xD2,0x0B,0x44,0x20,0x3F,0x6B,0x69,0x66,0x2C,0xCD,0x0F,0x03,0x59,0x73,
  0x45,0x95,0xA9,0x72,0xC8,0x9D,0xFE,0x33,0xA2,0x4E,0xFF,0x43,0x61,0x95,0x6F,0x97,0xE9,0x56,0x88,0xB1,0x74,0x75,0x6E,0xBA,0x25,0xEE,0x6D,0xB1,0xE6,0x6D,0x4A,0xE5,
  0x53,0xB0,0xE3,0xBB,0x5E,0x38,0x1B,0x87,0x38,0x43,0xA6,0xE0,0x1E,0x57,0xB7,0x5A,0xEA,0x20,0x77,0x7C,0xBC,0xBC,0x65,0xD1,0xA2,0x18,0x68,0xBD,0xB3,0x59,0xFE,0x18,
  0xF7,0x58,0x1C,0x97,0x66,0xAA,0xBB,0xDC,0xBC,0x83,0x0A,0x2D,0x96,0x9A,0x95,0x64,0x97,0x58,0x1C,0x55,0x68,0x85,0xCB,0xF7,0x88,0xB0,0xE4,0x9F,0x5C,0xBE,0x57,0x54,
  0x48,0xFE,0xC8,0xE5,0x86,0x18,0xD4,0x21,0x9F,0x26,0xFD,0x4E,0x75,0x39,0xB3,0x41,0xE9,0x77,0x75,0x7F,0x2A,0xCF,0x01,0x55,0xB9,0x82,0x55,0x4E,0xA9,0x71,0x12,0xE6,
  0x28,0x2D,0x4F,0xC2,0x1C,0x62,0x60,0xC8,0xD9,0x4A,0xB4,0x33,0x82,0x2F,0x09,0x83,0xA1,0xEC,0x29,0xAF,0x6F,0x16,0xBE,0x4E,0xE5,0xD9,0xE8,0x3A,0xC4,0x50,0x06,0x3E,
  0xBB,0x28,0x82,0x2C,0xC8,0x90,0xDC,0x97,0xCA,0x2F,0x45,0x21,0x9F,0x4A,0x61,0x3F,0xB2,0x8E,0x89,0x6F,0xCB,0x0D,0x0A,0xAC,0x6B,0x02,0xF2,0xCA,0x68,0x85,0x64,0x37,
  0x25,0xFB,0xA5,0xEF,0x95,0xEC,0xAE,0x64,0x8D,0xD1,0xE9,0x6B,0x14,0x5A,0xFD,0x10,0xE6,0x76,0x48,0xBF,0x6D,0x72,0xDF,0x7D,0x2D,0x25,0x1D,0x9E,0x73,0x10,0xDB,0xA2,
  0x28,0x3E,0xB4,0xE3,0x89,0x01,0xFB,0xFB,0xCE,0x03,0x6B,0x50,0x78,0xD3,0x4E,0xD6,0x20,0xC5,0xD7,0x60,0x0E,0x58,0xC3,0x62,0x2B,0x3D,0x62,0x7E,0x39,0xE0,0x4D,0xEE,
  0xD4,0x17,0xB6,0x39,0xF5,0x11,0xE1,0xCD,0xEF,0x2A,0x8F,0x3B,0x95,0xEF,0x22,0x2C,0xFF,0x2F,0x40,0x2F,0x18,0x63,0xDA,0xDD,0xAD,0xC0,0xFA,0x2B,0xBC,0xDD,0x80,0xB6,
  0x49,0x1A,0xE4,0x5F,0x35,0x1E,0xDD,0x14,0x9F,0x33,0x52,0x0F,0xF6,0x05,0xEF,0xC4,0x76,0x5E,0x39,0xBB,0x36,0x7B,0x3D,0x30,0xD2,0x7C,0x5A,0xF8,0xF0,0xE1,0xC3,0x87,
  0x0F,0x1F,0x3E,0x7C,0xF8,0xF0,0xE1,0xE3,0x3F,0xC3,0x2D,0xB0,0x2D,0x50,0x33,0x01,0x12,0x07,0xAB,0x23,0x21,0xFB,0x7D,0xDF,0x44,0x9A,0x7F,0x63,0x94,0xA0,0xDB
  };
#endif // ((EVE_DEVICE == BT817) || (EVE_DEVICE == BT818))


  
//---------------------------------------------------------------------------
uint16_t Write_BLOB_to_Flash_Sector_0(uint16_t FWol,
                                      uint32_t First_Unused_RAM_G_Address,
                                      uint32_t *Flash_Sector)
  {
  // Bridgetek: Let's be clear, "BLOB" stands for Binary Large OBject
  // and 4K is not large, 320 or 799 bytes less so :-(
  //
  // We have the zlib compressed BLOB in our flash at flash_blob[]
  // We need to expand that out to it's native 4K in to RAM_G first
  //Make sure the EVE is caught up
  FWol=Wait_for_EVE_Execution_Complete(FWol);
  //Expand the flash_blob, keep track of how big it expands to 
  uint32_t
    End_RAM_G_Address;
  End_RAM_G_Address=First_Unused_RAM_G_Address;
  FWol=EVE_Inflate_to_RAM_G(FWol,
                            flash_blob,
                            sizeof(flash_blob),
                            &End_RAM_G_Address);
  DBG_GEEK("RAM_G before flash_blob:  0x%08lX = %lu\n",First_Unused_RAM_G_Address,First_Unused_RAM_G_Address);
  DBG_GEEK("RAM_G after flash_blob:   0x%08lX = %lu\n",End_RAM_G_Address,End_RAM_G_Address);
  DBG_GEEK("expanded flash_blob_size: 0x%08lX = %lu\n",End_RAM_G_Address-First_Unused_RAM_G_Address,End_RAM_G_Address-First_Unused_RAM_G_Address);

  //Now ask the EVE to program the 1st sector of flash with the expanded
  //flash_blob sitting in RAMG at 
  FWol=EVE_Cmd_Dat_3(FWol,
                     //Command
                     EVE_ENC_CMD_FLASHUPDATE,
                     //destination in flash (must be 4096 byte aligned)
                     *Flash_Sector*4096,
                     //source in RAM_G (must be 4-byte aligned)
                     First_Unused_RAM_G_Address,
                     //Size (must be multiple iof 4096
                     End_RAM_G_Address-First_Unused_RAM_G_Address);

  // Update the ring buffer pointer so the coprocessor starts executing
  EVE_REG_Write_16(EVE_REG_CMD_WRITE, (FWol));
  //Remember that the flash sector 0 is used
  *Flash_Sector=1;
  //Now wait for the coprocessor to catch up
  FWol=Wait_for_EVE_Execution_Complete(FWol);
  return(FWol);  
  }
//===========================================================================
uint16_t Inflate_uSD_File_To_Flash(uint16_t FWol,
                                   const char *File_Name,
                                   uint32_t First_Unused_RAM_G_Address,
                                   uint32_t *Flash_Sector,
                                   uint32_t *Data_Length)
  {
  // Outline:
  //  * Read the compressed, 256-byte sectors from the uSD and inflate them into RAM_G
  //  * Figure out how big the resulting data in RAM_G is
  //  * Write the RAM_G data to flash, using 4096 byte sectors.
  //
  // This will use as much RAM_G as the file inflates to, but as soon as it is written
  // to flash, the RAM_G is no longer needed and can be re-used.
//  DBG_GEEK("\nInflate_uSD_File_To_Flash initial RAM_G: 0x%08lX = %lu\n",First_Unused_RAM_G_Address,First_Unused_RAM_G_Address);
    
  //Since the Arduino uSD card is slow, put up a "please wait" screen.
  FWol=Start_Busy_Spinner_Screen(FWol,
                                 //clear color
                                 EVE_ENC_CLEAR_COLOR_RGB(0x00,0x00,0xFF),
                                 //text color
                                 EVE_ENC_COLOR_RGB(0xFF,0xFF,0xFF),
                                 //spinner color
                                 EVE_ENC_COLOR_RGB(0x00,0xFF,0x00),
                                 F("Reading uSD, writing to flash: \"BLUEMARB.AZ\" . . ."));
                                 
  //Make sure the EVE is caught up from any previous commands
  FWol=Wait_for_EVE_Execution_Complete(FWol);

  File
    binary_file;
  binary_file = SD.open(File_Name,FILE_READ);    
  if(0 == binary_file)
    {
    DBG_STAT("\nWrite_uSD_File_To_Flash(): Can't open \"%s\".\n",File_Name);
    return(FWol);
    }

  //We are assuming that the entire inflated data will fit into
  //RAM_G.
  uint32_t
    bytes_remaining;
  bytes_remaining=binary_file.size();

  //We need to ensure 4-byte alignment.
  bytes_remaining=(bytes_remaining+0x03)&0xFFFFFFFC;
  
  DBG_GEEK("\nInflate_uSD_File_To_Flash: found \"%s\" size: %lu\n",
           binary_file.name(),binary_file.size());

  // Write the EVE_ENC_CMD_INFLATE and parameters
  FWol=EVE_Cmd_Dat_1(FWol,
                     //The command
                     EVE_ENC_CMD_INFLATE,
                     //First is 32-bit RAM_G offset.
                     First_Unused_RAM_G_Address);

  //Limited RAM on the Arduino, so use a reasonable block size
  #define CHUNK_SIZE (256)
  uint8_t
    this_chunk[CHUNK_SIZE];
  do
    {
    //Transfer a full chunk, or a partial chunk on the
    //last transfer.
    uint32_t
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
    //Keep track of bytes_remaining.
    bytes_remaining-=this_chunk_size;
    //Set the address in EVE_RAM_CMD for this block
    _EVE_Select_and_Address((uint32_t)EVE_RAM_CMD|(uint32_t)FWol,
                            EVE_MEM_WRITE);
    //Pipe out this_chunk_size of data from this_chunk[]
    //to the EVE.
    SPI.transfer(this_chunk,this_chunk_size);
    //Now we need to end this command.
    SET_EVE_CS_NOT;
    // Keep track that we sent bytes_this_block bytes.
    FWol=(FWol+this_chunk_size)&0xFFF;
    //OK, the data is in the EVE_RAM_CMD circular buffer, ask the chip
    //to process it.
    EVE_REG_Write_16(EVE_REG_CMD_WRITE, FWol);
    //Now wait for it to catch up
    FWol=Wait_for_EVE_Execution_Complete(FWol);
    } // chunk loop
  while(0 != bytes_remaining);
  //Release the BMP file handle
  binary_file.close();

  //Get the first free address in RAM_G from after the inflated data, so
  //we know how large the data inflated to. We only use RAM_G
  //temporarily, as soon as the data is flashed then we do not need
  //it in RAM_G and it can be overwritten for other uses.
  uint32_t
   Final_RAM_G_Address;
  FWol=Get_RAM_G_Pointer_After_INFLATE(FWol,
                                       &Final_RAM_G_Address);
  //Let the caller know how many bytes in RAM_G are relevant data
  *Data_Length=Final_RAM_G_Address-First_Unused_RAM_G_Address;
  
  DBG_GEEK("Inflate_uSD_File_To_Flash RAM_G: Start = %lu End = %lu Amount = %lu\n",First_Unused_RAM_G_Address,Final_RAM_G_Address,*Data_Length);

  //Calculate the length of flash to write
  uint32_t
    flash_length;
  flash_length=Final_RAM_G_Address-First_Unused_RAM_G_Address;
  //Make it a multiple of the 4096 flash sector size
  flash_length=(flash_length+0xFFF) & 0xFFFFF000;
  DBG_GEEK("Inflate_uSD_File_To_Flash FLASH: used = %lu, sectors = %lu\n",flash_length,(flash_length >> 12));

  //Now we need to tell the EVE to write the inflated data to flash.
//  DBG_GEEK("Writing %ld bytes to flash sector = %ld, flash address = %ld RAM_G = %ld. . .",flash_length,*Flash_Sector,(*Flash_Sector)<<12,First_Unused_RAM_G_Address);
  FWol=EVE_Cmd_Dat_3(FWol,
                     //Command
                     EVE_ENC_CMD_FLASHUPDATE,
                     //destination in flash (must be 4096 byte aligned)
                     (*Flash_Sector)<<12,
                     //source in RAM_G (must be 4-byte aligned)
                     First_Unused_RAM_G_Address,
                     //Size (must be multiple of 4096)
                     flash_length);

  // Update the ring buffer pointer so the coprocessor starts executing
  EVE_REG_Write_16(EVE_REG_CMD_WRITE, (FWol));
  //Remember that we used some flash sectors
  *Flash_Sector+=(flash_length >> 12);
  DBG_GEEK("Next free Flash_Sector = %ld\n",*Flash_Sector);

  //Now wait for the coprocessor to catch up
  FWol=Wait_for_EVE_Execution_Complete(FWol);
  return(FWol);  
  }
//===========================================================================
uint16_t Write_uSD_File_To_Flash(uint16_t FWol,
                                 const char *File_Name,
                                 uint32_t First_Unused_RAM_G_Address,
                                 uint32_t *Flash_Sector,
                                 uint32_t *Data_Length)
  {
  // Outline:
  //  * Read the 256-byte sectors from the uSD into RAM_G
  //  * Write the RAM_G data to flash, using 4096 byte sectors.
  //
  // This will use 4096 RAM_G, but as soon as it is written
  // to flash, the RAM_G is no longer needed and can be re-used.
    
  DBG_GEEK("\n");
  File
    binary_file;
  binary_file = SD.open(File_Name,FILE_READ);    
  if(0 == binary_file)
    {
    DBG_STAT("  Write_uSD_File_To_Flash(): Can't open \"%s\".\n",File_Name);
    return(FWol);
    }

  //Here would be a good place to do sanity checks with the size
  //of the file vs remaining RAM_G space. You would have to pass
  //in the available RAM_G.
  uint32_t
    RAM_G_Address;
  RAM_G_Address=First_Unused_RAM_G_Address;

  uint32_t
    bytes_remaining;
  bytes_remaining=binary_file.size();
  //Let the caller know how much of the flash is good data.
  *Data_Length=bytes_remaining;
  DBG_GEEK("  Write_uSD_File_To_Flash: found %s size: %lu\n",
           binary_file.name(),binary_file.size());
  //Limited RAM on the Arduino, so use a reasonable block size
  #define CHUNK_SIZE (256)
  uint8_t
    this_chunk[CHUNK_SIZE];
  //We need to write 16 of the 265 byte chunks (4096 bytes) to RAM_G
  //then we need to write those 4096 bytes to one flash sector.
  uint32_t
    flash_sector_bytes;
  flash_sector_bytes=0;
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
      //This is the last chunk, force a flash write.
      flash_sector_bytes=4096;
      }
    else
      {
      this_chunk_size=CHUNK_SIZE;
      flash_sector_bytes+=CHUNK_SIZE;
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

    //Check to see if we need to write this flash sector.
    if(4096 <= flash_sector_bytes)
      {
      //Write this flash sector
      FWol=EVE_Cmd_Dat_3(FWol,
                         //Command
                         EVE_ENC_CMD_FLASHUPDATE,
                         //destination in flash (must be 4096 byte aligned)
                         (*Flash_Sector)<<12,
                         //source in RAM_G (must be 4-byte aligned)
                         First_Unused_RAM_G_Address,
                         //Size (must be multiple of 4096)
                         4096);
      // Update the ring buffer pointer so the coprocessor starts executing
      EVE_REG_Write_16(EVE_REG_CMD_WRITE, (FWol));

      //Remember that we used some flash sectors
      (*Flash_Sector)++;
      DBG_STAT("Next free Flash_Sector = %ld, bytes remaining = %ld\n",*Flash_Sector,bytes_remaining);

      //Now wait for the coprocessor to catch up
      FWol=Wait_for_EVE_Execution_Complete(FWol);

      //Reset RAM_G address
      RAM_G_Address=First_Unused_RAM_G_Address;
      //Reset offset within the flash sector
      flash_sector_bytes=0;
      } // write a flash sector
    } // chunk loop
  while(0 != bytes_remaining);
  //Release the BMP file handle
  binary_file.close();
  return(FWol);  
  }
#endif //PROGRAM_FLASH_FROM_USD
//============================================================================
uint16_t EVE_Set_Bitmap(uint16_t FWol,
                        uint8_t  handle,
                        uint32_t source,
                        uint16_t format,
                        uint16_t width,
                        uint16_t height,
                        uint16_t colstride,
                        uint16_t rowstride,
                        uint8_t  filter,
                        uint8_t  wrapx,
                        uint8_t  wrapy)
  {
  FWol=EVE_Cmd_Dat_0(FWol,
                     EVE_ENC_BITMAP_HANDLE(handle));
  FWol=EVE_Cmd_Dat_0(FWol,
                     EVE_ENC_BITMAP_SOURCE(source));
  FWol=EVE_Cmd_Dat_0(FWol,
                     EVE_ENC_BITMAP_EXT_FORMAT(format));
  FWol=EVE_Cmd_Dat_0(FWol,
                     EVE_ENC_BITMAP_LAYOUT_H(colstride>>10,
                                             rowstride>>9));
  FWol=EVE_Cmd_Dat_0(FWol,
                     EVE_ENC_BITMAP_LAYOUT(EVE_GLFORMAT,
                                           colstride,
                                           rowstride));
  FWol=EVE_Cmd_Dat_0(FWol,
                     EVE_ENC_BITMAP_SIZE_H(width>>9,
                                           height>>9));
  FWol=EVE_Cmd_Dat_0(FWol,
                     EVE_ENC_BITMAP_SIZE(filter,
                                         wrapx,
                                         wrapy,
                                         width,
                                         height));
  return(FWol);
  }
//===========================================================================

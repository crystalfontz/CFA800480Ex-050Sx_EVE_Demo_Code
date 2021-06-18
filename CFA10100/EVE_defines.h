#ifndef __EVE_DEFINES_H__
#define __EVE_DEFINES_H__

// Don't change these
#define DEBUG_NONE   (0)
#define DEBUG_STATUS (1)
#define DEBUG_GEEK   (2)

/*
 * ============================================================================
 * History
 * =======
 * 2017-03-15 : Created
 *
 * (C) Copyright Bridgetek Pte Ltd
 * ============================================================================
 *
 * This source code ("the Software") is provided by Bridgetek Pte Ltd
 * ("Bridgetek") subject to the licence terms set out
 * http://www.ftdichip.com/FTSourceCodeLicenceTerms.htm ("the Licence Terms").
 * You must read the Licence Terms before downloading or using the Software.
 * By installing or using the Software you agree to the Licence Terms. If you
 * do not agree to the Licence Terms then do not download or use the Software.
 *
 * Without prejudice to the Licence Terms, here is a summary of some of the key
 * terms of the Licence Terms (and in the event of any conflict between this
 * summary and the Licence Terms then the text of the Licence Terms will
 * prevail).
 *
 * The Software is provided "as is".
 * There are no warranties (or similar) in relation to the quality of the
 * Software. You use it at your own risk.
 * The Software should not be used in, or for, any medical device, system or
 * appliance. There are exclusions of Bridgetek liability for certain types of loss
 * such as: special loss or damage; incidental loss or damage; indirect or
 * consequential loss or damage; loss of income; loss of business; loss of
 * profits; loss of revenue; loss of contracts; business interruption; loss of
 * the use of money or anticipated savings; loss of information; loss of
 * opportunity; loss of goodwill or reputation; and/or loss of, damage to or
 * corruption of data.
 * There is a monetary cap on Bridgetek's liability.
 * The Software may have subsequently been amended by another user and then
 * distributed by that other user ("Adapted Software").  If so that user may
 * have additional licence terms that apply to those amendments. However, Bridgetek
 * has no liability in relation to those amendments.
 * ============================================================================
 */

//---------------------------------------------------------------------------
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
 
//##################################################################################################
// This section are defines that I found useful / necessary that were
// not in the FTDI / BridgeTek provided files.
//##################################################################################################
//Read this address early on to tell what chip we are talking to.
//ref: uint8_t Validate_and_Print_Chip_ID()
#define EVE_CHIP_ID_ADDRESS             (0x000C0000UL) // = 786432UL
//My defines for the chip type, matches the 3rd byte of EVE_CHIP_ID_ADDRESS
#define FT800 (0x00)
#define FT801 (0x01)
#define FT810 (0x10)
#define FT811 (0x11)
#define FT812 (0x12)
#define FT813 (0x13)
#define BT815 (0x15)
#define BT816 (0x16)
#define BT817 (0x17)
#define BT818 (0x18)

// FT800 Memory Commands - use with ft800memWritexx and ft800memReadxx
// FT800 Host Memory Write
#define EVE_MEM_WRITE                   (0x00000080UL) // = 128UL      
// FT800 Host Memory Read
#define EVE_MEM_READ                    (0x00000000UL) // = 0UL

// Initializes FT800
#define EVE_ACTIVE                      (0x00000000UL) // = 0UL      
// Place FT800 in Standby (clk running)
#define EVE_STANDBY                     (0x00000041UL) // = 65UL      
// Place FT800 in Sleep (clk off)
#define EVE_SLEEP                       (0x00000042UL) // = 66UL
// Place FT800 in Power Down (core off)
#define EVE_PWRDOWN                     (0x00000050UL) // = 80UL
// Select external clock source
#define EVE_CLKEXT                      (0x00000044UL) // = 68UL
// Select internal clock source
#define EVE_CLKINT                      (0x00000048UL) // = 72UL
// Configure system clock
#define EVE_CLKSEL                      (0x00000061UL) // = 97UL
// Reset Pulse
#define EVE_RST_PULSE                   (0x00000068UL) // = 104UL
// Set pin drive strength
#define EVE_PINDRIVE                    (0x00000070UL) // = 112UL

//My defines for the clock source
#define EVE_CLOCK_SOURCE_INTERNAL   (0)
#define EVE_CLOCK_SOURCE_EXTERNAL   (1)

//My defines for the external crystal clock (multiplying is hard)
#define EVE_EXTERNAL_CLOCK_MUL_UNUSED     (0x00)
#define EVE_EXTERNAL_CLOCK_MUL_x2_24MHz   (0x02)
#define EVE_EXTERNAL_CLOCK_MUL_x3_36MHz   (0x03)
#define EVE_EXTERNAL_CLOCK_MUL_x4_48MHz   (0x44)
#define EVE_EXTERNAL_CLOCK_MUL_x5_60MHz   (0x45)
#define EVE_EXTERNAL_CLOCK_MUL_x6_72MHz   (0x46)
#define EVE_EXTERNAL_CLOCK_MUL_x7_84MHz   (0x47)

//My defines for the touch devices.
#define EVE_TOUCH_NONE       (0)
#define EVE_TOUCH_RESISTIVE  (1)
#define EVE_TOUCH_CAPACITIVE (2)
#define EVE_CAP_DEV_DEFAULT  (0)
#define EVE_CAP_DEV_GT911    (1)
#define EVE_CAP_DEV_FT5316   (2)

// Might be useful for the EVE's 16.16 fixed point
#define to_16_16_fp(int_part,frc_part)  ((uint32_t)((((uint32_t)int_part)<<16)|((uint32_t)frc_part)))

//For displaying a bitmap stored in flash, addresed by sector.
//The parameters filter/wrapx/wrapy are always set to NEAREST/BORDER/BORDER
#define  FLASH_SETBITMAP(FWO,SECTOR,FORMAT,WIDTH,HEIGHT)\
           EVE_Cmd_Dat_3(FWO,\
                         EVE_ENC_CMD_SETBITMAP,\
                         ((uint32_t)0x800000) | (((uint32_t)SECTOR)<<7),\
                         (((uint32_t)WIDTH) << 16) | ((uint32_t)FORMAT),\
                         (uint32_t)HEIGHT)

//The parameters filter/wrapx/wrapy are always set to NEAREST/BORDER/BORDER
#define  RAMG_SETBITMAP(FWO,RAM_G_ADDRESS,FORMAT,WIDTH,HEIGHT)\
           EVE_Cmd_Dat_3(FWO,\
                         EVE_ENC_CMD_SETBITMAP,\
                         RAM_G_ADDRESS,\
                         (((uint32_t)WIDTH) << 16) | ((uint32_t)FORMAT),\
                         (uint32_t)HEIGHT)

                         
//##################################################################################################
// Below are defines as provided by FRDI / BridgeTek, refactored / cleaned up a bit
// by Crystalfontz.
//## COMMOM ########################################################################################

// Definitions used for co-processor command buffer
#define EVE_CMD_SIZE          (4)       //4 byte per coprocessor command of EVE

//## FT80x #########################################################################################
#if ((EVE_DEVICE == FT800) || (EVE_DEVICE == FT801))

#define FT800_VERSION "1.9.0".

// Lower boundary of trimming
#define LOW_FREQ_BOUND                  (47040000L) //98% of 48Mhz

#define EVE_RAM_CMD                     (0x00108000UL) // = 1081344UL
#define EVE_RAM_CMD_SIZE                (4*1024L)
#define EVE_RAM_DL                      (0x00100000UL) // = 1048576UL
#define EVE_RAM_DL_SIZE                 (8*1024L)
#define EVE_RAM_G                       (0x00000000UL) // = 0UL
#define EVE_RAM_G_SIZE                  (256*1024L)
#define EVE_RAM_PAL                     (0x00102000UL) // = 1056768UL
#define EVE_RAM_REG                     (0x00102400UL) // = 1057792UL

#define EVE_ROMFONT_TABLEADDRESS        (0x00102570UL) // = 1058160UL
#define EVE_REG_ANA_COMP                (0x00102538UL) // = 1058104UL
#define EVE_REG_ANALOG                  (0x00102408UL) // = 1057800UL
#define EVE_REG_CLOCK                   (0x001024ECUL) // = 1058028UL
#define EVE_REG_CMD_DL                  (0x001024E4UL) // = 1058020UL
#define EVE_REG_CMD_READ                (0x001024E8UL) // = 1058024UL
#define EVE_REG_CMD_WRITE               (0x0010241CUL) // = 1057820UL
#define EVE_REG_CPURESET                (0x00102568UL) // = 1058152UL
#define EVE_REG_CRC                     (0x00102464UL) // = 1057892UL
#define EVE_REG_CSPREAD                 (0x001024F4UL) // = 1058036UL
#define EVE_REG_CTOUCH_EXTENDED         (0x00102538UL) // = 1058104UL
#define EVE_REG_CTOUCH_GESTURE          (0x0010250CUL) // = 1058060UL
#define EVE_REG_CTOUCH_IDS              (0x00102510UL) // = 1058064UL
#define EVE_REG_CTOUCH_TOUCH0_XY        (0x00102508UL) // = 1058056UL
#define EVE_REG_CTOUCH_TOUCH1_XY        (0x00102574UL) // = 1058164UL
#define EVE_REG_CTOUCH_TOUCH2_XY        (0x00102578UL) // = 1058168UL
#define EVE_REG_CTOUCH_TOUCH3_XY        (0x00102538UL) // = 1058104UL
#define EVE_REG_CTOUCH_TOUCH4_X         (0x0010250CUL) // = 1058060UL
#define EVE_REG_CTOUCH_TOUCH4_Y         (0x00102534UL) // = 1058100UL
#define EVE_REG_CYA_TOUCH               (0x001024D0UL) // = 1058000UL
#define EVE_REG_CYA0                    (0x001024D4UL) // = 1058004UL
#define EVE_REG_CYA1                    (0x0010253CUL) // = 1058108UL
#define EVE_REG_DATESTAMP               (0x0010245CUL) // = 1057884UL
#define EVE_REG_DITHER                  (0x00102450UL) // = 1057872UL
#define EVE_REG_DLSWAP                  (0x00102404UL) // = 1057796UL
#define EVE_REG_FRAMES                  (0x0010240CUL) // = 1057804UL
#define EVE_REG_FREQUENCY               (0x00102490UL) // = 1057936UL
#define EVE_REG_GPIO                    (0x0010248CUL) // = 1057932UL
#define EVE_REG_GPIO_DIR                (0x00102428UL) // = 1057832UL
#define EVE_REG_HCYCLE                  (0x0010242CUL) // = 1057836UL
#define EVE_REG_HOFFSET                 (0x00102430UL) // = 1057840UL
#define EVE_REG_HSIZE                   (0x00102434UL) // = 1057844UL
#define EVE_REG_HSYNC0                  (0x00102438UL) // = 1057848UL
#define EVE_REG_HSYNC1                  (0x00102400UL) // = 1057792UL
#define EVE_REG_ID                      (0x0010249CUL) // = 1057948UL
#define EVE_REG_INT_EN                  (0x00102498UL) // = 1057944UL
#define EVE_REG_INT_FLAGS               (0x001024A0UL) // = 1057952UL
#define EVE_REG_INT_MASK                (0x001024C8UL) // = 1057992UL
#define EVE_REG_MACRO_0                 (0x001024CCUL) // = 1057996UL
#define EVE_REG_MACRO_1                 (0x00102458UL) // = 1057880UL
#define EVE_REG_OUTBITS                 (0x0010246CUL) // = 1057900UL
#define EVE_REG_PCLK                    (0x00102468UL) // = 1057896UL
#define EVE_REG_PCLK_POL                (0x00102488UL) // = 1057928UL
#define EVE_REG_PLAY                    (0x001024B4UL) // = 1057972UL
#define EVE_REG_PLAYBACK_FORMAT         (0x001024B0UL) // = 1057968UL
#define EVE_REG_PLAYBACK_FREQ           (0x001024A8UL) // = 1057960UL
#define EVE_REG_PLAYBACK_LENGTH         (0x001024B8UL) // = 1057976UL
#define EVE_REG_PLAYBACK_LOOP           (0x001024BCUL) // = 1057980UL
#define EVE_REG_PLAYBACK_PLAY           (0x001024ACUL) // = 1057964UL
#define EVE_REG_PLAYBACK_READPTR        (0x001024A4UL) // = 1057956UL
#define EVE_REG_PLAYBACK_START          (0x001024C4UL) // = 1057988UL
#define EVE_REG_PWM_DUTY                (0x001024C0UL) // = 1057984UL
#define EVE_REG_PWM_HZ                  (0x00102410UL) // = 1057808UL
#define EVE_REG_RENDERMODE              (0x001024E0UL) // = 1058016UL
#define EVE_REG_ROMSUB_SEL              (0x00102454UL) // = 1057876UL
#define EVE_REG_ROTATE                  (0x00102418UL) // = 1057816UL
#define EVE_REG_SNAPSHOT                (0x00102414UL) // = 1057812UL
#define EVE_REG_SNAPY                   (0x00102484UL) // = 1057924UL
#define EVE_REG_SOUND                   (0x00102460UL) // = 1057888UL
#define EVE_REG_SWIZZLE                 (0x00102478UL) // = 1057912UL
#define EVE_REG_TAG                     (0x00102470UL) // = 1057904UL
#define EVE_REG_TAG_X                   (0x00102474UL) // = 1057908UL
#define EVE_REG_TAG_Y                   (0x00102420UL) // = 1057824UL
#define EVE_REG_TAP_CRC                 (0x00102424UL) // = 1057828UL
#define EVE_REG_TAP_MASK                (0x001024F4UL) // = 1058036UL
#define EVE_REG_TOUCH_ADC_MODE          (0x001024F8UL) // = 1058040UL
#define EVE_REG_TOUCH_CHARGE            (0x00102574UL) // = 1058164UL
#define EVE_REG_TOUCH_DIRECT_XY         (0x00102578UL) // = 1058168UL
#define EVE_REG_TOUCH_DIRECT_Z1Z2       (0x001024F0UL) // = 1058032UL
#define EVE_REG_TOUCH_MODE              (0x00102500UL) // = 1058048UL
#define EVE_REG_TOUCH_OVERSAMPLE        (0x00102508UL) // = 1058056UL
#define EVE_REG_TOUCH_RAW_XY            (0x0010250CUL) // = 1058060UL
#define EVE_REG_TOUCH_RZ                (0x00102504UL) // = 1058052UL
#define EVE_REG_TOUCH_RZTHRESH          (0x00102510UL) // = 1058064UL
#define EVE_REG_TOUCH_SCREEN_XY         (0x001024FCUL) // = 1058044UL
#define EVE_REG_TOUCH_SETTLE            (0x00102518UL) // = 1058072UL
#define EVE_REG_TOUCH_TAG               (0x00102514UL) // = 1058068UL
#define EVE_REG_TOUCH_TAG_XY            (0x0010251CUL) // = 1058076UL
#define EVE_REG_TOUCH_TRANSFORM_A       (0x00102520UL) // = 1058080UL
#define EVE_REG_TOUCH_TRANSFORM_B       (0x00102524UL) // = 1058084UL
#define EVE_REG_TOUCH_TRANSFORM_C       (0x00102528UL) // = 1058088UL
#define EVE_REG_TOUCH_TRANSFORM_D       (0x0010252CUL) // = 1058092UL
#define EVE_REG_TOUCH_TRANSFORM_E       (0x00102530UL) // = 1058096UL
#define EVE_REG_TOUCH_TRANSFORM_F       (0x00109000UL) // = 1085440UL
#define EVE_REG_TRACKER                 (0x0010256CUL) // = 1058156UL
#define EVE_REG_TRIM                    (0x0010243CUL) // = 1057852UL
#define EVE_REG_VCYCLE                  (0x00102440UL) // = 1057856UL
#define EVE_REG_VOFFSET                 (0x0010247CUL) // = 1057916UL
#define EVE_REG_VOL_PB                  (0x00102480UL) // = 1057920UL
#define EVE_REG_VOL_SOUND               (0x00102444UL) // = 1057860UL
#define EVE_REG_VSIZE                   (0x00102448UL) // = 1057864UL
#define EVE_REG_VSYNC0                  (0x0010244CUL) // = 1057868UL
#define EVE_REG_VSYNC1

#define EVE_ENC_ALPHA_FUNC(func,ref)                         ((0x9UL << 24)|(((func) & 0x7UL) << 8)|(((ref) & 0xffUL) << 0))
#define EVE_ENC_BEGIN(prim)                                  ((0x1fUL << 24)|(((prim) & 0xFUL) << 0))
#define EVE_ENC_BITMAP_HANDLE(handle)                        ((0x5UL << 24)|(((handle) & 0x1fUL) << 0))
#define EVE_ENC_BITMAP_LAYOUT(format,linestride,height)      ((0x7UL << 24)|(((format) & 0x1fUL) << 19)|(((linestride) & 0x3ffUL) << 9)|(((height) & 0x1ffUL) << 0))
#define EVE_ENC_BITMAP_SIZE(filter,wrapx,wrapy,width,height) ((0x8UL << 24)|(((filter) & 0x1UL) << 20)|(((wrapx) & 0x1UL) << 19)|(((wrapy) & 0x1UL) << 18)|(((width) & 0x1ffUL) << 9)|(((height) & 0x1ffUL) << 0))
#define EVE_ENC_BITMAP_SOURCE(addr)                          ((0x1UL << 24)|(((addr) & 0xffffffUL) << 0))
#define EVE_ENC_BITMAP_TRANSFORM_A(a)                        ((0x15UL << 24)|((((uint32_t)(a)) & 0x1FFFFUL) << 0))
#define EVE_ENC_BITMAP_TRANSFORM_B(b)                        ((0x16UL << 24)|((((uint32_t)(b)) & 0x1FFFFUL) << 0))
#define EVE_ENC_BITMAP_TRANSFORM_C(c)                        ((0x17UL << 24)|((((uint32_t)(c)) & 0xFFFFFFUL) << 0))
#define EVE_ENC_BITMAP_TRANSFORM_D(d)                        ((0x18UL << 24)|((((uint32_t)(d)) & 0x1FFFFUL) << 0))
#define EVE_ENC_BITMAP_TRANSFORM_E(e)                        ((0x19UL << 24)|((((uint32_t)(e)) & 0x1FFFFUL) << 0))
#define EVE_ENC_BITMAP_TRANSFORM_F(f)                        ((0x1aUL << 24)|((((uint32_t)(f)) & 0xFFFFFFUL) << 0))
#define EVE_ENC_BLEND_FUNC(src,dst)                          ((0xbUL << 24)|(((src) & 0x7UL) << 3)|(((dst) & 0x7UL) << 0))
#define EVE_ENC_CALL(dest)                                   ((0x1dUL << 24)|(((dest) & 0xFFFFUL) << 0))
#define EVE_ENC_CELL(cell)                                   ((0x6UL << 24)|(((cell) & 0x7fUL) << 0))
#define EVE_ENC_CLEAR_COLOR_A(alpha)                         ((0xfUL << 24)|(((alpha) & 0xffUL) << 0))
#define EVE_ENC_CLEAR_COLOR_RGB(red,green,blue)              ((0x2UL << 24)|(((red) & 0xffUL) << 16)|(((green) & 0xffUL) << 8)|(((blue) & 0xffUL) << 0))
#define EVE_ENC_CLEAR_COLOR(c)                               ((0x2UL << 24)|(((uint32_t)(c)) & 0x00ffffffUL))
#define EVE_ENC_CLEAR_STENCIL(s)                             ((0x11UL << 24)|((((uint32_t)(s)) & 0xffUL) << 0))
#define EVE_ENC_CLEAR_TAG(s)                                 ((0x12UL << 24)|((((uint32_t)(s)) & 0xffUL) << 0))
#define EVE_ENC_CLEAR(c,s,t)                                 ((0x26UL << 24)|((((uint32_t)(c)) & 0x1UL) << 2)|((((uint32_t)(s)) & 0x1UL) << 1)|((((uint32_t)(t)) & 0x1UL) << 0))
#define EVE_ENC_COLOR_A(alpha)                               ((0x10UL << 24)|(((alpha) & 0xffUL) << 0))
#define EVE_ENC_COLOR_MASK(r,g,b,a)                          ((0x20UL << 24)|((((uint32_t)(r)) & 0x1UL) << 3)|((((uint32_t)(g)) & 0x1UL) << 2)|((((uint32_t)(b)) & 0x1UL) << 1)|((((uint32_t)(a)) & 0x1UL) << 0))
#define EVE_ENC_COLOR_RGB(red,green,blue)                    ((0x4UL << 24)|(((red) & 0xffUL) << 16)|(((green) & 0xffUL) << 8)|(((blue) & 0xffUL) << 0))
#define EVE_ENC_COLOR(c)                                     ((0x4UL << 24)|(((uint32_t)(c)) & 0x00ffffffUL))
#define EVE_ENC_DISPLAY()                                    ((0x0UL << 24))
#define EVE_ENC_END()                                        ((0x21UL << 24))
#define EVE_ENC_JUMP(dest)                                   ((0x1eUL << 24)|(((dest) & 0xFFFFUL) << 0))
#define EVE_ENC_LINE_WIDTH(width)                            ((0xeUL << 24)|(((width) & 0xFFFUL) << 0))
#define EVE_ENC_MACRO(m)                                     ((0x25UL << 24)|((((uint32_t)(m)) & 0x1UL) << 0))
#define EVE_ENC_POINT_SIZE(size)                             ((0xdUL << 24)|(((size) & 0x1FFFUL) << 0))
#define EVE_ENC_RESTORE_CONTEXT()                            ((0x23UL << 24))
#define EVE_ENC_RETURN()                                     ((0x24UL << 24))
#define EVE_ENC_SAVE_CONTEXT()                               ((0x22UL << 24))
#define EVE_ENC_SCISSOR_SIZE(width,height)                   ((0x1cUL << 24)|(((width) & 0x3ffUL) << 10)|(((height) & 0x3ffUL) << 0))
#define EVE_ENC_SCISSOR_XY(x,y)                              ((0x1bUL << 24)|((((uint32_t)(x)) & 0x1ffUL) << 9)|((((uint32_t)(y)) & 0x1ffUL) << 0))
#define EVE_ENC_STENCIL_FUNC(func,ref,mask)                  ((0xaUL << 24)|(((func) & 0x7UL) << 16)|(((ref) & 0xffUL) << 8)|(((mask) & 0xffUL) << 0))
#define EVE_ENC_STENCIL_MASK(mask)                           ((0x13UL << 24)|(((mask) & 0xffUL) << 0))
#define EVE_ENC_STENCIL_OP(sfail,spass)                      ((0xcUL << 24)|(((sfail) & 0x7UL) << 3)|(((spass) & 0x7UL) << 0))
#define EVE_ENC_TAG_MASK(mask)                               ((0x14UL << 24)|(((mask) & 0x1UL) << 0))
#define EVE_ENC_TAG(s)                                       ((0x3UL << 24)|((((uint32_t)(s)) & 0xffUL) << 0))
#define EVE_ENC_VERTEX2F(x,y)                                ((0x1UL << 30)|((((uint32_t)(x)) & 0xffffUL) << 15)|((((uint32_t)(y)) & 0xffffUL) << 0))
#define EVE_ENC_VERTEX2II(x,y,handle,cell)                   ((0x2UL << 30)|((((uint32_t)(x)) & 0x1ffUL) << 21)|((((uint32_t)(y)) & 0x1ffUL) << 12)|(((handle) & 0x1fUL) << 7)|(((cell) & 0x7fUL) << 0))

#define EVE_ENC_CMD_APPEND              (0xFFFFFF1EUL) // = 4294967070UL
#define EVE_ENC_CMD_BGCOLOR             (0xFFFFFF09UL) // = 4294967049UL
#define EVE_ENC_CMD_BITMAP_TRANSFORM    (0xFFFFFF21UL) // = 4294967073UL
#define EVE_ENC_CMD_BUTTON              (0xFFFFFF0DUL) // = 4294967053UL
#define EVE_ENC_CMD_CALIBRATE           (0xFFFFFF15UL) // = 4294967061UL
#define EVE_ENC_CMD_CLOCK               (0xFFFFFF14UL) // = 4294967060UL
#define EVE_ENC_CMD_COLDSTART           (0xFFFFFF32UL) // = 4294967090UL
#define EVE_ENC_CMD_CRC                 (0xFFFFFF03UL) // = 4294967043UL
#define EVE_ENC_CMD_CSKETCH             (0xFFFFFF35UL) // = 4294967093UL
#define EVE_ENC_CMD_DIAL                (0xFFFFFF2DUL) // = 4294967085UL
#define EVE_ENC_CMD_DLSTART             (0xFFFFFF00UL) // = 4294967040UL
#define EVE_ENC_CMD_EXECUTE             (0xFFFFFF07UL) // = 4294967047UL
#define EVE_ENC_CMD_FGCOLOR             (0xFFFFFF0AUL) // = 4294967050UL
#define EVE_ENC_CMD_GAUGE               (0xFFFFFF13UL) // = 4294967059UL
#define EVE_ENC_CMD_GETMATRIX           (0xFFFFFF33UL) // = 4294967091UL
#define EVE_ENC_CMD_GETPOINT            (0xFFFFFF08UL) // = 4294967048UL
#define EVE_ENC_CMD_GETPROPS            (0xFFFFFF25UL) // = 4294967077UL
#define EVE_ENC_CMD_GETPTR              (0xFFFFFF23UL) // = 4294967075UL
#define EVE_ENC_CMD_GRADCOLOR           (0xFFFFFF34UL) // = 4294967092UL
#define EVE_ENC_CMD_GRADIENT            (0xFFFFFF0BUL) // = 4294967051UL
#define EVE_ENC_CMD_HAMMERAUX           (0xFFFFFF04UL) // = 4294967044UL
#define EVE_ENC_CMD_IDCT                (0xFFFFFF06UL) // = 4294967046UL
#define EVE_ENC_CMD_INFLATE             (0xFFFFFF22UL) // = 4294967074UL
#define EVE_ENC_CMD_INTERRUPT           (0xFFFFFF02UL) // = 4294967042UL
#define EVE_ENC_CMD_KEYS                (0xFFFFFF0EUL) // = 4294967054UL
#define EVE_ENC_CMD_LOADIDENTITY        (0xFFFFFF26UL) // = 4294967078UL
#define EVE_ENC_CMD_LOADIMAGE           (0xFFFFFF24UL) // = 4294967076UL
#define EVE_ENC_CMD_LOGO                (0xFFFFFF31UL) // = 4294967089UL
#define EVE_ENC_CMD_MARCH               (0xFFFFFF05UL) // = 4294967045UL
#define EVE_ENC_CMD_MEMCPY              (0xFFFFFF1DUL) // = 4294967069UL
#define EVE_ENC_CMD_MEMCRC              (0xFFFFFF18UL) // = 4294967064UL
#define EVE_ENC_CMD_MEMSET              (0xFFFFFF1BUL) // = 4294967067UL
#define EVE_ENC_CMD_MEMWRITE            (0xFFFFFF1AUL) // = 4294967066UL
#define EVE_ENC_CMD_MEMZERO             (0xFFFFFF1CUL) // = 4294967068UL
#define EVE_ENC_CMD_NUMBER              (0xFFFFFF2EUL) // = 4294967086UL
#define EVE_ENC_CMD_PROGRESS            (0xFFFFFF0FUL) // = 4294967055UL
#define EVE_ENC_CMD_REGREAD             (0xFFFFFF19UL) // = 4294967065UL
#define EVE_ENC_CMD_ROTATE              (0xFFFFFF29UL) // = 4294967081UL
#define EVE_ENC_CMD_SCALE               (0xFFFFFF28UL) // = 4294967080UL
#define EVE_ENC_CMD_SCREENSAVER         (0xFFFFFF2FUL) // = 4294967087UL
#define EVE_ENC_CMD_SCROLLBAR           (0xFFFFFF11UL) // = 4294967057UL
#define EVE_ENC_CMD_SETFONT             (0xFFFFFF2BUL) // = 4294967083UL
#define EVE_ENC_CMD_SETMATRIX           (0xFFFFFF2AUL) // = 4294967082UL
#define EVE_ENC_CMD_SKETCH              (0xFFFFFF30UL) // = 4294967088UL
#define EVE_ENC_CMD_SLIDER              (0xFFFFFF10UL) // = 4294967056UL
#define EVE_ENC_CMD_SNAPSHOT            (0xFFFFFF1FUL) // = 4294967071UL
#define EVE_ENC_CMD_SPINNER             (0xFFFFFF16UL) // = 4294967062UL
#define EVE_ENC_CMD_STOP                (0xFFFFFF17UL) // = 4294967063UL
#define EVE_ENC_CMD_SWAP                (0xFFFFFF01UL) // = 4294967041UL
#define EVE_ENC_CMD_TEXT                (0xFFFFFF0CUL) // = 4294967052UL
#define EVE_ENC_CMD_TOGGLE              (0xFFFFFF12UL) // = 4294967058UL
#define EVE_ENC_CMD_TOUCH_TRANSFORM     (0xFFFFFF20UL) // = 4294967072UL
#define EVE_ENC_CMD_TRACK               (0xFFFFFF2CUL) // = 4294967084UL
#define EVE_ENC_CMD_TRANSLATE           (0xFFFFFF27UL) // = 4294967079UL

#define EVE_BEGIN_BITMAPS               (0x00000001UL) // = 1UL
#define EVE_BEGIN_EDGE_STRIP_A          (0x00000007UL) // = 7UL
#define EVE_BEGIN_EDGE_STRIP_B          (0x00000008UL) // = 8UL
#define EVE_BEGIN_EDGE_STRIP_L          (0x00000006UL) // = 6UL
#define EVE_BEGIN_EDGE_STRIP_R          (0x00000005UL) // = 5UL
#define EVE_BEGIN_LINE_STRIP            (0x00000004UL) // = 4UL
#define EVE_BEGIN_LINES                 (0x00000003UL) // = 3UL
#define EVE_BEGIN_POINTS                (0x00000002UL) // = 2UL
#define EVE_BEGIN_RECTS                 (0x00000009UL) // = 9UL
#define EVE_BLEND_DST_ALPHA             (0x00000003UL) // = 3UL
#define EVE_BLEND_ONE                   (0x00000001UL) // = 1UL
#define EVE_BLEND_ONE_MINUS_DST_ALPHA   (0x00000005UL) // = 5UL
#define EVE_BLEND_ONE_MINUS_SRC_ALPHA   (0x00000004UL) // = 4UL
#define EVE_BLEND_SRC_ALPHA             (0x00000002UL) // = 2UL
#define EVE_BLEND_ZERO                  (0x00000000UL) // = 0UL
#define EVE_DLSWAP_DONE                 (0x00000000UL) // = 0UL
#define EVE_DLSWAP_FRAME                (0x00000002UL) // = 2UL
#define EVE_DLSWAP_LINE                 (0x00000001UL) // = 1UL
#define EVE_FILTER_BILINEAR             (0x00000001UL) // = 1UL
#define EVE_FILTER_NEAREST              (0x00000000UL) // = 0UL
#define EVE_FORMAT_ARGB1555             (0x00000000UL) // = 0UL
#define EVE_FORMAT_ARGB2                (0x00000005UL) // = 5UL
#define EVE_FORMAT_ARGB4                (0x00000006UL) // = 6UL
#define EVE_FORMAT_BARGRAPH             (0x0000000BUL) // = 11UL
#define EVE_FORMAT_L1                   (0x00000001UL) // = 1UL
#define EVE_FORMAT_L4                   (0x00000002UL) // = 2UL
#define EVE_FORMAT_L8                   (0x00000003UL) // = 3UL
#define EVE_FORMAT_PALETTED             (0x00000008UL) // = 8UL
#define EVE_FORMAT_RGB332               (0x00000004UL) // = 4UL
#define EVE_FORMAT_RGB565               (0x00000007UL) // = 7UL
#define EVE_FORMAT_TEXT8X8              (0x00000009UL) // = 9UL
#define EVE_FORMAT_TEXTVGA              (0x0000000AUL) // = 10UL
#define EVE_INT_CMDEMPTY                (0x00000020UL) // = 32UL
#define EVE_INT_CMDFLAG                 (0x00000040UL) // = 64UL
#define EVE_INT_CONVCOMPLETE            (0x00000080UL) // = 128UL
#define EVE_INT_PLAYBACK                (0x00000010UL) // = 16UL
#define EVE_INT_SOUND                   (0x00000008UL) // = 8UL
#define EVE_INT_SWAP                    (0x00000001UL) // = 1UL
#define EVE_INT_TAG                     (0x00000004UL) // = 4UL
#define EVE_INT_TOUCH                   (0x00000002UL) // = 2UL
#define EVE_LINEAR_SAMPLES              (0x00000000UL) // = 0UL
#define EVE_OPT_CENTER                  (0x00000600UL) // = 1536UL
#define EVE_OPT_CENTERX                 (0x00000200UL) // = 512UL
#define EVE_OPT_CENTERY                 (0x00000400UL) // = 1024UL
#define EVE_OPT_FLAT                    (0x00000100UL) // = 256UL
#define EVE_OPT_MONO                    (0x00000001UL) // = 1UL
#define EVE_OPT_NOBACK                  (0x00001000UL) // = 4096UL
#define EVE_OPT_NODL                    (0x00000002UL) // = 2UL
#define EVE_OPT_NOHANDS                 (0x0000C000UL) // = 49152UL
#define EVE_OPT_NOHM                    (0x00004000UL) // = 16384UL
#define EVE_OPT_NOPOINTER               (0x00004000UL) // = 16384UL
#define EVE_OPT_NOSECS                  (0x00008000UL) // = 32768UL
#define EVE_OPT_NOTICKS                 (0x00002000UL) // = 8192UL
#define EVE_OPT_RIGHTX                  (0x00000800UL) // = 2048UL
#define EVE_OPT_SIGNED                  (0x00000100UL) // = 256UL
#define EVE_STENCIL_DECR                (0x00000004UL) // = 4UL
#define EVE_STENCIL_INCR                (0x00000003UL) // = 3UL
#define EVE_STENCIL_INVERT              (0x00000005UL) // = 5UL
#define EVE_STENCIL_KEEP                (0x00000001UL) // = 1UL
#define EVE_STENCIL_REPLACE             (0x00000002UL) // = 2UL
#define EVE_STENCIL_ZERO                (0x00000000UL) // = 0UL
#define EVE_TEST_ALWAYS                 (0x00000007UL) // = 7UL
#define EVE_TEST_EQUAL                  (0x00000005UL) // = 5UL
#define EVE_TEST_GEQUAL                 (0x00000004UL) // = 4UL
#define EVE_TEST_GREATER                (0x00000003UL) // = 3UL
#define EVE_TEST_LEQUAL                 (0x00000002UL) // = 2UL
#define EVE_TEST_LESS                   (0x00000001UL) // = 1UL
#define EVE_TEST_NEVER                  (0x00000000UL) // = 0UL
#define EVE_TEST_NOTEQUAL               (0x00000006UL) // = 6UL
#define EVE_TOUCHMODE_CONTINUOUS        (0x00000003UL) // = 3UL
#define EVE_TOUCHMODE_FRAME             (0x00000002UL) // = 2UL
#define EVE_TOUCHMODE_OFF               (0x00000000UL) // = 0UL
#define EVE_TOUCHMODE_ONESHOT           (0x00000001UL) // = 1UL
#define EVE_ULAW_SAMPLES                (0x00000001UL) // = 1UL
#define EVE_VOL_ZERO                    (0x00000000UL) // = 0UL
#define EVE_WRAP_BORDER                 (0x00000000UL) // = 0UL
#define EVE_WRAP_REPEAT                 (0x00000001UL) // = 1UL
#define EVE_ADC_DIFFERENTIAL            (0x00000001UL) // = 1UL
#define EVE_ADC_SINGLE_ENDED            (0x00000000UL) // = 0UL
#define EVE_ADPCM_SAMPLES               (0x00000002UL) // = 2UL

//End FT80x parameters
#endif // ((EVE_DEVICE == FT800) || (EVE_DEVICE == FT801))

//## FT81x #########################################################################################
#if ((EVE_DEVICE == FT810) ||(EVE_DEVICE == FT811) ||(EVE_DEVICE == FT812) || (EVE_DEVICE == FT813))

#define EVE_VERSION "1.0.4"

// Lower boundary of trimming
#define LOW_FREQ_BOUND                  58800000L//98% of 60Mhz

#define EVE_RAM_CMD                     (0x00308000UL) // = 3178496UL
#define EVE_RAM_CMD_SIZE                (4*1024L)
#define EVE_RAM_DL                      (0x00300000UL) // = 3145728UL
#define EVE_RAM_DL_SIZE                 (8*1024L)
#define EVE_RAM_G                       (0x00000000UL) // = 0UL
#define EVE_RAM_G_SIZE                  (1024*1024L)
#define EVE_RAM_REG                     (0x00302000UL) // = 3153920UL
#define EVE_RAM_ROMSUB                  (0x0030A000UL) // = 3186688UL

#define EVE_ROMFONT_TABLEADDRESS        (0x002FFFFCUL) // = 3145724UL
#define EVE_REG_ANA_COMP                (0x00302184UL) // = 3154308UL
#define EVE_REG_ANALOG                  (0x0030216CUL) // = 3154284UL
#define EVE_REG_BIST_EN                 (0x00302174UL) // = 3154292UL
#define EVE_REG_BUSYBITS                (0x003020E8UL) // = 3154152UL
#define EVE_REG_CLOCK                   (0x00302008UL) // = 3153928UL
#define EVE_REG_CMD_DL                  (0x00302100UL) // = 3154176UL
#define EVE_REG_CMD_READ                (0x003020F8UL) // = 3154168UL
#define EVE_REG_CMD_WRITE               (0x003020FCUL) // = 3154172UL
#define EVE_REG_CMDB_SPACE              (0x00302574UL) // = 3155316UL
#define EVE_REG_CMDB_WRITE              (0x00302578UL) // = 3155320UL
#define EVE_REG_CPURESET                (0x00302020UL) // = 3153952UL
#define EVE_REG_CRC                     (0x00302178UL) // = 3154296UL
#define EVE_REG_CSPREAD                 (0x00302068UL) // = 3154024UL
#define EVE_REG_CTOUCH_EXTENDED         (0x00302108UL) // = 3154184UL
#define EVE_REG_CTOUCH_TOUCH0_XY        (0x00302124UL) // = 3154212UL
#define EVE_REG_CTOUCH_TOUCH1_XY        (0x0030211CUL) // = 3154204UL
#define EVE_REG_CTOUCH_TOUCH2_XY        (0x0030218CUL) // = 3154316UL
#define EVE_REG_CTOUCH_TOUCH3_XY        (0x00302190UL) // = 3154320UL
#define EVE_REG_CTOUCH_TOUCH4_X         (0x0030216CUL) // = 3154284UL
#define EVE_REG_CTOUCH_TOUCH4_Y         (0x00302120UL) // = 3154208UL
#define EVE_REG_CYA_TOUCH               (0x00302168UL) // = 3154280UL
#define EVE_REG_DATESTAMP               (0x00302564UL) // = 3155300UL
#define EVE_REG_DITHER                  (0x00302060UL) // = 3154016UL
#define EVE_REG_DLSWAP                  (0x00302054UL) // = 3154004UL
#define EVE_REG_FRAMES                  (0x00302004UL) // = 3153924UL
#define EVE_REG_FREQUENCY               (0x0030200CUL) // = 3153932UL
#define EVE_REG_GPIO                    (0x00302094UL) // = 3154068UL
#define EVE_REG_GPIO_DIR                (0x00302090UL) // = 3154064UL
#define EVE_REG_GPIOX                   (0x0030209CUL) // = 3154076UL
#define EVE_REG_GPIOX_DIR               (0x00302098UL) // = 3154072UL
#define EVE_REG_HCYCLE                  (0x0030202CUL) // = 3153964UL
#define EVE_REG_HOFFSET                 (0x00302030UL) // = 3153968UL
#define EVE_REG_HSIZE                   (0x00302034UL) // = 3153972UL
#define EVE_REG_HSYNC0                  (0x00302038UL) // = 3153976UL
#define EVE_REG_HSYNC1                  (0x0030203CUL) // = 3153980UL
#define EVE_REG_ID                      (0x00302000UL) // = 3153920UL
#define EVE_REG_INT_EN                  (0x003020ACUL) // = 3154092UL
#define EVE_REG_INT_FLAGS               (0x003020A8UL) // = 3154088UL
#define EVE_REG_INT_MASK                (0x003020B0UL) // = 3154096UL
#define EVE_REG_MACRO_0                 (0x003020D8UL) // = 3154136UL
#define EVE_REG_MACRO_1                 (0x003020DCUL) // = 3154140UL
#define EVE_REG_MEDIAFIFO_READ          (0x00309014UL) // = 3182612UL
#define EVE_REG_MEDIAFIFO_WRITE         (0x00309018UL) // = 3182616UL
#define EVE_REG_OUTBITS                 (0x0030205CUL) // = 3154012UL
#define EVE_REG_PATCHED_ANALOG          (0x00302170UL) // = 3154288UL
#define EVE_REG_PATCHED_TOUCH_FAULT     (0x0030216CUL) // = 3154284UL
#define EVE_REG_PCLK                    (0x00302070UL) // = 3154032UL
#define EVE_REG_PCLK_POL                (0x0030206CUL) // = 3154028UL
#define EVE_REG_PLAY                    (0x0030208CUL) // = 3154060UL
#define EVE_REG_PLAYBACK_FORMAT         (0x003020C4UL) // = 3154116UL
#define EVE_REG_PLAYBACK_FREQ           (0x003020C0UL) // = 3154112UL
#define EVE_REG_PLAYBACK_LENGTH         (0x003020B8UL) // = 3154104UL
#define EVE_REG_PLAYBACK_LOOP           (0x003020C8UL) // = 3154120UL
#define EVE_REG_PLAYBACK_PLAY           (0x003020CCUL) // = 3154124UL
#define EVE_REG_PLAYBACK_READPTR        (0x003020BCUL) // = 3154108UL
#define EVE_REG_PLAYBACK_START          (0x003020B4UL) // = 3154100UL
#define EVE_REG_PWM_DUTY                (0x003020D4UL) // = 3154132UL
#define EVE_REG_PWM_HZ                  (0x003020D0UL) // = 3154128UL
#define EVE_REG_RENDERMODE              (0x00302010UL) // = 3153936UL
#define EVE_REG_ROMSUB_SEL              (0x003020F0UL) // = 3154160UL
#define EVE_REG_ROTATE                  (0x00302058UL) // = 3154008UL
#define EVE_REG_SNAPFORMAT              (0x0030201CUL) // = 3153948UL
#define EVE_REG_SNAPSHOT                (0x00302018UL) // = 3153944UL
#define EVE_REG_SNAPY                   (0x00302014UL) // = 3153940UL
#define EVE_REG_SOUND                   (0x00302088UL) // = 3154056UL
#define EVE_REG_SPI_EARLY_TX            (0x0030217CUL) // = 3154300UL
#define EVE_REG_SPI_WIDTH               (0x00302188UL) // = 3154312UL
#define EVE_REG_SWIZZLE                 (0x00302064UL) // = 3154020UL
#define EVE_REG_TAG                     (0x0030207CUL) // = 3154044UL
#define EVE_REG_TAG_X                   (0x00302074UL) // = 3154036UL
#define EVE_REG_TAG_Y                   (0x00302078UL) // = 3154040UL
#define EVE_REG_TAP_CRC                 (0x00302024UL) // = 3153956UL
#define EVE_REG_TAP_MASK                (0x00302028UL) // = 3153960UL
#define EVE_REG_TOUCH_ADC_MODE          (0x00302108UL) // = 3154184UL
#define EVE_REG_TOUCH_CHARGE            (0x0030210CUL) // = 3154188UL
#define EVE_REG_TOUCH_DIRECT_XY         (0x0030218CUL) // = 3154316UL
#define EVE_REG_TOUCH_DIRECT_Z1Z2       (0x00302190UL) // = 3154320UL
#define EVE_REG_TOUCH_FAULT             (0x00302170UL) // = 3154288UL
#define EVE_REG_TOUCH_MODE              (0x00302104UL) // = 3154180UL
#define EVE_REG_TOUCH_OVERSAMPLE        (0x00302114UL) // = 3154196UL
#define EVE_REG_TOUCH_RAW_XY            (0x0030211CUL) // = 3154204UL
#define EVE_REG_TOUCH_RZ                (0x00302120UL) // = 3154208UL
#define EVE_REG_TOUCH_RZTHRESH          (0x00302118UL) // = 3154200UL
#define EVE_REG_TOUCH_SCREEN_XY         (0x00302124UL) // = 3154212UL
#define EVE_REG_TOUCH_SETTLE            (0x00302110UL) // = 3154192UL
#define EVE_REG_TOUCH_TAG               (0x0030212CUL) // = 3154220UL
#define EVE_REG_TOUCH_TAG_XY            (0x00302128UL) // = 3154216UL
#define EVE_REG_TOUCH_TAG1              (0x00302134UL) // = 3154228UL
#define EVE_REG_TOUCH_TAG1_XY           (0x00302130UL) // = 3154224UL
#define EVE_REG_TOUCH_TAG2              (0x0030213CUL) // = 3154236UL
#define EVE_REG_TOUCH_TAG2_XY           (0x00302138UL) // = 3154232UL
#define EVE_REG_TOUCH_TAG3              (0x00302144UL) // = 3154244UL
#define EVE_REG_TOUCH_TAG3_XY           (0x00302140UL) // = 3154240UL
#define EVE_REG_TOUCH_TAG4              (0x0030214CUL) // = 3154252UL
#define EVE_REG_TOUCH_TAG4_XY           (0x00302148UL) // = 3154248UL
#define EVE_REG_TOUCH_TRANSFORM_A       (0x00302150UL) // = 3154256UL
#define EVE_REG_TOUCH_TRANSFORM_B       (0x00302154UL) // = 3154260UL
#define EVE_REG_TOUCH_TRANSFORM_C       (0x00302158UL) // = 3154264UL
#define EVE_REG_TOUCH_TRANSFORM_D       (0x0030215CUL) // = 3154268UL
#define EVE_REG_TOUCH_TRANSFORM_E       (0x00302160UL) // = 3154272UL
#define EVE_REG_TOUCH_TRANSFORM_F       (0x00302164UL) // = 3154276UL
#define EVE_REG_TRACKER                 (0x00309000UL) // = 3182592UL
#define EVE_REG_TRACKER_1               (0x00309004UL) // = 3182596UL
#define EVE_REG_TRACKER_2               (0x00309008UL) // = 3182600UL
#define EVE_REG_TRACKER_3               (0x0030900CUL) // = 3182604UL
#define EVE_REG_TRACKER_4               (0x00309010UL) // = 3182608UL
#define EVE_REG_TRIM                    (0x00302180UL) // = 3154304UL
#define EVE_REG_VCYCLE                  (0x00302040UL) // = 3153984UL
#define EVE_REG_VOFFSET                 (0x00302044UL) // = 3153988UL
#define EVE_REG_VOL_PB                  (0x00302080UL) // = 3154048UL
#define EVE_REG_VOL_SOUND               (0x00302084UL) // = 3154052UL
#define EVE_REG_VSIZE                   (0x00302048UL) // = 3153992UL
#define EVE_REG_VSYNC0                  (0x0030204CUL) // = 3153996UL
#define EVE_REG_VSYNC1                  (0x00302050UL) // = 3154000UL

#define EVE_ENC_ALPHA_FUNC(func,ref)                         ((0x9UL << 24)|(((func) & 0x7UL) << 8)|(((ref) & 0xffUL) << 0))
#define EVE_ENC_BEGIN(prim)                                  ((0x1fUL << 24)|(((prim)&15UL) << 0))
#define EVE_ENC_BITMAP_HANDLE(handle)                        ((0x5UL << 24)|(((handle) & 0x1fUL) << 0))
#define EVE_ENC_BITMAP_LAYOUT_H(linestride,height)           ((0x28UL << 24)|(((linestride) & 0x3UL) << 2)|(((height) & 0x3UL) << 0))
#define EVE_ENC_BITMAP_LAYOUT(format,linestride,height)      ((0x7UL << 24)|(((format) & 0x1fUL) << 19)|(((linestride) & 0x3ffUL) << 9)|(((height) & 0x1ffUL) << 0))
#define EVE_ENC_BITMAP_SIZE_H(width,height)                  ((0x29UL << 24)|(((width) & 0x3UL) << 2)|(((height) & 0x3UL) << 0))
#define EVE_ENC_BITMAP_SIZE(filter,wrapx,wrapy,width,height) ((0x8UL << 24)|(((filter) & 0x1UL) << 20)|(((wrapx) & 0x1UL) << 19)|(((wrapy) & 0x1UL) << 18)|(((width) & 0x1ffUL) << 9)|(((height) & 0x1ffUL) << 0))
#define EVE_ENC_BITMAP_SOURCE(addr)                          ((0x1UL << 24)|(((addr) & 0x3FFFFFUL) << 0))
#define EVE_ENC_BITMAP_TRANSFORM_A(a)                        ((0x15UL << 24)|((((uint32_t)(a)) & 0x1FFFFUL) << 0))
#define EVE_ENC_BITMAP_TRANSFORM_B(b)                        ((0x16UL << 24)|((((uint32_t)(b)) & 0x1FFFFUL) << 0))
#define EVE_ENC_BITMAP_TRANSFORM_C(c)                        ((0x17UL << 24)|((((uint32_t)(c)) & 0xFFFFFFUL) << 0))
#define EVE_ENC_BITMAP_TRANSFORM_D(d)                        ((0x18UL << 24)|((((uint32_t)(d)) & 0x1FFFFUL) << 0))
#define EVE_ENC_BITMAP_TRANSFORM_E(e)                        ((0x19UL << 24)|((((uint32_t)(e)) & 0x1FFFFUL) << 0))
#define EVE_ENC_BITMAP_TRANSFORM_F(f)                        ((0x1aUL << 24)|((((uint32_t)(f)) & 0xFFFFFFUL) << 0))
#define EVE_ENC_BLEND_FUNC(src,dst)                          ((0xbUL << 24)|(((src) & 0x7UL) << 3)|(((dst) & 0x7UL) << 0))
#define EVE_ENC_CALL(dest)                                   ((0x1dUL << 24)|(((dest) & 0xFFFFUL) << 0))
#define EVE_ENC_CELL(cell)                                   ((0x6UL << 24)|(((cell) & 0x7fUL) << 0))
#define EVE_ENC_CLEAR_COLOR_A(alpha)                         ((0xfUL << 24)|(((alpha) & 0xffUL) << 0))
#define EVE_ENC_CLEAR_COLOR_RGB(red,green,blue)              ((0x2UL << 24)|(((red) & 0xffUL) << 16)|(((green) & 0xffUL) << 8)|(((blue) & 0xffUL) << 0))
#define EVE_ENC_CLEAR_COLOR(c)                               ((0x2UL << 24)|(((uint32_t)(c)) & 0x00ffffffUL))
#define EVE_ENC_CLEAR_STENCIL(s)                             ((0x11UL << 24)|((((uint32_t)(s)) & 0xffUL) << 0))
#define EVE_ENC_CLEAR_TAG(s)                                 ((0x12UL << 24)|((((uint32_t)(s)) & 0xffUL) << 0))
#define EVE_ENC_CLEAR(c,s,t)                                 ((0x26UL << 24)|((((uint32_t)(c)) & 0x1UL) << 2)|((((uint32_t)(s)) & 0x1UL) << 1)|((((uint32_t)(t)) & 0x1UL) << 0))
#define EVE_ENC_COLOR_A(alpha)                               ((0x10UL << 24)|(((alpha) & 0xffUL) << 0))
#define EVE_ENC_COLOR_MASK(r,g,b,a)                          ((0x20UL << 24)|((((uint32_t)(r)) & 0x1UL) << 3)|((((uint32_t)(g)) & 0x1UL) << 2)|((((uint32_t)(b)) & 0x1UL) << 1)|((((uint32_t)(a)) & 0x1UL) << 0))
#define EVE_ENC_COLOR_RGB(red,green,blue)                    ((0x4UL << 24)|(((red) & 0xffUL) << 16)|(((green) & 0xffUL) << 8)|(((blue) & 0xffUL) << 0))
#define EVE_ENC_COLOR(c)                                     ((0x4UL << 24)|(((uint32_t)(c)) & 0x00ffffffUL))
#define EVE_ENC_DISPLAY()                                    ((0x0UL << 24))
#define EVE_ENC_END()                                        ((0x21UL << 24))
#define EVE_ENC_JUMP(dest)                                   ((0x1eUL << 24)|(((dest) & 0xFFFFUL) << 0))
#define EVE_ENC_LINE_WIDTH(width)                            ((0xeUL << 24)|(((width) & 0xFFFUL) << 0))
#define EVE_ENC_MACRO(m)                                     ((0x25UL << 24)|((((uint32_t)(m)) & 0x1UL) << 0))
#define EVE_ENC_NOP()                                        ((0x2dUL << 24))
#define EVE_ENC_PALETTE_SOURCE(addr)                         ((0x2aUL << 24)|(((addr) & 0x3FFFFFUL) << 0))
#define EVE_ENC_POINT_SIZE(size)                             ((0xdUL << 24)|(((size) & 0x1FFFUL) << 0))
#define EVE_ENC_RESTORE_CONTEXT()                            ((0x23UL << 24))
#define EVE_ENC_RETURN()                                     ((0x24UL << 24))
#define EVE_ENC_SAVE_CONTEXT()                               ((0x22UL << 24))
#define EVE_ENC_SCISSOR_SIZE(width,height)                   ((0x1cUL << 24)|(((width) & 0xFFFUL) << 12)|(((height) & 0xFFFUL) << 0))
#define EVE_ENC_SCISSOR_XY(x,y)                              ((0x1bUL << 24)|((((uint32_t)(x)) & 0x7FFUL) << 11)|((((uint32_t)(y)) & 0x7FFUL) << 0))
#define EVE_ENC_STENCIL_FUNC(func,ref,mask)                  ((0xaUL << 24)|(((func) & 0x7UL) << 16)|(((ref) & 0xffUL) << 8)|(((mask) & 0xffUL) << 0))
#define EVE_ENC_STENCIL_MASK(mask)                           ((0x13UL << 24)|(((mask) & 0xffUL) << 0))
#define EVE_ENC_STENCIL_OP(sfail,spass)                      ((0xcUL << 24)|(((sfail) & 0x7UL) << 3)|(((spass) & 0x7UL) << 0))
#define EVE_ENC_TAG_MASK(mask)                               ((0x14UL << 24)|(((mask) & 0x1UL) << 0))
#define EVE_ENC_TAG(s)                                       ((0x3UL << 24)|((((uint32_t)(s)) & 0xffUL) << 0))
#define EVE_ENC_VERTEX_FORMAT(frac)                          ((0x27UL << 24)|(((frac) & 0x7UL) << 0))
#define EVE_ENC_VERTEX_TRANSLATE_X(x)                        ((0x2bUL << 24)|((((uint32_t)(x)) & 0x1FFFFUL) << 0))
#define EVE_ENC_VERTEX_TRANSLATE_Y(y)                        ((0x2cUL << 24)|((((uint32_t)(y)) & 0x1FFFFUL) << 0))
#define EVE_ENC_VERTEX2F(x,y)                                ((0x1UL << 30)|((((uint32_t)(x)) & 0xffffUL) << 15)|((((uint32_t)(y)) & 0xffffUL) << 0))
#define EVE_ENC_VERTEX2II(x,y,handle,cell)                   ((0x2UL << 30)|((((uint32_t)(x)) & 0x1ffUL) << 21)|((((uint32_t)(y)) & 0x1ffUL) << 12)|(((handle) & 0x1fUL) << 7)|(((cell) & 0x7fUL) << 0))

#define EVE_ENC_CMD_APPEND              (0xFFFFFF1EUL) // = 4294967070UL
#define EVE_ENC_CMD_BGCOLOR             (0xFFFFFF09UL) // = 4294967049UL
#define EVE_ENC_CMD_BITMAP_TRANSFORM    (0xFFFFFF21UL) // = 4294967073UL
#define EVE_ENC_CMD_BUTTON              (0xFFFFFF0DUL) // = 4294967053UL
#define EVE_ENC_CMD_CALIBRATE           (0xFFFFFF15UL) // = 4294967061UL
#define EVE_ENC_CMD_CLOCK               (0xFFFFFF14UL) // = 4294967060UL
#define EVE_ENC_CMD_COLDSTART           (0xFFFFFF32UL) // = 4294967090UL
#define EVE_ENC_CMD_CRC                 (0xFFFFFF03UL) // = 4294967043UL
#define EVE_ENC_CMD_CSKETCH             (0xFFFFFF35UL) // = 4294967093UL
#define EVE_ENC_CMD_DIAL                (0xFFFFFF2DUL) // = 4294967085UL
#define EVE_ENC_CMD_DLSTART             (0xFFFFFF00UL) // = 4294967040UL
#define EVE_ENC_CMD_EXECUTE             (0xFFFFFF07UL) // = 4294967047UL
#define EVE_ENC_CMD_FGCOLOR             (0xFFFFFF0AUL) // = 4294967050UL
#define EVE_ENC_CMD_GAUGE               (0xFFFFFF13UL) // = 4294967059UL
#define EVE_ENC_CMD_GETMATRIX           (0xFFFFFF33UL) // = 4294967091UL
#define EVE_ENC_CMD_GETPOINT            (0xFFFFFF08UL) // = 4294967048UL
#define EVE_ENC_CMD_GETPROPS            (0xFFFFFF25UL) // = 4294967077UL
#define EVE_ENC_CMD_GETPTR              (0xFFFFFF23UL) // = 4294967075UL
#define EVE_ENC_CMD_GRADCOLOR           (0xFFFFFF34UL) // = 4294967092UL
#define EVE_ENC_CMD_GRADIENT            (0xFFFFFF0BUL) // = 4294967051UL
#define EVE_ENC_CMD_HAMMERAUX           (0xFFFFFF04UL) // = 4294967044UL
#define EVE_ENC_CMD_IDCT_DELETED        (0xFFFFFF06UL) // = 4294967046UL
#define EVE_ENC_CMD_INFLATE             (0xFFFFFF22UL) // = 4294967074UL
#define EVE_ENC_CMD_INT_RAMSHARED       (0xFFFFFF3DUL) // = 4294967101UL
#define EVE_ENC_CMD_INT_SWLOADIMAGE     (0xFFFFFF3EUL) // = 4294967102UL
#define EVE_ENC_CMD_INTERRUPT           (0xFFFFFF02UL) // = 4294967042UL
#define EVE_ENC_CMD_KEYS                (0xFFFFFF0EUL) // = 4294967054UL
#define EVE_ENC_CMD_LOADIDENTITY        (0xFFFFFF26UL) // = 4294967078UL
#define EVE_ENC_CMD_LOADIMAGE           (0xFFFFFF24UL) // = 4294967076UL
#define EVE_ENC_CMD_LOGO                (0xFFFFFF31UL) // = 4294967089UL
#define EVE_ENC_CMD_MARCH               (0xFFFFFF05UL) // = 4294967045UL
#define EVE_ENC_CMD_MEDIAFIFO           (0xFFFFFF39UL) // = 4294967097UL
#define EVE_ENC_CMD_MEMCPY              (0xFFFFFF1DUL) // = 4294967069UL
#define EVE_ENC_CMD_MEMCRC              (0xFFFFFF18UL) // = 4294967064UL
#define EVE_ENC_CMD_MEMSET              (0xFFFFFF1BUL) // = 4294967067UL
#define EVE_ENC_CMD_MEMWRITE            (0xFFFFFF1AUL) // = 4294967066UL
#define EVE_ENC_CMD_MEMZERO             (0xFFFFFF1CUL) // = 4294967068UL
#define EVE_ENC_CMD_NUMBER              (0xFFFFFF2EUL) // = 4294967086UL
#define EVE_ENC_CMD_PLAYVIDEO           (0xFFFFFF3AUL) // = 4294967098UL
#define EVE_ENC_CMD_PROGRESS            (0xFFFFFF0FUL) // = 4294967055UL
#define EVE_ENC_CMD_REGREAD             (0xFFFFFF19UL) // = 4294967065UL
#define EVE_ENC_CMD_ROMFONT             (0xFFFFFF3FUL) // = 4294967103UL
#define EVE_ENC_CMD_ROTATE              (0xFFFFFF29UL) // = 4294967081UL
#define EVE_ENC_CMD_SCALE               (0xFFFFFF28UL) // = 4294967080UL
#define EVE_ENC_CMD_SCREENSAVER         (0xFFFFFF2FUL) // = 4294967087UL
#define EVE_ENC_CMD_SCROLLBAR           (0xFFFFFF11UL) // = 4294967057UL
#define EVE_ENC_CMD_SETBASE             (0xFFFFFF38UL) // = 4294967096UL
#define EVE_ENC_CMD_SETBITMAP           (0xFFFFFF43UL) // = 4294967107UL
#define EVE_ENC_CMD_SETFONT             (0xFFFFFF2BUL) // = 4294967083UL
#define EVE_ENC_CMD_SETFONT2            (0xFFFFFF3BUL) // = 4294967099UL
#define EVE_ENC_CMD_SETMATRIX           (0xFFFFFF2AUL) // = 4294967082UL
#define EVE_ENC_CMD_SETROTATE           (0xFFFFFF36UL) // = 4294967094UL
#define EVE_ENC_CMD_SETSCRATCH          (0xFFFFFF3CUL) // = 4294967100UL
#define EVE_ENC_CMD_SKETCH              (0xFFFFFF30UL) // = 4294967088UL
#define EVE_ENC_CMD_SLIDER              (0xFFFFFF10UL) // = 4294967056UL
#define EVE_ENC_CMD_SNAPSHOT            (0xFFFFFF1FUL) // = 4294967071UL
#define EVE_ENC_CMD_SNAPSHOT2           (0xFFFFFF37UL) // = 4294967095UL
#define EVE_ENC_CMD_SPINNER             (0xFFFFFF16UL) // = 4294967062UL
#define EVE_ENC_CMD_STOP                (0xFFFFFF17UL) // = 4294967063UL
#define EVE_ENC_CMD_SWAP                (0xFFFFFF01UL) // = 4294967041UL
#define EVE_ENC_CMD_SYNC                (0xFFFFFF42UL) // = 4294967106UL
#define EVE_ENC_CMD_TEXT                (0xFFFFFF0CUL) // = 4294967052UL
#define EVE_ENC_CMD_TOGGLE              (0xFFFFFF12UL) // = 4294967058UL
#define EVE_ENC_CMD_TOUCH_TRANSFORM     (0xFFFFFF20UL) // = 4294967072UL
#define EVE_ENC_CMD_TRACK               (0xFFFFFF2CUL) // = 4294967084UL
#define EVE_ENC_CMD_TRANSLATE           (0xFFFFFF27UL) // = 4294967079UL
#define EVE_ENC_CMD_VIDEOFRAME          (0xFFFFFF41UL) // = 4294967105UL
#define EVE_ENC_CMD_VIDEOSTART          (0xFFFFFF40UL) // = 4294967104UL

#define EVE_BEGIN_BITMAPS               (0x00000001UL) // = 1UL
#define EVE_BEGIN_EDGE_STRIP_A          (0x00000007UL) // = 7UL
#define EVE_BEGIN_EDGE_STRIP_B          (0x00000008UL) // = 8UL
#define EVE_BEGIN_EDGE_STRIP_L          (0x00000006UL) // = 6UL
#define EVE_BEGIN_EDGE_STRIP_R          (0x00000005UL) // = 5UL
#define EVE_BEGIN_LINE_STRIP            (0x00000004UL) // = 4UL
#define EVE_BEGIN_LINES                 (0x00000003UL) // = 3UL
#define EVE_BEGIN_POINTS                (0x00000002UL) // = 2UL
#define EVE_BEGIN_RECTS                 (0x00000009UL) // = 9UL
#define EVE_BLEND_DST_ALPHA             (0x00000003UL) // = 3UL
#define EVE_BLEND_ONE                   (0x00000001UL) // = 1UL
#define EVE_BLEND_ONE_MINUS_DST_ALPHA   (0x00000005UL) // = 5UL
#define EVE_BLEND_ONE_MINUS_SRC_ALPHA   (0x00000004UL) // = 4UL
#define EVE_BLEND_SRC_ALPHA             (0x00000002UL) // = 2UL
#define EVE_BLEND_ZERO                  (0x00000000UL) // = 0UL
#define EVE_DLSWAP_DONE                 (0x00000000UL) // = 0UL
#define EVE_DLSWAP_FRAME                (0x00000002UL) // = 2UL
#define EVE_DLSWAP_LINE                 (0x00000001UL) // = 1UL
#define EVE_FILTER_BILINEAR             (0x00000001UL) // = 1UL
#define EVE_FILTER_NEAREST              (0x00000000UL) // = 0UL
#define EVE_FORMAT_ARGB1555             (0x00000000UL) // = 0UL
#define EVE_FORMAT_ARGB2                (0x00000005UL) // = 5UL
#define EVE_FORMAT_ARGB4                (0x00000006UL) // = 6UL
#define EVE_FORMAT_BARGRAPH             (0x0000000BUL) // = 11UL
#define EVE_FORMAT_L1                   (0x00000001UL) // = 1UL
#define EVE_FORMAT_L2                   (0x00000011UL) // = 17UL
#define EVE_FORMAT_L4                   (0x00000002UL) // = 2UL
#define EVE_FORMAT_L8                   (0x00000003UL) // = 3UL
#define EVE_FORMAT_PALETTED             (0x00000008UL) // = 8UL
#define EVE_FORMAT_PALETTED4444         (0x0000000FUL) // = 15UL
#define EVE_FORMAT_PALETTED565          (0x0000000EUL) // = 14UL
#define EVE_FORMAT_PALETTED8            (0x00000010UL) // = 16UL
#define EVE_FORMAT_RGB332               (0x00000004UL) // = 4UL
#define EVE_FORMAT_RGB565               (0x00000007UL) // = 7UL
#define EVE_FORMAT_TEXT8X8              (0x00000009UL) // = 9UL
#define EVE_FORMAT_TEXTVGA              (0x0000000AUL) // = 10UL
#define EVE_INT_CMDEMPTY                (0x00000020UL) // = 32UL
#define EVE_INT_CMDFLAG                 (0x00000040UL) // = 64UL
#define EVE_INT_CONVCOMPLETE            (0x00000080UL) // = 128UL
#define EVE_INT_G8                      (0x00000012UL) // = 18UL
#define EVE_INT_L8C                     (0x0000000CUL) // = 12UL
#define EVE_INT_PLAYBACK                (0x00000010UL) // = 16UL
#define EVE_INT_SOUND                   (0x00000008UL) // = 8UL
#define EVE_INT_SWAP                    (0x00000001UL) // = 1UL
#define EVE_INT_TAG                     (0x00000004UL) // = 4UL
#define EVE_INT_TOUCH                   (0x00000002UL) // = 2UL
#define EVE_INT_VGA                     (0x0000000DUL) // = 13UL
#define EVE_LINEAR_SAMPLES              (0x00000000UL) // = 0UL
#define EVE_OPT_CENTER                  (0x00000600UL) // = 1536UL
#define EVE_OPT_CENTERX                 (0x00000200UL) // = 512UL
#define EVE_OPT_CENTERY                 (0x00000400UL) // = 1024UL
#define EVE_OPT_FLAT                    (0x00000100UL) // = 256UL
#define EVE_OPT_FULLSCREEN              (0x00000008UL) // = 8UL
#define EVE_OPT_MEDIAFIFO               (0x00000010UL) // = 16UL
#define EVE_OPT_MONO                    (0x00000001UL) // = 1UL
#define EVE_OPT_NOBACK                  (0x00001000UL) // = 4096UL
#define EVE_OPT_NODL                    (0x00000002UL) // = 2UL
#define EVE_OPT_NOHANDS                 (0x0000C000UL) // = 49152UL
#define EVE_OPT_NOHM                    (0x00004000UL) // = 16384UL
#define EVE_OPT_NOPOINTER               (0x00004000UL) // = 16384UL
#define EVE_OPT_NOSECS                  (0x00008000UL) // = 32768UL
#define EVE_OPT_NOTEAR                  (0x00000004UL) // = 4UL
#define EVE_OPT_NOTICKS                 (0x00002000UL) // = 8192UL
#define EVE_OPT_RIGHTX                  (0x00000800UL) // = 2048UL
#define EVE_OPT_SIGNED                  (0x00000100UL) // = 256UL
#define EVE_OPT_SOUND                   (0x00000020UL) // = 32UL
#define EVE_STENCIL_DECR                (0x00000004UL) // = 4UL
#define EVE_STENCIL_INCR                (0x00000003UL) // = 3UL
#define EVE_STENCIL_INVERT              (0x00000005UL) // = 5UL
#define EVE_STENCIL_KEEP                (0x00000001UL) // = 1UL
#define EVE_STENCIL_REPLACE             (0x00000002UL) // = 2UL
#define EVE_STENCIL_ZERO                (0x00000000UL) // = 0UL
#define EVE_TEST_ALWAYS                 (0x00000007UL) // = 7UL
#define EVE_TEST_EQUAL                  (0x00000005UL) // = 5UL
#define EVE_TEST_GEQUAL                 (0x00000004UL) // = 4UL
#define EVE_TEST_GREATER                (0x00000003UL) // = 3UL
#define EVE_TEST_LEQUAL                 (0x00000002UL) // = 2UL
#define EVE_TEST_LESS                   (0x00000001UL) // = 1UL
#define EVE_TEST_NEVER                  (0x00000000UL) // = 0UL
#define EVE_TEST_NOTEQUAL               (0x00000006UL) // = 6UL
#define EVE_TOUCHMODE_CONTINUOUS        (0x00000003UL) // = 3UL
#define EVE_TOUCHMODE_FRAME             (0x00000002UL) // = 2UL
#define EVE_TOUCHMODE_OFF               (0x00000000UL) // = 0UL
#define EVE_TOUCHMODE_ONESHOT           (0x00000001UL) // = 1UL
#define EVE_ULAW_SAMPLES                (0x00000001UL) // = 1UL
#define EVE_VOL_ZERO                    (0x00000000UL) // = 0UL
#define EVE_WRAP_BORDER                 (0x00000000UL) // = 0UL
#define EVE_WRAP_REPEAT                 (0x00000001UL) // = 1UL
#define EVE_ADC_DIFFERENTIAL            (0x00000001UL) // = 1UL
#define EVE_ADC_SINGLE_ENDED            (0x00000000UL) // = 0UL
#define EVE_ADPCM_SAMPLES               (0x00000002UL) // = 2UL

#define EVE_REG_TOUCH_CONFIG            (0x00302168UL) // = 3154280UL
#define EVE_CTOUCH_MODE_COMPATIBILITY   (0x00000001UL) // = 1UL
#define EVE_CTOUCH_MODE_EXTENDED        (0x00000000UL) // = 0UL

//End EVE parameters
#endif // ((EVE_DEVICE == FT810) ||(EVE_DEVICE == FT811) ||(EVE_DEVICE == FT812) || (EVE_DEVICE == FT813))

//## BT81x #########################################################################################
#if ((EVE_DEVICE == BT815) ||(EVE_DEVICE == BT816) ||(EVE_DEVICE == BT817) || (EVE_DEVICE == BT818))

// My defines for the BT8xx PINDRIVE registers and settings
// Byte 2, bits 1:0 specifies the drive level
#define EVE_PINDRIVE_HR_5mA             (0x00)
#define EVE_PINDRIVE_HR_10mA            (0x01)
#define EVE_PINDRIVE_HR_15mA            (0x02)
#define EVE_PINDRIVE_HR_20mA            (0x03)
#define EVE_PINDRIVE_LR_1p2mA           (0x00)
#define EVE_PINDRIVE_LR_2p4mA           (0x01)
#define EVE_PINDRIVE_LR_3p6mA           (0x02)
#define EVE_PINDRIVE_LR_4p8mA           (0x03)
// Byte 2, bits 7:2 specifies the pin(s)
#define EVE_PINDRIVE_GPIO_0_HR          (0x00) // 0x00 << 2 =  0 * 4 =   0
#define EVE_PINDRIVE_GPIO_1_HR          (0x04) // 0x01 << 2 =  1 * 4 =   4
#define EVE_PINDRIVE_GPIO_2_HR          (0x08) // 0x02 << 2 =  2 * 4 =   8
#define EVE_PINDRIVE_GPIO_3_HR          (0x0C) // 0x03 << 2 =  3 * 4 =  12
#define EVE_PINDRIVE_DISP_LR            (0x20) // 0x08 << 2 =  8 * 4 =  32
#define EVE_PINDRIVE_DE_LR              (0x24) // 0x09 << 2 =  9 * 4 =  36
#define EVE_PINDRIVE_VSYNC_HSYNC_LR     (0x28) // 0x0A << 2 = 10 * 4 =  40
#define EVE_PINDRIVE_PCLK_LR            (0x2C) // 0x0B << 2 = 11 * 4 =  44
#define EVE_PINDRIVE_BACKLIGHT_LR       (0x30) // 0x0C << 2 = 12 * 4 =  48
#define EVE_PINDRIVE_RGB_LR             (0x34) // 0x0D << 2 = 13 * 4 =  52
#define EVE_PINDRIVE_AUDIO_L_HR         (0x38) // 0x0E << 2 = 14 * 4 =  56
#define EVE_PINDRIVE_INT_N_HR           (0x3C) // 0x0F << 2 = 15 * 4 =  60
#define EVE_PINDRIVE_CTP_RST_N_HR       (0x40) // 0x10 << 2 = 16 * 4 =  64
#define EVE_PINDRIVE_CTP_SCL_HR         (0x44) // 0x11 << 2 = 17 * 4 =  68
#define EVE_PINDRIVE_CTP_SDA_HR         (0x48) // 0x12 << 2 = 18 * 4 =  72
#define EVE_PINDRIVE_SPI_DATA_HR        (0x4C) // 0x13 << 2 = 19 * 4 =  76
#define EVE_PINDRIVE_SPIM_SCLK_HR       (0x50) // 0x14 << 2 = 20 * 4 =  80
#define EVE_PINDRIVE_SPIM_SS_N_HR       (0x54) // 0x15 << 2 = 21 * 4 =  84
#define EVE_PINDRIVE_SPIM_MISO_HR       (0x58) // 0x16 << 2 = 22 * 4 =  88
#define EVE_PINDRIVE_SPIM_MOSI_HR       (0x5C) // 0x17 << 2 = 23 * 4 =  92
#define EVE_PINDRIVE_SPIM_IO2_HR        (0x60) // 0x18 << 2 = 24 * 4 =  96
#define EVE_PINDRIVE_SPIM_IO3_HR        (0x64) // 0x19 << 2 = 25 * 4 = 100


#define EVE_LOW_FREQ_BOUND              (58800000L)    //98% of 60Mhz

#define EVE_RAM_G_SIZE                  (1024*1024L)
#define EVE_RAM_BIST                    (0x00380000UL) // = 3670016UL
#define EVE_RAM_CMD                     (0x00308000UL) // = 3178496UL
#define EVE_RAM_DL                      (0x00300000UL) // = 3145728UL
#define EVE_RAM_G                       (0x00000000UL) // = 0UL
#define EVE_RAM_REG                     (0x00302000UL) // = 3153920UL
#define EVE_RAM_ROMSUB                  (0x0030A000UL) // = 3186688UL
#define EVE_RAM_TOP                     (0x00304000UL) // = 3162112UL
#define EVE_CMDBUF_SIZE                 (0x00001000UL) // = 4096UL
#define EVE_RAM_CMD_SIZE                (4*1024L)
#define EVE_RAM_ERR_REPORT              (0x00309800UL) // = 3184640UL
#define ROMFONT_TABLEADDRESS            (0x002FFFFCUL) // = 3145724UL
#define EVE_REG_ADAPTIVE_FRAMERATE      (0x0030257CUL) // = 3155324UL
#define EVE_REG_AH_HCYCLE_MAX           (0x00302610UL) // = 3155472UL
#define EVE_REG_ANALOG                  (0x0030216CUL) // = 3154284UL
#define EVE_REG_ANA_COMP                (0x00302184UL) // = 3154308UL
#define EVE_REG_ANIM_ACTIVE             (0x0030902CUL) // = 3182636UL
#define EVE_REG_BIST_EN                 (0x00302174UL) // = 3154292UL
#define EVE_REG_BUSYBITS                (0x003020E8UL) // = 3154152UL
#define EVE_REG_CLOCK                   (0x00302008UL) // = 3153928UL
#define EVE_REG_CMDB_SPACE              (0x00302574UL) // = 3155316UL
#define EVE_REG_CMDB_WRITE              (0x00302578UL) // = 3155320UL
#define EVE_REG_CMD_DL                  (0x00302100UL) // = 3154176UL
#define EVE_REG_CMD_READ                (0x003020F8UL) // = 3154168UL
#define EVE_REG_CMD_WRITE               (0x003020FCUL) // = 3154172UL
#define EVE_REG_CPURESET                (0x00302020UL) // = 3153952UL
#define EVE_REG_CRC                     (0x00302178UL) // = 3154296UL
#define EVE_REG_CSPREAD                 (0x00302068UL) // = 3154024UL
#define EVE_REG_CTOUCH_EXTENDED         (0x00302108UL) // = 3154184UL
#define EVE_REG_CTOUCH_TOUCH0_XY        (0x00302124UL) // = 3154212UL
#define EVE_REG_CTOUCH_TOUCH4_X         (0x0030216CUL) // = 3154284UL
#define EVE_REG_CTOUCH_TOUCH4_Y         (0x00302120UL) // = 3154208UL
#define EVE_REG_CTOUCH_TOUCH1_XY        (0x0030211CUL) // = 3154204UL
#define EVE_REG_CTOUCH_TOUCH2_XY        (0x0030218CUL) // = 3154316UL
#define EVE_REG_CTOUCH_TOUCH3_XY        (0x00302190UL) // = 3154320UL
#define EVE_REG_TOUCH_CONFIG            (0x00302168UL) // = 3154280UL
#define EVE_REG_DATESTAMP               (0x00302564UL) // = 3155300UL
#define EVE_REG_DF_TUNED                (0x00309030UL) // = 3182640UL
#define EVE_REG_DITHER                  (0x00302060UL) // = 3154016UL
#define EVE_REG_DLSWAP                  (0x00302054UL) // = 3154004UL
#define EVE_REG_EHOST_TOUCH_ACK         (0x00302170UL) // = 3154288UL
#define EVE_REG_EHOST_TOUCH_ID          (0x00302114UL) // = 3154196UL
#define EVE_REG_EHOST_TOUCH_X           (0x0030210CUL) // = 3154188UL
#define EVE_REG_EHOST_TOUCH_Y           (0x00302118UL) // = 3154200UL
#define EVE_REG_EJPG_ACC                (0x00302358UL) // = 3154776UL
#define EVE_REG_EJPG_BUSY               (0x00302198UL) // = 3154328UL
#define EVE_REG_EJPG_DAT                (0x0030219CUL) // = 3154332UL
#define EVE_REG_EJPG_DCC                (0x00302340UL) // = 3154752UL
#define EVE_REG_EJPG_DEBUG              (0x0030255CUL) // = 3155292UL
#define EVE_REG_EJPG_DST                (0x003021A4UL) // = 3154340UL
#define EVE_REG_EJPG_FORMAT             (0x003021B0UL) // = 3154352UL
#define EVE_REG_EJPG_H                  (0x003021ACUL) // = 3154348UL
#define EVE_REG_EJPG_HT                 (0x00302240UL) // = 3154496UL
#define EVE_REG_EJPG_OPTIONS            (0x003021A0UL) // = 3154336UL
#define EVE_REG_EJPG_Q                  (0x003021C0UL) // = 3154368UL
#define EVE_REG_EJPG_READY              (0x00302194UL) // = 3154324UL
#define EVE_REG_EJPG_RI                 (0x003021B4UL) // = 3154356UL
#define EVE_REG_EJPG_SCALE              (0x00302558UL) // = 3155288UL
#define EVE_REG_EJPG_TDA                (0x003021BCUL) // = 3154364UL
#define EVE_REG_EJPG_TQ                 (0x003021B8UL) // = 3154360UL
#define EVE_REG_EJPG_W                  (0x003021A8UL) // = 3154344UL
#define EVE_REG_ESPIM_ADD               (0x0030259CUL) // = 3155356UL
#define EVE_REG_ESPIM_COUNT             (0x003025A0UL) // = 3155360UL
#define EVE_REG_ESPIM_DUMMY             (0x003025E4UL) // = 3155428UL
#define EVE_REG_ESPIM_READSTART         (0x00302588UL) // = 3155336UL
#define EVE_REG_ESPIM_SEQ               (0x0030258CUL) // = 3155340UL
#define EVE_REG_ESPIM_TRIG              (0x003025E8UL) // = 3155432UL
#define EVE_REG_ESPIM_WINDOW            (0x003025A4UL) // = 3155364UL
#define EVE_REG_FLASH_SIZE              (0x00309024UL) // = 3182628UL
#define EVE_REG_FLASH_STATUS            (0x003025F0UL) // = 3155440UL
#define EVE_REG_FRAMES                  (0x00302004UL) // = 3153924UL
#define EVE_REG_FREQUENCY               (0x0030200CUL) // = 3153932UL
#define EVE_REG_FULLBUSYBITS            (0x003025F4UL) // = 3155444UL
#define EVE_REG_GPIO                    (0x00302094UL) // = 3154068UL
#define EVE_REG_GPIOX                   (0x0030209CUL) // = 3154076UL
#define EVE_REG_GPIOX_DIR               (0x00302098UL) // = 3154072UL
#define EVE_REG_GPIO_DIR                (0x00302090UL) // = 3154064UL
#define EVE_REG_HCYCLE                  (0x0030202CUL) // = 3153964UL
#define EVE_REG_HOFFSET                 (0x00302030UL) // = 3153968UL
#define EVE_REG_HSIZE                   (0x00302034UL) // = 3153972UL
#define EVE_REG_HSYNC0                  (0x00302038UL) // = 3153976UL
#define EVE_REG_HSYNC1                  (0x0030203CUL) // = 3153980UL
#define EVE_REG_ID                      (0x00302000UL) // = 3153920UL
#define EVE_REG_INT_EN                  (0x003020ACUL) // = 3154092UL
#define EVE_REG_INT_FLAGS               (0x003020A8UL) // = 3154088UL
#define EVE_REG_INT_MASK                (0x003020B0UL) // = 3154096UL
#define EVE_REG_MACRO_0                 (0x003020D8UL) // = 3154136UL
#define EVE_REG_MACRO_1                 (0x003020DCUL) // = 3154140UL
#define EVE_REG_MEDIAFIFO_BASE          (0x0030901CUL) // = 3182620UL
#define EVE_REG_MEDIAFIFO_READ          (0x00309014UL) // = 3182612UL
#define EVE_REG_MEDIAFIFO_SIZE          (0x00309020UL) // = 3182624UL
#define EVE_REG_MEDIAFIFO_WRITE         (0x00309018UL) // = 3182616UL
#define EVE_REG_OUTBITS                 (0x0030205CUL) // = 3154012UL
#define EVE_REG_PCLK                    (0x00302070UL) // = 3154032UL
#define EVE_REG_PCLK_POL                (0x0030206CUL) // = 3154028UL
#define EVE_REG_PLAY                    (0x0030208CUL) // = 3154060UL
#define EVE_REG_PLAYBACK_FORMAT         (0x003020C4UL) // = 3154116UL
#define EVE_REG_PLAYBACK_FREQ           (0x003020C0UL) // = 3154112UL
#define EVE_REG_PLAYBACK_LENGTH         (0x003020B8UL) // = 3154104UL
#define EVE_REG_PLAYBACK_LOOP           (0x003020C8UL) // = 3154120UL
#define EVE_REG_PLAYBACK_PAUSE          (0x003025ECUL) // = 3155436UL
#define EVE_REG_PLAYBACK_PLAY           (0x003020CCUL) // = 3154124UL
#define EVE_REG_PLAYBACK_READPTR        (0x003020BCUL) // = 3154108UL
#define EVE_REG_PLAYBACK_START          (0x003020B4UL) // = 3154100UL
#define EVE_REG_PLAY_CONTROL            (0x0030914EUL) // = 3182926UL
#define EVE_REG_COPRO_PATCH_PTR         (0x00309162UL) // = 3182946UL
#define EVE_REG_PWM_DUTY                (0x003020D4UL) // = 3154132UL
#define EVE_REG_PWM_HZ                  (0x003020D0UL) // = 3154128UL
#define EVE_REG_RAM_FOLD                (0x003020F4UL) // = 3154164UL
#define EVE_REG_RASTERY                 (0x00302560UL) // = 3155296UL
#define EVE_REG_RENDERMODE              (0x00302010UL) // = 3153936UL
#define EVE_REG_ROMSUB_SEL              (0x003020F0UL) // = 3154160UL
#define EVE_REG_ROTATE                  (0x00302058UL) // = 3154008UL
#define EVE_REG_SNAPFORMAT              (0x0030201CUL) // = 3153948UL
#define EVE_REG_SNAPSHOT                (0x00302018UL) // = 3153944UL
#define EVE_REG_SNAPY                   (0x00302014UL) // = 3153940UL
#define EVE_REG_SOUND                   (0x00302088UL) // = 3154056UL
#define EVE_REG_SPIM                    (0x00302584UL) // = 3155332UL
#define EVE_REG_SPIM_DIR                (0x00302580UL) // = 3155328UL
#define EVE_REG_SPI_EARLY_TX            (0x0030217CUL) // = 3154300UL
#define EVE_REG_SPI_WIDTH               (0x00302188UL) // = 3154312UL
#define EVE_REG_SWIZZLE                 (0x00302064UL) // = 3154020UL
#define EVE_REG_TAG                     (0x0030207CUL) // = 3154044UL
#define EVE_REG_TAG_X                   (0x00302074UL) // = 3154036UL
#define EVE_REG_TAG_Y                   (0x00302078UL) // = 3154040UL
#define EVE_REG_TAP_CRC                 (0x00302024UL) // = 3153956UL
#define EVE_REG_TAP_MASK                (0x00302028UL) // = 3153960UL
#define EVE_REG_TOUCH_ADC_MODE          (0x00302108UL) // = 3154184UL
#define EVE_REG_TOUCH_CHARGE            (0x0030210CUL) // = 3154188UL
#define EVE_REG_TOUCH_DIRECT_XY         (0x0030218CUL) // = 3154316UL
#define EVE_REG_TOUCH_DIRECT_Z1Z2       (0x00302190UL) // = 3154320UL
#define EVE_REG_TOUCH_FAULT             (0x00302170UL) // = 3154288UL
#define EVE_REG_TOUCH_MODE              (0x00302104UL) // = 3154180UL
#define EVE_REG_TOUCH_OVERSAMPLE        (0x00302114UL) // = 3154196UL
#define EVE_REG_TOUCH_RAW_XY            (0x0030211CUL) // = 3154204UL
#define EVE_REG_TOUCH_RZ                (0x00302120UL) // = 3154208UL
#define EVE_REG_TOUCH_RZTHRESH          (0x00302118UL) // = 3154200UL
#define EVE_REG_TOUCH_SCREEN_XY         (0x00302124UL) // = 3154212UL
#define EVE_REG_TOUCH_SETTLE            (0x00302110UL) // = 3154192UL
#define EVE_REG_TOUCH_TAG               (0x0030212CUL) // = 3154220UL
#define EVE_REG_TOUCH_TAG1              (0x00302134UL) // = 3154228UL
#define EVE_REG_TOUCH_TAG1_XY           (0x00302130UL) // = 3154224UL
#define EVE_REG_TOUCH_TAG2              (0x0030213CUL) // = 3154236UL
#define EVE_REG_TOUCH_TAG2_XY           (0x00302138UL) // = 3154232UL
#define EVE_REG_TOUCH_TAG3              (0x00302144UL) // = 3154244UL
#define EVE_REG_TOUCH_TAG3_XY           (0x00302140UL) // = 3154240UL
#define EVE_REG_TOUCH_TAG4              (0x0030214CUL) // = 3154252UL
#define EVE_REG_TOUCH_TAG4_XY           (0x00302148UL) // = 3154248UL
#define EVE_REG_TOUCH_TAG_XY            (0x00302128UL) // = 3154216UL
#define EVE_REG_TOUCH_TRANSFORM_A       (0x00302150UL) // = 3154256UL
#define EVE_REG_TOUCH_TRANSFORM_B       (0x00302154UL) // = 3154260UL
#define EVE_REG_TOUCH_TRANSFORM_C       (0x00302158UL) // = 3154264UL
#define EVE_REG_TOUCH_TRANSFORM_D       (0x0030215CUL) // = 3154268UL
#define EVE_REG_TOUCH_TRANSFORM_E       (0x00302160UL) // = 3154272UL
#define EVE_REG_TOUCH_TRANSFORM_F       (0x00302164UL) // = 3154276UL
#define EVE_REG_TRACKER                 (0x00309000UL) // = 3182592UL
#define EVE_REG_TRACKER_1               (0x00309004UL) // = 3182596UL
#define EVE_REG_TRACKER_2               (0x00309008UL) // = 3182600UL
#define EVE_REG_TRACKER_3               (0x0030900CUL) // = 3182604UL
#define EVE_REG_TRACKER_4               (0x00309010UL) // = 3182608UL
#define EVE_REG_TRIM                    (0x00302180UL) // = 3154304UL
#define EVE_REG_VCYCLE                  (0x00302040UL) // = 3153984UL
#define EVE_REG_VOFFSET                 (0x00302044UL) // = 3153988UL
#define EVE_REG_VOL_PB                  (0x00302080UL) // = 3154048UL
#define EVE_REG_VOL_SOUND               (0x00302084UL) // = 3154052UL
#define EVE_REG_VSIZE                   (0x00302048UL) // = 3153992UL
#define EVE_REG_VSYNC0                  (0x0030204CUL) // = 3153996UL
#define EVE_REG_VSYNC1                  (0x00302050UL) // = 3154000UL

#define EVE_ENC_CMD_ANIMDRAW            (0xFFFFFF56UL) // = 4294967126UL
#define EVE_ENC_CMD_ANIMFRAME           (0xFFFFFF5AUL) // = 4294967130UL
#define EVE_ENC_CMD_ANIMSTART           (0xFFFFFF53UL) // = 4294967123UL
#define EVE_ENC_CMD_ANIMSTOP            (0xFFFFFF54UL) // = 4294967124UL
#define EVE_ENC_CMD_ANIMXY              (0xFFFFFF55UL) // = 4294967125UL
#define EVE_ENC_CMD_APPEND              (0xFFFFFF1EUL) // = 4294967070UL
#define EVE_ENC_CMD_APPENDF             (0xFFFFFF59UL) // = 4294967129UL
#define EVE_ENC_CMD_BGCOLOR             (0xFFFFFF09UL) // = 4294967049UL
#define EVE_ENC_CMD_BITMAP_TRANSFORM    (0xFFFFFF21UL) // = 4294967073UL
#define EVE_ENC_CMD_BUTTON              (0xFFFFFF0DUL) // = 4294967053UL
#define EVE_ENC_CMD_CALIBRATE           (0xFFFFFF15UL) // = 4294967061UL
#define EVE_ENC_CMD_CLEARCACHE          (0xFFFFFF4FUL) // = 4294967119UL
#define EVE_ENC_CMD_CLOCK               (0xFFFFFF14UL) // = 4294967060UL
#define EVE_ENC_CMD_COLDSTART           (0xFFFFFF32UL) // = 4294967090UL
#define EVE_ENC_CMD_CRC                 (0xFFFFFF03UL) // = 4294967043UL
#define EVE_ENC_CMD_DEPRECATED_CSKETCH  (0xFFFFFF35UL) // = 4294967093UL
#define EVE_ENC_CMD_DIAL                (0xFFFFFF2DUL) // = 4294967085UL
#define EVE_ENC_CMD_DLSTART             (0xFFFFFF00UL) // = 4294967040UL
#define EVE_ENC_CMD_EXECUTE             (0xFFFFFF07UL) // = 4294967047UL
#define EVE_ENC_CMD_FGCOLOR             (0xFFFFFF0AUL) // = 4294967050UL
#define EVE_ENC_CMD_FILLWIDTH           (0xFFFFFF58UL) // = 4294967128UL
#define EVE_ENC_CMD_FLASHATTACH         (0xFFFFFF49UL) // = 4294967113UL
#define EVE_ENC_CMD_FLASHDETACH         (0xFFFFFF48UL) // = 4294967112UL
#define EVE_ENC_CMD_FLASHERASE          (0xFFFFFF44UL) // = 4294967108UL
#define EVE_ENC_CMD_FLASHFAST           (0xFFFFFF4AUL) // = 4294967114UL
#define EVE_ENC_CMD_FLASHREAD           (0xFFFFFF46UL) // = 4294967110UL
#define EVE_ENC_CMD_FLASHSOURCE         (0xFFFFFF4EUL) // = 4294967118UL
#define EVE_ENC_CMD_FLASHSPIDESEL       (0xFFFFFF4BUL) // = 4294967115UL
#define EVE_ENC_CMD_FLASHSPIRX          (0xFFFFFF4DUL) // = 4294967117UL
#define EVE_ENC_CMD_FLASHSPITX          (0xFFFFFF4CUL) // = 4294967116UL
#define EVE_ENC_CMD_FLASHUPDATE         (0xFFFFFF47UL) // = 4294967111UL
#define EVE_ENC_CMD_FLASHWRITE          (0xFFFFFF45UL) // = 4294967109UL
#define EVE_ENC_CMD_GAUGE               (0xFFFFFF13UL) // = 4294967059UL
#define EVE_ENC_CMD_GETMATRIX           (0xFFFFFF33UL) // = 4294967091UL
#define EVE_ENC_CMD_GETPOINT            (0xFFFFFF08UL) // = 4294967048UL
#define EVE_ENC_CMD_GETPROPS            (0xFFFFFF25UL) // = 4294967077UL
#define EVE_ENC_CMD_GETPTR              (0xFFFFFF23UL) // = 4294967075UL
#define EVE_ENC_CMD_GRADCOLOR           (0xFFFFFF34UL) // = 4294967092UL
#define EVE_ENC_CMD_GRADIENT            (0xFFFFFF0BUL) // = 4294967051UL
#define EVE_ENC_CMD_GRADIENTA           (0xFFFFFF57UL) // = 4294967127UL
#define EVE_ENC_CMD_HAMMERAUX           (0xFFFFFF04UL) // = 4294967044UL
#define EVE_ENC_CMD_HMAC                (0xFFFFFF5DUL) // = 4294967133UL
#define EVE_ENC_CMD_IDCT_DELETED        (0xFFFFFF06UL) // = 4294967046UL
#define EVE_ENC_CMD_INFLATE             (0xFFFFFF22UL) // = 4294967074UL
#define EVE_ENC_CMD_INFLATE2            (0xFFFFFF50UL) // = 4294967120UL
#define EVE_ENC_CMD_INTERRUPT           (0xFFFFFF02UL) // = 4294967042UL
#define EVE_ENC_CMD_INT_RAMSHARED       (0xFFFFFF3DUL) // = 4294967101UL
#define EVE_ENC_CMD_INT_SWLOADIMAGE     (0xFFFFFF3EUL) // = 4294967102UL
#define EVE_ENC_CMD_KEYS                (0xFFFFFF0EUL) // = 4294967054UL
#define EVE_ENC_CMD_LAST_               (0xFFFFFF5EUL) // = 4294967134UL
#define EVE_ENC_CMD_LOADIDENTITY        (0xFFFFFF26UL) // = 4294967078UL
#define EVE_ENC_CMD_LOADIMAGE           (0xFFFFFF24UL) // = 4294967076UL
#define EVE_ENC_CMD_LOGO                (0xFFFFFF31UL) // = 4294967089UL
#define EVE_ENC_CMD_MARCH               (0xFFFFFF05UL) // = 4294967045UL
#define EVE_ENC_CMD_MEDIAFIFO           (0xFFFFFF39UL) // = 4294967097UL
#define EVE_ENC_CMD_MEMCPY              (0xFFFFFF1DUL) // = 4294967069UL
#define EVE_ENC_CMD_MEMCRC              (0xFFFFFF18UL) // = 4294967064UL
#define EVE_ENC_CMD_MEMSET              (0xFFFFFF1BUL) // = 4294967067UL
#define EVE_ENC_CMD_MEMWRITE            (0xFFFFFF1AUL) // = 4294967066UL
#define EVE_ENC_CMD_MEMZERO             (0xFFFFFF1CUL) // = 4294967068UL
#define EVE_ENC_CMD_NOP                 (0xFFFFFF5BUL) // = 4294967131UL
#define EVE_ENC_CMD_NUMBER              (0xFFFFFF2EUL) // = 4294967086UL
#define EVE_ENC_CMD_PLAYVIDEO           (0xFFFFFF3AUL) // = 4294967098UL
#define EVE_ENC_CMD_PROGRESS            (0xFFFFFF0FUL) // = 4294967055UL
#define EVE_ENC_CMD_REGREAD             (0xFFFFFF19UL) // = 4294967065UL
#define EVE_ENC_CMD_RESETFONTS          (0xFFFFFF52UL) // = 4294967122UL
#define EVE_ENC_CMD_ROMFONT             (0xFFFFFF3FUL) // = 4294967103UL
#define EVE_ENC_CMD_ROTATE              (0xFFFFFF29UL) // = 4294967081UL
#define EVE_ENC_CMD_ROTATEAROUND        (0xFFFFFF51UL) // = 4294967121UL
#define EVE_ENC_CMD_SCALE               (0xFFFFFF28UL) // = 4294967080UL
#define EVE_ENC_CMD_SCREENSAVER         (0xFFFFFF2FUL) // = 4294967087UL
#define EVE_ENC_CMD_SCROLLBAR           (0xFFFFFF11UL) // = 4294967057UL
#define EVE_ENC_CMD_SETBASE             (0xFFFFFF38UL) // = 4294967096UL
#define EVE_ENC_CMD_SETBITMAP           (0xFFFFFF43UL) // = 4294967107UL
#define EVE_ENC_CMD_SETFONT             (0xFFFFFF2BUL) // = 4294967083UL
#define EVE_ENC_CMD_SETFONT2            (0xFFFFFF3BUL) // = 4294967099UL
#define EVE_ENC_CMD_SETMATRIX           (0xFFFFFF2AUL) // = 4294967082UL
#define EVE_ENC_CMD_SETROTATE           (0xFFFFFF36UL) // = 4294967094UL
#define EVE_ENC_CMD_SETSCRATCH          (0xFFFFFF3CUL) // = 4294967100UL
#define EVE_ENC_CMD_SHA1                (0xFFFFFF5CUL) // = 4294967132UL
#define EVE_ENC_CMD_SKETCH              (0xFFFFFF30UL) // = 4294967088UL
#define EVE_ENC_CMD_SLIDER              (0xFFFFFF10UL) // = 4294967056UL
#define EVE_ENC_CMD_SNAPSHOT            (0xFFFFFF1FUL) // = 4294967071UL
#define EVE_ENC_CMD_SNAPSHOT2           (0xFFFFFF37UL) // = 4294967095UL
#define EVE_ENC_CMD_SPINNER             (0xFFFFFF16UL) // = 4294967062UL
#define EVE_ENC_CMD_STOP                (0xFFFFFF17UL) // = 4294967063UL
#define EVE_ENC_CMD_SWAP                (0xFFFFFF01UL) // = 4294967041UL
#define EVE_ENC_CMD_SYNC                (0xFFFFFF42UL) // = 4294967106UL
#define EVE_ENC_CMD_TEXT                (0xFFFFFF0CUL) // = 4294967052UL
#define EVE_ENC_CMD_TOGGLE              (0xFFFFFF12UL) // = 4294967058UL
#define EVE_ENC_CMD_TOUCH_TRANSFORM     (0xFFFFFF20UL) // = 4294967072UL
#define EVE_ENC_CMD_TRACK               (0xFFFFFF2CUL) // = 4294967084UL
#define EVE_ENC_CMD_TRANSLATE           (0xFFFFFF27UL) // = 4294967079UL
#define EVE_ENC_CMD_VIDEOFRAME          (0xFFFFFF41UL) // = 4294967105UL
#define EVE_ENC_CMD_VIDEOSTART          (0xFFFFFF40UL) // = 4294967104UL
#define EVE_ENC_CMD_VIDEOSTARTF         (0xFFFFFF5FUL) // = 4294967135UL

#define EVE_ENC_VERTEX2F(x,y)                                (( 1UL<<30)|(((x)&0x07FFFUL)<<15)|(((y)&0x07FFFUL)<<0))
#define EVE_ENC_VERTEX2II(x,y,handle,cell)                   (( 2UL<<30)|(((x)&0x01FFUL)<<21)|(((y)&0x01FFUL)<<12)|(((handle)&0x01FUL)<<7)|(((cell)&0x07FUL)<<0))
//This throws "warning: comparison of unsigned expression < 0 is always false"
//#define EVE_ENC_BITMAP_SOURCE(addr)                          (( 1UL<<24)|((addr) < 0 ?  (((addr)&0x007FFFFFUL)) : ((addr)&0x00FFFFFFUL)))
//My edit:
#define EVE_ENC_BITMAP_SOURCE(addr)                          (( 1UL<<24)|((addr)&0x00FFFFFFUL))
#define EVE_ENC_BITMAP_SOURCE2(flash_or_ram, addr)           (( 1UL<<24)|((flash_or_ram) << 23)|(((addr)&0x007FFFFFUL)<<0))
#define EVE_ENC_CLEAR_COLOR_RGB(red,green,blue)              (( 2UL<<24)|(((red)&0x0FFUL)<<16)|(((green)&0x0FFUL)<<8)|(((blue)&0x0FFUL)<<0))
#define EVE_ENC_TAG(s)                                       (( 3UL<<24)|(((s)&0x0FFUL)<<0))
#define EVE_ENC_COLOR_RGB(red,green,blue)                    (( 4UL<<24)|(((red)&0x0FFUL)<<16)|(((green)&0x0FFUL)<<8)|(((blue)&0x0FFUL)<<0))
#define EVE_ENC_BITMAP_HANDLE(handle)                        (( 5UL<<24)|(((handle)&0x01FUL)<<0))
#define EVE_ENC_CELL(cell)                                   (( 6UL<<24)|(((cell)&0x07FUL)<<0))
#define EVE_ENC_BITMAP_LAYOUT(format,linestride,height)      (( 7UL<<24)|(((format)&0x031FUL)<<19)|(((linestride)&0x03FFUL)<<9)|(((height)&0x01FFUL)<<0))
#define EVE_ENC_BITMAP_SIZE(filter,wrapx,wrapy,width,height) (( 8UL<<24)|(((filter)&0x01UL)<<20)|(((wrapx)&0x01UL)<<19)|(((wrapy)&0x01UL)<<18)|(((width)&0x01FFUL)<<9)|(((height)&0x01FFUL)<<0))
#define EVE_ENC_ALPHA_FUNC(func,ref)                         (( 9UL<<24)|(((func)&0x07UL)<<8)|(((ref)&0x0FFUL)<<0))
#define EVE_ENC_STENCIL_FUNC(func,ref,mask)                  ((10UL<<24)|(((func)&0x07UL)<<16)|(((ref)&0x0FFUL)<<8)|(((mask)&0x0FFUL)<<0))
#define EVE_ENC_BLEND_FUNC(src,dst)                          ((11UL<<24)|(((src)&0x07UL)<<3)|(((dst)&0x07UL)<<0))
#define EVE_ENC_STENCIL_OP(sfail,spass)                      ((12UL<<24)|(((sfail)&0x07UL)<<3)|(((spass)&0x07UL)<<0))
#define EVE_ENC_POINT_SIZE(size)                             ((13UL<<24)|(((size)&0x01FFFUL)<<0))
#define EVE_ENC_LINE_WIDTH(width)                            ((14UL<<24)|(((width)&0x0FFFUL)<<0))
#define EVE_ENC_CLEAR_COLOR_A(alpha)                         ((15UL<<24)|(((alpha)&0x0FFUL)<<0))
#define EVE_ENC_COLOR_A(alpha)                               ((16UL<<24)|(((alpha)&0x0FFUL)<<0))
#define EVE_ENC_CLEAR_STENCIL(s)                             ((17UL<<24)|(((s)&0x0FFUL)<<0))
#define EVE_ENC_CLEAR_TAG(s)                                 ((18UL<<24)|(((s)&0x0FFUL)<<0))
#define EVE_ENC_STENCIL_MASK(mask)                           ((19UL<<24)|(((mask)&0x0FFUL)<<0))
#define EVE_ENC_TAG_MASK(mask)                               ((20UL<<24)|(((mask)&0x01UL)<<0))
#define EVE_ENC_BITMAP_TRANSFORM_A(p,v)                      ((21UL<<24)|(((p)&0x01UL)<<17)|(((v)&0x01FFFFUL)<<0))
#define EVE_ENC_BITMAP_TRANSFORM_B(p,v)                      ((22UL<<24)|(((p)&0x01UL)<<17)|(((v)&0x01FFFFUL)<<0))
#define EVE_ENC_BITMAP_TRANSFORM_C(c)                        ((23UL<<24)|(((c)&0x0FFFFFFUL)<<0))
#define EVE_ENC_BITMAP_TRANSFORM_D(p,v)                      ((24UL<<24)|(((p)&0x01UL)<<17)|(((v)&0x01FFFFUL)<<0))
#define EVE_ENC_BITMAP_TRANSFORM_E(p,v)                      ((25UL<<24)|(((p)&0x01UL)<<17)|(((v)&0x01FFFFUL)<<0))
#define EVE_ENC_BITMAP_TRANSFORM_F(f)                        ((26UL<<24)|(((f)&0x0FFFFFFUL)<<0))
#define EVE_ENC_SCISSOR_XY(x,y)                              ((27UL<<24)|(((x)&0x07FFUL)<<11)|(((y)&0x07FFUL)<<0))
#define EVE_ENC_SCISSOR_SIZE(width,height)                   ((28UL<<24)|(((width)&0x0FFFUL)<<12)|(((height)&0x0FFFUL)<<0))
#define EVE_ENC_CALL(dest)                                   ((29UL<<24)|(((dest)&0x0FFFFUL)<<0))
#define EVE_ENC_JUMP(dest)                                   ((30UL<<24)|(((dest)&0x0FFFFUL)<<0))
#define EVE_ENC_BEGIN(prim)                                  ((31UL<<24)|(((prim)&15UL)<<0))
#define EVE_ENC_COLOR_MASK(r,g,b,a)                          ((32UL<<24)|(((r)&0x01UL)<<3)|(((g)&0x01UL)<<2)|(((b)&0x01UL)<<1)|(((a)&0x01UL)<<0))
#define EVE_ENC_END()                                        ((33UL<<24))
#define EVE_ENC_SAVE_CONTEXT                               ((34UL<<24))
#define EVE_ENC_RESTORE_CONTEXT                            ((35UL<<24))
#define EVE_ENC_RETURN()                                     ((36UL<<24))
#define EVE_ENC_MACRO(m)                                     ((37UL<<24)|(((m)&0x01UL)<<0))
#define EVE_ENC_CLEAR(c,s,t)                                 ((38UL<<24)|(((c)&0x01UL)<<2)|(((s)&0x01UL)<<1)|(((t)&0x01UL)<<0))
#define EVE_ENC_VERTEX_FORMAT(frac)                          ((39UL<<24)|(((frac)&0x07UL)<<0))
#define EVE_ENC_BITMAP_LAYOUT_H(linestride,height)           ((40UL<<24)|(((linestride)&0x03UL)<<2)|(((height)&0x03UL)<<0))
#define EVE_ENC_BITMAP_SIZE_H(width,height)                  ((41UL<<24)|(((width)&0x03UL)<<2)|(((height)&0x03UL)<<0))
#define EVE_ENC_PALETTE_SOURCE(addr)                         ((42UL<<24)|(((addr)&0x03FFFFFUL)<<0))
#define EVE_ENC_VERTEX_TRANSLATE_X(x)                        ((43UL<<24)|(((x)&0x01FFFFUL)<<0))
#define EVE_ENC_VERTEX_TRANSLATE_Y(y)                        ((44UL<<24)|(((y)&0x01FFFFUL)<<0))
#define EVE_ENC_NOP()                                        ((45UL<<24))
#define EVE_ENC_BITMAP_EXT_FORMAT(format)                    ((46UL<<24)|(((format)&0x0FFFFUL)<<0))
#define EVE_ENC_BITMAP_SWIZZLE(r,g,b,a)                      ((47UL<<24)|(((r)&0x07UL)<<9)|(((g)&0x07UL)<<6)|(((b)&0x07UL)<<3)|(((a)&0x07UL)<<0))
#define EVE_ENC_DISPLAY()                                    ((0UL<<24))

#define EVE_FORMAT_COMPRESSED_RGBA_ASTC_4x4_KHR              (0x000093B0UL) // = 37808UL
#define EVE_FORMAT_COMPRESSED_RGBA_ASTC_5x4_KHR              (0x000093B1UL) // = 37809UL
#define EVE_FORMAT_COMPRESSED_RGBA_ASTC_5x5_KHR              (0x000093B2UL) // = 37810UL
#define EVE_FORMAT_COMPRESSED_RGBA_ASTC_6x5_KHR              (0x000093B3UL) // = 37811UL
#define EVE_FORMAT_COMPRESSED_RGBA_ASTC_6x6_KHR              (0x000093B4UL) // = 37812UL
#define EVE_FORMAT_COMPRESSED_RGBA_ASTC_8x5_KHR              (0x000093B5UL) // = 37813UL
#define EVE_FORMAT_COMPRESSED_RGBA_ASTC_8x6_KHR              (0x000093B6UL) // = 37814UL
#define EVE_FORMAT_COMPRESSED_RGBA_ASTC_8x8_KHR              (0x000093B7UL) // = 37815UL
#define EVE_FORMAT_COMPRESSED_RGBA_ASTC_10x10_KHR            (0x000093BBUL) // = 37819UL
#define EVE_FORMAT_COMPRESSED_RGBA_ASTC_10x5_KHR             (0x000093B8UL) // = 37816UL
#define EVE_FORMAT_COMPRESSED_RGBA_ASTC_10x6_KHR             (0x000093B9UL) // = 37817UL
#define EVE_FORMAT_COMPRESSED_RGBA_ASTC_10x8_KHR             (0x000093BAUL) // = 37818UL
#define EVE_FORMAT_COMPRESSED_RGBA_ASTC_12x10_KHR            (0x000093BCUL) // = 37820UL
#define EVE_FORMAT_COMPRESSED_RGBA_ASTC_12x12_KHR            (0x000093BDUL) // = 37821UL

#define EVE_RAM_FLASH           0x800000UL
#define EVE_RAM_FLASH_POSTBLOB  0x801000UL

#define EVE_CTOUCH_MODE_COMPATIBILITY   (0x00000001UL) // = 1UL
#define EVE_CTOUCH_MODE_EXTENDED        (0x00000000UL) // = 0UL

#define EVE_STENCIL_DECR                (0x00000004UL) // = 4UL
#define EVE_DLSWAP_DONE                 (0x00000000UL) // = 0UL
#define EVE_DLSWAP_FRAME                (0x00000002UL) // = 2UL
#define EVE_DLSWAP_LINE                 (0x00000001UL) // = 1UL
#define EVE_BLEND_DST_ALPHA             (0x00000003UL) // = 3UL
#define EVE_BEGIN_EDGE_STRIP_A          (0x00000007UL) // = 7UL
#define EVE_BEGIN_EDGE_STRIP_B          (0x00000008UL) // = 8UL
#define EVE_BEGIN_EDGE_STRIP_L          (0x00000006UL) // = 6UL
#define EVE_BEGIN_EDGE_STRIP_R          (0x00000005UL) // = 5UL
#define EVE_TEST_EQUAL                  (0x00000005UL) // = 5UL
// Return values from EVE_REG_FLASH_STATUS
#define EVE_FLASH_STATUS_INIT           (0x00000000UL) // = 0UL
#define EVE_FLASH_STATUS_DETACHED       (0x00000001UL) // = 1UL
#define EVE_FLASH_STATUS_BASIC          (0x00000002UL) // = 2UL
#define EVE_FLASH_STATUS_FULL           (0x00000003UL) // = 3UL
#define EVE_TEST_GEQUAL                 (0x00000004UL) // = 4UL
#define EVE_GLFORMAT                    (0x0000001FUL) // = 31UL
#define EVE_TEST_GREATER                (0x00000003UL) // = 3UL
#define EVE_GREEN                       (0x00000003UL) // = 3UL
#define EVE_STENCIL_INCR                (0x00000003UL) // = 3UL
#define EVE_INT_CMDEMPTY                (0x00000020UL) // = 32UL
#define EVE_INT_CMDFLAG                 (0x00000040UL) // = 64UL
#define EVE_INT_CONVCOMPLETE            (0x00000080UL) // = 128UL
#define EVE_INT_G8                      (0x00000012UL) // = 18UL
#define EVE_INT_L8C                     (0x0000000CUL) // = 12UL
#define EVE_INT_PLAYBACK                (0x00000010UL) // = 16UL
#define EVE_INT_SOUND                   (0x00000008UL) // = 8UL
#define EVE_INT_SWAP                    (0x00000001UL) // = 1UL
#define EVE_INT_TAG                     (0x00000004UL) // = 4UL
#define EVE_INT_TOUCH                   (0x00000002UL) // = 2UL
#define EVE_INT_VGA                     (0x0000000DUL) // = 13UL
#define EVE_STENCIL_INVERT              (0x00000005UL) // = 5UL

#define EVE_STENCIL_KEEP                (0x00000001UL) // = 1UL
#define EVE_FORMAT_L1                   (0x00000001UL) // = 1UL
#define EVE_FORMAT_L2                   (0x00000011UL) // = 17UL
#define EVE_FORMAT_L4                   (0x00000002UL) // = 2UL
#define EVE_FORMAT_L8                   (0x00000003UL) // = 3UL
#define EVE_TEST_LEQUAL                 (0x00000002UL) // = 2UL
#define EVE_TEST_LESS                   (0x00000001UL) // = 1UL
#define EVE_LINEAR_SAMPLES              (0x00000000UL) // = 0UL
#define EVE_BEGIN_LINES                 (0x00000003UL) // = 3UL
#define EVE_BEGIN_LINE_STRIP            (0x00000004UL) // = 4UL
#define EVE_FILTER_NEAREST              (0x00000000UL) // = 0UL
#define EVE_TEST_NEVER                  (0x00000000UL) // = 0UL
#define EVE_TEST_NOTEQUAL               (0x00000006UL) // = 6UL
#define EVE_BLEND_ONE                   (0x00000001UL) // = 1UL
#define EVE_BLEND_ONE_MINUS_DST_ALPHA   (0x00000005UL) // = 5UL
#define EVE_BLEND_ONE_MINUS_SRC_ALPHA   (0x00000004UL) // = 4UL
#define EVE_OPT_CENTER                  (0x00000600UL) // = 1536UL
#define EVE_OPT_CENTERX                 (0x00000200UL) // = 512UL
#define EVE_OPT_CENTERY                 (0x00000400UL) // = 1024UL
#define EVE_OPT_FILL                    (0x00002000UL) // = 8192UL
#define EVE_OPT_FLASH                   (0x00000040UL) // = 64UL
#define EVE_OPT_FLAT                    (0x00000100UL) // = 256UL
#define EVE_OPT_FORMAT                  (0x00001000UL) // = 4096UL
#define EVE_OPT_FULLSCREEN              (0x00000008UL) // = 8UL
#define EVE_OPT_MEDIAFIFO               (0x00000010UL) // = 16UL
#define EVE_OPT_MONO                    (0x00000001UL) // = 1UL
#define EVE_OPT_NOBACK                  (0x00001000UL) // = 4096UL
#define EVE_OPT_NODL                    (0x00000002UL) // = 2UL
#define EVE_OPT_NOHANDS                 (0x0000C000UL) // = 49152UL
#define EVE_OPT_NOHM                    (0x00004000UL) // = 16384UL
#define EVE_OPT_NOPOINTER               (0x00004000UL) // = 16384UL
#define EVE_OPT_NOSECS                  (0x00008000UL) // = 32768UL
#define EVE_OPT_NOTEAR                  (0x00000004UL) // = 4UL
#define EVE_OPT_NOTICKS                 (0x00002000UL) // = 8192UL
#define EVE_OPT_OVERLAY                 (0x00000080UL) // = 128UL
#define EVE_OPT_RIGHTX                  (0x00000800UL) // = 2048UL
#define EVE_OPT_SIGNED                  (0x00000100UL) // = 256UL
#define EVE_OPT_SOUND                   (0x00000020UL) // = 32UL
#define EVE_FORMAT_PALETTED             (0x00000008UL) // = 8UL
#define EVE_FORMAT_PALETTED4444         (0x0000000FUL) // = 15UL
#define EVE_FORMAT_PALETTED565          (0x0000000EUL) // = 14UL
#define EVE_FORMAT_PALETTED8            (0x00000010UL) // = 16UL
#define EVE_BEGIN_POINTS                (0x00000002UL) // = 2UL
#define EVE_BEGIN_RECTS                 (0x00000009UL) // = 9UL
#define EVE_RED                         (0x00000002UL) // = 2UL
//#define PPC                           (0x00000010UL) // = 16UL
//#define BANKS                         (0x00000010UL) // = 16UL
//#define BANKW                         (0x00000010UL) // = 16UL
#define EVE_ADC_DIFFERENTIAL            (0x00000001UL) // = 1UL
#define EVE_ADC_SINGLE_ENDED            (0x00000000UL) // = 0UL
#define EVE_ADPCM_SAMPLES               (0x00000002UL) // = 2UL
#define EVE_ALPHA                       (0x00000005UL) // = 5UL
#define EVE_TEST_ALWAYS                 (0x00000007UL) // = 7UL
#define EVE_ANIM_HOLD                   (0x00000002UL) // = 2UL
#define EVE_ANIM_LOOP                   (0x00000001UL) // = 1UL
#define EVE_ANIM_ONCE                   (0x00000000UL) // = 0UL
#define EVE_FORMAT_ARGB1555             (0x00000000UL) // = 0UL
#define EVE_FORMAT_ARGB2                (0x00000005UL) // = 5UL
#define EVE_FORMAT_ARGB4                (0x00000006UL) // = 6UL
#define EVE_FORMAT_BARGRAPH             (0x0000000BUL) // = 11UL
#define EVE_FILTER_BILINEAR             (0x00000001UL) // = 1UL
#define EVE_BEGIN_BITMAPS               (0x00000001UL) // = 1UL
#define EVE_BLUE                        (0x00000004UL) // = 4UL
#define EVE_WRAP_BORDER                 (0x00000000UL) // = 0UL
#define EVE_WRAP_REPEAT                 (0x00000001UL) // = 1UL
#define EVE_STENCIL_REPLACE             (0x00000002UL) // = 2UL
#define EVE_FORMAT_RGB332               (0x00000004UL) // = 4UL
#define EVE_FORMAT_RGB565               (0x00000007UL) // = 7UL
#define EVE_BLEND_SRC_ALPHA             (0x00000002UL) // = 2UL
#define EVE_SS_A0                       (0x00000013UL) // = 19UL
#define EVE_SS_A1                       (0x00000014UL) // = 20UL
#define EVE_SS_A2                       (0x00000015UL) // = 21UL
#define EVE_SS_A3                       (0x00000016UL) // = 22UL
#define EVE_SS_A4                       (0x00000017UL) // = 23UL
#define EVE_SS_A5                       (0x00000018UL) // = 24UL
#define EVE_SS_A6                       (0x00000019UL) // = 25UL
#define EVE_SS_A7                       (0x0000001AUL) // = 26UL
#define EVE_SS_PAUSE                    (0x00000012UL) // = 18UL
#define EVE_SS_Q0                       (0x00000000UL) // = 0UL
#define EVE_SS_Q1                       (0x00000001UL) // = 1UL
#define EVE_SS_Q2                       (0x00000002UL) // = 2UL
#define EVE_SS_Q3                       (0x00000003UL) // = 3UL
#define EVE_SS_Q4                       (0x00000004UL) // = 4UL
#define EVE_SS_Q5                       (0x00000005UL) // = 5UL
#define EVE_SS_Q6                       (0x00000006UL) // = 6UL
#define EVE_SS_Q7                       (0x00000007UL) // = 7UL
#define EVE_SS_Q8                       (0x00000008UL) // = 8UL
#define EVE_SS_Q9                       (0x00000009UL) // = 9UL
#define EVE_SS_QA                       (0x0000000AUL) // = 10UL
#define EVE_SS_QB                       (0x0000000BUL) // = 11UL
#define EVE_SS_QC                       (0x0000000CUL) // = 12UL
#define EVE_SS_QD                       (0x0000000DUL) // = 13UL
#define EVE_SS_QE                       (0x0000000EUL) // = 14UL
#define EVE_SS_QF                       (0x0000000FUL) // = 15UL
#define EVE_SS_QI                       (0x0000001FUL) // = 31UL
#define EVE_SS_S0                       (0x00000010UL) // = 16UL
#define EVE_SS_S1                       (0x00000011UL) // = 17UL
#define EVE_TEXT8EVE_SS                 (0x00000009UL) // = 9UL
#define EVE_TEXTVGA                     (0x0000000AUL) // = 10UL
#define EVE_TOUCHMODE_CONTINUOUS        (0x00000003UL) // = 3UL
#define EVE_TOUCHMODE_FRAME             (0x00000002UL) // = 2UL
#define EVE_TOUCHMODE_OFF               (0x00000000UL) // = 0UL
#define EVE_TOUCHMODE_ONESHOT           (0x00000001UL) // = 1UL
#define EVE_ULAW_SAMPLES                (0x00000001UL) // = 1UL
#define EVE_VOL_ZERO                    (0x00000000UL) // = 0UL

#undef EVE_ENC_BITMAP_TRANSFORM_A //New BT815 file do not compatible with the legacy code
#undef EVE_ENC_BITMAP_TRANSFORM_B //New BT815 file do not compatible with the legacy code
#undef EVE_ENC_BITMAP_TRANSFORM_D //New BT815 file do not compatible with the legacy code
#undef EVE_ENC_BITMAP_TRANSFORM_E //New BT815 file do not compatible with the legacy code

#define EVE_ENC_BITMAP_TRANSFORM_A_EXT(p,v) ((21UL<<24)|(((p)&0x01UL)<<17)|(((v)&0x01FFFFUL)<<0))
#define EVE_ENC_BITMAP_TRANSFORM_B_EXT(p,v) ((22UL<<24)|(((p)&0x01UL)<<17)|(((v)&0x01FFFFUL)<<0))
#define EVE_ENC_BITMAP_TRANSFORM_C(c)       ((23UL<<24)|(((c)&0x0FFFFFFUL)<<0))
#define EVE_ENC_BITMAP_TRANSFORM_D_EXT(p,v) ((24UL<<24)|(((p)&0x01UL)<<17)|(((v)&0x01FFFFUL)<<0))
#define EVE_ENC_BITMAP_TRANSFORM_E_EXT(p,v) ((25UL<<24)|(((p)&0x01UL)<<17)|(((v)&0x01FFFFUL)<<0))
#define EVE_ENC_BITMAP_TRANSFORM_F(f)       ((26UL<<24)|(((f)&0x0FFFFFFUL)<<0))

// Added to ensure previous macros are fine
#define EVE_ENC_BITMAP_TRANSFORM_A(a) EVE_ENC_BITMAP_TRANSFORM_A_EXT(0,a)
#define EVE_ENC_BITMAP_TRANSFORM_B(b) EVE_ENC_BITMAP_TRANSFORM_B_EXT(0,b)
#define EVE_ENC_BITMAP_TRANSFORM_D(d) EVE_ENC_BITMAP_TRANSFORM_D_EXT(0,d)
#define EVE_ENC_BITMAP_TRANSFORM_E(e) EVE_ENC_BITMAP_TRANSFORM_E_EXT(0,e)

//End BT81x parameters
#endif // ((EVE_DEVICE == BT815) ||(EVE_DEVICE == BT816) ||(EVE_DEVICE == BT817) || (EVE_DEVICE == BT818))

//##################################################################################################

#define FT_GPU_NUMCHAR_PERFONT (128)
#define FT_GPU_FONT_TABLE_SIZE (148)

// EVE and FT80x font table structure
// Font table address in ROM can be found by reading the address from 0xFFFFC location.
// 16 font tables are present at the address read from location 0xFFFFC
typedef struct
  {
  // All the values are in bytes
  // Width of each character font from 0 to 127
  uint8_t
    FontWidth[FT_GPU_NUMCHAR_PERFONT];
  //Bitmap format of font wrt bitmap formats supported by FT800 - L1, L4, L8
  uint32_t
    FontBitmapFormat;
  // Font line stride in FT800 ROM
  uint32_t
    FontLineStride;
  uint32_t
    FontWidthInPixels;
  uint32_t
    FontHeightInPixels;
  //Pointer to font graphics raw data
  uint32_t
    PointerToFontGraphicsData;
  } EVE_GPU_FONT_HEADER;

//##################################################################################################

#endif  /* __EVE_DEFINES_H__ */

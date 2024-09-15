#ifndef _VARIANTS_H_
#define _VARIANTS_H_

#include "../Configuration.h"

/*
 * Hardware source
 * TIM3 for Buzzer timer
 * TIM6 for Neopixel RGB
 * TIM7 for OS Timer
 */

// Type of LCD driver
#define HX8558  (1 << 4)

#include "pin_ARTILLERY_TFT_CONTROLLER_V1_2.h"
#include "pin_MKS_TFT32_V1_4.h"

#define LCD_ENCODER_SUPPORT (defined(LCD_ENCA_PIN) && defined(LCD_ENCB_PIN) && defined(LCD_BTN_PIN))

#define LCD_DRIVER_IS(n)  ((TFTLCD_DRIVER) == (n))
#define LCD_DRIVER_HAS(n) (((TFTLCD_DRIVER) & (n)) == (n))

#define _FONT_SIZE(h, w) ((h << 8) | w)
#define _FONT_H(size) (size >> 8)
#define _FONT_W(size) (size & 0xFF)

#define FONT_SIZE_NORMAL _FONT_SIZE(BYTE_HEIGHT, BYTE_WIDTH)              // Normal font for common menu etc.
#define FONT_SIZE_LARGE  _FONT_SIZE(LARGE_BYTE_HEIGHT, LARGE_BYTE_WIDTH)  // Large font for numpad menu etc.

#endif

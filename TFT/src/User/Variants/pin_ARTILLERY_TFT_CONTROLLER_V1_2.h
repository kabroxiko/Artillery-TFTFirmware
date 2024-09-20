#ifndef _PIN_ARTILLERY_TFT_CONTROLLER_V1_2_H_  // modify to actual filename !!!
#define _PIN_ARTILLERY_TFT_CONTROLLER_V1_2_H_  // modify to actual filename !!!

// MCU type (STM32F10x, STM32F2xx, STM32F4xx, GD32F20x, GD32F30x)
#ifndef MCU_TYPE
  #define MCU_TYPE
  #include "stm32f10x.h"
#endif

// LCD resolution, font and icon size
#include "./Resolution/TFT_320X240.h"

// Update folder for fonts and icons
#ifndef UPDATE_DIR
  #define UPDATE_DIR "ARTILLERY"
#endif

// Hardware manufacturer
#ifndef HARDWARE_MANUFACTURER
  #define HARDWARE_MANUFACTURER "ARTILLERY_"
#endif

// Hardware version config
#ifndef HARDWARE_VERSION
  #define HARDWARE_VERSION "TFT_CONTROLLER_V1_2"
#endif

// Software manufacturer
#ifndef SOFTWARE_MANUFACTURER
  #define SOFTWARE_MANUFACTURER HARDWARE_VERSION"."
#endif

// XPT2046 Software SPI pins for touch screen
// It needs CS/SCK/MISO/MOSI for Software SPI and TPEN for pen interrupt
#define XPT2046_CS   PC9
#define XPT2046_SCK  PC10
#define XPT2046_MISO PC11
#define XPT2046_MOSI PC12
#define XPT2046_TPEN PC5

// W25Qxx SPI Flash Memory pins
#define W25Qxx_SPEED  1
#define W25Qxx_SPI    _SPI1
#define W25Qxx_CS_PIN PB9

// LCD interface
#define TFTLCD_DRIVER HX8558

// FSMC 8080 interface (high speed) or normal IO interface (low speed)
#ifndef STM32_HAS_FSMC
  //#define STM32_HAS_FSMC
#endif

// LCD data 16bit or 8bit
#ifndef LCD_DATA_16BIT
  #define LCD_DATA_16BIT 1
#endif

// SERIAL_PORT:   communicating with Klipper host
// SERIAL_PORT_X: communicating with other controllers (OctoPrint, ESP3D, other UART Touch Screen etc.)
#ifndef SERIAL_PORT
  #define SERIAL_PORT   _USART2  // default USART port
  #define USART2_TX_PIN PD5
  #define USART2_RX_PIN PD6
  #define USART3_TX_PIN PD8
  #define USART3_RX_PIN PD9
#endif

// Serial port for debugging
#ifdef SERIAL_DEBUG_ENABLED
  #define SERIAL_DEBUG_PORT SERIAL_PORT_3
#endif

// SD Card SPI pins
#ifndef SD_SPI_SUPPORT
  #define SD_SPI_SUPPORT
  #define SD_LOW_SPEED   7      // 2^(SPEED+1) = 256 frequency division
  #define SD_HIGH_SPEED  1      // 4 frequency division
  #define SD_SPI         _SPI1
  #define SD_CS_PIN      PD11
#endif

// SD Card CD Detect pin
#ifndef SD_CD_PIN
  #define SD_CD_PIN PB15
#endif

// USB Disk support
#ifndef USB_FLASH_DRIVE_SUPPORT
  #define USB_FLASH_DRIVE_SUPPORT
  #define USE_USB_OTG_FS
#endif

// Power Supply Auto Shutdown Detection pin
#ifndef PS_ON_PIN
  #define PS_ON_PIN PB1
#endif

// Filament Runout Detection pin
#ifndef FIL_RUNOUT_PIN
  #define FIL_RUNOUT_PIN PB0
#endif

// Buzzer PWM pin
#ifndef BUZZER_PIN
  #define BUZZER_PIN PA2
#endif

#endif

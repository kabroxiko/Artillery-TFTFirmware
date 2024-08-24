#ifndef _PIN_ARTILLERY_TFT_CONTROLLER_V1_2_H_  // modify to actual filename !!!
#define _PIN_ARTILLERY_TFT_CONTROLLER_V1_2_H_  // modify to actual filename !!!

// Update folder for fonts and icons
#ifndef UPDATE_DIR
  #define UPDATE_DIR "TFT28"
#endif

// Hardware manufacturer
#ifndef HARDWARE_MANUFACTURER
  #define HARDWARE_MANUFACTURER "ARTILLERY_"
#endif

// Hardware version config
#ifndef HARDWARE_VERSION
  #define HARDWARE_VERSION "TFT_CONTROLLER_V1_2"
#endif

#if !defined(ST7920_EMULATOR) || defined(SPI3_PIN_SMART_USAGE)
  // Power Supply Auto Shutdown Detection pin
  #ifndef PS_ON_PIN
    #define PS_ON_PIN PB1
  #endif

  // Filament Runout Detection pin
  #ifndef FIL_RUNOUT_PIN
    #define FIL_RUNOUT_PIN PB0
  #endif
#endif

#include "pin_MKS_TFT32_V1_4.h"

#endif

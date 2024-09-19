#include "BedLevelingLayer2.h"
#include "includes.h"

void menuBedLevelingLayer2(void)
{
  MENUITEMS bedLevelingLayer2Items = {
    // title
    LABEL_ABL_SETTINGS,
    // icon                          label
    {
      {ICON_LEVELING,                LABEL_START},
      {ICON_NULL,                    LABEL_NULL},
      {ICON_NULL,                    LABEL_NULL},
      {ICON_NULL,                    LABEL_NULL},
      {ICON_NULL,                    LABEL_NULL},
      {ICON_NULL,                    LABEL_NULL},
      {ICON_NULL,                    LABEL_NULL},
      {ICON_BACK,                    LABEL_BACK},
    }
  };

  KEY_VALUES key_num = KEY_IDLE;

  switch (infoMachineSettings.leveling)
  {
    case BL_MBL:
      bedLevelingLayer2Items.title.index = LABEL_MBL_SETTINGS;
      break;

    default:
      break;
  }

  if (infoMachineSettings.zProbe == ENABLED)
  {
    bedLevelingLayer2Items.items[3].icon = ICON_LEVEL_CORNER;
    bedLevelingLayer2Items.items[3].label.index = LABEL_LEVEL_CORNER;

    if (infoSettings.touchmi_sensor != 0)
    {
      bedLevelingLayer2Items.items[4].icon = ICON_NOZZLE;
      bedLevelingLayer2Items.items[4].label.index = LABEL_TOUCHMI;
    }
    else
    {
      bedLevelingLayer2Items.items[4].icon = ICON_BLTOUCH;
      bedLevelingLayer2Items.items[4].label.index = LABEL_BLTOUCH;
    }

    if (infoSettings.z_steppers_alignment != 0)
    {
      bedLevelingLayer2Items.items[5].icon = ICON_Z_ALIGN;
      bedLevelingLayer2Items.items[5].label.index = LABEL_Z_ALIGN;
    }
  }

  menuDrawPage(&bedLevelingLayer2Items);

  while (MENU_IS(menuBedLevelingLayer2))
  {
    key_num = menuKeyGetValue();

    switch (key_num)
    {
      case KEY_ICON_0:
        if (infoMachineSettings.leveling < BL_MBL)  // if ABL
        {
          ablStart();
        }
        else  // if MBL
        {
          OPEN_MENU(menuMBL);
        }
        break;

      case KEY_ICON_3:
        if (infoMachineSettings.zProbe == ENABLED)
          OPEN_MENU(menuLevelCorner);
        break;

      case KEY_ICON_4:
        if (infoMachineSettings.zProbe == ENABLED)
        {
          if (infoSettings.touchmi_sensor != 0)
            OPEN_MENU(menuTouchMi);
          else
            OPEN_MENU(menuBLTouch);
        }
        break;

      case KEY_ICON_5:
        if (infoMachineSettings.zProbe == ENABLED && infoSettings.z_steppers_alignment != 0)
        {
          storeCmd("G34\n");
          storeCmd("M18 S0 X Y Z\n");
        }
        break;

      case KEY_ICON_7:
        CLOSE_MENU();
        break;

      default:
        break;
    }

    loopProcess();
  }
}

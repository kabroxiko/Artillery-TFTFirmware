#include "BedLeveling.h"
#include "includes.h"

static void blUpdateState(MENUITEMS * menu, const uint8_t bedLevelState)
{
  if (bedLevelState == ENABLED)
  {
    menu->items[3].icon = ICON_LEVELING_ON;
    menu->items[3].label.index = LABEL_BL_ENABLE;
  }
  else
  {
    menu->items[3].icon = ICON_LEVELING_OFF;
    menu->items[3].label.index = LABEL_BL_DISABLE;
  }
}

void menuBedLeveling(void)
{
  MENUITEMS bedLevelingItems = {
    // title
    LABEL_ABL_SETTINGS,
    // icon                          label
    {
      {ICON_LEVELING,                LABEL_ABL},
      {ICON_MESH_EDITOR,             LABEL_MESH_EDITOR},
      {ICON_MESH_VALID,              LABEL_MESH_VALID},
      {ICON_NULL,                    LABEL_NULL},
      {ICON_NULL,                    LABEL_NULL},
      {ICON_NULL,                    LABEL_NULL},
      {ICON_HEAT_FAN,                LABEL_UNIFIEDHEAT},
      {ICON_BACK,                    LABEL_BACK},
    }
  };

  KEY_VALUES key_num = KEY_IDLE;
  uint8_t levelStateOld = UNDEFINED;
  uint8_t levelStateNew = UNDEFINED;

  switch (infoMachineSettings.leveling)
  {
    case BL_MBL:
      bedLevelingItems.title.index = LABEL_MBL_SETTINGS;
      bedLevelingItems.items[0].label.index = LABEL_MBL;
      break;

    default:
      break;
  }

  if (infoMachineSettings.zProbe == ENABLED)
  {
    bedLevelingItems.items[5].icon = ICON_PROBE_OFFSET;
    bedLevelingItems.items[5].label.index = LABEL_P_OFFSET;
  }

  menuDrawPage(&bedLevelingItems);

  while (MENU_IS(menuBedLeveling))
  {
    key_num = menuKeyGetValue();

    switch (key_num)
    {
      case KEY_ICON_0:
        OPEN_MENU(menuBedLevelingLayer2);
        break;

      case KEY_ICON_1:
        OPEN_MENU(menuMeshEditor);
        break;

      case KEY_ICON_2:
        OPEN_MENU(menuMeshValid);
        break;

      case KEY_ICON_3:
        if (levelStateNew != UNDEFINED)
          storeCmd((levelStateNew == ENABLED) ?
                   ("M420 S0\n") :
                   ("M420 S1\n"));

        break;

      case KEY_ICON_4:
      {
        if (levelStateNew != UNDEFINED)
        {
          float val = editFloatValue(Z_FADE_MIN_VALUE, Z_FADE_MAX_VALUE, 0.0f, getParameter(P_ABL_STATE, 1));

          if (val != getParameter(P_ABL_STATE, 1))
            storeCmd("M420 Z%.2f\n", val);

          menuDrawPage(&bedLevelingItems);
        }
        break;
      }

      case KEY_ICON_5:
        if (infoMachineSettings.zProbe == ENABLED)
        {
          storeCmd("M851\n");
          zOffsetSetMenu(true);  // use Probe Offset menu
          OPEN_MENU(menuZOffset);
        }
        break;

      case KEY_ICON_6:
        OPEN_MENU(menuUnifiedHeat);
        break;

      case KEY_ICON_7:
        COOLDOWN_TEMPERATURE();

        CLOSE_MENU();
        break;

      default:
        break;
    }

    if (levelStateNew != UNDEFINED)  // it's Marlin or RepRap firmware
    {
      levelStateNew = getParameter(P_ABL_STATE, 0);

      if (levelStateOld != levelStateNew)  // check for bed leveling On/Off change
      {
        levelStateOld = levelStateNew;

        blUpdateState(&bedLevelingItems, levelStateOld);  // update icon & label 3
        menuDrawItem(&bedLevelingItems.items[3], 3);
      }
    }

    loopProcess();
  }
}

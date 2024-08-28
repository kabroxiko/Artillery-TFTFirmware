#include "ABL.h"
#include "includes.h"

// called by parseAck() to notify ABL process status
void ablUpdateStatus(bool succeeded)
{
  bool savingEnabled = true;

  init_label(tempTitle);
  tempTitle.index = LABEL_ABL_SETTINGS;
  LABELCHAR(tempMsg, LABEL_BL_COMPLETE);

  switch (infoMachineSettings.leveling)
  {
    case BL_BBL:
      tempTitle.index = LABEL_ABL_SETTINGS_BBL;
      break;

    default:
      break;
  }

  if (succeeded)  // if bed leveling process successfully terminated, allow to save to EEPROM
  {
    BUZZER_PLAY(SOUND_SUCCESS);

    if (savingEnabled && infoMachineSettings.EEPROM == 1)
    {
      sprintf(strchr(tempMsg, '\0'), "\n %s", textSelect(LABEL_EEPROM_SAVE_INFO));

      popupDialog(DIALOG_TYPE_SUCCESS, tempTitle.index, (uint8_t *) tempMsg, LABEL_CONFIRM, LABEL_CANCEL, saveEepromSettings, NULL, NULL);
    }
    else
    {
      popupReminder(DIALOG_TYPE_SUCCESS, tempTitle.index, (uint8_t *) tempMsg);
    }
  }
  else  // if bed leveling process failed, provide an error dialog
  {
    BUZZER_PLAY(SOUND_ERROR);

    popupReminder(DIALOG_TYPE_ERROR, tempTitle.index, LABEL_PROCESS_ABORTED);
  }
}

// start ABL process
void ablStart(void)
{
  storeCmd("G28\n");

  switch (infoMachineSettings.leveling)
  {
    case BL_BBL:  // if Bilinear Bed Leveling
      storeCmd("BED_MESH_CALIBRATE PROFILE=default\n");
      break;

    default:  // if any other Auto Bed Leveling
      storeCmd("BED_MESH_CALIBRATE PROFILE=default\n");
      break;
  }

  if (infoMachineSettings.firmwareType != FW_REPRAPFW)
    storeCmd("M118 P0 ABL Completed\n");
}

#include "StatusScreen.h"
#include "includes.h"

#define KEY_SPEEDMENU         KEY_ICON_3
#define KEY_MAINMENU          (KEY_SPEEDMENU + 1)
#define SET_SPEEDMENUINDEX(x)

#define XYZ_STATUS "   X: %.2f   Y: %.2f   Z: %.2f   "

#define TOOL_TOGGLE_TIME 2000  // 1 seconds is 1000

// text position rectangles for Live icons
static const GUI_POINT ss_title_point   = {SS_ICON_WIDTH - BYTE_WIDTH / 2, SS_ICON_NAME_Y0};
static const GUI_POINT ss_val_point     = {SS_ICON_WIDTH / 2, SS_ICON_VAL_Y0};

// info box msg area
const  GUI_RECT msgRect = {START_X + 1 * ICON_WIDTH + 1 * SPACE_X + 2, ICON_START_Y + 1 * ICON_HEIGHT + 1 * SPACE_Y + STATUS_MSG_BODY_YOFFSET,
                           START_X + 3 * ICON_WIDTH + 2 * SPACE_X - 2, ICON_START_Y + 2 * ICON_HEIGHT + 1 * SPACE_Y - STATUS_MSG_BODY_BOTTOM};

const GUI_RECT recGantry = {START_X,                                SS_ICON_HEIGHT + ICON_START_Y + STATUS_GANTRY_YOFFSET,
                            START_X + 4 * ICON_WIDTH + 3 * SPACE_X, ICON_HEIGHT + SPACE_Y + ICON_START_Y - STATUS_GANTRY_YOFFSET};

static const MENUITEMS statusItems = {
  // title
  LABEL_READY,
  // icon                          label
  {
    {ICON_STATUS_NOZZLE,           LABEL_NULL},
    {ICON_STATUS_BED,              LABEL_NULL},
    {ICON_STATUS_FAN,              LABEL_NULL},
    {ICON_STATUS_SPEED,            LABEL_NULL},
    {ICON_MAINMENU,                LABEL_MAINMENU},
    {ICON_NULL,                    LABEL_NULL},
    {ICON_NULL,                    LABEL_NULL},
    {ICON_PRINT,                   LABEL_PRINT},
  }
};

static const uint8_t bedIcons[2]     = {ICON_STATUS_BED, ICON_STATUS_CHAMBER};
static const uint8_t speedIcons[2] = {ICON_STATUS_SPEED, ICON_STATUS_FLOW};
static const char * const speedID[2] = SPEED_ID;

static int8_t lastConnectionStatus = -1;
static bool msgNeedRefresh = false;

static char msgTitle[20];
static char msgBody[MAX_MSG_LENGTH];

void statusSetMsg(const uint8_t * title, const uint8_t * msg)
{
  strncpy_no_pad(msgTitle, (char *)title, sizeof(msgTitle));
  strncpy_no_pad(msgBody, (char *)msg, sizeof(msgBody));
  msgNeedRefresh = true;
}

void statusSetReady(void)
{
  strncpy_no_pad(msgTitle, (char *)textSelect(LABEL_STATUS), sizeof(msgTitle));

  if (infoHost.connected == false)
    strncpy_no_pad(msgBody, (char *)textSelect(LABEL_UNCONNECTED), sizeof(msgBody));
  else
    snprintf(msgBody, sizeof(msgBody), "%s %s", machine_type, (char *)textSelect(LABEL_READY));

  msgNeedRefresh = true;
}

static void statusDraw(void)
{
  // icons and their values are updated one by one to reduce flicker/clipping
  char tempstr[45];
  LIVE_INFO lvIcon;

  lvIcon.enabled[0] = true;
  lvIcon.lines[0].h_align = RIGHT;
  lvIcon.lines[0].v_align = TOP;
  lvIcon.lines[0].pos = ss_title_point;
  lvIcon.lines[0].font = SS_ICON_TITLE_FONT_SIZE;
  lvIcon.lines[0].fn_color = SS_NAME_COLOR;
  lvIcon.lines[0].text_mode = GUI_TEXTMODE_TRANS;  // default value

  lvIcon.enabled[1] = true;
  lvIcon.lines[1].h_align = CENTER;
  lvIcon.lines[1].v_align = CENTER;
  lvIcon.lines[1].pos = ss_val_point;
  lvIcon.lines[1].font = SS_ICON_VAL_FONT_SIZE;
  lvIcon.lines[1].fn_color = SS_VAL_COLOR;
  lvIcon.lines[1].text_mode = GUI_TEXTMODE_TRANS;  // default value

  lvIcon.enabled[2] = false;

  // TOOL / EXT
  lvIcon.iconIndex = ICON_STATUS_NOZZLE;
  lvIcon.lines[0].text = (uint8_t *)heatShortID[currentTool];
  sprintf(tempstr, "%3d/%-3d", heatGetCurrentTemp(currentTool), heatGetTargetTemp(currentTool));
  lvIcon.lines[1].text = (uint8_t *)tempstr;

  showLiveInfo(0, &lvIcon, false);

  // BED
  lvIcon.iconIndex = bedIcons[currentBCIndex];
  lvIcon.lines[0].text = (uint8_t *)heatShortID[BED + currentBCIndex];
  sprintf(tempstr, "%3d/%-3d", heatGetCurrentTemp(BED + currentBCIndex), heatGetTargetTemp(BED + currentBCIndex));
  lvIcon.lines[1].text = (uint8_t *)tempstr;

  showLiveInfo(1, &lvIcon, infoSettings.chamber_en == 1);

  // FAN
  lvIcon.iconIndex = ICON_STATUS_FAN;
  lvIcon.lines[0].text = (uint8_t *)fanID[currentFan];

  if (infoSettings.fan_percentage == 1)
    sprintf(tempstr, "%3d%%", fanGetCurPercent(currentFan));
  else
    sprintf(tempstr, "%3d", fanGetCurSpeed(currentFan));

  lvIcon.lines[1].text = (uint8_t *)tempstr;

  showLiveInfo(2, &lvIcon, false);

  // SPEED / FLOW
  lvIcon.iconIndex = speedIcons[currentSpeedID];
  lvIcon.lines[0].text = (uint8_t *)speedID[currentSpeedID];
  sprintf(tempstr, "%3d%%", speedGetCurPercent(currentSpeedID));
  lvIcon.lines[1].text = (uint8_t *)tempstr;

  showLiveInfo(3, &lvIcon, true);

  sprintf(tempstr, XYZ_STATUS, coordinateGetAxisActual(X_AXIS), coordinateGetAxisActual(Y_AXIS), coordinateGetAxisActual(Z_AXIS));

  GUI_SetTextMode(GUI_TEXTMODE_NORMAL);
  GUI_SetColor(GANTRY_XYZ_FONT_COLOR);
  GUI_SetBkColor(GANTRY_XYZ_BG_COLOR);
  GUI_DispStringInPrect(&recGantry, (uint8_t *)tempstr);

  GUI_RestoreColorDefault();
}

static void statusDrawMsg(void)
{
  GUI_SetTextMode(GUI_TEXTMODE_TRANS);

  IMAGE_ReadDisplay(rect_of_keySS[KEY_INFOBOX].x0, rect_of_keySS[KEY_INFOBOX].y0, INFOBOX_ADDR);
  GUI_SetColor(INFOMSG_BG_COLOR);
  GUI_DispString(rect_of_keySS[KEY_INFOBOX].x0 + STATUS_MSG_ICON_XOFFSET,
                 rect_of_keySS[KEY_INFOBOX].y0 + STATUS_MSG_ICON_YOFFSET,
                 IconCharSelect(CHARICON_INFO));

  GUI_DispString(rect_of_keySS[KEY_INFOBOX].x0 + BYTE_HEIGHT + STATUS_MSG_TITLE_XOFFSET,
                 rect_of_keySS[KEY_INFOBOX].y0 + STATUS_MSG_ICON_YOFFSET,
                 (uint8_t *)msgTitle);

  GUI_SetBkColor(INFOMSG_BG_COLOR);
  GUI_FillPrect(&msgRect);
  Scroll_CreatePara(&scrollLine, (uint8_t *)msgBody, &msgRect);
  GUI_RestoreColorDefault();

  msgNeedRefresh = false;
}

static inline void statusScrollMsg(void)
{
  GUI_SetBkColor(INFOMSG_BG_COLOR);
  GUI_SetColor(INFOMSG_FONT_COLOR);
  Scroll_DispString(&scrollLine, CENTER);
  GUI_RestoreColorDefault();
}

static inline void statusToggleTool(void)
{
  if (nextScreenUpdate(TOOL_TOGGLE_TIME))
  {
    // increment hotend index
    if (infoSettings.hotend_count > 1)
      currentTool = (currentTool + 1) % infoSettings.hotend_count;

    // switch bed/chamber index
    if (infoSettings.chamber_en == 1)
      TOGGLE_BIT(currentBCIndex, 0);

    // increment fan index
    if ((infoSettings.fan_count + infoSettings.ctrl_fan_en) > 1)
    {
      do
      {
        currentFan = (currentFan + 1) % MAX_FAN_COUNT;
      } while (!fanIsValid(currentFan));
    }

    // switch speed/flow
    TOGGLE_BIT(currentSpeedID, 0);

    statusDraw();

    speedGetCurPercent(0);
    speedGetCurPercent(1);

    ctrlFanQuery();
  }
}

void menuStatus(void)
{
  KEY_VALUES key_num = KEY_IDLE;

  GUI_SetBkColor(infoSettings.bg_color);
  menuDrawPage(&statusItems);
  GUI_SetColor(GANTRY_XYZ_BG_COLOR);
  GUI_FillPrect(&recGantry);

  statusDraw();
  statusDrawMsg();

  while (MENU_IS(menuStatus))
  {
    if (infoHost.connected != lastConnectionStatus)
    {
      statusSetReady();
      lastConnectionStatus = infoHost.connected;
    }

    if (msgNeedRefresh)
      statusDrawMsg();

    statusScrollMsg();

    key_num = menuKeyGetValue();

    switch (key_num)
    {
      case KEY_ICON_0:
        heatSetCurrentIndex(LAST_NOZZLE);  // preselect last selected nozzle for "Heat" menu

        OPEN_MENU(menuHeat);
        break;

      case KEY_ICON_1:
        heatSetCurrentIndex(BED);  // preselect the bed for "Heat" menu

        OPEN_MENU(menuHeat);
        break;

      case KEY_ICON_2:
        OPEN_MENU(menuFan);
        break;

      case KEY_SPEEDMENU:
        SET_SPEEDMENUINDEX(0);
        OPEN_MENU(menuSpeed);
        break;

      case KEY_MAINMENU:
        OPEN_MENU(menuMain);
        break;

      case KEY_ICON_7:
        OPEN_MENU(menuPrint);
        break;

      case KEY_INFOBOX:
        OPEN_MENU(menuNotification);
      default:
        break;
    }

    statusToggleTool();

    loopProcess();
  }
}

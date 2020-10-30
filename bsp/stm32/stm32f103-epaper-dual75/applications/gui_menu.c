#include <stdint.h>
#include "GUI.h"
#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"

#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"
#include "WIDGET.h"




static uint8_t menu_item = 1;
#define X_OFFSET  0
#define Y_OFFSET  2
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    
    { FRAMEWIN_CreateIndirect,  "Caption",           0,                        0,  0,  128,56, 0,0},
 
    { BUTTON_CreateIndirect,    "abc",           GUI_ID_BUTTON0,          X_OFFSET +7,  Y_OFFSET +0,  54, 15, 0,0},
    { BUTTON_CreateIndirect,    "def",           GUI_ID_BUTTON1,          X_OFFSET +67, Y_OFFSET +0,  54, 15, 0,0},
    { BUTTON_CreateIndirect,    "hgi",           GUI_ID_BUTTON2,          X_OFFSET +7,  Y_OFFSET +16, 54, 15, 0,0},
    { BUTTON_CreateIndirect,    "jkl",           GUI_ID_BUTTON3,          X_OFFSET +67, Y_OFFSET +16, 54, 15, 0,0},
    { BUTTON_CreateIndirect,    "mlp",           GUI_ID_BUTTON4,          X_OFFSET +7,  Y_OFFSET +32, 54, 15, 0,0},
    { BUTTON_CreateIndirect,    "xyz",           GUI_ID_BUTTON5,          X_OFFSET +67, Y_OFFSET +32, 54, 15, 0,0}
};
WM_HWIN active_dialog;
static I8 r = 0, l = 0;
static const I16 ButtonMatrix[3][2] = {
	GUI_ID_BUTTON0, GUI_ID_BUTTON1,
	GUI_ID_BUTTON2, GUI_ID_BUTTON3,
	GUI_ID_BUTTON4, GUI_ID_BUTTON5
};
extern void start_time_set(WM_HWIN hWin,void (*on_leave)(WM_HWIN hWin),void* param);
extern void start_sensor_task_info(WM_HWIN hWin,void (*on_leave)(WM_HWIN hWin),void* param);
extern void start_system_info(WM_HWIN hWin,void (*on_leave)(WM_HWIN hWin),void* param);
extern void start_history_data(WM_HWIN hWin,void (*on_leave)(WM_HWIN hWin),void* param);
extern void start_query_history_data(WM_HWIN hWin,void (*on_leave)(WM_HWIN hWin),void* param);
extern void start_system_config_set(WM_HWIN hWin,void (*on_leave)(WM_HWIN hWin),void* param);
static void on_sub_leave(WM_HWIN hWin)
{
    active_dialog = hWin;
    WM_SetFocus(WM_GetClientWindow(hWin));
}

static void start_sensor_task(WM_HWIN hWin,void (*on_leave)(WM_HWIN hWin),void* param)
{

  	
}
typedef struct {
    short id;
    void(*start_menu)(WM_HWIN hWin,void(*on_leave)(WM_HWIN hWin), void* param);
    void(*on_leave)(WM_HWIN hWin);
}MENU_START_ROUTINE;
static const MENU_START_ROUTINE routines[] = {
#if defined(SINGLE_TASK) || defined (SINGLE_BUTTON)
  {GUI_ID_BUTTON0,NULL,on_sub_leave},
#else
  {GUI_ID_BUTTON0,NULL,on_sub_leave},
#endif
  {GUI_ID_BUTTON1,NULL,on_sub_leave},
  {GUI_ID_BUTTON2,NULL,on_sub_leave},
  {GUI_ID_BUTTON3,NULL,on_sub_leave},
  {GUI_ID_BUTTON4,NULL,on_sub_leave},
  {GUI_ID_BUTTON5,NULL,on_sub_leave},
};
static void start_sub_menu(WM_HWIN hWin)
{

  uint8_t i = 0;
  for(i = 0;i< GUI_COUNTOF(routines) ;i ++)
  {
    if(WM_GetFocussedWindow() == WM_GetDialogItem(hWin,routines[i].id))
    {
        if(routines[i].start_menu != NULL)
        {
            routines[i].start_menu(hWin,on_sub_leave,0);
        }
    }
  }
}
static void PaintDialog(WM_MESSAGE * pMsg)
{
}
static void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;
    //
    //FRAMEWIN
    //
    FRAMEWIN_SetTitleVis(hWin,0);

    WM_SetFocus(WM_GetDialogItem(hWin,_aDialogCreate[menu_item].Id));
}

static uint8_t GetMenuItem(WM_HWIN hWin)
{
	uint8_t i;
	for(i = 0; i < GUI_COUNTOF(routines) ;i++)
	{
		if(WM_GetFocussedWindow() == WM_GetDialogItem(hWin,routines[i].id))
		{
			i++;
			break;
		}
	}
	
	return i;
}

static void _cbCallback(WM_MESSAGE * pMsg) 
{
  
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId) 
    {
        case WM_PAINT:
            PaintDialog(pMsg);
            break;
        case WM_INIT_DIALOG:
            InitDialog(pMsg);
            break;
        case WM_KEY:
            switch (((WM_KEY_INFO*)(pMsg->Data.p))->Key) 
            {
                case GUI_KEY_ESCAPE:
                    //GUI_EndDialog(hWin,0);
                    break;

                case GUI_KEY_ENTER:
                    start_sub_menu(hWin);
                    
                    break;
#if defined(SINGLE_TASK) || defined (SINGLE_BUTTON)
				case GUI_KEY_END:
				  	if(WM_GetFocussedWindow() == WM_GetDialogItem(hWin,GUI_ID_BUTTON0))
					{
						start_sensor_task_set(hWin,botton0_on_sub_leave,"fixed.tsk");
					}
			  		break;
#endif
				case GUI_KEY_UP:
					menu_item = GetMenuItem(hWin);
					r = (menu_item - 1) / 2;
					l = (menu_item - 1) % 2;
					r -= 1;
					r = (r < 0) ? 2 : r;
					WM_SetFocus(WM_GetDialogItem(hWin,ButtonMatrix[r][l]));
					menu_item = r * 2 + l + 1;
					break;
				case GUI_KEY_DOWN:
					menu_item = GetMenuItem(hWin);
					r = (menu_item - 1) / 2;
					l = (menu_item - 1) % 2;
					r += 1;
					r %= 3;
					WM_SetFocus(WM_GetDialogItem(hWin,ButtonMatrix[r][l]));
					menu_item = r * 2 + l + 1;
					break;
				case GUI_KEY_LEFT:
					menu_item = GetMenuItem(hWin);
					r = (menu_item - 1) / 2;
					l = (menu_item - 1) % 2;
					l -= 1;
					l = (l < 0) ? 1 : l;
					WM_SetFocus(WM_GetDialogItem(hWin,ButtonMatrix[r][l]));
					menu_item = r * 2 + l + 1;
					break;
				case GUI_KEY_RIGHT:
					menu_item = GetMenuItem(hWin);
					r = (menu_item - 1) / 2;
					l = (menu_item - 1) % 2;
					l += 1;
					l %= 2;
					WM_SetFocus(WM_GetDialogItem(hWin,ButtonMatrix[r][l]));
					menu_item = r * 2 + l + 1;
					break;
            }
            break;
        case WM_NOTIFY_PARENT:
           
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}

void start_main_menu()
{
    active_dialog = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);

}
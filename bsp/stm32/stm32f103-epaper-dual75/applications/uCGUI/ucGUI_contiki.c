#include "contiki.h"
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
#include "key.h"
#include "gui_define.h"
#include "sensor_task.h"

PROCESS(ucgui_process, "ucgui_process");

WM_HWIN active_dialog = 0;


static void pollhandler()
{
    process_poll(&ucgui_process);
    GUI_Exec();
}

void update_gui()
{
	process_post(&ucgui_process,PROCESS_EVENT_POLL,NULL);
}
extern void start_main_menu();
extern int start_logo();
PROCESS_THREAD(ucgui_process, ev, data)
{
  	static struct etimer et;
    //PROCESS_POLLHANDLER(pollhandler());
	PROCESS_BEGIN();
    GUI_Init();
    GUI_UC_SetEncodeGBK();
    //GUI_SetFont(&GUI_Font13_2);
    GUI_SetFont(&GUI_Font13_HZ);
    //TEXT_SetDefaultFont(&GUI_Font13_2);
    TEXT_SetDefaultFont(&GUI_Font13_HZ);
    //BUTTON_SetDefaultFont(&GUI_Font13_2);
    BUTTON_SetDefaultFont(&GUI_Font13_HZ);
    //LISTBOX_SetDefaultFont(&GUI_Font13_2);
    LISTBOX_SetDefaultFont(&GUI_Font13_HZ);
    //FRAMEWIN_SetDefaultFont(&GUI_Font13_2);
    FRAMEWIN_SetDefaultFont(&GUI_Font13_HZ);
    WM_SetDesktopColor(GUI_WHITE);      /* Automacally update desktop window */

    WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
    WIDGET_SetDefaultEffect_Simple();
    //WIDGET_SetDefaultEffect_3D();
    //whDialog = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), &_cbCallback, 0, 0, 0);
    if(start_logo())
	{
		etimer_set(&et, 2 * CLOCK_SECOND);
  		
	}else
	{
		etimer_set(&et, CLOCK_SECOND/2);
	}
	PROCESS_WAIT_UNTIL(etimer_expired(&et));
	end_logo();
	start_status_bar_process();
	check_usb_debug();
	
#ifdef SINGLE_BUTTON
	start_collect_result_listbox("fixed.tsk","","");
	start_sensor_task();
#else
    start_main_menu();
#endif
    //process_poll(&ucgui_process);
	GUI_Exec();
    while(1)
    {
        GUI_SendKeyMsg(GUI_KEY_HOME,1);
        PROCESS_WAIT_EVENT();
        if(ev == key_event)
        {
            key_event_data_t * ked = (key_event_data_t *) data;
            if(ked->key_code == KEY_ENTER)
            {
                if(ked->press_type == KEY_LONG_PRESS)
                {
                    GUI_SendKeyMsg(GUI_KEY_END,1);
                }
                else
                {
                    GUI_SendKeyMsg(GUI_KEY_ENTER,1);
                }
            }
            else if(ked->key_code == KEY_CANCEL)
            {
                if(ked->press_type == KEY_LONG_PRESS)
                {
                    GUI_SendKeyMsg(GUI_KEY_BACKSPACE,1);
                }
                else
                {
                    GUI_SendKeyMsg(GUI_KEY_ESCAPE,1);
                }
            }
            else if(ked->key_code == KEY_UP)
            {
                    GUI_SendKeyMsg(GUI_KEY_UP,1);
            }
            else if(ked->key_code == KEY_DOWN)
            {
                    GUI_SendKeyMsg(GUI_KEY_DOWN,1);
            }
			else if(ked->key_code == KEY_LEFT)
            {
                    GUI_SendKeyMsg(GUI_KEY_LEFT,1);
            }
			else if(ked->key_code == KEY_RIGHT)
            {
                    GUI_SendKeyMsg(GUI_KEY_RIGHT,1);
            }
            else if(ked->key_code == KEY_TAB)
            {
                    GUI_SendKeyMsg(GUI_KEY_TAB,1);
            }
            //GUI_SendKeyMsg(int Key, int PressedCnt);
        }
		GUI_Exec();
    }
	PROCESS_END();
}
void start_ucgui_process()
{
    process_start(&ucgui_process,NULL);
}
    
   
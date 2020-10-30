#define UCGUI_STACK_SIZE 2048
#define UCGUI_PRIORITY	10

#include <rtthread.h>
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
#include "epaper_dvr.h"

void ucgui_draw_string(uint8_t* str,uint16_t x,uint16_t y, GUI_FONT * font)
{
	GUI_Init();
    GUI_UC_SetEncodeGBK();
	
	GUI_CONST_STORAGE GUI_FONT * dFont = &GUI_Font13_HZ;
	if(font != NULL)
	{
		dFont = font;
	}
	GUI_SetFont(dFont);
	GUI_DispStringAt(str,x,y);

	GUI_Exec();
}

void ucgui_draw(void *parameter)
{

	  GUI_Init();
    GUI_UC_SetEncodeGBK();
//    GUI_SetFont(&GUI_Font24_ASCII);
    GUI_SetFont(&GUI_Font13_HZ);
    //TEXT_SetDefaultFont(&GUI_Font13_2);
//    TEXT_SetDefaultFont(&GUI_Font13_HZ);
    //BUTTON_SetDefaultFont(&GUI_Font13_2);
//    BUTTON_SetDefaultFont(&GUI_Font13_HZ);
    //LISTBOX_SetDefaultFont(&GUI_Font13_2);
//    LISTBOX_SetDefaultFont(&GUI_Font13_HZ);
    //FRAMEWIN_SetDefaultFont(&GUI_Font13_2);
//    FRAMEWIN_SetDefaultFont(&GUI_Font13_HZ);
//    WM_SetDesktopColor(GUI_WHITE);      /* Automacally update desktop window */ 
//
//    WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
//    WIDGET_SetDefaultEffect_Simple();
	GUI_SetColor(GUI_WHITE);
	GUI_FillRect(0,0,400,90);
	GUI_FillRect(500,300,600,360);
	GUI_SetBkColor(GUI_WHITE);
	
	GUI_SetColor(GUI_RED);
	GUI_DispStringAt("你好, 这是一块电子墨水屏！ 红色",10,20);
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_Font24_ASCII);
	GUI_DispStringAt("Welcome (~_~) ! Black",20,50);
	GUI_Exec();
	
	

}

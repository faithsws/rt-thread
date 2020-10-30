/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : F13_1.c
Purpose     : ISO 8859-1 West European Character Set similar to Swiss
Height      : 13
---------------------------END-OF-HEADER------------------------------
*/

#include "GUI_FontIntern.h"
//#include "w25q256.h"

#define	FONT_START_ADDRESS	(824*1024 + 0x08000000)
static unsigned char char_buf[26] = {0};
static GUI_CHARINFO char_info;
static const GUI_CHARINFO GUI_UNI_PTR * GetCharInfo(const struct GUI_FONT_PROP * prop, U16P c)
{
	unsigned short qu, wei;
	qu = ((c & 0xff00) >> 8) - 0xa0;
	wei = (c & 0x00ff) - 0xa0;
	unsigned int offset = ((qu - 1) * 94 + (wei - 1)) * 24;
//	w25x_read(char_buf + 2, FONT_START_ADDRESS + offset,24);
	
	memcpy(char_buf + 2,FONT_START_ADDRESS + offset,24);
	
	char_info.XSize = 12;
	char_info.XDist = 12;
	char_info.BytesPerLine = 2;
	char_info.pData = char_buf;
	
    return &char_info;
}

extern GUI_CONST_STORAGE GUI_FONT_PROP GUI_Font13H_ASCII_Prop1;
GUI_CONST_STORAGE GUI_FONT_PROP GUI_Font13_HZ_Prop1 = {
   0xa0a0                         /* first character               */
  ,65535                         /* last character                */
  ,0 /* address of first character    */
  ,&GUI_Font13H_ASCII_Prop1                         /* pointer to next GUI_FONT_PROP */
  ,GetCharInfo
};

GUI_CONST_STORAGE GUI_FONT GUI_Font13_HZ = {
   GUI_FONTTYPE_PROP /* type of font    */
  ,13                /* height of font  */
  ,13                /* space of font y */
  ,1                /* magnification x */
  ,1                /* magnification y */
  ,{&GUI_Font13_HZ_Prop1}
  , 11, 6, 8
};


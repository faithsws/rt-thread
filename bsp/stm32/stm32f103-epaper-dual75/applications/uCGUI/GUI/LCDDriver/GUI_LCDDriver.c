#include "LCD_Private.h"      /* private modul definitions & config */
#include "GUI_Private.h"
#include "GUIDebug.h"
#include <string.h>
#include <rtthread.h>
#include "epaper_dvr.h"
#define	BKCOLOR LCD_BKCOLORINDEX
#define	COLOR LCD_COLORINDEX

extern void epaper_io_init();

extern uint8_t lcd_buffer[640 * 384 / 4]; //显示缓冲区， 每个点2bit

int LCD_L0_Init(void)
{  
//	epaper_io_init();
//	rt_kprintf("%s\r\n",__FUNCTION__);
  	return 0;
}

void LCD_L0_SetPixelIndex(int x, int y, int PixelIndex)
{
//	rt_kprintf("%s\r\n",__FUNCTION__);
}

unsigned int LCD_L0_GetPixelIndex(int x, int y)
{
//	rt_kprintf("%s\r\n",__FUNCTION__);
  return 1;
}

void LCD_L0_SetOrg(int x,int y)
{
//	rt_kprintf("%s\r\n",__FUNCTION__);
}

void LCD_L0_XorPixel(int x, int y)
{
// 	rt_kprintf("%s\r\n",__FUNCTION__);
}

void LCD_L0_DrawHLine  (int x0, int y,  int x1)
{
//  	rt_kprintf("%s\r\n",__FUNCTION__);
}

void LCD_L0_DrawVLine  (int x, int y0,  int y1)
{
//  rt_kprintf("%s\r\n",__FUNCTION__);
}

void set_pixel_wb(int x, int y, LCD_PIXELINDEX color)
{
//    rt_kprintf("%02x ",color);
	if(color== 0)
	{
		lcd_buffer[x/4 + y*640/4] &= ~(0x03 << ((x%4)*2)); //黑色
	}
	else if(color == 2)
	{
		lcd_buffer[x/4 + y*640/4] |= (0x02 << ((x%4)*2)); //红色
	}
	else
	{
		lcd_buffer[x/4 + y*640/4] |= (0x01 << ((x%4)*2));	//白色
	}
}
void LCD_L0_FillRect(int x0, int y0, int x1, int y1) 
{
//	rt_kprintf("%s x0=%d,y0=%d,x1=%d,y1=%d\r\n",__FUNCTION__,x0,y0,x1,y1);
	uint8_t c = 0;
	if(	GUI_Context.Color == GUI_BLACK)
	{
		c = 0;
	}
	else if(GUI_Context.Color == GUI_WHITE)
	{
		c = 1;
	}
	else
	{
		c = 2;
	}
	for(int y = y0; y < y1; y++)
	{
		for(int x = x0; x < x1; x++)
		{
			set_pixel_wb(x,y,c);
		}
	}
	
	
}



//void LCD_L0_DrawBitmap   (int x0, int y0,
//                       int xsize, int ysize,
//                       int BitsPerPixel, 
//                       int BytesPerLine,
//                       const U8* pData, int Diff,
//                       const LCD_PIXELINDEX* pTrans)
//{
//  rt_kprintf("%s x=%d,y=%d,xs=%d,ys=%d, bitsP=%d,bytesLine=%d,diff=%d, pTrans=0x%02x\r\n",__FUNCTION__,
//			 x0,y0,xsize,ysize,BitsPerPixel,BytesPerLine,Diff,pTrans
//			 
//			 );
//  if(BitsPerPixel == 8)
//  {
//	  for(int y = y0; y < y0+ ysize; y++)
//	  {
//		for(int x = x0; x < x0 + xsize; x ++)
//		{
//			rt_kprintf("%02x ",*pData);
//			if(*pData == 1)
//			{
//				lcd_buffer[x/4 + y*640/4] &= ~(0x03 << ((x%4)*2));
//			}
//			else if(*pData == 0)
//			{
//				lcd_buffer[x/4 + y*640/4] |= (0x01 << ((x%4)*2));
//			}
//			else
//			{
//				lcd_buffer[x/4 + y*640/4] |= (0x02 << ((x%4)*2));
//				rt_kprintf("%02x ",*pData);
//			}
//			pData ++;	
//		}
//		pData += BytesPerLine;
//	  }
//  
//  }else if(BitsPerPixel == 1)
//  {
//  	for(int y = y0; y < y0+ ysize; y++)
//	  {
//		for(int x = x0; x < x0 + xsize; x ++)
//		{
//			rt_kprintf("%02x ",*pData);
//			
//			uint8_t val = *pData 
//			
//			for(int i = 0;i< 8;i++)
//			{
//				if(*pData & (0x01 << i))
//				{
//					lcd_buffer[x/4 + y*640/4] &= ~(0x03 << ((x%4)*2));
//				}
//			
//			}
//			
//
//			pData ++;	
//		}
//		pData += BytesPerLine;
//	  }
//  
//  }
//  rt_kprintf("\r\n",*pData);
//}


void DrawBitLine1BPP(int x, int y, U8 const*p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans)
{
  LCD_PIXELINDEX pixels;
  LCD_PIXELINDEX Index0 = *(pTrans+0);
  LCD_PIXELINDEX Index1 = *(pTrans+1);
/*
// Jump to right entry point
*/
  pixels = *p;
  switch (GUI_Context.DrawMode & (LCD_DRAWMODE_TRANS|LCD_DRAWMODE_XOR)) {
  case 0:
    #if defined (SETNEXTPIXEL)   /* Optimization ! */
      x+=Diff;
      ili9320_SetCursor(x,y);
    #endif
    switch (Diff&7) {
    case 0:   
      goto WriteBit0;
    case 1:   
      goto WriteBit1;
    case 2:
      goto WriteBit2;
    case 3:
      goto WriteBit3;
    case 4:
      goto WriteBit4;
    case 5:   
      goto WriteBit5;
    case 6:   
      goto WriteBit6;
    case 7:   
      goto WriteBit7;
    }
    break;
  case LCD_DRAWMODE_TRANS:
    switch (Diff&7) {
    case 0:
      goto WriteTBit0;
    case 1:
      goto WriteTBit1;
    case 2:
      goto WriteTBit2;
    case 3:
      goto WriteTBit3;
    case 4:
      goto WriteTBit4;
    case 5:   
      goto WriteTBit5;
    case 6:   
      goto WriteTBit6;
    case 7:   
      goto WriteTBit7;
    }
    break;
  case LCD_DRAWMODE_XOR:
    switch (Diff&7) {
    case 0:   
      goto WriteXBit0;
    case 1:   
      goto WriteXBit1;
    case 2:
      goto WriteXBit2;
    case 3:
      goto WriteXBit3;
    case 4:
      goto WriteXBit4;
    case 5:   
      goto WriteXBit5;
    case 6:   
      goto WriteXBit6;
    case 7:   
      goto WriteXBit7;
    }
  }
/*
        Write with transparency
*/

  WriteTBit0:
   if (pixels&(1<<7)) set_pixel_wb(x+0, y, Index1);
    if (!--xsize)
      return;
  WriteTBit1:
    if (pixels&(1<<6)) set_pixel_wb(x+1, y, Index1);
    if (!--xsize)
      return;
  WriteTBit2:
    if (pixels&(1<<5)) set_pixel_wb(x+2, y, Index1);
    if (!--xsize)
      return;
  WriteTBit3:
    if (pixels&(1<<4)) set_pixel_wb(x+3, y, Index1);
    if (!--xsize)
      return;
  WriteTBit4:
    if (pixels&(1<<3)) set_pixel_wb(x+4, y, Index1);
    if (!--xsize)
      return;
  WriteTBit5:
    if (pixels&(1<<2)) set_pixel_wb(x+5, y, Index1);
    if (!--xsize)
      return;
  WriteTBit6:
    if (pixels&(1<<1)) set_pixel_wb(x+6, y, Index1);
    if (!--xsize)
      return;
  WriteTBit7:
    if (pixels&(1<<0)) set_pixel_wb(x+7, y, Index1);
    if (!--xsize)
      return;
    x+=8;
    pixels = *(++p);
    goto WriteTBit0;

/*
        Write without transparency
*/

  WriteBit0:
    set_pixel_wb(x+0, y, (pixels&(1<<7)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit1:
    set_pixel_wb(x+1, y, (pixels&(1<<6)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit2:
    set_pixel_wb(x+2, y, (pixels&(1<<5)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit3:
    set_pixel_wb(x+3, y, (pixels&(1<<4)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit4:
    set_pixel_wb(x+4, y, (pixels&(1<<3)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit5:
    set_pixel_wb(x+5, y, (pixels&(1<<2)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit6:
    set_pixel_wb(x+6, y, (pixels&(1<<1)) ? Index1 : Index0);
    if (!--xsize)
      return;
  WriteBit7:
    set_pixel_wb(x+7, y, (pixels&(1<<0)) ? Index1 : Index0);
    if (!--xsize)
      return;
    x+=8;
    pixels = *(++p);
    goto WriteBit0;

/*
        Write XOR mode
*/

  WriteXBit0:
    if (pixels&(1<<7))
      LCD_L0_XorPixel(x+0, y);
    if (!--xsize)
      return;
  WriteXBit1:
    if (pixels&(1<<6))
      LCD_L0_XorPixel(x+1, y);
    if (!--xsize)
      return;
  WriteXBit2:
    if (pixels&(1<<5))
      LCD_L0_XorPixel(x+2, y);
    if (!--xsize)
      return;
  WriteXBit3:
    if (pixels&(1<<4))
      LCD_L0_XorPixel(x+3, y);
    if (!--xsize)
      return;
  WriteXBit4:
    if (pixels&(1<<3))
      LCD_L0_XorPixel(x+4, y);
    if (!--xsize)
      return;
  WriteXBit5:
    if (pixels&(1<<2))
      LCD_L0_XorPixel(x+5, y);
    if (!--xsize)
      return;
  WriteXBit6:
    if (pixels&(1<<1))
      LCD_L0_XorPixel(x+6, y);
    if (!--xsize)
      return;
  WriteXBit7:
    if (pixels&(1<<0))
      LCD_L0_XorPixel(x+7, y);
    if (!--xsize)
      return;
    x+=8;
    pixels = *(++p);
    goto WriteXBit0;
	
}

static void  DrawBitLine2BPP(int x, int y, U8 const*p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans) {
  LCD_PIXELINDEX pixels;
/*
// Jump to right entry point
*/
  pixels = *p;
  if (pTrans) {
    /*
      with palette
    */
    if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) switch (Diff&3) {
    case 0:
      goto WriteTBit0;
    case 1:
      goto WriteTBit1;
    case 2:
      goto WriteTBit2;
    default:
      goto WriteTBit3;
    } else switch (Diff&3) {
    case 0:
      goto WriteBit0;
    case 1:
      goto WriteBit1;
    case 2:
      goto WriteBit2;
    default:
      goto WriteBit3;
    }
  /*
          Write without transparency
  */
  WriteBit0:
    set_pixel_wb(x+0, y, *(pTrans+(pixels>>6)));
    if (!--xsize)
      return;
  WriteBit1:
    set_pixel_wb(x+1, y, *(pTrans+(3&(pixels>>4))));
    if (!--xsize)
      return;
  WriteBit2:
    set_pixel_wb(x+2, y, *(pTrans+(3&(pixels>>2))));
    if (!--xsize)
      return;
  WriteBit3:
    set_pixel_wb(x+3, y, *(pTrans+(3&(pixels))));
    if (!--xsize)
      return;
    pixels = *(++p);
    x+=4;
    goto WriteBit0;
  /*
          Write with transparency
  */
  WriteTBit0:
    if (pixels&(3<<6))
      set_pixel_wb(x+0, y, *(pTrans+(pixels>>6)));
    if (!--xsize)
      return;
  WriteTBit1:
    if (pixels&(3<<4))
      set_pixel_wb(x+1, y, *(pTrans+(3&(pixels>>4))));
    if (!--xsize)
      return;
  WriteTBit2:
    if (pixels&(3<<2))
      set_pixel_wb(x+2, y, *(pTrans+(3&(pixels>>2))));
    if (!--xsize)
      return;
  WriteTBit3:
    if (pixels&(3<<0))
      set_pixel_wb(x+3, y, *(pTrans+(3&(pixels))));
    if (!--xsize)
      return;
    pixels = *(++p);
    x+=4;
    goto WriteTBit0;
  } else { 
    /* 
      without palette 
    */
    if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) switch (Diff&3) {
    case 0:
      goto WriteDDPTBit0;
    case 1:
      goto WriteDDPTBit1;
    case 2:
      goto WriteDDPTBit2;
    default:
      goto WriteDDPTBit3;
    } else switch (Diff&3) {
    case 0:
      goto WriteDDPBit0;
    case 1:
      goto WriteDDPBit1;
    case 2:
      goto WriteDDPBit2;
    default:
      goto WriteDDPBit3;
    }
  /*
          Write without transparency
  */
  WriteDDPBit0:
    set_pixel_wb(x+0, y, (pixels>>6));
    if (!--xsize)
      return;
  WriteDDPBit1:
    set_pixel_wb(x+1, y, (3&(pixels>>4)));
    if (!--xsize)
      return;
  WriteDDPBit2:
    set_pixel_wb(x+2, y, (3&(pixels>>2)));
    if (!--xsize)
      return;
  WriteDDPBit3:
    set_pixel_wb(x+3, y, (3&(pixels)));
    if (!--xsize)
      return;
    pixels = *(++p);
    x+=4;
    goto WriteDDPBit0;
  /*
          Write with transparency
  */
  WriteDDPTBit0:
    if (pixels&(3<<6))
      set_pixel_wb(x+0, y, (pixels>>6));
    if (!--xsize)
      return;
  WriteDDPTBit1:
    if (pixels&(3<<4))
      set_pixel_wb(x+1, y, (3&(pixels>>4)));
    if (!--xsize)
      return;
  WriteDDPTBit2:
    if (pixels&(3<<2))
      set_pixel_wb(x+2, y, (3&(pixels>>2)));
    if (!--xsize)
      return;
  WriteDDPTBit3:
    if (pixels&(3<<0))
      set_pixel_wb(x+3, y, (3&(pixels)));
    if (!--xsize)
      return;
    pixels = *(++p);
    x+=4;
    goto WriteDDPTBit0;
  }
}

static void  DrawBitLine4BPP(int x, int y, U8 const*p, int Diff, int xsize, const LCD_PIXELINDEX*pTrans)
{
  LCD_PIXELINDEX pixels;

  pixels = *p;
  if (pTrans)
  {
    if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)
    {
      if ((Diff&1) ==0)
        goto WriteTBit0;
        goto WriteTBit1;
    }
    else
    {
      if ((Diff&1) ==0)
        goto WriteBit0;
        goto WriteBit1;
    }

  WriteBit0:
    set_pixel_wb(x+0, y, *(pTrans+(pixels>>4)));
    if (!--xsize)
      return;
  WriteBit1:
    set_pixel_wb(x+1, y, *(pTrans+(pixels&0xf)));
    if (!--xsize)
      return;
    x+=2;
    pixels = *(++p);
    goto WriteBit0;
  /*
          Write with transparency
  */
  WriteTBit0:
    if (pixels>>4)
      set_pixel_wb(x+0, y, *(pTrans+(pixels>>4)));
    if (!--xsize)
      return;
  WriteTBit1:
    if (pixels&0xf)
      set_pixel_wb(x+1, y, *(pTrans+(pixels&0xf)));
    if (!--xsize)
      return;
    x+=2;
    pixels = *(++p);
    goto WriteTBit0;
  } else {
    /*
      without palette
    */
    if (GUI_Context.DrawMode & LCD_DRAWMODE_TRANS) {
      if ((Diff&1) ==0)
        goto WriteDDPTBit0;
      goto WriteDDPTBit1;
    } else {
      if ((Diff&1) ==0)
        goto WriteDDPBit0;
      goto WriteDDPBit1;
    }
  /*
          Write without transparency
  */
  WriteDDPBit0:
    set_pixel_wb(x+0, y, (pixels>>4));
    if (!--xsize)
      return;
  WriteDDPBit1:
    set_pixel_wb(x+1, y, (pixels&0xf));
    if (!--xsize)
      return;
    x+=2;
    pixels = *(++p);
    goto WriteDDPBit0;
  /*
          Write with transparency
  */
  WriteDDPTBit0:
    if (pixels>>4)
      set_pixel_wb(x+0, y, (pixels>>4));
    if (!--xsize)
      return;
  WriteDDPTBit1:
    if (pixels&0xf)
      set_pixel_wb(x+1, y, (pixels&0xf));
    if (!--xsize)
      return;
    x+=2;
    pixels = *(++p);
    goto WriteDDPTBit0;
  }
}

void DrawBitLine8BPP(int x, int y, U8 const*p, int xsize, const LCD_PIXELINDEX*pTrans) {
  LCD_PIXELINDEX pixel;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)==0) {
    if (pTrans) {
      for (;xsize > 0; xsize--,x++,p++) {
        pixel = *p;
        set_pixel_wb(x, y, *(pTrans+pixel));
      }
    } else {
      for (;xsize > 0; xsize--,x++,p++) {
        set_pixel_wb(x, y, *p);
      }
    }
  } else {   /* Handle transparent bitmap */
    if (pTrans) {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          set_pixel_wb(x+0, y, *(pTrans+pixel));
        }
      }
    } else {
      for (; xsize > 0; xsize--, x++, p++) {
        pixel = *p;
        if (pixel) {
          set_pixel_wb(x+0, y, pixel);
        }
      }
    }
  }
}

void DrawBitLine16BPP(int x, int y, U16 const*p, int xsize)
{
  LCD_PIXELINDEX Index;
  if ((GUI_Context.DrawMode & LCD_DRAWMODE_TRANS)==0)
  {
    for (;xsize > 0; xsize--,x++,p++)
    {
      set_pixel_wb(x, y, *p);
    }
  }
  else
  {   /* Handle transparent bitmap */
    for (; xsize > 0; xsize--, x++, p++)
    {
      Index = *p;
      if (Index)
      {
        set_pixel_wb(x+0, y, Index);
      }
    }
  }
}

void LCD_L0_DrawBitmap   (int x0, int y0,
                       int xsize, int ysize,
                       int BitsPerPixel, 
                       int BytesPerLine,
                       const U8* pData, int Diff,
                       const LCD_PIXELINDEX* pTrans)
{
//	  rt_kprintf("%s x=%d,y=%d,xs=%d,ys=%d, bitsP=%d,bytesLine=%d,diff=%d, pTrans=0x%02x\r\n",__FUNCTION__,
//			 x0,y0,xsize,ysize,BitsPerPixel,BytesPerLine,Diff,pTrans
//			 
//			 );
  int i;
  switch (BitsPerPixel)
  {
  case 1:
    for (i=0; i<ysize; i++)
    {
      DrawBitLine1BPP(x0, i+y0, pData, Diff, xsize, pTrans);
      pData += BytesPerLine;
    }
    break;
  case 2:
    for (i=0; i<ysize; i++)
    {
      DrawBitLine2BPP(x0, i+y0, pData, Diff, xsize, pTrans);
      pData += BytesPerLine;
    }
    break;
  case 4:
    for (i=0; i<ysize; i++)
    {
      DrawBitLine4BPP(x0, i+y0, pData, Diff, xsize, pTrans);
      pData += BytesPerLine;
    }
    break;
  case 8:

    for (i=0; i<ysize; i++)
    {
      DrawBitLine8BPP(x0, i+y0, pData, xsize, pTrans);
      pData += BytesPerLine;
    }

    break;
  case 16:
    for (i=0; i<ysize; i++)
    {
      DrawBitLine16BPP(x0, i+y0, (U16*)pData, xsize);
      pData += BytesPerLine;
    }
    break;
  }
}













void LCD_L0_SetLUTEntry(U8 Pos, LCD_COLOR color){
//rt_kprintf("%s\r\n",__FUNCTION__);
}
void LCD_L0_On(void)
{
//	rt_kprintf("%s\r\n",__FUNCTION__);

	memset(lcd_buffer,0x00,sizeof(lcd_buffer));
}

extern uint32_t epaper_attr;
unsigned LCD_Color2Index_User(LCD_COLOR Color) 
{
	if(Color == GUI_BLACK)
	{
		return 0;
	}else if(Color == GUI_WHITE)
	{
		return 1;
	}else
	{
		if(epaper_attr & ATR_COLOR_3)
		{
			return 2;
		}
		else
		{
			return 1;
		}
	}
}
LCD_COLOR LCD_Index2Color_User(int Index) 
{
	if(Index == 0)return GUI_BLACK;
	if(Index == 1)return GUI_WHITE;
	
	if(epaper_attr & ATR_COLOR_3)
	{
		return GUI_RED;
	}else
	{
		return GUI_WHITE;
	}
}
unsigned LCD_GetIndexMask_User()
{
	return 0x03;
}



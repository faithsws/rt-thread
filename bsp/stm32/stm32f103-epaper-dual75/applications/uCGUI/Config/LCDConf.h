#ifndef LCDCONF_H
#define LCDCONF_H

#define LCD_XSIZE          		(640)
#define LCD_YSIZE          		(384)

#define LCD_XSIZE_1          		(640)
#define LCD_YSIZE_1          		(384)

#define LCD_CONTROLLER     		(9320)
#define LCD_BITSPERPIXEL   		(1)
#define LCD_BITSPERPIXEL_1		(1)
#define LCD_FIXEDPALETTE		(-1)
#define LCD_FIXEDPALETTE_1 		(1)
#define LCD_SWAP_RB				(1)
//#define LCD_SWAP_XY        (1)
#define LCD_INIT_CONTROLLER()  ili9320_Initializtion();

#endif /* LCDCONF_H */


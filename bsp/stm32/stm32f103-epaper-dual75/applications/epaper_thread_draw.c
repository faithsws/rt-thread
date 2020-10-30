#include "epaper_dvr.h"

#include <rtthread.h>
#define EPAPER_STACK_SIZE 1024
#define EPAPER_PRIORITY		30
static void draw_thread_entry(void *parameter)
{
	struct epaper_dvr_t * epaper = (struct epaper_dvr_t *)parameter;
	epaper_draw(epaper);
}

int epaper_thread_draw(struct epaper_dvr_t * epaper)
{
	rt_thread_t tid;
	rt_err_t result = RT_EOK;
	tid = rt_thread_create("epaper_thread",
                           draw_thread_entry, epaper,
                           EPAPER_STACK_SIZE, EPAPER_PRIORITY, 10);
	
	if (tid != NULL && result == RT_EOK)
	{
        rt_thread_startup(tid);
		return 0;
	}
    else
	{
		return -1;
	}
}

#if 0

#if defined(RT_USING_FINSH) && defined(FINSH_USING_MSH)
#include <finsh.h>
static void epaper_color(uint8_t argc, char **argv)
{
	if(argc != 3)
	{
		rt_kprintf("Format error \n");
		return;
	}
	if( argv[1][0] == '0')
	{
		epaper_select(&epaper_front);		
	}
	else if( argv[1][0] == '1')
	{
		epaper_select(&epaper_rear);		
	}
	else
	{
		rt_kprintf("Index error \n");
		return;
	}
	if( argv[2][0] == '0')
	{
		fill_buffer(0x00);
	}else if( argv[2][0] == '1') 
	{
		fill_buffer(0x55);
	}else if( argv[2][0] == '2')
	{
		fill_buffer(0xaa);
	}else
	{
		rt_kprintf("Color error \n");
		return;
	}
	if( argv[1][0] == '0')
	{
		epaper_thread_draw(&epaper_front);		
	}
	else if( argv[1][0] == '1')
	{
		epaper_thread_draw(&epaper_rear);		
	}
	else
	{
		rt_kprintf("Index error \n");
		return;
	}
	
}
MSH_CMD_EXPORT(epaper_color, epaper fill:  epaper_color 0/1 0/1/2);


static void epaper_qr(uint8_t argc, char **argv)
{
	if(argc != 3)
	{
		rt_kprintf("Format error \n");
		return;
	}
	if( argv[1][0] == '0')
	{
		epaper_select(&epaper_front);		
	}
	else if( argv[1][0] == '1')
	{
		epaper_select(&epaper_rear);		
	}
	else
	{
		rt_kprintf("Index error \n");
		return;
	}
	fill_buffer(0x55);
	gui_draw_qr_at(argv[2],3,100,100);
	
	
	if( argv[1][0] == '0')
	{
		epaper_thread_draw(&epaper_front);		
	}
	else if( argv[1][0] == '1')
	{
		epaper_thread_draw(&epaper_rear);		
	}
	else
	{
		rt_kprintf("Index error \n");
		return;
	}
	
}
MSH_CMD_EXPORT(epaper_qr, epaper qr:  epaper_qr 0/1 string);
#include "GUI.h"
static void epaper_str(uint8_t argc, char **argv)
{
	if(argc != 3)
	{
		rt_kprintf("Format error \n");
		return;
	}
	if( argv[1][0] == '0')
	{
		epaper_select(&epaper_front);		
	}
	else if( argv[1][0] == '1')
	{
		epaper_select(&epaper_rear);		
	}
	else
	{
		rt_kprintf("Index error \n");
		return;
	}
	fill_buffer(0x55);
	extern void ucgui_draw_string(uint8_t* str,uint16_t x,uint16_t y, GUI_FONT * font);
	ucgui_draw_string(argv[2],10,10,NULL);
	
	if( argv[1][0] == '0')
	{
		epaper_thread_draw(&epaper_front);		
	}
	else if( argv[1][0] == '1')
	{
		epaper_thread_draw(&epaper_rear);		
	}
	else
	{
		rt_kprintf("Index error \n");
		return;
	}
	
}
MSH_CMD_EXPORT(epaper_str, epaper str:  epaper_str 0/1 string);
#endif

#endif
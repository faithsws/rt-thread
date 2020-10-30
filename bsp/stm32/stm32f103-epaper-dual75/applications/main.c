/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-03-08     obito0   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "key_button.h"
#include "spi_flash_sfud.h"
#include "epaper_dvr.h"
#include <fal.h>
DMA_HandleTypeDef hdma_usart3_rx;

/* defined the LED0 pin: PC13 */
#define LEDR_PIN    GET_PIN(B, 14)
#define LEDG_PIN    GET_PIN(B, 13)
#define LEDB_PIN    GET_PIN(B, 12)

#define MODEM_POWER_PIN	GET_PIN(A, 1)
	 
#define POWER_HOLD_PIN	GET_PIN(C, 6)

#define DTU_RX_PIN	GET_PIN(A, 2)
#define DTU_TX_PIN	GET_PIN(A, 3)
#define KEY_PIN	GET_PIN(B, 15)

#define TEST_BTN
//#define TEST_EPAPER
#define TEST_BLINK


#ifdef TEST_BTN
Button_t Button1;
static rt_timer_t           keyTimer         = RT_NULL;
static void key_tmr(void *p)
{
	Button_Process();

}
#define PRINT_INFO rt_kprintf
void Btn1_Dowm_CallBack(void *btn)
{
  PRINT_INFO("Button1 Click!\n");
}

void Btn1_Double_CallBack(void *btn)
{
  PRINT_INFO("Button1 Double click!\n");
}

void Btn1_Long_CallBack(void *btn)
{
  PRINT_INFO("Button1 Long press!\n");
  rt_kprintf("power down!!! \n");
  
  rt_pin_write(POWER_HOLD_PIN, PIN_HIGH);
}


rt_uint8_t read_btn1(void)
{
	return rt_pin_read(KEY_PIN);
}
#endif


#ifdef TEST_BLINK
#include "blink_all.h"
#endif

int main(void)
{
    /* set LED0 pin mode to output */

	fal_init();
	
#ifdef TEST_EPAPER
	epaper_select(&epaper_front);
	ucgui_draw();
	gui_draw_qr_at("https://faithsws.github.io",5,100,100);

	epaper_thread_draw(&epaper_front);
	epaper_thread_draw(&epaper_rear);
#endif
	
#ifdef TEST_BTN
	rt_pin_mode(KEY_PIN, PIN_MODE_INPUT_PULLUP);
	Button_Create("Button1",
              &Button1, 
              read_btn1, 
              0);
	  Button_Attach(&Button1,BUTTON_DOWM,Btn1_Dowm_CallBack);                       //Click
	  Button_Attach(&Button1,BUTTON_DOUBLE,Btn1_Double_CallBack);                   //Double click
	  Button_Attach(&Button1,BUTTON_LONG,Btn1_Long_CallBack);  
	
	
	if ((keyTimer = rt_timer_create(
            "key_tmr",
            key_tmr,
            RT_NULL,
            20,
            RT_TIMER_FLAG_PERIODIC)) == RT_NULL)
        {
            rt_kprintf("key timer: Create timer failed!\n");
        }
	rt_timer_start(keyTimer);
#endif
  
#ifdef TEST_BLINK
	extern rt_err_t blink_regisger_gpio(const char* name,rt_uint32_t interval,rt_uint32_t repeat_cnt, rt_base_t pin, int on_level);
	
	blink_regisger_gpio("led1",500,BLINK_REP_FOREVER,LEDR_PIN,PIN_LOW);
	blink_regisger_gpio("led2",BLINK_ON_FOVEVER,0,LEDG_PIN,PIN_LOW);
//	blink_regisger_gpio("led3",200,20,LEDB_PIN,PIN_LOW);
	extern rt_err_t blink_device_start(const char* name);
	blink_device_start("led1");
	blink_device_start("led2");
//	blink_device_start("led3");
#endif
	
	while (1)
    {

//        rt_pin_write(LEDR_PIN, PIN_LOW);
        rt_thread_mdelay(500);
//		rt_pin_write(LEDR_PIN, PIN_HIGH);
//        rt_thread_mdelay(500);
		

//        rt_pin_write(LEDG_PIN, PIN_LOW);
//        rt_thread_mdelay(500);
//		rt_pin_write(LEDG_PIN, PIN_HIGH);
//        rt_thread_mdelay(500);
//		
//
//        rt_pin_write(LEDB_PIN, PIN_LOW);
//        rt_thread_mdelay(500);
//		rt_pin_write(LEDB_PIN, PIN_HIGH);
//        rt_thread_mdelay(500);
    }

    return RT_EOK;
}

static void mpower(uint8_t argc, char **argv)
{
	rt_pin_mode(MODEM_POWER_PIN, PIN_MODE_OUTPUT);
	
	rt_pin_write(MODEM_POWER_PIN, PIN_HIGH);
	rt_thread_mdelay(500);
	rt_pin_write(MODEM_POWER_PIN, PIN_LOW);
}
MSH_CMD_EXPORT(mpower, epaper str:  epaper_str 0/1 string);

static int config_led()
{
	rt_pin_mode(LEDR_PIN, PIN_MODE_OUTPUT);
	
//	rt_pin_mode(LEDG_PIN, PIN_MODE_OUTPUT);
	rt_pin_mode(LEDB_PIN, PIN_MODE_OUTPUT);
	rt_pin_write(LEDR_PIN, PIN_LOW);
	return 0;
}
INIT_DEVICE_EXPORT(config_led);
int power_on()
{
		/* 保持开机上电状态 */
	rt_pin_mode(POWER_HOLD_PIN, PIN_MODE_OUTPUT_OD);
	rt_pin_write(POWER_HOLD_PIN, PIN_LOW);
	
	return 0;
}

INIT_BOARD_EXPORT(power_on);


static int rt_hw_spi_flash_init(void)
{
	extern rt_err_t rt_emu_spi_device_attach(const char *bus_name, const char *device_name, rt_base_t cs_pin);
    rt_emu_spi_device_attach("emu_spi", "spi20", GET_PIN(A, 10));

    if (RT_NULL == rt_sfud_flash_probe("W25Q256", "spi20"))
    {
        return -RT_ERROR;
    };

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_spi_flash_init);
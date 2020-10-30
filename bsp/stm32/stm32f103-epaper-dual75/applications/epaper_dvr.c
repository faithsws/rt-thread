#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <string.h>
#include "epaper_dvr.h"


#define EP_SW_Pin    GET_PIN(C, 0)
#define EP2_SDI_Pin    GET_PIN(B, 0)
#define EP2_SCK_Pin    GET_PIN(C, 5)
#define EP2_CS_Pin    GET_PIN(C, 4)
#define EP2_DC_Pin    GET_PIN(A, 7)
#define EP2_RST_Pin    GET_PIN(A, 6)
#define EP2_BS_Pin    GET_PIN(A, 4)
#define EP2_BUSY_Pin    GET_PIN(A, 5)
#define EP2_CSB_Pin    GET_PIN(B, 1)

#define EP1_SDI_Pin    GET_PIN(B, 9)
#define EP1_SCK_Pin    GET_PIN(B, 8)
#define EP1_CS_Pin    GET_PIN(B, 7)
#define EP1_DC_Pin    GET_PIN(B, 6)
#define EP1_RST_Pin    GET_PIN(B, 5)
#define EP1_BS_Pin    GET_PIN(B, 3)
#define EP1_CSB_Pin    GET_PIN(C, 13)
#define EP1_BUSY_Pin    GET_PIN(B, 4)

uint32_t epaper_attr = 0;

static void delay_us(uint16_t us)
{
	//rt_hw_us_delay(us);
}

static uint8_t epaper_io_inited = 0;
void epaper_io_init()
{
//	if(epaper_io_inited) return;
//	epaper_io_inited = 1;
	rt_pin_mode(EP2_SDI_Pin    , PIN_MODE_OUTPUT);
	rt_pin_mode(EP2_SCK_Pin    , PIN_MODE_OUTPUT);
	rt_pin_mode(EP2_CS_Pin     , PIN_MODE_OUTPUT);
	rt_pin_mode(EP2_DC_Pin     , PIN_MODE_OUTPUT);
	rt_pin_mode(EP2_RST_Pin    , PIN_MODE_OUTPUT);
	rt_pin_mode(EP2_BS_Pin     , PIN_MODE_OUTPUT);
	rt_pin_mode(EP2_CSB_Pin    , PIN_MODE_INPUT);
	rt_pin_mode(EP2_BUSY_Pin   , PIN_MODE_INPUT_PULLDOWN);
	
	
	rt_pin_write(EP2_BS_Pin, PIN_LOW);
	rt_pin_write(EP2_CS_Pin, PIN_HIGH);
	rt_pin_write(EP2_RST_Pin, PIN_HIGH);
	
	rt_pin_mode(EP1_SDI_Pin    , PIN_MODE_OUTPUT);
	rt_pin_mode(EP1_SCK_Pin    , PIN_MODE_OUTPUT);
	rt_pin_mode(EP1_CS_Pin     , PIN_MODE_OUTPUT);
	rt_pin_mode(EP1_DC_Pin     , PIN_MODE_OUTPUT);
	rt_pin_mode(EP1_RST_Pin    , PIN_MODE_OUTPUT);
	rt_pin_mode(EP1_BS_Pin     , PIN_MODE_OUTPUT);
	rt_pin_mode(EP1_CSB_Pin    , PIN_MODE_INPUT);
	rt_pin_mode(EP1_BUSY_Pin   , PIN_MODE_INPUT_PULLDOWN);
	
	rt_pin_write(EP1_BS_Pin, PIN_LOW);
	
	rt_pin_write(EP1_CS_Pin, PIN_HIGH);
	rt_pin_write(EP1_RST_Pin, PIN_HIGH);
//	rt_thread_mdelay(100);
}
static uint8_t epaper_sw_inited = 0;
static uint8_t epaper_drawing_ref = 0;
void epaper_power_sw(uint8_t on)
{
	if(!epaper_sw_inited)
	{
		rt_pin_mode(EP_SW_Pin, PIN_MODE_OUTPUT);
		epaper_sw_inited = 1;
	}
	
	if(on)
	{
		epaper_drawing_ref ++;
		rt_pin_write(EP_SW_Pin, PIN_LOW);
	}
	else
	{
		epaper_drawing_ref --;
		if(epaper_drawing_ref == 0)
		{
			rt_pin_write(EP_SW_Pin, PIN_HIGH);
		}
	}
	rt_thread_mdelay(200);
}

void epaper1_spi_write_byte(uint8_t TxData)
{				   			 
	uint8_t temp_data;
	uint8_t scnt;
	temp_data = TxData;
	
	
	rt_pin_write(EP1_SCK_Pin, PIN_LOW);
	for(scnt=0;scnt<8;scnt++)
	{ 
		if(temp_data&0x80)
		{
			rt_pin_write(EP1_SDI_Pin, PIN_HIGH);
		}
		else
		{
			rt_pin_write(EP1_SDI_Pin, PIN_LOW);
		}
		rt_pin_write(EP1_SCK_Pin, PIN_HIGH);
		
		delay_us(5) ; 
		rt_pin_write(EP1_SCK_Pin, PIN_LOW);
		temp_data = temp_data<<1;
		delay_us(5) ; 
	}
}
/*
* 函数名：HalLcd_READBUSY
* 描述  ：等待电子纸busy信号由高变低 
* 输入  ：无
* 输出  ：无
*/	
int epaper1_busy(void)
{
	uint32_t try_cnt = 10000000;
	while(try_cnt > 0)
	{	
		if(rt_pin_read(EP1_BUSY_Pin) != 0) return 1;
		delay_us(5) ;
		
		try_cnt --;
	}  
	return 0;
}

/*
* 函数名：epaper1_write_cmd
* 描述  ：写命令
* 输入  ：cmd
* 输出  ：无
*/	
void epaper1_write_cmd(uint8_t cmd)
{
	rt_pin_write(EP1_CS_Pin, PIN_HIGH);
	rt_pin_write(EP1_CS_Pin, PIN_LOW);
	
	rt_pin_write(EP1_DC_Pin, PIN_LOW); // D/C#   0:command  1:data
	
	delay_us(5);
	epaper1_spi_write_byte(cmd);
	delay_us(5) ;
	rt_pin_write(EP1_CS_Pin, PIN_HIGH);
}


/*
* 函数名：epaper1_write_cmd
* 描述  ：写数据
* 输入  ：cmd
* 输出  ：无
*/	
void epaper1_write_data(uint8_t data)
{
	rt_pin_write(EP1_CS_Pin, PIN_HIGH);
	rt_pin_write(EP1_CS_Pin, PIN_LOW);
	
	rt_pin_write(EP1_DC_Pin, PIN_HIGH); // D/C#   0:command  1:data
	delay_us(5) ;
	epaper1_spi_write_byte(data);
	delay_us(5) ;
	rt_pin_write(EP1_CS_Pin, PIN_HIGH);
}


void epaper2_spi_write_byte(uint8_t TxData)
{				   			 
	uint8_t temp_data;
	uint8_t scnt;
	temp_data = TxData;
	
	
	rt_pin_write(EP2_SCK_Pin, PIN_LOW);
	for(scnt = 0; scnt < 8; scnt ++)
	{ 
		if(temp_data&0x80)
		{
			rt_pin_write(EP2_SDI_Pin, PIN_HIGH);
		}
		else
		{
			rt_pin_write(EP2_SDI_Pin, PIN_LOW);
		}
		rt_pin_write(EP2_SCK_Pin, PIN_HIGH);
		
		delay_us(5) ; 
		rt_pin_write(EP2_SCK_Pin, PIN_LOW);
		temp_data = temp_data<<1;
		delay_us(5) ; 
	}
}
/*
* 函数名：HalLcd_READBUSY
* 描述  ：等待电子纸busy信号由高变低 
* 输入  ：无
* 输出  ：无
*/	
int epaper2_busy(void)
{
	uint32_t try_cnt = 10000000;
	while(try_cnt > 0)
	{	
		if(rt_pin_read(EP2_BUSY_Pin) != 0) return 1;
		delay_us(5) ;
		
		try_cnt --;
	}  
	return 0;
}

/*
* 函数名：epaper1_write_cmd
* 描述  ：写命令
* 输入  ：cmd
* 输出  ：无
*/	
void epaper2_write_cmd(uint8_t cmd)
{
	rt_pin_write(EP2_CS_Pin, PIN_HIGH);
	rt_pin_write(EP2_CS_Pin, PIN_LOW);
	
	rt_pin_write(EP2_DC_Pin, PIN_LOW); // D/C#   0:command  1:data
	
	delay_us(5);
	epaper2_spi_write_byte(cmd);
	delay_us(5) ;
	rt_pin_write(EP2_CS_Pin, PIN_HIGH);
}


/*
* 函数名：epaper1_write_cmd
* 描述  ：写数据
* 输入  ：cmd
* 输出  ：无
*/	
void epaper2_write_data(uint8_t data)
{
	rt_pin_write(EP2_CS_Pin, PIN_HIGH);
	rt_pin_write(EP2_CS_Pin, PIN_LOW);
	
	rt_pin_write(EP2_DC_Pin, PIN_HIGH); // D/C#   0:command  1:data
	delay_us(5) ;
	epaper2_spi_write_byte(data);
	delay_us(5) ;
	rt_pin_write(EP2_CS_Pin, PIN_HIGH);
}
static void epaper2_resset()
{
	rt_thread_mdelay(150);
	epaper_io_init();
	
	rt_pin_write(EP2_RST_Pin, PIN_LOW);
	
	rt_thread_mdelay(20); 
	rt_pin_write(EP2_RST_Pin, PIN_HIGH); //hard reset 
	rt_thread_mdelay(100);  
}









static void epaper1_resset()
{
	rt_thread_mdelay(150);
	epaper_io_init();
	
	rt_pin_write(EP1_RST_Pin, PIN_LOW);
	
	rt_thread_mdelay(20); 
	rt_pin_write(EP1_RST_Pin, PIN_HIGH); //hard reset 
	rt_thread_mdelay(100);  
}

struct epaper_dvr_t epaper_front = {
	.name = "front-3color",
	.attribute = ATR_COLOR_3,
	.ops = {
		.write_cmd = epaper1_write_cmd,
		.write_data = epaper1_write_data,
		.wait_busy = epaper1_busy,
		.reset = epaper1_resset,
	},
};

struct epaper_dvr_t epaper_rear = {
	.name = "rear-2color",
	.attribute = ATR_COLOR_2,
	.ops = {
		.write_cmd = epaper2_write_cmd,
		.write_data = epaper2_write_data,
		.wait_busy = epaper2_busy,
		.reset = epaper2_resset,
	},
};
/*
* 函数名：Epaper_Update
* 描述  ：电子纸初始化程序，给电子纸发送命令及数据
* 输入  ：wave_data
* 输出  ：无
*/	
static void epaper_update(struct epaper_ops_t * ops)
{
	ops->write_cmd(0x12);        //display      
	ops->wait_busy();			
	ops->write_cmd(0x65);
	ops->write_data(0x01);
	
	ops->write_cmd(0x65);
	ops->write_data(0x00); 
	ops->wait_busy();	
	ops->write_cmd(0x02);        //power off 
	rt_thread_mdelay(60);
	ops->wait_busy(); 
	ops->write_cmd(0x07); //deep sleep
	ops->write_data(0xa5);  
	rt_thread_mdelay(500); 
}


uint8_t lcd_buffer[640 * 384 / 4] = {0}; //显示缓冲区， 每个点2bit
void epaper_clear_buffer()
{
	memset(lcd_buffer,0,sizeof(lcd_buffer));
}

static struct epaper_ops_t * last_ops = NULL;

static void epaper_draw_buffer(struct epaper_dvr_t * epaper)
{
	
	struct epaper_ops_t * ops = &epaper->ops;
	last_ops = ops;
	ops->wait_busy();	     
	ops->write_cmd(0x10);	
	for(int i = 0; i < 640*384/4; i++)
	{
		uint8_t val = 0;
		val = 0;
		if((lcd_buffer[i] & 0x03) == 0) //black
		{
			
		}else if((lcd_buffer[i] & 0x03) == 1) //white
		{
			val |= 0x03 << 4;
		}
		else
		{
			val |= 0x04 << 4;
		}
		
		if(((lcd_buffer[i] & 0x0c)>>2) == 0) //black
		{
			
		}else if(((lcd_buffer[i] & 0x0c)>>2) == 1) //white
		{
			val |= 0x03 ;
		}
		else
		{
			val |= 0x04;
		}
		
		ops->write_data(val);	  
		
		val = 0;
		if(((lcd_buffer[i] & 0x30)>>4) == 0) //black
		{
			
		}else if(((lcd_buffer[i] & 0x30)>>4) == 1) //white
		{
			val |= 0x03 << 4;
		}
		else
		{
			val |= 0x04 << 4;
		}
		
		if(((lcd_buffer[i] & 0xc0)>>6) == 0) //black
		{
			
		}else if(((lcd_buffer[i] & 0xc0)>>6) == 1) //white
		{
			val |= 0x03;
		}
		else
		{
			val |= 0x04;
		}
		
		ops->write_data(val);	 
	}

}

void epaper_draw_buffer_on_last()
{
	struct epaper_ops_t * ops = last_ops;
	if(ops == NULL) return;
	ops->wait_busy();	     
	ops->write_cmd(0x10);	
	for(int i = 0; i < 640*384/4; i++)
	{
		uint8_t val = 0;
		val = 0;
		if((lcd_buffer[i] & 0x03) == 0) //black
		{
			
		}else if((lcd_buffer[i] & 0x03) == 1) //white
		{
			val |= 0x03 << 4;
		}
		else
		{
			val |= 0x04 << 4;
		}
		
		if(((lcd_buffer[i] & 0x0c)>>2) == 0) //black
		{
			
		}else if(((lcd_buffer[i] & 0x0c)>>2) == 1) //white
		{
			val |= 0x03 ;
		}
		else
		{
			val |= 0x04;
		}
		
		ops->write_data(val);	  
		
		val = 0;
		if(((lcd_buffer[i] & 0x30)>>4) == 0) //black
		{
			
		}else if(((lcd_buffer[i] & 0x30)>>4) == 1) //white
		{
			val |= 0x03 << 4;
		}
		else
		{
			val |= 0x04 << 4;
		}
		
		if(((lcd_buffer[i] & 0xc0)>>6) == 0) //black
		{
			
		}else if(((lcd_buffer[i] & 0xc0)>>6) == 1) //white
		{
			val |= 0x03;
		}
		else
		{
			val |= 0x04;
		}
		
		ops->write_data(val);	 
	}
}



static void epaper_init(struct epaper_ops_t * ops)
{
	ops->reset();
	ops->write_cmd(0x06);         //boost setting      	  
	ops->write_data (0xd7);
	ops->write_data (0xd7);
	ops->write_data (0x2f);	   	
	rt_thread_mdelay(1);      
	ops->write_cmd(0x01);		// power setting
	ops->write_data (0x37);	   
	ops->write_data (0x00);	      
	ops->write_data (0x19);
	ops->write_data (0x3c); 
	ops->write_cmd(0x04);         //power on   
	rt_thread_mdelay(10);  ;//10MS     
	ops->wait_busy();  		
	ops->write_cmd(0x65); 
	ops->write_data (0x01);		
	ops->write_cmd(0x65); 
	ops->write_data (0x00);   		
	//		RR_GetTemperature(); 
	ops->write_cmd(0x30);
	ops->write_data(0x3A);
	ops->write_cmd(0x00); 
	ops->write_data (0x0f);	 //  640*480 起始点在右上角   
	ops->write_data (0x00); 
	ops->write_cmd(0X50);  //border      
	ops->write_data(0x77);     //W   
	ops->write_cmd(0x61);	 // resolution setting
	ops->write_data (0x02);	   
	ops->write_data (0x80);	 //640gate     
	ops->write_data (0x01);
	ops->write_data (0x80);    //384source	
	ops->write_cmd(0x82);			//vcom setting	   	   				
	ops->write_data (0x24);     //vcom平均值
	ops->wait_busy();			
	ops->write_cmd(0xe5);
	ops->write_data (0x03);		
	ops->wait_busy();
}

void epaper_select(struct epaper_dvr_t * epaper)
{
	epaper_attr = epaper->attribute;
	
	last_ops = &epaper->ops;
}

void epaper_draw(struct epaper_dvr_t * epaper)
{
	struct epaper_ops_t * ops = &epaper->ops;
	last_ops = ops;
	epaper_power_sw(1);
	epaper_init(ops);
	
	epaper_draw_buffer(epaper);
	epaper_update(ops);
	epaper_power_sw(0);
}


void fill_buffer(uint8_t color)
{
	memset(lcd_buffer,color, sizeof(lcd_buffer));
}





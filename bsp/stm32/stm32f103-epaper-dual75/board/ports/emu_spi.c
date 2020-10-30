#include "drv_config.h"
#include <string.h>

#define DRV_DEBUG
#define LOG_TAG              "drv.spi"
#include <rtdbg.h>


struct emu_spi_bus_t{
	rt_base_t mosi_pin;
	rt_base_t miso_pin;
	rt_base_t clk_pin;
	
	struct rt_spi_bus spi_bus;
};

static struct emu_spi_bus_t   emu_spi_bus = {
	.mosi_pin = GET_PIN(A, 8),
	.miso_pin = GET_PIN(A, 9),
	.clk_pin = GET_PIN(C, 7),
};
                            

uint8_t emu_spi_xfer_byte(struct emu_spi_bus_t* spi,uint8_t byte)
{
	uint8_t i;
	uint8_t Temp = 0x00;
	int SDI;

	for(i = 0; i < 8; i++)
	{
		rt_pin_write(spi->clk_pin, PIN_HIGH);
		
		if(byte & 0x80)
		{
			rt_pin_write(spi->mosi_pin, PIN_HIGH);
		}
		else
		{
			rt_pin_write(spi->mosi_pin, PIN_LOW);
		}
		
		byte <<= 1;
		rt_pin_write(spi->clk_pin, PIN_LOW);
		
		SDI = rt_pin_read(spi->miso_pin);//判断si是否有输入
		
		Temp <<= 1;
		
		if(SDI) //读到1时
		{
			Temp++; //置1 即向右移动一位同时加1 因上边有《《=1
		}
		
		rt_pin_write(spi->clk_pin, PIN_HIGH);
		
	}
	return Temp; //返回读到miso输入的值
}


static rt_err_t emu_spi_init(struct emu_spi_bus_t *emp_spi, struct rt_spi_configuration *cfg)
{
    RT_ASSERT(emp_spi != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);


	rt_pin_mode(emp_spi->mosi_pin, PIN_MODE_OUTPUT);
	rt_pin_mode(emp_spi->miso_pin, PIN_MODE_INPUT_PULLUP);
	rt_pin_mode(emp_spi->clk_pin, PIN_MODE_OUTPUT);
	
	return RT_EOK;
}
static rt_err_t spi_configure(struct rt_spi_device *device,
                              struct rt_spi_configuration *configuration)
{
    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(configuration != RT_NULL);

    struct emu_spi_bus_t *emp_spi =  rt_container_of(device->bus, struct emu_spi_bus_t, spi_bus);


    return emu_spi_init(emp_spi, configuration);

}		

static rt_uint32_t spixfer(struct rt_spi_device *device, struct rt_spi_message *message)
{
	struct emu_spi_bus_t *spi = (struct emu_spi_bus_t *)(device->bus->parent.user_data); 
	int i = 0;
	rt_base_t *p_cs_pin =  device->parent.user_data;
	
	if (message->cs_take)
    {
		rt_pin_write(*p_cs_pin, PIN_LOW);
    }
	
//	rt_kprintf("emu spi xfer: %d",message->length);
	const rt_uint8_t *send_buf;
	rt_uint8_t *recv_buf;
	send_buf = message->send_buf;
	recv_buf = message->recv_buf;
	
	for(i = 0;i< message->length; i++)
	{
//		rt_kprintf("%02x ", send_buf[i]);
		
		if((send_buf == RT_NULL) && (recv_buf != RT_NULL))
		{
			recv_buf[i] = emu_spi_xfer_byte(spi,0xff);
		}
		else if((send_buf != RT_NULL) && (recv_buf != RT_NULL))
		{
			recv_buf[i] = emu_spi_xfer_byte(spi,send_buf[i]);
		}
		else if((send_buf != RT_NULL) && (recv_buf == RT_NULL))
		{
			emu_spi_xfer_byte(spi,send_buf[i]);
		}
	}
	
	
	if (message->cs_release)
    {
       rt_pin_write(*p_cs_pin, PIN_HIGH);
    }
	
	return message->length;
}
static const struct rt_spi_ops emu_spi_ops =
{
    .configure = spi_configure,
    .xfer = spixfer,
};

rt_err_t rt_emu_spi_device_attach(const char *bus_name, const char *device_name, rt_base_t cs_pin)
{
	rt_err_t result;
	rt_pin_mode(cs_pin, PIN_MODE_OUTPUT);
	rt_pin_write(cs_pin, PIN_HIGH);
	rt_base_t *p_cs_pin = (rt_base_t*)rt_malloc(sizeof(rt_base_t));
	RT_ASSERT(p_cs_pin != RT_NULL);
	*p_cs_pin = cs_pin;
	struct rt_spi_device *spi_device = (struct rt_spi_device *)rt_malloc(sizeof(struct rt_spi_device));
	
    RT_ASSERT(spi_device != RT_NULL);
	result = rt_spi_bus_attach_device(spi_device, device_name, bus_name, (void *)p_cs_pin);
	
	LOG_D("%s attach to %s done", device_name, bus_name);
	return result;
}
static int rt_hw_spi_bus_init(void)
{
    rt_err_t result;

	emu_spi_bus.spi_bus.parent.user_data = &emu_spi_bus;
	
	result = rt_spi_bus_register(&emu_spi_bus.spi_bus,"emu_spi", &emu_spi_ops);
	RT_ASSERT(result == RT_EOK);

	LOG_D("%s bus init done", "emu_spi");
   

    return result;
}
INIT_PREV_EXPORT(rt_hw_spi_bus_init);
#include <rtthread.h>
#include <rtdevice.h>
#include "epaper_dvr.h"

#define DBG_TAG "epaper"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

#ifdef RT_USING_FINSH
#include <finsh.h>
#endif

static rt_size_t epaper_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    
    return size;
}


int register_epaper_device(struct epaper_dvr_t * ep0, struct epaper_dvr_t *ep1)
{
	struct rt_device *device0 = (struct rt_device *)rt_malloc(sizeof(struct rt_device));
	
	if(device0 == RT_NULL)
	{
		LOG_E("epaper device alloc failed!");
		return -1;
	}
	device0->type = RT_Device_Class_Char;
	device0->rx_indicate  = RT_NULL;
    device0->tx_complete  = RT_NULL;
    device0->init         = RT_NULL;
    device0->open         = RT_NULL;
    device0->close        = RT_NULL;
    device0->read         = RT_NULL;
    device0->write        = epaper_write;
    device0->control      = RT_NULL;
    device0->user_data    = ep0;
	
	
	rt_device_register(device0, "epaper0", RT_DEVICE_FLAG_WRONLY);
	
//	struct rt_device *device1 = (struct rt_device *)rt_malloc(sizeof(struct rt_device));
//	if(device1 == RT_NULL)
//	{
//		LOG_E("epaper device alloc failed!");
//		return;
//	}
//	
//	rt_device_register(device1, "epaper1", RT_DEVICE_FLAG_WRONLY);
	
	
	return 0;
}
int _register_epaper_device()
{
	register_epaper_device(&epaper_front, NULL);
	
	return RT_EOK;
}
#if 0
INIT_APP_EXPORT(_register_epaper_device);
#endif
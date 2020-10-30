#ifndef __EPAPER_DVR_H__
#define __EPAPER_DVR_H__
#include <stdio.h>
#include <stdint.h>
#include "GUI.h"

struct epaper_ops_t {
	void (*write_cmd)(uint8_t cmd);
	void (*write_data)(uint8_t cmd);
	int (*wait_busy)();
	void (*reset)();
};


struct epaper_dvr_t{
	uint8_t *name;
	uint32_t attribute;
	
	struct epaper_ops_t ops;

};

#define ATR_COLOR_2	0x01
#define ATR_COLOR_3	0x02


int gui_draw_qr_at(uint8_t *string, uint8_t scale, uint16_t x, uint16_t y);
void gui_fill(uint8_t data);

void epaper_select(struct epaper_dvr_t * epaper);
void epaper_draw(struct epaper_dvr_t * epaper);
int epaper_thread_draw(struct epaper_dvr_t * epaper);
void fill_buffer(uint8_t c);

extern struct epaper_dvr_t epaper_front;
extern struct epaper_dvr_t epaper_rear;
#endif
#ifndef WATCHY_Mickey_H
#define WATCHY_Mickey_H

#include <Watchy.h>
#include "graphics.h"

class Mickey : public Watchy{
    public:
        Mickey();
		void drawPixel(int16_t x, int16_t y,uint16_t col);
		void drawBitmapCol(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color1);
		int getPixel(int16_t x, int16_t y, const uint8_t *bitmap);
		void drawMyRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1);
		void drawWatchFace();
		void drawBitmapRotate(int xx, int yy, const uint8_t *bitmap, unsigned int fAngle, uint16_t color);
};

#endif

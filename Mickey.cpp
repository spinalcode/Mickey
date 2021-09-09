#include "Mickey.h"

Mickey::Mickey(){} //constructor

#define GREY 0x7BEF

void Mickey::drawPixel(int16_t x, int16_t y,uint16_t col){
  switch (col){
    case GREY:
      if(y&1){
        if(x&1){
          display.drawPixel(x, y, GxEPD_BLACK);
        }else{
          display.drawPixel(x, y, GxEPD_WHITE);
        }
      }else{
        if(x&1){
          display.drawPixel(x, y, GxEPD_WHITE);
        }else{
          display.drawPixel(x, y, GxEPD_BLACK);
        }
      }
      break;
    default:
      display.drawPixel(x, y, col);
      break;
  }
}

void Mickey::drawBitmapCol(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color1){
  int16_t i, j, byteWidth = (w + 7) / 8;
  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if((pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7)))==0) {
        drawPixel(x+i, y+j, color1);
      }
    }
  }
}

int Mickey::getPixel(int16_t x, int16_t y, const uint8_t *bitmap){
  int16_t imageWidth = pgm_read_byte(bitmap);
  int16_t byteWidth = (imageWidth + 7) / 8;
  return (pgm_read_byte(bitmap + 2 + y * byteWidth + x / 8) & (128 >> (x & 7)));
}

void Mickey::drawMyRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1){
  for(int j=0; j<h; j++) {
    for(int i=0; i<w; i++ ) {
      drawPixel(x+i, y+j, color1);
    }
  }  
}

void Mickey::drawBitmapRotate(int xx, int yy, const uint8_t *bitmap, unsigned int fAngle, uint16_t color=GxEPD_BLACK){

  int iWidth = pgm_read_byte(bitmap);
  int iHeight = pgm_read_byte(bitmap + 1);
  int hX = iWidth/2;
  int hY = iHeight;
  float angle = fAngle * PI / 180.0;

  int startX = -hX;
  int endX = startX + iWidth;
  int startY = -hY;
  int endY = startY + iHeight;

  // we only need to draw pixels in the circular area the image can reach from its center.
  // For simplicity, we search the bounding box of this area.
  // assumes that the vertical dimension of the image is the larger one
  assert(hX <= hY);
  int startAreaX = xx - hY;
  int startAreaY = yy - hY;
  int endAreaX = 200 - startAreaX;
  int endAreaY = 200 - startAreaY;
  for (int x = 0; x < 200; x++) {
    yield();
    for (int y = 0; y < 200; y++) {
      int ux = (x-xx) * cos(-angle) - (y-yy) * sin(-angle);
      int uy = (x-xx) * sin(-angle) + (y-yy) * cos(-angle);
      
      if(ux >= startX && ux < endX && uy >= startY && uy < endY){
        if(!getPixel(ux + hX, uy + hY, bitmap)){
          drawPixel(x, y, color);
        }
      }
    }
  }
}


void Mickey::drawWatchFace() { //override this method to customize how the watch face looks

    int theHour = currentTime.Hour;
    int theMinute = currentTime.Minute;
    
    // background
    drawMyRect(0, 0, 200, 200, GxEPD_WHITE);
    drawBitmapCol(0, 0, ClockBG, 200, 200, GxEPD_BLACK);

    // hour hand
    int hourAngle = ((theHour%12)*60 + theMinute) * 360 / 720;
    drawBitmapRotate(100,100, hourHandMask, hourAngle, GxEPD_WHITE);
    drawBitmapRotate(100,100, hourHand, hourAngle, GxEPD_BLACK);

    // minute hand
    drawBitmapRotate(100,100, minHandMask, theMinute * 6, GxEPD_WHITE);
    drawBitmapRotate(100,100, minHand, theMinute * 6, GxEPD_BLACK);

    // dot in middle
    drawBitmapCol(92, 92, dot, 16, 16, GxEPD_WHITE);
    drawBitmapCol(92, 92, dot2, 16, 16, GxEPD_BLACK);
    
} // end function

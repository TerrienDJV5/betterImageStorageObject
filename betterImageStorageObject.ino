#include "BetterImageStorageObject.hpp"

BetterImageStorageObject<128, 128, 32, 4> image;

int freeRam () {
    extern int __heap_start, *__brkval; 
    int v; 
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
};


void setup()
{
  //image.begin(128,128,8);
  image.config("rgba", 5,6,5,1);
}

void loop()
{
  for (int indexY; indexY < image.getHeight(); indexY++){
    for (int indexX; indexX < image.getWidth(); indexX++){
      unsigned char pRed = random(255);
      unsigned char pGreen = random(255);
      unsigned char pBlue = random(255);
      unsigned char pAlpha = random(255);
      PixelStruct pixel = {pRed, pGreen, pBlue, pAlpha};
      image.setPixelValue(indexX, indexY, pixel);
    }
  }
  for (int indexY; indexY < image.getHeight(); indexY++){
    for (int indexX; indexX < image.getWidth(); indexX++){
      PixelStruct pixel = image.getPixelValue(indexX, indexY);
    }
  }
}

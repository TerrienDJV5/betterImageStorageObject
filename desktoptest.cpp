/*
#include <iostream>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <bitset>
#include <cstdint>
#include <cinttypes>

#include "BetterImageStorageObject.hpp"
using namespace std;

BetterImageStorageObject image(false, true);




int main() {
  // initialize random seed:
  srand (time(NULL));
  
  image.begin(2,2,5,6,5,1);
  for (int indexY = 0; indexY < image.getHeight(); indexY++){
    for (int indexX = 0; indexX < image.getWidth(); indexX++){
      // generate secret number between 0 and 255:
      unsigned char pRed = rand() % 256 + 0;
      unsigned char pGreen = rand() % 256 + 0;
      unsigned char pBlue = rand() % 256 + 0;
      unsigned char pAlpha = rand() % 256 + 0;
      PixelStruct pixel = {pRed, pGreen, pBlue, pAlpha};
      printf("px.    {%0d,%0d,%0d,%0d},\n", pixel.red, pixel.green, pixel.blue, pixel.alpha);
      image.setPixelValue(indexX, indexY, pixel);
      printf("in.    {%0d,%0d,%0d,%0d},\n", pixel.red, pixel.green, pixel.blue, pixel.alpha);
      PixelStruct pixel2 = image.getPixelValue(indexX, indexY);
      printf("out    {%0d,%0d,%0d,%0d},\n", pixel2.red, pixel2.green, pixel2.blue, pixel2.alpha);
    }
  }
  
    
  return 0;
}

//*/

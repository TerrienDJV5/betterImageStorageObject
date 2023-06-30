#include "BetterImageStorageObject.h"

BetterImageStorageObject image(true, false);
void setup()
{
  image.begin(128,128,8);
}

void loop()
{
  for (int indexY; indexY < image.getHeight(); indexY++){
    for (int indexX; indexX < image.getWidth(); indexX++){
      PixelStruct pixel = image.setPixelStructValues(random(255));
      image.setPixelValue(indexX, indexY, pixel);
    }
  }
  for (int indexY; indexY < image.getHeight(); indexY++){
    for (int indexX; indexX < image.getWidth(); indexX++){
      PixelStruct pixel = image.getPixelValue(indexX, indexY);
    }
  }
}

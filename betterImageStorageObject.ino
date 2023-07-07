#include "BetterImageStorageObject.hpp"

BetterImageStorageObject imageV1(true, false);
BetterImageStorageObjectTemplate<128, 128, 32, 4> imageV2;


int freeRam () {
    extern int __heap_start, *__brkval; 
    int v; 
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
};


PixelStruct pixelArray[128*128];

void setup()
{
  //Format
  //program storage space //dynamic memory
  //Before Serial
  //485801//29676
  //After Serial.begin(115200);
  //506209//29804
  //Serial Addition Sketch Diff
  //20408//128
  //Serial Usage Diff After Serial Addition
  //328//0
  Serial.begin(115200);
  imageV1.begin(128,128,8);
  imageV2.configChannelBitSize(5,6,5,1);
  imageV2.printChannelBitLengths(Serial);
  delay(10000);
}

void loop()
{
  PixelStruct pixelOUT;
  
  for (int indexY; indexY < 128; indexY++){
    for (int indexX; indexX < 128; indexX++){
      unsigned char pRed = random(255);
      unsigned char pGreen = random(255);
      unsigned char pBlue = random(255);
      unsigned char pAlpha = random(255);
      pixelArray[ indexY*128 + indexX ] = {pRed, pGreen, pBlue, pAlpha};
    }
  }
  
  for (int indexY; indexY < imageV2.getHeight(); indexY++){
    for (int indexX; indexX < imageV2.getWidth(); indexX++){
      unsigned char pRed = random(255);
      unsigned char pGreen = random(255);
      unsigned char pBlue = random(255);
      unsigned char pAlpha = random(255);
      PixelStruct pixelIN = {pRed, pGreen, pBlue, pAlpha};
      imageV2.setPixelValue(indexX, indexY, pixelIN);
    }
  }
  for (int indexY; indexY < imageV2.getHeight(); indexY++){
    for (int indexX; indexX < imageV2.getWidth(); indexX++){
      pixelOUT = imageV2.getPixelValue(indexX, indexY);
      Serial.println("imageV2 -> ");
      Serial.print("indexX:");Serial.print(indexX);
      Serial.println();
      Serial.print("indexY:");Serial.print(indexY);
      Serial.println();
      Serial.println(pixelOUT.red);
      Serial.println(pixelOUT.green);
      Serial.println(pixelOUT.blue);
      Serial.println(pixelOUT.alpha);
    }
  }
  
  for (int indexY; indexY < imageV1.getHeight(); indexY++){
    for (int indexX; indexX < imageV1.getWidth(); indexX++){
      unsigned char pRed = random(255);
      unsigned char pGreen = random(255);
      unsigned char pBlue = random(255);
      unsigned char pAlpha = random(255);
      PixelStruct pixelIN = {pRed, pGreen, pBlue, pAlpha};
      imageV1.setPixelValue(indexX, indexY, pixelIN);
    }
  }
  for (int indexY; indexY < imageV1.getHeight(); indexY++){
    for (int indexX; indexX < imageV1.getWidth(); indexX++){
      pixelOUT = imageV1.getPixelValue(indexX, indexY);
      Serial.println("imageV1 -> ");
      Serial.print("indexX:");Serial.print(indexX);
      Serial.println();
      Serial.print("indexY:");Serial.print(indexY);
      Serial.println();
      Serial.println(pixelOUT.red);
      Serial.println(pixelOUT.green);
      Serial.println(pixelOUT.blue);
      Serial.println(pixelOUT.alpha);
    }
  }
}

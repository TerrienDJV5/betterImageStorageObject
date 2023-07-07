#include "BetterImageStorageObject.hpp"

BetterImageStorageObject imageV1(true, false);
BetterImageStorageObjectTemplate<64, 64, 32, 4> imageV2;


int freeRam () {
    extern int __heap_start, *__brkval; 
    int v; 
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
};


PixelStruct pixelArray[64*64];

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
  //imageV1.begin(64,64,8);
  imageV2.configChannelBitSize(5,6,5,1);
  imageV2.printChannelBitLengths(Serial);
  delay(500);
}

void loop()
{
  for (int indexY = 0; indexY < 64; indexY++){
    for (int indexX = 0; indexX < 64; indexX++){
      unsigned char pRed = random(pow(2, 5));
      unsigned char pGreen = random(pow(2, 6));
      unsigned char pBlue = random(pow(2, 5));
      unsigned char pAlpha = random(pow(2, 1));
      pixelArray[ indexY*64 + indexX ] = {pRed, pGreen, pBlue, pAlpha};
    }
  }
  Serial.println(imageV2.getWidth());
  Serial.println(imageV2.getHeight());
  
  for (int indexY = 0; indexY < imageV2.getHeight(); indexY++){
    for (int indexX = 0; indexX < imageV2.getWidth(); indexX++){
      /*
      Serial.println("Set imageV2 -> ");
      Serial.print("indexX:");Serial.print(indexX);
      Serial.println();
      Serial.print("indexY:");Serial.print(indexY);
      Serial.println();
      */
      imageV2.setPixelValue(indexX, indexY, pixelArray[ indexY*64 + indexX ]);
    }
  }
  
  bool imageV2Status = true;
  
  for (int indexY = 0; indexY < imageV2.getHeight(); indexY++){
    for (int indexX = 0; indexX < imageV2.getWidth(); indexX++){
      PixelStruct pixelOUT = imageV2.getPixelValue(indexX, indexY);
      
      if (pixelOUT.red != pixelArray[ indexY*64 + indexX ].red ){
        imageV2Status = false;
      }
      if (pixelOUT.green != pixelArray[ indexY*64 + indexX ].green ){
        imageV2Status = false;
      }
      if (pixelOUT.blue != pixelArray[ indexY*64 + indexX ].blue ){
        imageV2Status = false;
      }
      if (pixelOUT.alpha != pixelArray[ indexY*64 + indexX ].alpha ){
        imageV2Status = false;
      }
      
      Serial.println("Get imageV2 -> ");
      Serial.print("indexX:");Serial.print(indexX);
      Serial.println();
      Serial.print("indexY:");Serial.print(indexY);
      Serial.println();
      Serial.println(pixelOUT.red);
      Serial.println(pixelOUT.green);
      Serial.println(pixelOUT.blue);
      Serial.println(pixelOUT.alpha);
      Serial.println(pixelArray[ indexY*64 + indexX ].red);
      Serial.println(pixelArray[ indexY*64 + indexX ].green);
      Serial.println(pixelArray[ indexY*64 + indexX ].blue);
      Serial.println(pixelArray[ indexY*64 + indexX ].alpha);
    }
  }
  Serial.print("imageV2 Usage Pass:");
  Serial.println(imageV2Status);
  delay(5000);
  
  /*
  for (int indexY = 0; indexY < imageV1.getHeight(); indexY++){
    for (int indexX = 0; indexX < imageV1.getWidth(); indexX++){
      unsigned char pRed = random(255);
      unsigned char pGreen = random(255);
      unsigned char pBlue = random(255);
      unsigned char pAlpha = random(255);
      PixelStruct pixelIN = {pRed, pGreen, pBlue, pAlpha};
      imageV1.setPixelValue(indexX, indexY, pixelIN);
    }
  }
  for (int indexY = 0; indexY < imageV1.getHeight(); indexY++){
    for (int indexX = 0; indexX < imageV1.getWidth(); indexX++){
      PixelStruct pixelOUT = imageV1.getPixelValue(indexX, indexY);
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
  //*/
}

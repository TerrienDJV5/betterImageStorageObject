//By Terrien-DJV5
//BetterImageStorageObject.cpp
//Date Created: jun 29 2023 6:48PM

#include "BetterImageStorageObject.h"






ImageObject::ImageObject(bool grayscaleFlag, bool alphaChannelFlag, int w, int h, uint8_t inputValue0, uint8_t inputValue1, uint8_t inputValue2, uint8_t inputValue3)
{
  this->grayscaleFlag = grayscaleFlag;
  this->alphaChannelFlag = alphaChannelFlag;
  uint8_t redBitLength = 0;
  uint8_t greenBitLength = 0;
  uint8_t blueBitLength = 0;
  uint8_t alphaBitLength = 0;
  
  if (grayscaleFlag){
    if (alphaChannelFlag){
      //Use inputValue0, inputValue1
      redBitLength = inputValue0;
      greenBitLength = inputValue0;
      blueBitLength = inputValue0;
      alphaBitLength = inputValue1;
    }else{
      //Use inputValue0
      redBitLength = inputValue0;
      greenBitLength = inputValue0;
      blueBitLength = inputValue0;
    }
  }else{
    if (alphaChannelFlag){
      //Use inputValue0, inputValue1, inputValue2, inputValue3
      redBitLength = inputValue0;
      greenBitLength = inputValue1;
      blueBitLength = inputValue2;
      alphaBitLength = inputValue3;
    }else{
      //Use inputValue0, inputValue1, inputValue2
      redBitLength = inputValue0;
      greenBitLength = inputValue1;
      blueBitLength = inputValue2;
    }
  }
  createImageFunc(w, h, redBitLength, greenBitLength, blueBitLength, alphaBitLength);
}


void ImageObject::createImageFunc(int w, int h, int firstBitLength, int greenBitLength, int blueBitLength, int alphaBitLength)
{
  this->width = w;
  this->height = h;
  this->_alphaBitOffset = 0;
  if (grayscaleFlag){
    PixelArray.pixelBitLength = firstBitLength;
    PixelArray.redBitLength = firstBitLength;
    PixelArray.greenBitLength = firstBitLength;
    PixelArray.blueBitLength = firstBitLength;
    this->_redBitOffset = 0;
    this->_greenBitOffset = 0;
    this->_blueBitOffset = 0;
  }else{
    PixelArray.pixelBitLength = firstBitLength+greenBitLength+blueBitLength;
    this->_redBitOffset = 0;
    PixelArray.redBitLength = firstBitLength;
    
    this->_greenBitOffset = _redBitOffset + PixelArray.redBitLength;
    PixelArray.greenBitLength = greenBitLength;
    
    this->_blueBitOffset = _greenBitOffset + PixelArray.greenBitLength;
    PixelArray.blueBitLength = blueBitLength;

    if (alphaChannelFlag){
      this->_alphaBitOffset = _blueBitOffset + PixelArray.blueBitLength;
      PixelArray.alphaBitLength = alphaBitLength;
    }
  }
  if (alphaChannelFlag){
    PixelArray.pixelBitLength += (alphaBitLength);
  }
  
  
  uint64_t dataBitLength = ((width*height)*PixelArray.pixelBitLength);
  PixelArray.dataByteLength = (dataBitLength - dataBitLength%8)>>3 + (dataBitLength%8 != 0);
  setPixelMask(&PixelsMask);
  PixelArray.data = (unsigned char*)calloc(PixelArray.dataByteLength, sizeof(unsigned char));
}



uint32_t ImageObject::bitSelect_i32o32(uint32_t input, unsigned int offset, unsigned int length)
{
  //offset = 4
  //length = 12
  const unsigned char inputLength = 32;
  unsigned char shiftLeftCount = inputLength-(offset+length);
  uint32_t bitSelectMask = ~(0B11111111111111111111111111111111<<length);
  
  //"|" Selected bits
  //  ----||||||||||||----------------
  //0B11110000111100001111000011110000
  //x = unused Bits
  //output
  //  --------------------||||||||||||
  //0Bxxxxxxxxxxxxxxxxxxxx000011110000
  
  return (input>>shiftLeftCount) & bitSelectMask;
}

uint64_t ImageObject::arraybitSelect_o64(uint8_t inputArray[], unsigned int bitIndex)
{
  //bitIndex = 10
  //bitGrabCount = 64
  //byteIndex = 1
  //startMask = 0B00111111;
  //endMask = 0B11000000;
  //xxxxxxxx xx|||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| ||xxxxxx
  //11000000 00111111 11000000 00111111 11000000 00111111 11000000 00111111 11000000 00111111
  //output
  //|||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| ||||||||
  //11111111 00000000 11111111 00000000 11111111 00000000 11111111 00000000
  byte endMask = (0B11111111)<<(8-bitIndex%8);
  byte startMask = ~(endMask);
  unsigned int byteIndex = ((bitIndex-(bitIndex%8))>>3);
  uint64_t output;
  uint8_t outputList[8];
  outputList[0] = (inputArray[ (byteIndex+0) ]&startMask) | (inputArray[ (byteIndex+1) ]&endMask);
  outputList[1] = (inputArray[ (byteIndex+1) ]&startMask) | (inputArray[ (byteIndex+2) ]&endMask);
  outputList[2] = (inputArray[ (byteIndex+2) ]&startMask) | (inputArray[ (byteIndex+3) ]&endMask);
  outputList[3] = (inputArray[ (byteIndex+3) ]&startMask) | (inputArray[ (byteIndex+4) ]&endMask);
  outputList[4] = (inputArray[ (byteIndex+4) ]&startMask) | (inputArray[ (byteIndex+5) ]&endMask);
  outputList[5] = (inputArray[ (byteIndex+5) ]&startMask) | (inputArray[ (byteIndex+6) ]&endMask);
  outputList[6] = (inputArray[ (byteIndex+6) ]&startMask) | (inputArray[ (byteIndex+7) ]&endMask);
  outputList[7] = (inputArray[ (byteIndex+7) ]&startMask) | (inputArray[ (byteIndex+8) ]&endMask);
  output = (outputList[0]<<(64-8))|(outputList[1]<<(64-8-8))|(outputList[2]<<(64-8-8-8))|(outputList[3]<<(64-8-8-16))|(outputList[4]<<(64-8-16-16))|(outputList[5]<<(64-8-8-32))|(outputList[6]<<(64-8-16-32))|(outputList[7]<<(64-8-8-16-32));
  return output;
}

uint32_t ImageObject::arraybitSelect_o32(uint8_t inputArray[], unsigned int bitIndex)
{
  //bitIndex = 10
  //bitGrabCount = 632
  //byteIndex = 1
  //startMask = 0B00111111;
  //endMask = 0B11000000;
  //xxxxxxxx xx|||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| ||xxxxxx
  //11000000 00111111 11000000 00111111 11000000 00111111 11000000 00111111 11000000 00111111
  byte endMask = (0B11111111)<<(8-bitIndex%8);
  byte startMask = ~(endMask);
  unsigned int byteIndex = ((bitIndex-(bitIndex%8))>>3);
  uint32_t output;
  uint8_t outputList[4];
  outputList[0] = (inputArray[ (byteIndex+0) ]&startMask) | (inputArray[ (byteIndex+1) ]&endMask);
  outputList[1] = (inputArray[ (byteIndex+1) ]&startMask) | (inputArray[ (byteIndex+2) ]&endMask);
  outputList[2] = (inputArray[ (byteIndex+2) ]&startMask) | (inputArray[ (byteIndex+3) ]&endMask);
  outputList[3] = (inputArray[ (byteIndex+3) ]&startMask) | (inputArray[ (byteIndex+4) ]&endMask);
  output = (outputList[0]<<(32-8))|(outputList[1]<<(32-16))|(outputList[2]<<(32-16-8))|(outputList[3]<<(32-32));
  return output;
}

void ImageObject::uint32_convert_to_4xuint8Array(uint8_t inputArray[], uint32_t input)
{
  //inputArray must be atleast 4 items
  inputArray[0] = (input>>(24));
  inputArray[1] = (input>>(16));
  inputArray[2] = (input>>(8));
  inputArray[3] = (input>>(0));
}


void ImageObject::byteArray_bitWrite_i32(uint8_t inputArray[], uint32_t input, uint64_t bitoffset, uint8_t inputbitlength)
{
  unsigned int byteIndex = ((bitoffset-(bitoffset%8))>>3);
  uint8_t bufferBitOffset = (bitoffset%8);
  uint8_t bufferBitStartMask = ~(0B11111111 << bufferBitOffset);
  uint8_t bufferBitEndMask = (0B11111111 << bufferBitOffset);
  uint32_t inputMask = ~(0B11111111111111111111111111111111<<inputbitlength);
  input &= inputMask;
  input = input<<(32-inputbitlength);//00001111 -> 11110000
  //shift buffer right for bitoffset
  uint8_t bufferList[8];
  uint8_t bufferMaskList[8];
  //                       ||||||||||||||||||||||||||||||||||||||||xxxxxxxxxxxxxxxxxxxxxxxx
  uint64_t bufferValue = 0B0000000000000000000000000000000000000000000000000000000000000000;
  uint64_t bufferMask = inputMask<<32>>bufferBitOffset;
  bufferValue = input<<32>>bufferBitOffset;
  bufferValue &= bufferMask;
  uint32_convert_to_4xuint8Array(&bufferMaskList[0], ~(bufferMask>>32)&0b0000000000000000000000000000000011111111111111111111111111111111);
  uint32_convert_to_4xuint8Array(&bufferMaskList[4], ~(bufferMask)&0b0000000000000000000000000000000011111111111111111111111111111111);
  /*
  bufferMaskList[0] = (~bufferMask)>>(64-8);
  bufferMaskList[1] = (~bufferMask)>>(64-16);
  bufferMaskList[2] = (~bufferMask)>>(64-24);
  bufferMaskList[3] = (~bufferMask)>>(64-32);
  bufferMaskList[4] = (~bufferMask)>>(64-40);
  bufferMaskList[5] = (~bufferMask)>>(64-48);//likely unused
  bufferMaskList[6] = (~bufferMask)>>(64-56);//likely unused
  bufferMaskList[7] = (~bufferMask)>>(64-64);//likely unused
  */
  uint32_convert_to_4xuint8Array(&bufferList[0], (bufferValue>>32)&0b0000000000000000000000000000000011111111111111111111111111111111);
  uint32_convert_to_4xuint8Array(&bufferList[4], (bufferValue)&0b0000000000000000000000000000000011111111111111111111111111111111);
  /*
  bufferList[0] = bufferValue>>(64-8);
  bufferList[1] = bufferValue>>(64-16);
  bufferList[2] = bufferValue>>(64-24);
  bufferList[3] = bufferValue>>(64-32);
  bufferList[4] = bufferValue>>(64-40);
  bufferList[5] = bufferValue>>(64-48);//likely unused
  bufferList[6] = bufferValue>>(64-56);//likely unused
  bufferList[7] = bufferValue>>(64-64);//likely unused
  */
  inputArray[ byteIndex+0 ] &= bufferMaskList[0];
  inputArray[ byteIndex+1 ] &= bufferMaskList[1];
  inputArray[ byteIndex+2 ] &= bufferMaskList[2];
  inputArray[ byteIndex+3 ] &= bufferMaskList[3];
  inputArray[ byteIndex+4 ] &= bufferMaskList[4];
  inputArray[ byteIndex+5 ] &= bufferMaskList[5];//likely unused
  inputArray[ byteIndex+6 ] &= bufferMaskList[6];//likely unused
  inputArray[ byteIndex+7 ] &= bufferMaskList[7];//likely unused
  
  inputArray[ byteIndex+0 ] |= bufferList[0];
  inputArray[ byteIndex+1 ] |= bufferList[1];
  inputArray[ byteIndex+2 ] |= bufferList[2];
  inputArray[ byteIndex+3 ] |= bufferList[3];
  inputArray[ byteIndex+4 ] |= bufferList[4];
  inputArray[ byteIndex+5 ] |= bufferList[5];//likely unused
  inputArray[ byteIndex+6 ] |= bufferList[6];//likely unused
  inputArray[ byteIndex+7 ] |= bufferList[7];//likely unused
  
}


uint8_t ImageObject::getWidth()
{
  return width;
}
uint8_t ImageObject::getHeight()
{
  return height;
}

unsigned short int ImageObject::getPixelCount()
{
  return (width * height);
}

PixelStruct ImageObject::getPixelValue(int xPos, int yPos)
{
  PixelStruct pixel;
  unsigned int pixelBitindex = (xPos + yPos*width)*(PixelArray.pixelBitLength);
  unsigned int pixelindex = pixelBitindex/8;
  unsigned int pixelData = arraybitSelect_o32(PixelArray.data, pixelBitindex);
  pixelData = pixelData & (~(0B11111111111111111111111111111111 << PixelArray.pixelBitLength));
  pixel.red = bitSelect_i32o32(pixelData, _redBitOffset, PixelArray.redBitLength);
  pixel.green = bitSelect_i32o32(pixelData, _greenBitOffset, PixelArray.greenBitLength);
  pixel.blue = bitSelect_i32o32(pixelData, _blueBitOffset, PixelArray.blueBitLength);
  pixel.alpha = bitSelect_i32o32(pixelData, _alphaBitOffset, PixelArray.alphaBitLength);
  return pixel;
}

void ImageObject::setPixelValue(int xPos, int yPos, PixelStruct pixel)
{
  unsigned long int pixelBitindex = (xPos + yPos*width)*(PixelArray.pixelBitLength);
  unsigned int pixelValue = ConvertPixelStruct_to_Binary(pixel);
  byteArray_bitWrite_i32(PixelArray.data, pixelValue, pixelBitindex, PixelArray.pixelBitLength);
}


PixelStruct ImageObject::setPixelStructValues_Base(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
  PixelStruct pixel;
  if (grayscaleFlag){
    pixel.red = mean3input8Bit(r, g, b);
    pixel.green = mean3input8Bit(r, g, b);
    pixel.blue = mean3input8Bit(r, g, b);
  }else{
    pixel.red = r;
    pixel.green = g;
    pixel.blue = b;
  }
  if (alphaChannelFlag){
    pixel.alpha = a;
  }else{
    pixel.alpha = 255;
  }
  return applyPixelMask( pixel );
}

PixelStruct ImageObject::setPixelStructValues(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
  return setPixelStructValues_Base(r, g, b, a);
}

PixelStruct ImageObject::setPixelStructValues(unsigned char w, unsigned char a)
{
  return setPixelStructValues_Base(w, w, w, a);
}



PixelStruct ImageObject::filterPixelValues(PixelStruct pixelIn, uint8_t rBitLength, uint8_t gBitLength, uint8_t bBitLength, uint8_t aBitLength)
{
  PixelStruct pixelOut;
  pixelOut.red = pixelIn.red;
  pixelOut.green = pixelIn.green;
  pixelOut.blue = pixelIn.blue;
  pixelOut.alpha = pixelIn.alpha;
  byte rMask = ~(0B11111111<<rBitLength);
  byte gMask = ~(0B11111111<<gBitLength);
  byte bMask = ~(0B11111111<<bBitLength);
  byte aMask = ~(0B11111111<<aBitLength);
  pixelOut.red &= rMask;
  pixelOut.green &= gMask;
  pixelOut.blue &= bMask;
  pixelOut.alpha &= aMask;
  return pixelOut;
}


PixelStruct ImageObject::applyPixelMask(PixelStruct pixelIn)
{
  PixelStruct pixelOut;
  pixelOut.red = pixelIn.red & PixelsMask.redMask;
  pixelOut.green = pixelIn.green & PixelsMask.greenMask;
  pixelOut.blue = pixelIn.blue & PixelsMask.blueMask;
  pixelOut.alpha = pixelIn.alpha & PixelsMask.alphaMask;
  return pixelOut;
}


uint32_t ImageObject::ConvertPixelStruct_to_Binary(PixelStruct pixel)
{
  uint32_t pixelBinaryOut = 0B00000000000000000000000000000000;
  //If color bit lengths are less then 8 then the bits will be LSB
  //Example rBitLength = 5, gBitLength = 6, bBitLength = 5, aBitLength = 1
  //0Bxxxxxxxx xxxxxxxr rrrrgggg ggbbbbba
  PixelStruct pixelTemp = filterPixelValues(pixel, PixelArray.redBitLength, PixelArray.greenBitLength, PixelArray.blueBitLength, PixelArray.alphaBitLength);
  pixelBinaryOut = pixelTemp.red<<(PixelArray.greenBitLength+PixelArray.blueBitLength+PixelArray.alphaBitLength) | pixelTemp.green<<(PixelArray.blueBitLength+PixelArray.alphaBitLength) | pixelTemp.blue<<(PixelArray.alphaBitLength) | pixelTemp.alpha;
  return pixelBinaryOut;
}





void ImageObject::setPixelMask(PixelMaskStruct *pixelMaskInput){
  pixelMaskInput->redMask = ~(0B11111111 << PixelArray.redBitLength);
  pixelMaskInput->greenMask = ~(0B11111111 << PixelArray.greenBitLength);
  pixelMaskInput->blueMask = ~(0B11111111 << PixelArray.blueBitLength);
  if (alphaChannelFlag){
    pixelMaskInput->alphaMask = ~(0B11111111 << PixelArray.alphaBitLength);
  }else{
    pixelMaskInput->alphaMask = 0B11111111;//solid
  }
}

unsigned char ImageObject::mean3input8Bit(unsigned char i0, unsigned char i1, unsigned char i2)
{
  unsigned int sum = (i0+i1+i2);
  return (unsigned char)((sum)<<3);
}




/*




//###############################################################




//https://www.tutorialspoint.com/cplusplus/cpp_interfaces.htm
#include <iostream>
using namespace std;
 
// Base class
class Shape {
   public:
      // pure virtual function providing interface framework.
      virtual int getArea() = 0;
      void setWidth(int w) {
         width = w;
      }
   
      void setHeight(int h) {
         height = h;
      }
   
   protected:
      int width;
      int height;
};
 
// Derived classes
class Rectangle: public Shape {
   public:
      int getArea() { 
         return (width * height); 
      }
};

class Triangle: public Shape {
   public:
      int getArea() { 
         return (width * height)/2; 
      }
};



//#################################################


 */

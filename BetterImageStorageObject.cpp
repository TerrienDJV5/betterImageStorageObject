//By Terrien-DJV5
//BetterImageStorageObject.cpp
//Date Created: jun 29 2023 6:48PM

#include "BetterImageStorageObject.h"






BetterImageStorageObject::BetterImageStorageObject(bool grayscaleFlag, bool alphaChannelFlag)
{
  this->grayscaleFlag = grayscaleFlag;
  this->alphaChannelFlag = alphaChannelFlag;
  this->_imageModeFlags = 0B00000000;
  if (grayscaleFlag){
    this->_imageModeFlags |= BImgSObj_IFlag_Grayscale;
  }
  if (alphaChannelFlag){
    this->_imageModeFlags |= BImgSObj_IFlag_AlphaChannel;
  }
}

bool BetterImageStorageObject::begin(uint32_t w, uint32_t h, uint8_t inputValue0)
{
  return begin_Base(w, h, inputValue0, -1, -1, -1);
}
bool BetterImageStorageObject::begin(uint32_t w, uint32_t h, uint8_t inputValue0, uint8_t inputValue1)
{
  return begin_Base(w, h, inputValue0, inputValue1, -1, -1);
}
bool BetterImageStorageObject::begin(uint32_t w, uint32_t h, uint8_t inputValue0, uint8_t inputValue1, uint8_t inputValue2)
{
  return begin_Base(w, h, inputValue0, inputValue1, inputValue2, -1);
}
bool BetterImageStorageObject::begin(uint32_t w, uint32_t h, uint8_t inputValue0, uint8_t inputValue1, uint8_t inputValue2, uint8_t inputValue3)
{
  return begin_Base(w, h, inputValue0, inputValue1, inputValue2, inputValue3);
}

bool BetterImageStorageObject::begin_Base(uint32_t w, uint32_t h, uint8_t inputValue0 = -1, uint8_t inputValue1 = -1, uint8_t inputValue2 = -1, uint8_t inputValue3 = -1)
{
  uint8_t redBitLength = 0;
  uint8_t greenBitLength = 0;
  uint8_t blueBitLength = 0;
  uint8_t alphaBitLength = 0;
  uint8_t imageModeFlags;
  
  switch( imageModeFlags )
  {
    case BImgSObj_IFlag_Grayscale    : {
      //Use inputValue0
      redBitLength = inputValue0;
      greenBitLength = inputValue0;
      blueBitLength = inputValue0;
      };break;
    case BImgSObj_IFlag_AlphaChannel : {
      //Use inputValue0, inputValue1, inputValue2, inputValue3
      redBitLength = inputValue0;
      greenBitLength = inputValue1;
      blueBitLength = inputValue2;
      alphaBitLength = inputValue3;
      };break;
    case BImgSObj_IFlag_Grayscale|BImgSObj_IFlag_AlphaChannel  : {
      //Use inputValue0, inputValue1
      redBitLength = inputValue0;
      greenBitLength = inputValue0;
      blueBitLength = inputValue0;
      alphaBitLength = inputValue1;
      };break;
    default: {
      //Use inputValue0, inputValue1, inputValue2
      redBitLength = inputValue0;
      greenBitLength = inputValue1;
      blueBitLength = inputValue2;
      };break;
  }
  
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
  return true;//Success!
}


void BetterImageStorageObject::createImageFunc(uint32_t w, uint32_t h, uint8_t firstBitLength, uint8_t secondBitLength, uint8_t blueBitLength, uint8_t alphaBitLength)
{
  this->width = w;
  this->height = h;
  this->_alphaBitOffset = 0;
  if (grayscaleFlag){
    PixelArray.pixelBitLength = firstBitLength;
    
    ChannelBitLength.set_redChannel( firstBitLength );
    ChannelBitLength.set_greenChannel( firstBitLength );
    ChannelBitLength.set_blueChannel( firstBitLength );
    this->_redBitOffset = 0;
    this->_greenBitOffset = 0;
    this->_blueBitOffset = 0;
  }else{
    PixelArray.pixelBitLength = firstBitLength+secondBitLength+blueBitLength;
    this->_redBitOffset = 0;
    ChannelBitLength.set_redChannel( firstBitLength );
    
    this->_greenBitOffset = _redBitOffset + ChannelBitLength.get_redChannel();
    ChannelBitLength.set_greenChannel( secondBitLength );
    
    
    this->_blueBitOffset = _greenBitOffset + ChannelBitLength.get_greenChannel();
    ChannelBitLength.set_blueChannel( blueBitLength );

    if (alphaChannelFlag){
      this->_alphaBitOffset = _blueBitOffset + ChannelBitLength.get_blueChannel();
      ChannelBitLength.set_alphaChannel( alphaBitLength );
    }
  }
  if (alphaChannelFlag){
    PixelArray.pixelBitLength += (alphaBitLength);
  }
  
  
  uint64_t dataBitLength = ((width*height)*PixelArray.pixelBitLength);
  PixelArray.dataByteLength = (dataBitLength - dataBitLength%8)>>3 + (dataBitLength%8 != 0);
  setPixelMask(&PixelMask);
  PixelArray.data = (uint8_t*)calloc(PixelArray.dataByteLength, sizeof(uint8_t));
}



uint32_t BetterImageStorageObject::bitSelect_i32o32(uint32_t input, uint32_t offset, uint32_t length)
{
  //offset = 4
  //length = 12
  const uint8_t inputLength = 32;
  uint8_t shiftLeftCount = inputLength-(offset+length);
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

uint64_t BetterImageStorageObject::arraybitSelect_o64(uint8_t inputArray[], uint32_t bitIndex)
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
  uint8_t endMask = (0B11111111)<<(8-bitIndex%8);
  uint8_t startMask = ~(endMask);
  uint32_t byteIndex = ((bitIndex-(bitIndex%8))>>3);
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

uint32_t BetterImageStorageObject::arraybitSelect_o32(uint8_t inputArray[], uint32_t bitIndex)
{
  //bitIndex = 10
  //bitGrabCount = 632
  //byteIndex = 1
  //startMask = 0B00111111;
  //endMask = 0B11000000;
  //xxxxxxxx xx|||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| |||||||| ||xxxxxx
  //11000000 00111111 11000000 00111111 11000000 00111111 11000000 00111111 11000000 00111111
  uint8_t endMask = (0B11111111)<<(8-bitIndex%8);
  uint8_t startMask = ~(endMask);
  uint32_t byteIndex = ((bitIndex-(bitIndex%8))>>3);
  uint32_t output;
  uint8_t outputList[4];
  outputList[0] = (inputArray[ (byteIndex+0) ]&startMask) | (inputArray[ (byteIndex+1) ]&endMask);
  outputList[1] = (inputArray[ (byteIndex+1) ]&startMask) | (inputArray[ (byteIndex+2) ]&endMask);
  outputList[2] = (inputArray[ (byteIndex+2) ]&startMask) | (inputArray[ (byteIndex+3) ]&endMask);
  outputList[3] = (inputArray[ (byteIndex+3) ]&startMask) | (inputArray[ (byteIndex+4) ]&endMask);
  output = (outputList[0]<<(32-8))|(outputList[1]<<(32-16))|(outputList[2]<<(32-16-8))|(outputList[3]<<(32-32));
  return output;
}

void BetterImageStorageObject::uint32_convert_to_4xuint8Array(uint8_t inputArray[], uint32_t input)
{
  //inputArray must be atleast 4 items
  inputArray[0] = (input>>(24));
  inputArray[1] = (input>>(16));
  inputArray[2] = (input>>(8));
  inputArray[3] = (input>>(0));
}


void BetterImageStorageObject::byteArray_bitWrite_i32(uint8_t inputArray[], uint32_t input, uint64_t bitoffset, uint8_t inputbitlength)
{
  uint32_t byteIndex = ((bitoffset-(bitoffset%8))>>3);
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


uint8_t BetterImageStorageObject::getWidth()
{
  return width;
}
uint8_t BetterImageStorageObject::getHeight()
{
  return height;
}

uint16_t BetterImageStorageObject::getPixelCount()
{
  return (width * height);
}

PixelStruct BetterImageStorageObject::getPixelValue(uint32_t xPos, uint32_t yPos)
{
  PixelStruct pixel;
  uint32_t pixelBitindex = (xPos + yPos*width)*(PixelArray.pixelBitLength);
  uint32_t pixelindex = pixelBitindex/8;
  uint32_t pixelData = arraybitSelect_o32(PixelArray.data, pixelBitindex);
  pixelData = pixelData & (~(0B11111111111111111111111111111111 << PixelArray.pixelBitLength));
  pixel.red = bitSelect_i32o32(pixelData, _redBitOffset, ChannelBitLength.get_redChannel());
  pixel.green = bitSelect_i32o32(pixelData, _greenBitOffset, ChannelBitLength.get_greenChannel());
  pixel.blue = bitSelect_i32o32(pixelData, _blueBitOffset, ChannelBitLength.get_blueChannel());
  pixel.alpha = bitSelect_i32o32(pixelData, _alphaBitOffset, ChannelBitLength.get_alphaChannel());
  return pixel;
}

void BetterImageStorageObject::setPixelValue(uint32_t xPos, uint32_t yPos, PixelStruct pixel)
{
  uint64_t pixelBitindex = (xPos + yPos*width)*(PixelArray.pixelBitLength);
  uint32_t pixelValue = ConvertPixelStruct_to_Binary(pixel);
  byteArray_bitWrite_i32(PixelArray.data, pixelValue, pixelBitindex, PixelArray.pixelBitLength);
}


PixelStruct BetterImageStorageObject::setPixelStructValues_Base(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
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

PixelStruct BetterImageStorageObject::setPixelStructValues(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
  return setPixelStructValues_Base(r, g, b, a);
}

PixelStruct BetterImageStorageObject::setPixelStructValues(uint8_t w, uint8_t a)
{
  return setPixelStructValues_Base(w, w, w, a);
}



PixelStruct BetterImageStorageObject::filterPixelValues(PixelStruct pixelIn, uint8_t rBitLength, uint8_t gBitLength, uint8_t bBitLength, uint8_t aBitLength)
{
  PixelStruct pixelOut;
  pixelOut.red = pixelIn.red;
  pixelOut.green = pixelIn.green;
  pixelOut.blue = pixelIn.blue;
  pixelOut.alpha = pixelIn.alpha;
  uint8_t rMask = ~(0B11111111<<rBitLength);
  uint8_t gMask = ~(0B11111111<<gBitLength);
  uint8_t bMask = ~(0B11111111<<bBitLength);
  uint8_t aMask = ~(0B11111111<<aBitLength);
  pixelOut.red &= rMask;
  pixelOut.green &= gMask;
  pixelOut.blue &= bMask;
  pixelOut.alpha &= aMask;
  return pixelOut;
}


PixelStruct BetterImageStorageObject::applyPixelMask(PixelStruct pixelIn)
{
  PixelStruct pixelOut;
  pixelOut.red = pixelIn.red & PixelMask.redMask;
  pixelOut.green = pixelIn.green & PixelMask.greenMask;
  pixelOut.blue = pixelIn.blue & PixelMask.blueMask;
  pixelOut.alpha = pixelIn.alpha & PixelMask.alphaMask;
  return pixelOut;
}


uint32_t BetterImageStorageObject::ConvertPixelStruct_to_Binary(PixelStruct pixel)
{
  uint32_t pixelBinaryOut = 0B00000000000000000000000000000000;
  //If color bit lengths are less then 8 then the bits will be LSB
  //Example rBitLength = 5, gBitLength = 6, bBitLength = 5, aBitLength = 1
  //0Bxxxxxxxx xxxxxxxr rrrrgggg ggbbbbba
  PixelStruct pixelTemp = filterPixelValues(pixel, ChannelBitLength.get_redChannel(), ChannelBitLength.get_greenChannel(), ChannelBitLength.get_blueChannel(), ChannelBitLength.get_alphaChannel());
  pixelBinaryOut = pixelTemp.red<<(ChannelBitLength.get_greenChannel()+ChannelBitLength.get_blueChannel()+ChannelBitLength.get_alphaChannel()) | pixelTemp.green<<(ChannelBitLength.get_blueChannel()+ChannelBitLength.get_alphaChannel()) | pixelTemp.blue<<(ChannelBitLength.get_alphaChannel()) | pixelTemp.alpha;
  return pixelBinaryOut;
}





void BetterImageStorageObject::setPixelMask(PixelMaskStruct *pixelMaskInput){
  pixelMaskInput->redMask = ~(0B11111111 << ChannelBitLength.get_redChannel());
  pixelMaskInput->greenMask = ~(0B11111111 << ChannelBitLength.get_greenChannel());
  pixelMaskInput->blueMask = ~(0B11111111 << ChannelBitLength.get_blueChannel());
  if (alphaChannelFlag){
    pixelMaskInput->alphaMask = ~(0B11111111 << ChannelBitLength.get_alphaChannel());
  }else{
    pixelMaskInput->alphaMask = 0B11111111;//solid
  }
}

uint8_t BetterImageStorageObject::mean3input8Bit(uint8_t i0, uint8_t i1, uint8_t i2)
{
  uint32_t sum = (i0+i1+i2);
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
      virtual uint32_t getArea() = 0;
      void setWidth(uint32_t w) {
         width = w;
      }
   
      void setHeight(uint32_t h) {
         height = h;
      }
   
   protected:
      uint32_t width;
      uint32_t height;
};
 
// Derived classes
class Rectangle: public Shape {
   public:
      uint32_t getArea() { 
         return (width * height); 
      }
};

class Triangle: public Shape {
   public:
      uint32_t getArea() { 
         return (width * height)/2; 
      }
};



//#################################################


 */

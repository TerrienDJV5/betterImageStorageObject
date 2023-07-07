//By Terrien-DJV5
//BetterImageStorageObject.hpp
//Version CodeName Template
//Date Created: jun 29 2023 6:48PM

//https://cplusplus.com/reference/


#ifndef BETTERIMAGESTORAGEOBJECT_HPP
#define BETTERIMAGESTORAGEOBJECT_HPP
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>

#include <iostream>

#include <cstdint>
#include <cinttypes>
#include <stdlib.h>

#include <stdlib.h>

#include <bitset>




#define BImgSObj_IFlag_Grayscale 0B01000000
#define BImgSObj_IFlag_AlphaChannel 0B10000000



enum IFlags{
  RGB_NoAlpha, Grayscale_NoAlpha, RGB_Alpha, Grayscale_Alpha
};

/*
enum smallenum: std::uint32_t
{
    a,
    b,
    c
};
//*/




typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t alpha;
} PixelStruct;


class RGBAChannelDataStorage{
  private:
    uint8_t _ChannelData[4];
    uint8_t maxLength = 4;
  public:
    uint8_t get_Channel(uint8_t cIndex){
      return _ChannelData[cIndex % maxLength];
    }
    void set_Channel(uint8_t cIndex, uint8_t newValue){
      _ChannelData[cIndex % maxLength] = newValue;
    }
    virtual uint8_t get_redChannel() = 0;
    virtual uint8_t get_greenChannel() = 0;
    virtual uint8_t get_blueChannel() = 0;
    virtual uint8_t get_alphaChannel() = 0;
    virtual void set_redChannel(uint8_t inputValue) = 0;
    virtual void set_greenChannel(uint8_t inputValue) = 0;
    virtual void set_blueChannel(uint8_t inputValue) = 0;
    virtual void set_alphaChannel(uint8_t inputValue) = 0;
};

class BitLengthStorage : public RGBAChannelDataStorage{
  public:
    uint8_t get_redChannel(){return get_Channel(0);};
    uint8_t get_greenChannel(){return get_Channel(1);};
    uint8_t get_blueChannel(){return get_Channel(2);};
    uint8_t get_alphaChannel(){return get_Channel(3);};
    void set_redChannel(uint8_t inputValue){set_Channel(0, inputValue);};
    void set_greenChannel(uint8_t inputValue){set_Channel(1, inputValue);};
    void set_blueChannel(uint8_t inputValue){set_Channel(2, inputValue);};
    void set_alphaChannel(uint8_t inputValue){set_Channel(3, inputValue);};
};
class BitOffsetStorage : public RGBAChannelDataStorage{
  public:
    uint8_t get_redChannel(){return get_Channel(0);};
    uint8_t get_greenChannel(){return get_Channel(1);};
    uint8_t get_blueChannel(){return get_Channel(2);};
    uint8_t get_alphaChannel(){return get_Channel(3);};
    void set_redChannel(uint8_t inputValue){set_Channel(0, inputValue);};
    void set_greenChannel(uint8_t inputValue){set_Channel(1, inputValue);};
    void set_blueChannel(uint8_t inputValue){set_Channel(2, inputValue);};
    void set_alphaChannel(uint8_t inputValue){set_Channel(3, inputValue);};
};






//fix Shit Make this Work and Clean up Code!

class BetterImageStorageObjectBase{
  public:
    //imageStartModeType imagemode = grayscale|alphaChannel;
    bool begin(uint32_t w, uint32_t h, uint8_t inputValue0);//grayscale, No alpha
    bool begin(uint32_t w, uint32_t h, uint8_t inputValue0, uint8_t inputValue1);//grayscale, with alpha
    bool begin(uint32_t w, uint32_t h, uint8_t inputValue0, uint8_t inputValue1, uint8_t inputValue2);//RGB, No alpha
    bool begin(uint32_t w, uint32_t h, uint8_t inputValue0, uint8_t inputValue1, uint8_t inputValue2, uint8_t inputValue3);//RGB, with alpha
  private:
    bool begin_Base(uint32_t w, uint32_t h, uint8_t inputValue0, uint8_t inputValue1, uint8_t inputValue2, uint8_t inputValue3);
  private:
    typedef struct {
      uint8_t redMask;
      uint8_t greenMask;
      uint8_t blueMask;
      uint8_t alphaMask;
    } PixelMaskStruct;
    void setPixelMask( PixelMaskStruct *MaskStruct);
    
  public:
    uint16_t getWidth();
    uint16_t getHeight();
    uint32_t getPixelCount();
    PixelStruct getPixelValue(uint32_t xPos, uint32_t yPos);
    void setPixelValue(uint32_t xPos, uint32_t yPos, PixelStruct pixel);
    
  public://private:
    void createImageFunc(uint32_t w, uint32_t h, uint8_t redChannelBitLength, uint8_t greenBitLength, uint8_t blueBitLength, uint8_t alphaBitLength);
    uint32_t bitSelect_i32o32(uint32_t input, uint32_t offset, uint32_t length);
    
    uint64_t arraybitSelect_o64(uint8_t inputArray[], uint32_t bitIndex);
    uint32_t arraybitSelect_o32(uint8_t inputArray[], uint32_t bitIndex);
    void uint32_convert_to_4xuint8Array(uint8_t inputArray[], uint32_t input);
    void byteArray_bitWrite_i32(uint8_t inputArray[], uint32_t input, uint64_t bitoffset, uint8_t inputbitlength);
    
    PixelStruct filterPixelValues(PixelStruct pixelIn, uint8_t rBitLength, uint8_t gBitLength, uint8_t bBitLength, uint8_t aBitLength);
    PixelStruct applyPixelMask(PixelStruct pixelIn);
    uint32_t ConvertPixelStruct_to_Binary(PixelStruct pixel);
    uint8_t mean3input8Bit(uint8_t i0, uint8_t i1, uint8_t i2);
    
  private:
    PixelMaskStruct PixelMask;
    
    BitLengthStorage ChannelBitLength;
    BitOffsetStorage ChannelBitOffset;
    
    //how/where image is stored im memory
    uint8_t* _data;//Array
    uint8_t _Data_PixelBitLength;//Max is 32
    uint64_t _Data_ByteLength;//ArrayLength
    
    
  protected:
    uint8_t _imageModeFlags;
    bool grayscaleFlag;
    bool alphaChannelFlag;
    uint32_t width;
    uint32_t height;
    
};

class BetterImageStorageObject : public BetterImageStorageObjectBase{
  public:
    BetterImageStorageObject( bool grayscaleFlag, bool alphaChannelFlag );
};

BetterImageStorageObject::BetterImageStorageObject(bool grayscaleFlag, bool alphaChannelFlag)
{
  this->grayscaleFlag = grayscaleFlag;
  this->alphaChannelFlag = alphaChannelFlag;
  this->_imageModeFlags = 0B00000000;
  if (this->grayscaleFlag){
    this->_imageModeFlags |= BImgSObj_IFlag_Grayscale;
  }
  if (this->alphaChannelFlag){
    this->_imageModeFlags |= BImgSObj_IFlag_AlphaChannel;
  }
}







#include <stdio.h>

template< unsigned int MaxWidth, unsigned int MaxHeight, unsigned int MaxPixelBitWidth, unsigned int MaxChannelCount >
class BetterImageStorageObjectTemplate{
public:
  bool configChannelBitSize(int num, ...) {
    int valu = num;
    //Declare a va_list macro and initialize it with va_start
    va_list argList;
    va_start(argList, num);
    for (byte index = 0; index < MaxChannelCount; index++) {
      _channel_DataBitLength[ index ] = valu;
      Serial.print("valu: ");
      Serial.println(valu, DEC);
      valu = va_arg(argList, int);
    }
    va_end(argList);
    return true;
  }
  bool configChanName(const char channelString[]) {
    for (byte i = 0; i < sizeof(channelString); i++) {
      Serial.print(i, DEC);
      Serial.print(" = ");
      Serial.write(channelString[i]);
      Serial.println();
    }
    uint8_t channel_R = 0;
    uint8_t channel_G = 0;
    uint8_t channel_B = 0;
    uint8_t channel_A = 0;
    
    char *p_R, *p_G, *p_B, *p_A, *p_r, *p_g, *p_b, *p_a;
    char *p_Rr = NULL, *p_Gg = NULL, *p_Bb = NULL, *p_Aa = NULL;
    p_R = strstr(channelString, "R");p_G = strstr(channelString, "G");p_B = strstr(channelString, "B");p_A = strstr(channelString, "A");
    p_r = strstr(channelString, "r");p_g = strstr(channelString, "g");p_b = strstr(channelString, "b");p_a = strstr(channelString, "a");
    if (p_R and !p_r){p_Rr = p_R;}else if (!p_R and p_r){p_Rr = p_r;}else {p_Rr = NULL;};
    if (p_G and !p_g){p_Gg = p_G;}else if (!p_G and p_g){p_Gg = p_g;}else {p_Gg = NULL;};
    if (p_B and !p_b){p_Bb = p_B;}else if (!p_B and p_b){p_Bb = p_b;}else {p_Bb = NULL;};
    if (p_A and !p_a){p_Aa = p_A;}else if (!p_A and p_a){p_Aa = p_a;}else {p_Aa = NULL;};
    if (p_Rr and p_Gg and p_Bb and p_Aa){
      this->baseIMGOBJ.begin(MaxHeight, MaxWidth, channel_R, channel_G, channel_B, channel_A);
    }
    return true;
  }
  uint16_t getWidth(){return width;}
  uint16_t getHeight(){return height;}
  uint32_t getPixelCount(){return (width * height);}
  void printChannelBitLengths(Stream &serial){
    for (byte index = 0; index < MaxChannelCount; index++) {
      serial.print("channel<");serial.print(index);serial.print(">: ");serial.println(_channel_DataBitLength[ index ], DEC);
    }
  }
  PixelStruct getPixelValue(uint32_t xPos, uint32_t yPos)
  {
    PixelStruct pixel;
    
    return pixel;
  }
  void setPixelValue(uint32_t xPos, uint32_t yPos, PixelStruct pixel)
  {
    printf(" set pixel value %0d,%0d,%0d,%0d \n", pixel.red, pixel.green, pixel.blue, pixel.alpha);
    uint64_t pixelBitindex = (xPos + yPos*width)*(_Data_PixelBitLength);
    uint32_t pixelValue = baseIMGOBJ.ConvertPixelStruct_to_Binary( pixel );
    std::bitset<32> z(pixelValue);
    std::cout << "set " << z << '\n';
    //make me work
    baseIMGOBJ.byteArray_bitWrite_i32(_data, pixelValue, pixelBitindex, _Data_PixelBitLength);
  }
private:
  BetterImageStorageObjectBase baseIMGOBJ;
  uint32_t width = MaxWidth;
  uint32_t height = MaxHeight;
  unsigned int _MaxPixelBitWidth = MaxPixelBitWidth;
  unsigned int _MaxChannelCount = MaxChannelCount;
  
  BitLengthStorage ChannelBitLength;
  BitOffsetStorage ChannelBitOffset;
  uint8_t _channel_DataBitLength[ MaxChannelCount ];//sum of all values must be equal to "MaxPixelBitWidth"
  uint8_t _data[ (int)((MaxWidth * MaxHeight * MaxPixelBitWidth)/8)+1 ];//Array
  uint8_t _Data_PixelBitLength;//Max is 32
  uint64_t _Data_ByteLength;//ArrayLength
};











bool BetterImageStorageObjectBase::begin(uint32_t w, uint32_t h, uint8_t inputValue0)
{
  return begin_Base(w, h, inputValue0, 0, 0, 0);
}
bool BetterImageStorageObjectBase::begin(uint32_t w, uint32_t h, uint8_t inputValue0, uint8_t inputValue1)
{
  return begin_Base(w, h, inputValue0, inputValue1, 0, 0);
}
bool BetterImageStorageObjectBase::begin(uint32_t w, uint32_t h, uint8_t inputValue0, uint8_t inputValue1, uint8_t inputValue2)
{
  return begin_Base(w, h, inputValue0, inputValue1, inputValue2, 0);
}
bool BetterImageStorageObjectBase::begin(uint32_t w, uint32_t h, uint8_t inputValue0, uint8_t inputValue1, uint8_t inputValue2, uint8_t inputValue3)
{
  return begin_Base(w, h, inputValue0, inputValue1, inputValue2, inputValue3);
}

bool BetterImageStorageObjectBase::begin_Base(uint32_t w, uint32_t h, uint8_t inputValue0, uint8_t inputValue1, uint8_t inputValue2, uint8_t inputValue3)
{
  uint8_t redBitLength = 0;
  uint8_t greenBitLength = 0;
  uint8_t blueBitLength = 0;
  uint8_t alphaBitLength = 0;
  uint8_t imageModeFlags;
  /*
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
    case (BImgSObj_IFlag_Grayscale|BImgSObj_IFlag_AlphaChannel)  : {
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
  //*/
  
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


void BetterImageStorageObjectBase::createImageFunc(uint32_t w, uint32_t h, uint8_t firstBitLength, uint8_t secondBitLength, uint8_t blueBitLength, uint8_t alphaBitLength)
{
  width = w;
  height = h;
  
  
  if (grayscaleFlag){
    _Data_PixelBitLength = firstBitLength;
    ChannelBitLength.set_redChannel( firstBitLength );
    ChannelBitLength.set_greenChannel( firstBitLength );
    ChannelBitLength.set_blueChannel( firstBitLength );
    ChannelBitOffset.set_redChannel( 0 );
    ChannelBitOffset.set_greenChannel( 0 );
    ChannelBitOffset.set_blueChannel( 0 );
    ChannelBitOffset.set_alphaChannel( 0 );
  }else{
    _Data_PixelBitLength = firstBitLength+secondBitLength+blueBitLength;
    ChannelBitLength.set_redChannel( firstBitLength );
    ChannelBitLength.set_greenChannel( secondBitLength );
    ChannelBitLength.set_blueChannel( blueBitLength );
    
    ChannelBitOffset.set_redChannel( 0 );
    ChannelBitOffset.set_greenChannel( ChannelBitOffset.get_redChannel() + ChannelBitLength.get_redChannel() );
    ChannelBitOffset.set_blueChannel( ChannelBitOffset.get_greenChannel() + ChannelBitLength.get_greenChannel() );
    
    if (alphaChannelFlag){
      ChannelBitLength.set_alphaChannel( alphaBitLength );
      ChannelBitOffset.set_alphaChannel( ChannelBitOffset.get_blueChannel() + ChannelBitLength.get_blueChannel() );
    }
    
  }
  if (alphaChannelFlag){
    _Data_PixelBitLength += (alphaBitLength);
  }
  
  setPixelMask( &PixelMask );

  
  uint64_t dataBitLength = ((width*height)*_Data_PixelBitLength);
  uint32_t fullByteCount = (dataBitLength - (dataBitLength%8)) / 8;
  bool nonfullBytePresent = ((dataBitLength%8) != 0);
  
  
  _Data_ByteLength = fullByteCount + 1*(nonfullBytePresent==true);
  _data = (uint8_t*)calloc(_Data_ByteLength, sizeof(uint8_t));
  
  printf("Lengths %0d,%0d,%0d,%0d \n", ChannelBitLength.get_redChannel(), ChannelBitLength.get_greenChannel(), ChannelBitLength.get_blueChannel(), ChannelBitLength.get_alphaChannel());
  printf("Offsets %0d,%0d,%0d,%0d \n", ChannelBitOffset.get_redChannel(), ChannelBitOffset.get_greenChannel(), ChannelBitOffset.get_blueChannel(), ChannelBitOffset.get_alphaChannel());
  
}



uint32_t BetterImageStorageObjectBase::bitSelect_i32o32(uint32_t input, uint32_t offset, uint32_t length)
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

uint64_t BetterImageStorageObjectBase::arraybitSelect_o64(uint8_t inputArray[], uint32_t bitIndex)
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
  output = ((uint64_t)outputList[0]<<(56))|((uint64_t)outputList[1]<<(48))|((uint64_t)outputList[2]<<(40))|((uint64_t)outputList[3]<<(32))|(outputList[4]<<(24))|(outputList[5]<<(16))|(outputList[6]<<(8))|(outputList[7]<<(0));
  return output;
}

uint32_t BetterImageStorageObjectBase::arraybitSelect_o32(uint8_t inputArray[], uint32_t bitIndex)
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
  output = (outputList[0]<<(24))|(outputList[1]<<(16))|(outputList[2]<<(8))|(outputList[3]<<(0));
  return output;
}

void BetterImageStorageObjectBase::uint32_convert_to_4xuint8Array(uint8_t inputArray[], uint32_t input)
{
  //inputArray must be atleast 4 items
  inputArray[0] = (uint8_t)(input>>(24));
  inputArray[1] = (uint8_t)(input>>(16));
  inputArray[2] = (uint8_t)(input>>(8));
  inputArray[3] = (uint8_t)(input>>(0));
}


void BetterImageStorageObjectBase::byteArray_bitWrite_i32(uint8_t inputArray[], uint32_t input, uint64_t bitoffset, uint8_t inputbitlength)
{
  printf("inputbitlength %d\n", inputbitlength);
  uint32_t byteIndex = ((bitoffset-(bitoffset%8))>>3);
  uint8_t bufferBitOffset = (bitoffset%8);
  uint8_t bufferBitStartMask = ~(0B11111111 << bufferBitOffset);
  uint8_t bufferBitEndMask = (0B11111111 << bufferBitOffset);
  uint32_t inputMask = ~(0B11111111111111111111111111111111<<(inputbitlength));
  input &= inputMask;
  //input = input>>(32 - inputbitlength);//00001111 -> 11110000
  input &= inputMask;
  
  //shift buffer right for bitoffset
  uint8_t bufferList[8];
  uint8_t bufferMaskList[8];
  //                       ||||||||||||||||||||||||||||||||||||||||xxxxxxxxxxxxxxxxxxxxxxxx
  uint64_t bufferValue = 0B0000000000000000000000000000000000000000000000000000000000000000;
  uint64_t bufferMask = ~((uint64_t)(~inputMask)<<32);
  bufferValue = ((uint64_t)input<<32)>>bufferBitOffset;
  bufferValue &= bufferMask;
  
  
  //uint32_convert_to_4xuint8Array(&bufferMaskList[0], ~(bufferMask>>32)&0b0000000000000000000000000000000011111111111111111111111111111111);
  //uint32_convert_to_4xuint8Array(&bufferMaskList[4], ~(bufferMask)&0b0000000000000000000000000000000011111111111111111111111111111111);
  ///*
  bufferMaskList[0] = (~bufferMask)>>(64-8);
  bufferMaskList[1] = (~bufferMask)>>(64-16);
  bufferMaskList[2] = (~bufferMask)>>(64-24);
  bufferMaskList[3] = (~bufferMask)>>(64-32);
  bufferMaskList[4] = (~bufferMask)>>(64-40);
  bufferMaskList[5] = (~bufferMask)>>(64-48);//likely unused
  bufferMaskList[6] = (~bufferMask)>>(64-56);//likely unused
  bufferMaskList[7] = (~bufferMask)>>(64-64);//likely unused
  //*/
  //uint32_convert_to_4xuint8Array(&bufferList[0], (bufferValue>>32)&0b0000000000000000000000000000000011111111111111111111111111111111);
  //uint32_convert_to_4xuint8Array(&bufferList[4], (bufferValue)&0b0000000000000000000000000000000011111111111111111111111111111111);
  ///*
  bufferList[0] = bufferValue>>(64-8);
  bufferList[1] = bufferValue>>(64-16);
  bufferList[2] = bufferValue>>(64-24);
  bufferList[3] = bufferValue>>(64-32);
  bufferList[4] = bufferValue>>(64-40);
  bufferList[5] = bufferValue>>(64-48);//likely unused
  bufferList[6] = bufferValue>>(64-56);//likely unused
  bufferList[7] = bufferValue>>(64-64);//likely unused
  //*/
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


uint16_t BetterImageStorageObjectBase::getWidth()
{
  return width;
}
uint16_t BetterImageStorageObjectBase::getHeight()
{
  return height;
}
uint32_t BetterImageStorageObjectBase::getPixelCount()
{
  return (width * height);
}


PixelStruct BetterImageStorageObjectBase::getPixelValue(uint32_t xPos, uint32_t yPos)
{
  PixelStruct pixel;
  uint32_t pixelBitindex = (xPos + yPos*width)*(_Data_PixelBitLength);
  uint32_t pixelindex = pixelBitindex/8;
  uint32_t pixelData = arraybitSelect_o32(_data, pixelBitindex);
  pixelData = pixelData & (~(0B11111111111111111111111111111111 << _Data_PixelBitLength));
  pixel.red = bitSelect_i32o32(pixelData, ChannelBitOffset.get_redChannel(), ChannelBitLength.get_redChannel());
  pixel.green = bitSelect_i32o32(pixelData, ChannelBitOffset.get_greenChannel(), ChannelBitLength.get_greenChannel());
  pixel.blue = bitSelect_i32o32(pixelData, ChannelBitOffset.get_blueChannel(), ChannelBitLength.get_blueChannel());
  pixel.alpha = bitSelect_i32o32(pixelData, ChannelBitOffset.get_alphaChannel(), ChannelBitLength.get_alphaChannel());
  return pixel;
}

void BetterImageStorageObjectBase::setPixelValue(uint32_t xPos, uint32_t yPos, PixelStruct pixel)
{
  printf(" set pixel value %0d,%0d,%0d,%0d \n", pixel.red, pixel.green, pixel.blue, pixel.alpha);
  uint64_t pixelBitindex = (xPos + yPos*width)*(_Data_PixelBitLength);
  uint32_t pixelValue = ConvertPixelStruct_to_Binary( pixel );
  std::bitset<32> z(pixelValue);
  std::cout << "set " << z << '\n';
  //make me work
  byteArray_bitWrite_i32(_data, pixelValue, pixelBitindex, _Data_PixelBitLength);
}







PixelStruct BetterImageStorageObjectBase::filterPixelValues(PixelStruct pixelIn, uint8_t rBitLength, uint8_t gBitLength, uint8_t bBitLength, uint8_t aBitLength)
{
  PixelStruct pixelOut;
  printf("filter in %0d,%0d,%0d,%0d \n", pixelIn.red, pixelIn.green, pixelIn.blue, pixelIn.alpha);
  pixelOut.red = pixelIn.red;
  pixelOut.green = pixelIn.green;
  pixelOut.blue = pixelIn.blue;
  pixelOut.alpha = pixelIn.alpha;
  uint8_t rMask = (0B11111111<<(8-rBitLength));
  uint8_t gMask = (0B11111111<<(8-gBitLength));
  uint8_t bMask = (0B11111111<<(8-bBitLength));
  uint8_t aMask = (0B11111111<<(8-aBitLength));
  printf("filterMasks %0d,%0d,%0d,%0d \n", rMask, gMask, bMask, aMask);
  printf("filterLengths %0d,%0d,%0d,%0d \n", rBitLength, gBitLength, bBitLength, aBitLength);
  pixelOut.red &= rMask;
  pixelOut.green &= gMask;
  pixelOut.blue &= bMask;
  pixelOut.alpha &= aMask;
  printf("filterOut %0d,%0d,%0d,%0d \n", pixelOut.red, pixelOut.green, pixelOut.blue, pixelOut.alpha);
  return pixelOut;
}


PixelStruct BetterImageStorageObjectBase::applyPixelMask(PixelStruct pixelIn)
{
  pixelIn.red &= PixelMask.redMask;
  pixelIn.green &= PixelMask.greenMask;
  pixelIn.blue &= PixelMask.blueMask;
  pixelIn.alpha &= PixelMask.alphaMask;
  return pixelIn;
}


uint32_t BetterImageStorageObjectBase::ConvertPixelStruct_to_Binary(PixelStruct pixel)
{
  printf("%0d,%0d,%0d,%0d \n", pixel.red, pixel.green, pixel.blue, pixel.alpha);
  uint32_t pixelBinaryOut = 0B00000000000000000000000000000000;
  //If color bit lengths are less then 8 then the bits will be LSB
  //Example rBitLength = 5, gBitLength = 6, bBitLength = 5, aBitLength = 1
  //0Bxxxxxxxx xxxxxxxr rrrrgggg ggbbbbba
  PixelStruct pixelTemp = filterPixelValues(pixel, ChannelBitLength.get_redChannel(), ChannelBitLength.get_greenChannel(), ChannelBitLength.get_blueChannel(), ChannelBitLength.get_alphaChannel());
  //PixelStruct pixelTemp = applyPixelMask(pixel);
  uint32_t total = (ChannelBitLength.get_redChannel() + ChannelBitLength.get_greenChannel()+ChannelBitLength.get_blueChannel()+ChannelBitLength.get_alphaChannel());
  
  pixelBinaryOut = pixelTemp.red << (ChannelBitLength.get_redChannel() + ChannelBitOffset.get_redChannel());
  pixelBinaryOut |= pixelTemp.green << (ChannelBitLength.get_greenChannel() + ChannelBitOffset.get_greenChannel());
  pixelBinaryOut |= pixelTemp.blue << (ChannelBitLength.get_blueChannel() + ChannelBitOffset.get_blueChannel());
  pixelBinaryOut |= pixelTemp.alpha << (ChannelBitLength.get_alphaChannel() + ChannelBitOffset.get_alphaChannel());
  printf("%0d,%0d,%0d,%0d \n", pixelTemp.red, pixelTemp.green, pixelTemp.blue, pixelTemp.alpha);
  
  std::bitset<32> z(pixelBinaryOut);
  std::cout << "pixelBinaryOut " << z << '\n';
  return pixelBinaryOut;
}





void BetterImageStorageObjectBase::setPixelMask( PixelMaskStruct *MaskStruct )
{
  MaskStruct->redMask = ~(0B11111111 << ChannelBitLength.get_redChannel());
  MaskStruct->greenMask = ~(0B11111111 << ChannelBitLength.get_greenChannel());
  MaskStruct->blueMask = ~(0B11111111 << ChannelBitLength.get_blueChannel());
  MaskStruct->alphaMask = ~(0B11111111 << ChannelBitLength.get_alphaChannel());
}

uint8_t BetterImageStorageObjectBase::mean3input8Bit(uint8_t i0, uint8_t i1, uint8_t i2)
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



#endif //BETTERIMAGESTORAGEOBJECT_HPP

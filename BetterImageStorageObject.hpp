//By Terrien-DJV5
//BetterImageStorageObject.hpp
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

//230209
//22124

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





class BetterImageStorageObject{
  public:
    BetterImageStorageObject( bool grayscaleFlag, bool alphaChannelFlag );
    //imageStartModeType imagemode = grayscale|alphaChannel;
    bool begin(uint32_t w, uint32_t h, uint8_t inputValue0);
    bool begin(uint32_t w, uint32_t h, uint8_t inputValue0, uint8_t inputValue1);
    bool begin(uint32_t w, uint32_t h, uint8_t inputValue0, uint8_t inputValue1, uint8_t inputValue2);
    bool begin(uint32_t w, uint32_t h, uint8_t inputValue0, uint8_t inputValue1, uint8_t inputValue2, uint8_t inputValue3);
  private:
    typedef struct {
      uint8_t redMask;
      uint8_t greenMask;
      uint8_t blueMask;
      uint8_t alphaMask;
    } PixelMaskStruct;
    void setPixelMask( PixelMaskStruct *MaskStruct);
    
  public:
    //how/where image is stored im memory
    typedef struct {
      uint8_t* data;//Array
      uint8_t pixelBitLength;//Max is 32
      uint64_t dataByteLength;//ArrayLength
      
    } PixelArrayStruct;
    
    uint16_t getWidth();
    uint16_t getHeight();
    uint32_t getPixelCount();
    
  public:
    PixelStruct getPixelValue(uint32_t xPos, uint32_t yPos);
    void setPixelValue(uint32_t xPos, uint32_t yPos, PixelStruct pixel);
    
  private:
    bool begin_Base(uint32_t w, uint32_t h, uint8_t inputValue0, uint8_t inputValue1, uint8_t inputValue2, uint8_t inputValue3);
    
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
    uint8_t _imageModeFlags;
    bool grayscaleFlag;
    bool alphaChannelFlag;
    PixelMaskStruct PixelMask;
    PixelArrayStruct PixelArray;
    
    BitLengthStorage ChannelBitLength;
    BitOffsetStorage ChannelBitOffset;
  protected:
    uint32_t width;
    uint32_t height;
    
};


#include "BetterImageStorageObject.cpp"



#endif //BETTERIMAGESTORAGEOBJECT_HPP



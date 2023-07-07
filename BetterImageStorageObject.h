//By Terrien-DJV5
//BetterImageStorageObject.hpp
//Date Created: jun 29 2023 6:48PM

//https://cplusplus.com/reference/


#ifndef MY_BETTERIMAGESTORAGEOBJECT_H
#define MY_BETTERIMAGESTORAGEOBJECT_H
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>



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


class BetterImageStorageObject
{
  private:
    typedef struct {
      uint16_t channelDataValues;
      //rrrgggbbbaaa0000
      enum {
        redOffset = 0,
        greenOffset = 3,
        blueOffset = 6,
        alphaOffset = 9,
      };
      // function pointer
      uint8_t get_Channel(uint8_t offset, uint8_t length)
      {
        length &= 0B00000111;//Max is 8
        offset &= 0B00001111;//Max is 16
        return (uint8_t)(((channelDataValues&((~(uint16_t)(0B1111111111111111<<(uint8_t)(16-length)))>>offset))<<offset>>(uint8_t)(16-length))&0B0000000011111111);
      }
      virtual uint8_t get_redChannel();
      virtual uint8_t get_greenChannel();
      virtual uint8_t get_blueChannel();
      virtual uint8_t get_alphaChannel();
      
      void set_Channel(uint8_t inputValue, uint8_t offset, uint8_t length){
        length &= 0B00000111;//Max is 8
        offset &= 0B00001111;//Max is 16
        inputValue &= ~(0B11111111<<length);
        channelDataValues |= ((uint16_t)inputValue<<(uint8_t)(16-length)>>(uint8_t)(offset));
      }
      virtual void set_redChannel(uint8_t inputValue);
      virtual void set_greenChannel(uint8_t inputValue);
      virtual void set_blueChannel(uint8_t inputValue);
      virtual void set_alphaChannel(uint8_t inputValue);
    } ChannelDataStorageStruct;

    typedef struct ChannelDataStorageStruct : ChannelDataStorageStruct{
      uint8_t get_redChannel(){return get_Channel(0, 3);};
      uint8_t get_greenChannel(){return get_Channel(3, 3);};
      uint8_t get_blueChannel(){return get_Channel(6, 3);};
      uint8_t get_alphaChannel(){return get_Channel(9, 3);};
      uint8_t get_extraNibbleValue(){return get_Channel(12, 4);};
      
      void set_redChannel(uint8_t inputValue){set_Channel(inputValue, 0, 3);};
      void set_greenChannel(uint8_t inputValue){set_Channel(inputValue, 3, 3);};
      void set_blueChannel(uint8_t inputValue){set_Channel(inputValue, 6, 3);};
      void set_alphaChannel(uint8_t inputValue){set_Channel(inputValue, 9, 3);};
      void set_extraNibbleValue(uint8_t inputValue){set_Channel(inputValue, 12, 4);};
    } ChannelBitLengthStorageStruct;
    
    typedef struct ChannelDataStorageStruct : ChannelDataStorageStruct{
      uint8_t get_redChannel(){return get_Channel(0, 1);};//red offset is always 0
      uint8_t get_greenChannel(){return get_Channel(1, 5);};//max value = 31, min value = 0
      uint8_t get_blueChannel(){return get_Channel(6, 5);};//max value = 31, min value = 0
      uint8_t get_alphaChannel(){return get_Channel(11, 5);};//max value = 31, min value = 0
      
      void set_redChannel(uint8_t inputValue){set_Channel(inputValue, 0, 1);};//red offset is always 0
      void set_greenChannel(uint8_t inputValue){set_Channel(inputValue, 1, 5);};//max value = 31, min value = 0
      void set_blueChannel(uint8_t inputValue){set_Channel(inputValue, 6, 5);};//max value = 31, min value = 0
      void set_alphaChannel(uint8_t inputValue){set_Channel(inputValue, 11, 5);};//max value = 31, min value = 0
    } ChannelBitOffsetStorageStruct;
    
    typedef struct {
      uint8_t redMask;
      uint8_t greenMask;
      uint8_t blueMask;
      uint8_t alphaMask;
    } PixelMaskStruct;
    void setPixelMask(PixelMaskStruct *pixelMaskInput); 
  public:
    //how/where image is stored im memory
    typedef struct {
      unsigned char* data;//Array
      uint8_t pixelBitLength;//Max is 32
      uint64_t dataByteLength;//ArrayLength
      
    } PixelArrayStruct;
  public:
    BetterImageStorageObject( bool grayscaleFlag, bool alphaChannelFlag );
    //imageStartModeType imagemode = grayscale|alphaChannel;
    bool begin(uint32_t w, uint32_t h, uint8_t inputValue0);
    bool begin(uint32_t w, uint32_t h, uint8_t inputValue0, uint8_t inputValue1);
    bool begin(uint32_t w, uint32_t h, uint8_t inputValue0, uint8_t inputValue1, uint8_t inputValue2);
    bool begin(uint32_t w, uint32_t h, uint8_t inputValue0, uint8_t inputValue1, uint8_t inputValue2, uint8_t inputValue3);
  private:
    bool begin_Base(uint32_t w, uint32_t h, uint8_t inputValue0, uint8_t inputValue1, uint8_t inputValue2, uint8_t inputValue3);
  public:
    uint8_t getWidth();
    uint8_t getHeight();
    uint16_t getPixelCount();
  private:
    PixelStruct setPixelStructValues_Base(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
  public:
    PixelStruct setPixelStructValues(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    PixelStruct setPixelStructValues(uint8_t w, uint8_t a = 255);
  public:
    PixelStruct getPixelValue(uint32_t xPos, uint32_t yPos);
    void setPixelValue(uint32_t xPos, uint32_t yPos, PixelStruct pixel);
  private:
    void createImageFunc(uint32_t w, uint32_t h, uint8_t redChannelBitLength, uint8_t greenBitLength, uint8_t blueBitLength, uint8_t alphaBitLength);
    uint32_t bitSelect_i32o32(uint32_t input, uint32_t offset, uint32_t length);
    uint64_t arraybitSelect_o64(uint8_t inputArray[], uint32_t bitIndex);
    uint32_t arraybitSelect_o32(uint8_t inputArray[], uint32_t bitIndex);
    void uint32_convert_to_4xuint8Array(uint8_t inputArray[], uint32_t input);
    void byteArray_bitWrite_i32(uint8_t inputArray[], uint32_t input, uint64_t bitoffset, uint8_t inputbitlength);
    PixelStruct filterPixelValues(PixelStruct pixelIn, uint8_t rBitLength, uint8_t gBitLength, uint8_t bBitLength, uint8_t aBitLength);
    PixelStruct applyPixelMask(PixelStruct pixelIn);
    uint32_t ConvertPixelStruct_to_Binary(PixelStruct pixel);
  private:
    uint8_t mean3input8Bit(uint8_t i0, uint8_t i1, uint8_t i2);
  private:
    uint8_t _imageModeFlags;
    bool grayscaleFlag;
    bool alphaChannelFlag;
    //MSB -> LSB
    uint8_t _redBitOffset;
    uint8_t _greenBitOffset;
    uint8_t _blueBitOffset;
    uint8_t _alphaBitOffset;
    ChannelBitOffsetStorageStruct ChannelBitOffset;
    ChannelBitLengthStorageStruct ChannelBitLength;
    PixelMaskStruct PixelMask;
    PixelArrayStruct PixelArray;
    
    
  protected:
    uint32_t width;
    uint32_t height;
    
};



#endif

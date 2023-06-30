//By Terrien-DJV5
//BetterImageStorageObject.h
//Date Created: jun 29 2023 6:48PM


#ifndef MY_BETTERIMAGESTORAGEOBJECT_H
#define MY_BETTERIMAGESTORAGEOBJECT_H
#include <Arduino.h>

typedef struct {
  unsigned char white;
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  unsigned char alpha;
} PixelStruct;

class ImageObject {
  public:
    //how/where image is stored im memory
    typedef struct {
      unsigned char* data;//Array
      unsigned char pixelBitLength;//Max is 32
      unsigned long int dataByteLength;//ArrayLength
      
      
      unsigned char redBitLength;//Max is 8
      unsigned char greenBitLength;//Max is 8
      unsigned char blueBitLength;//Max is 8
      unsigned char alphaBitLength;//Max is 8
    } PixelArrayStruct;
  private:
    typedef struct {
      unsigned char redMask;
      unsigned char greenMask;
      unsigned char blueMask;
      unsigned char alphaMask;
    } PixelMaskStruct;
    void setPixelMask(PixelMaskStruct *pixelMaskInput);
  public:
    ImageObject(bool grayscaleFlag, bool alphaChannelFlag, int w, int h, uint8_t inputValue0 = -1, uint8_t inputValue1 = -1, uint8_t inputValue2 = -1, uint8_t inputValue3 = -1);
    uint8_t getWidth();
    uint8_t getHeight();
    unsigned short int getPixelCount();
  private:
    PixelStruct setPixelStructValues_Base(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
  public:
    PixelStruct setPixelStructValues(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
    PixelStruct setPixelStructValues(unsigned char w, unsigned char a = 255);
  public:
    PixelStruct getPixelValue(int xPos, int yPos);
    void setPixelValue(int xPos, int yPos, PixelStruct pixel);
  private:
    void createImageFunc(int w, int h, int redBitLength, int greenBitLength, int blueBitLength, int alphaBitLength);
    uint32_t bitSelect_i32o32(uint32_t input, unsigned int offset, unsigned int length);
    uint64_t arraybitSelect_o64(uint8_t inputArray[], unsigned int bitIndex);
    uint32_t arraybitSelect_o32(uint8_t inputArray[], unsigned int bitIndex);
    void uint32_convert_to_4xuint8Array(uint8_t inputArray[], uint32_t input);
    void byteArray_bitWrite_i32(uint8_t inputArray[], uint32_t input, uint64_t bitoffset, uint8_t inputbitlength);
    PixelStruct filterPixelValues(PixelStruct pixelIn, uint8_t rBitLength, uint8_t gBitLength, uint8_t bBitLength, uint8_t aBitLength);
    PixelStruct applyPixelMask(PixelStruct pixelIn);
    uint32_t ConvertPixelStruct_to_Binary(PixelStruct pixel);
  private:
    unsigned char mean3input8Bit(unsigned char i0, unsigned char i1, unsigned char i2);
  private:
    bool grayscaleFlag;
    bool alphaChannelFlag;
    PixelMaskStruct PixelsMask;
    PixelArrayStruct PixelArray;
  protected:
    unsigned int width;
    unsigned int height;
    //MSB -> LSB
    unsigned char redBitOffset;
    unsigned char greenBitOffset;
    unsigned char blueBitOffset;
    unsigned char alphaBitOffset;
};



#endif

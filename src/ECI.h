/*
    n     -->  Total number of images             32 bit unsigned int
    sizes -->  size of each image                 32 bit unsigned int

    WHAAA...   -->  W is width and H is Height    32 bit usigned int whole RGBA data
    WHBBB...   -->  W is width and H is Height
*/

#pragma once
#include <iostream>

struct ECIHeader
{
    int numbers;     // Number of imgs in file
    uint32_t *sizes; // Sizes of each img
    std::string *imgNames;
};

struct ECIImg
{
    int w;         // Width of Img
    int h;         // Height of Img
    int channels;  // numbers of channeles
    uint8_t *data; // Raw JPG data of Img
};

class ECI
{
public:
    void Write(const char *fileName);
    static ECI *Read(const char *fileName, bool withData = true);
    static ECIImg *ReadAt(const char *fileName, int pos, ECIHeader *newHead, bool withData = true);
    static void ReadImg(std::ifstream *fil, ECIImg *img, uint32_t filsize, bool withData = true);
    static void ReadHeader(std::ifstream *fil, ECIHeader *header);
    ECIHeader header;
    ECIImg *imgs;
};

uint32_t rgb(int r, int g, int b, int a);
char *intToRgba(const uint32_t val);
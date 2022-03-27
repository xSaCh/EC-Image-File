#include <fstream>
#include "ECI.h"

void ECI::Write(const char *fileName)
{
    std::ofstream fil(fileName, std::ios::binary);

    fil.write((char *)&(header.numbers), sizeof(header.numbers));
    fil.write((char *)(header.sizes), sizeof(header.sizes) * header.numbers);

    for (int i = 0; i < header.numbers; i++)
    {
        fil.write((char *)&(imgs[i].w), sizeof(imgs[i].w));
        fil.write((char *)&(imgs[i].h), sizeof(imgs[i].h));
        fil.write((char *)&(imgs[i].channels), sizeof(imgs[i].channels));
        // fil.write((char *)(imgs[i].data), sizeof(imgs[i].data) * header.sizes[i]);
        fil.write((char *)(imgs[i].data), sizeof(imgs[i].data[0]) * header.sizes[i]);
    }
    fil.close();
}

ECI *ECI::Read(const char *fileName)
{
    ECI *newEl = (ECI *)malloc(sizeof(ECI));
    std::ifstream fil(fileName, std::ios::binary);

    // Read number of images
    fil.read((char *)&(newEl->header.numbers), sizeof(newEl->header.numbers));

    newEl->imgs = (ECIImg *)malloc(sizeof(ECIImg) * newEl->header.numbers);
    newEl->header.sizes = (uint32_t *)malloc(sizeof(uint32_t) * newEl->header.numbers);
    // Read sizes of images
    fil.read((char *)(newEl->header.sizes), sizeof(newEl->header.sizes) * newEl->header.numbers);

    // Read all images
    for (int i = 0; i < newEl->header.numbers; i++)
    {
        newEl->imgs[i].data = (uint8_t *)malloc(sizeof(uint8_t) * newEl->header.sizes[i]);

        fil.read((char *)&(newEl->imgs[i].w), sizeof(newEl->imgs[i].w));
        fil.read((char *)&(newEl->imgs[i].h), sizeof(newEl->imgs[i].h));
        fil.read((char *)&(newEl->imgs[i].channels), sizeof(newEl->imgs[i].channels));
        fil.read((char *)(newEl->imgs[i].data), sizeof(newEl->imgs[i].data[0]) * newEl->header.sizes[i]);
    }

    fil.close();
    return newEl;
}
ECIImg *ECI::ReadAt(const char *fileName, int pos, ECIHeader *newHead)
{
    // ECIHeader *newHead = (ECIHeader *)malloc(sizeof(ECIHeader));
    std::ifstream fil(fileName, std::ios::binary);

    // Read number of images
    fil.read((char *)&(newHead->numbers), sizeof(newHead->numbers));

    if (pos >= newHead->numbers)
        return NULL;

    ECIImg *newImg = (ECIImg *)malloc(sizeof(ECIImg) * newHead->numbers);
    newHead->sizes = (uint32_t *)malloc(sizeof(uint32_t) * newHead->numbers);

    // Read sizes of images
    fil.read((char *)(newHead->sizes), sizeof(newHead->sizes) * newHead->numbers);

    // Read all images
    int offset = 0;
    for (int i = 0; i < pos; i++)
        offset += sizeof(ECIImg) - sizeof(newImg->data) + (newHead->sizes[i] * sizeof(uint8_t));

    fil.seekg(offset, std::ios::cur); // 8 is offset for width and height

    newImg->data = (uint8_t *)malloc(sizeof(uint8_t) * newHead->sizes[pos]);

    fil.read((char *)&(newImg->w), sizeof(newImg->w));
    fil.read((char *)&(newImg->h), sizeof(newImg->h));
    fil.read((char *)&(newImg->channels), sizeof(newImg->channels));
    fil.read((char *)(newImg->data), sizeof(newImg->data[0]) * newHead->sizes[pos]);

    fil.close();
    return newImg;
}
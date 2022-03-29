#include <fstream>
#include "ECI.h"

void ECI::Write(const char *fileName)
{
    std::ofstream fil(fileName, std::ios::binary);

    fil.write((char *)&(header.numbers), sizeof(header.numbers));
    fil.write((char *)(header.sizes), sizeof(header.sizes) * header.numbers);

    for (int i = 0; i < header.numbers; i++)
        fil.write(header.imgNames[i].c_str(), header.imgNames[i].length() + 1);

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

ECI *ECI::Read(const char *fileName, bool withData)
{
    ECI *newEl = (ECI *)malloc(sizeof(ECI));
    std::ifstream fil(fileName, std::ios::binary);

    ECI::ReadHeader(&fil, &newEl->header);

    newEl->imgs = (ECIImg *)malloc(sizeof(ECIImg) * newEl->header.numbers);
    // Read all images
    for (int i = 0; i < newEl->header.numbers; i++)
    {
        newEl->imgs[i].data = (uint8_t *)malloc(sizeof(uint8_t) * newEl->header.sizes[i]);

        ECI::ReadImg(&fil, &(newEl->imgs[i]), newEl->header.sizes[i], withData);
    }

    fil.close();
    return newEl;
}

ECIImg *ECI::ReadAt(const char *fileName, int pos, ECIHeader *newHead, bool withData)
{
    // ECIHeader *newHead = (ECIHeader *)malloc(sizeof(ECIHeader));
    std::ifstream fil(fileName, std::ios::binary);

    ECI::ReadHeader(&fil, newHead);

    if (pos >= newHead->numbers)
        return NULL;

    ECIImg *newImg = (ECIImg *)malloc(sizeof(ECIImg) * newHead->numbers);

    // Read all images
    int offset = 0;
    for (int i = 0; i < pos; i++)
        offset += sizeof(ECIImg) - sizeof(newImg->data) + (newHead->sizes[i] * sizeof(uint8_t));

    fil.seekg(offset, std::ios::cur); // 8 is offset for width and height

    newImg->data = (uint8_t *)malloc(sizeof(uint8_t) * newHead->sizes[pos]);

    ECI::ReadImg(&fil, newImg, newHead->sizes[pos], withData);
    fil.close();
    return newImg;
}

void ECI::ReadImg(std::ifstream *fil, ECIImg *img, uint32_t filsize, bool withData)
{
    fil->read((char *)&(img->w), sizeof(img->w));
    fil->read((char *)&(img->h), sizeof(img->h));
    fil->read((char *)&(img->channels), sizeof(img->channels));

    //If data is required then read it or skip that data
    if (withData)
        fil->read((char *)(img->data), filsize);
    else
        fil->seekg(filsize, std::ios::cur);
}

void ECI::ReadHeader(std::ifstream *fil, ECIHeader *header)
{
    fil->read((char *)&(header->numbers), sizeof(header->numbers));

    header->sizes = (uint32_t *)malloc(sizeof(uint32_t) * header->numbers);
    header->imgNames = (std::string *)malloc(sizeof(std::string) * header->numbers);

    // Read sizes of images
    fil->read((char *)(header->sizes), sizeof(header->sizes) * header->numbers);

    char c;
    for (int i = 0; i < header->numbers; i++)
    {
        new (header->imgNames + i) std::string("");
        while (fil->get(c) && c != '\0')
        {
            header->imgNames[i].push_back(c);
        }
    }
}
#include <iostream>
#include <fstream>
#include <string>
#include "ECI.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"

using namespace std;

ECIHeader h;
ECIImg img[2];

// EL el;
ECI *WriteEl(int imgNum, const string imgsPath[], const char *outFile);

int main(int argc, char *args[])
{
    for (int i = 1; i < argc; i++)
    {
        cout << i << " : " << args[i] << '\n';
    }

    if (argc >= 4)
    {
        if (strcmp(args[1], "-r") == 0)
        {
            char *inpFile = args[2];
            int pos = atoi(args[3]);
            char *outFile = args[4];

            //Read  args[3]

            ECIImg *nn = ECI::ReadAt(inpFile, pos);
            if (nn == NULL)
            {
                printf("Input file path doesn't exists.\n");
                return 1;
            }

            cout << "Size of " << pos << ": " << nn->w << '\n';

            uint8_t *rawData = new uint8_t[nn->w * nn->h * 4];

            // Convert single rgba value to seperated rgba value
            for (int i = 0; i < nn->w * nn->h * 4; i += 4)
            {
                char *tD = intToRgba(nn->data[i / 4]);
                rawData[i] = tD[0];
                rawData[i + 1] = tD[1];
                rawData[i + 2] = tD[2];
                rawData[i + 3] = tD[3];
            }

            stbi_write_jpg(outFile, nn->w, nn->h, 4, rawData, 100);
        }
        else if (strcmp(args[1], "-w") == 0)
        {
            char *outFile = args[2];
            int n = atoi(args[3]);
            string *numImgs = new string[n];

            for (int i = 4; i < 4 + n; i++)
            {
                numImgs[i - 4] = args[i];
                cout << numImgs[i - 4] << '\n';
            }

            ECI *newEl = WriteEl(n, numImgs, outFile);
        }
    }
    else
    {
        printf("Usage \n");
        printf("For Read Mode  '-r', fileName, ImgIndex, outFileName.\n");
        printf("For Write Mode '-w', outFileName, ImgIndex, imgFileNames. \n");
    }

    return 0;
}

ECI *WriteEl(int imgNum, const string imgsPath[], const char *outFile)
{
    ECI *el = (ECI *)malloc(sizeof(ECI));
    el->header.numbers = imgNum;
    el->header.sizes = new uint32_t[imgNum];

    el->imgs = new ECIImg[imgNum];

    for (int nI = 0; nI < imgNum; nI++)
    {
        int a;
        unsigned char *da = stbi_load(imgsPath[nI].c_str(), &(el->imgs[nI].w), &(el->imgs[nI].h), &a, STBI_rgb_alpha);

        el->header.sizes[nI] = el->imgs[nI].w * el->imgs[nI].h;
        el->imgs[nI].data = new uint32_t[el->header.sizes[nI]];

        for (int i = 0; i < el->header.sizes[nI]; i++)
        {
            el->imgs[nI].data[i] = rgb(da[i * 4], da[(i * 4) + 1], da[(i * 4) + 2], da[(i * 4) + 3]);
        }
    }
    el->Write(outFile);
    return el;
}
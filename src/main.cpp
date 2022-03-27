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
            ECIHeader head;
            ECIImg *nn = ECI::ReadAt(inpFile, pos, &head);
            if (nn == NULL)
            {
                printf("Input file path doesn't exists.\n");
                return 1;
            }

            cout << "Size of " << pos << ": " << nn->w << '\n';

            ofstream f(outFile, ios::binary);
            f.write((char *)nn->data, head.sizes[pos]);
            f.close();
        }
        else if (strcmp(args[1], "-w") == 0)
        {
            char *outFile = args[2];
            int n = atoi(args[3]);
            string *numImgs = new string[n];

            cout << "Imgs Files are: \n";
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
        cout << "LOADING " << imgsPath[nI] << "...\n";
        uint8_t *da = stbi_load(imgsPath[nI].c_str(), &(el->imgs[nI].w), &(el->imgs[nI].h), &(el->imgs[nI].channels), 0);

        ifstream refil(imgsPath[nI].c_str(), ios::binary | ios::ate);

        auto siz = refil.tellg();
        refil.seekg(0, ios::beg);
        uint8_t *d = (uint8_t *)malloc(sizeof(uint8_t) * siz);
        refil.read((char *)d, siz);
        refil.close();

        el->imgs[nI].data = d;
        el->header.sizes[nI] = siz;
    }
    el->Write(outFile);
    return el;
}

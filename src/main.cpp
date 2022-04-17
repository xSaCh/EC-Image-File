#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "ECI.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"

//Width of Img after Resize
//#define RESIZE_WIDTH 256

using namespace std;

ECIHeader h;
ECIImg img[2];

// EL el;
ECI *WriteEl(int imgNum, const string imgsPath[], const char *outFile);
uint8_t *ImgResizeData(const int resizeWidth, const uint8_t *img_data, const uint32_t imgSize);
string fileSizeToName(const uint32_t fileSize);

int main(int argc, char *args[])
{
    if (argc > 4)
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

            cout << nn->w << '\n';
            ofstream f(outFile, ios::binary);
            f.write((char *)nn->data, head.sizes[pos]);
            f.close();
        }
        else if (strcmp(args[1], "-rs") == 0)
        {
            char *inpFile = args[2];
            int pos = atoi(args[3]);
	    int r_width = atoi(args[4]);
            char *outFile = args[5];

            //Read  args[3]
            ECIHeader head;
            ECIImg *nn = ECI::ReadAt(inpFile, pos, &head);
            if (nn == NULL)
            {
                printf("Input file path doesn't exists.\n");
                return 1;
            }

            uint8_t *reData = ImgResizeData(r_width,nn->data, head.sizes[pos]);

            float scale = (r_width / (float)nn->w);
            stbi_write_jpg(((string) "resize_" + outFile).c_str(), nn->w * scale, nn->h * scale, nn->channels, reData, 50);
        }
        else if (strcmp(args[1], "-f") == 0)
        {
            char *inpFile = args[2];
            int pos = atoi(args[3]);
            char *outFile = args[4];

            ECI *eci = ECI::Read(inpFile, true);

            cout << "Total Images: " << eci->header.numbers << '\n';
            for (int i = 0; i < eci->header.numbers; i++)
            {
                string s = fileSizeToName(eci->header.sizes[i]);

                printf("\n%i) %s := W:%i H:%i C:%i Size:%s\n", i, eci->header.imgNames[i].c_str(),
                       eci->imgs[i].w, eci->imgs[i].h, eci->imgs[i].channels, s.c_str());
            }
        }
        else if (strcmp(args[1], "-w") == 0)
        {
            char *outFile = args[2];
            int n = atoi(args[3]);
            string *numImgs = new string[n];

            for (int i = 4; i < 4 + n; i++)
                numImgs[i - 4] = args[i];

            ECI *newEl = WriteEl(n, numImgs, outFile);
        }
    }
    else
    {
        printf("Usage \n");
        printf("For Read Mode  '-r', fileName, ImgIndex, outFileName.\n");
        printf("For Read with resize Mode  '-rs', fileName, ImgIndex, outFileName.\n");
        printf("For Write Mode '-w', outFileName, ImgIndex, imgFileNames. \n");
    }

    return 0;
}

ECI *WriteEl(int imgNum, const string imgsPath[], const char *outFile)
{
    ECI *el = (ECI *)malloc(sizeof(ECI));
    el->header.numbers = imgNum;
    el->header.sizes = new uint32_t[imgNum];
    el->header.imgNames = new string[imgNum];

    el->imgs = new ECIImg[imgNum];

    for (int nI = 0; nI < imgNum; nI++)
    {
        cout << "LOADING " << imgsPath[nI].c_str() << "...\n";
        int data = stbi_info(imgsPath[nI].c_str(), &(el->imgs[nI].w), &(el->imgs[nI].h), &(el->imgs[nI].channels));

        ifstream refil(imgsPath[nI].c_str(), ios::binary | ios::ate);

        auto siz = refil.tellg();
        refil.seekg(0, ios::beg);
        uint8_t *d = (uint8_t *)malloc(sizeof(uint8_t) * siz);
        refil.read((char *)d, siz);
        refil.close();

        el->imgs[nI].data = d;
        el->header.sizes[nI] = siz;
        el->header.imgNames[nI] = imgsPath[nI].substr(imgsPath[nI].find_last_of("/\\") + 1);
    }
    el->Write(outFile);
    return el;
}

uint8_t *ImgResizeData(const int resizeWidth, const uint8_t *img_data, const uint32_t imgSize)
{
    int x, y, c;
    uint8_t *imdDa = stbi_load_from_memory(img_data, imgSize, &x, &y, &c, 0);

    float scale = resizeWidth/ (float)x;

    uint8_t *data = (uint8_t *)malloc(sizeof(uint8_t) * x * y * c * scale);

    int code = stbir_resize_uint8(imdDa, x, y, 0, data, x * scale, y * scale, 0, c);

    if (code == 0)
    {
        cout << "RESIZE BULL\n";
        free(data);
        return NULL;
    }

    img->w = x * scale;
    img->h = y * scale;
    img->channels = c;

    return data;
}

string fileSizeToName(const uint32_t fileSize)
{
    int i = log(fileSize) / log(1024);
    int p = pow(1024, i);
    string siz = to_string(fileSize / (float)p);

    const char *sizes[] = {"B", "KB", "MB", "GB"};
    siz += sizes[i];
    return siz;
}
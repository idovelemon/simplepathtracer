//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 07
// Brief: Some useful macro and helper method
//----------------------------------------------------------
#ifndef _SPT_SPTUTIL_H_
#define _SPT_SPTUTIL_H_

#include <Windows.h>
#include <wingdi.h>

#include <stdint.h>

namespace spt {

#define m 0x100000000LL
#define c 0xB16
#define a 0x5DEECE66DLL

static unsigned long long seed = 1;

inline double DRand48(void) {
    seed = (a * seed + c) & 0xFFFFFFFFFFFFLL;
    unsigned int x = seed >> 16;
    return  ((double)x / (double)m);
}

inline float Clamp(float v, float s = 0.0f, float e = 1.0f) {
    if (v < s) {
        v = s;
    } else if (v > e) {
        v = e;
    }

    return v;
}

template<typename T>
inline void Swap(T v0, T v1) {
    T temp = v0;
    v0 = v1;
    v1 = temp;
}

inline void SetRGB(float* buf,
                   int32_t x, int32_t y,
                   int32_t width, int32_t height,
                   float r, float g, float b) {
    if (buf == NULL) return;
    buf[y * width * 3 + x * 3 + 0] = r;
    buf[y * width * 3 + x * 3 + 1] = g;
    buf[y * width * 3 + x * 3 + 2] = b;
}

inline void ToneMapping(float& x, float& y, float& z) {
    x = x / (1.0f + x);
    y = y / (1.0f + y);
    z = z / (1.0f + z);
}

inline void SaveToBmpFile(float* buf, int32_t width, int32_t height, const char* name) {
    if (buf == NULL || name == NULL) return;

    FILE* file = fopen(name, "wb");

    if (file) {
        uint8_t* image_buf = new uint8_t[width * height * 3];
        for (int32_t y = 0; y < height; y++) {
            for (int32_t x = 0; x < width; x++) {
                uint8_t r = buf[y * width * 3 + x * 3 + 0] * 255;
                uint8_t g = buf[y * width * 3 + x * 3 + 1] * 255;
                uint8_t b = buf[y * width * 3 + x * 3 + 2] * 255;

                // Bitmap's color format is BGR
                image_buf[y * width * 3 + x * 3 + 0] = b;
                image_buf[y * width * 3 + x * 3 + 1] = g;
                image_buf[y * width * 3 + x * 3 + 2] = r;
            }
        }

        BITMAPFILEHEADER file_header;
        file_header.bfType = (WORD)0x4d42;
        file_header.bfSize = width * height * 3  // Image data size
              + sizeof( BITMAPFILEHEADER )  // File header size
              + sizeof( BITMAPINFOHEADER );  // Info header size
        file_header.bfReserved1 = 0;
        file_header.bfReserved2 = 0;
        file_header.bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER );  // Image data's offset

        BITMAPINFOHEADER info_header;
        info_header.biSize = sizeof(BITMAPINFOHEADER);
        info_header.biWidth = width;
        info_header.biHeight = -height;  // Flip the image, so that it won't display wrong
        info_header.biPlanes = 1;
        info_header.biBitCount = 24;
        info_header.biCompression = 0;
        info_header.biSizeImage = width * height * 3;
        info_header.biXPelsPerMeter = 2835;
        info_header.biYPelsPerMeter = 2835;
        info_header.biClrUsed = 0;
        info_header.biClrImportant = 0;

        fwrite(&file_header, sizeof(file_header), 1, file);
        fwrite(&info_header, sizeof(info_header), 1, file);
        fwrite(image_buf, width * height * 3, 1, file);

        fclose(file);
        delete[] image_buf;
        image_buf = NULL;
    }
}

};  // namespace spt

#endif  // _SPT_SPTUTIL_H_
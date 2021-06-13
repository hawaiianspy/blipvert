
//
//  blipvert C++ library
//
//  MIT License
//
//  Copyright(c) 2021 Don Jordan
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this softwareand associated documentation files(the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions :
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//  SOFTWARE.
//

#include "pch.h"
#include "RGBtoRGB.h"
#include "CommonMacros.h"

using namespace blipvert;


//
// RGB color fill functions
//

void blipvert::Fill_RGB32(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width * 4;

    uint32_t fill = static_cast<uint32_t>(((((alpha) & 0xFF) << 24) | (((red) & 0xFF) << 16) | (((green) & 0xFF) << 8) | ((blue) & 0xFF)));
    do
    {
        uint32_t* pdst = reinterpret_cast<uint32_t*>(pBuffer);
        int32_t hcount = width;
        do
        {
            *pdst++ = fill;
        } while (--hcount);

        pBuffer += stride;
    } while (--height);
}

bool blipvert::Check_RGB32(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width * 4;

    uint32_t fill = static_cast<uint32_t>(((((alpha) & 0xFF) << 24) | (((red) & 0xFF) << 16) | (((green) & 0xFF) << 8) | ((blue) & 0xFF)));
    do
    {
        uint32_t* pdst = reinterpret_cast<uint32_t*>(pBuffer);
        int32_t hcount = width;
        do
        {
            if (*pdst++ != fill) return false;
        } while (--hcount);

        pBuffer += stride;
    } while (--height);

    return true;
}

bool blipvert::Check_RGB24(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width * 3;

    uint32_t fill = static_cast<uint32_t>((((red) & 0xFF) << 16) | (((green) & 0xFF) << 8) | ((blue) & 0xFF));
    do
    {
        uint8_t* pdst = pBuffer;
        int32_t hcount = width;
        do
        {
            if ((*reinterpret_cast<uint32_t*>(pdst) & 0x00FFFFFF) != fill) return false;
            pdst += 3;
        } while (--hcount);

        pBuffer += stride;
    } while (--height);

    return true;
}


void blipvert::Fill_RGB565(uint8_t red, uint8_t green, uint8_t blue, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width * 2;

    uint16_t fill;
    PackRGB565Word(fill, red, green, blue);

    do
    {
        uint16_t* pdst = reinterpret_cast<uint16_t*>(pBuffer);
        int32_t hcount = width;
        do
        {
            *pdst = fill;
            pdst++;
        } while (--hcount);

        pBuffer += stride;
    } while (--height);
}

bool blipvert::Check_RGB565(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width * 2;

    uint16_t fill;
    PackRGB565Word(fill, red, green, blue);

    do
    {
        uint16_t* pdst = reinterpret_cast<uint16_t*>(pBuffer);
        int32_t hcount = width;
        do
        {
            if (*pdst != fill) return false;
            pdst++;
        } while (--hcount);

        pBuffer += stride;
    } while (--height);

    return true;
}

void blipvert::Fill_RGB555(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width * 2;

    uint16_t fill;
    PackRGB555Word(fill, red, green, blue);
    if (alpha)
        fill |= 0x8000;
    else
        fill &= 0x7FFF;

    do
    {
        uint16_t* pdst = reinterpret_cast<uint16_t*>(pBuffer);
        int32_t hcount = width;
        do
        {
            *pdst = fill;
            pdst++;
        } while (--hcount);

        pBuffer += stride;
    } while (--height);
}

bool blipvert::Check_RGB555(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, int32_t width, int32_t height, uint8_t* pBuffer, int32_t stride)
{
    if (!stride)
        stride = width * 2;

    uint16_t fill;
    PackRGB555Word(fill, red, green, blue);
    if (alpha)
        fill |= 0x8000;
    else
        fill &= 0x7FFF;

    do
    {
        uint16_t* pdst = reinterpret_cast<uint16_t*>(pBuffer);
        int32_t hcount = width;
        do
        {
            if (*pdst != fill) return false;
            pdst++;
        } while (--hcount);

        pBuffer += stride;
    } while (--height);

    return true;
}

//
// RGB32 to RGBX conversions
//

void blipvert::RGB32_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 3;

    if (!in_stride)
        in_stride = width * 4;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint32_t* psrc = reinterpret_cast<uint32_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width / 4;
        do
        {
            *reinterpret_cast<uint32_t*>(pdst) = *psrc++;
            pdst += 3;

            *reinterpret_cast<uint32_t*>(pdst) = *psrc++;
            pdst += 3;

            *reinterpret_cast<uint32_t*>(pdst) = *psrc++;
            pdst += 3;

            *reinterpret_cast<uint32_t*>(pdst) = *psrc++;
            pdst += 3;

        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB32_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 4;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            PackRGB565Word(*pdst++, psrc[2], psrc[1], psrc[0]);
            psrc += 4;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB32_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 4;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            PackRGB555Word(*pdst++, psrc[2], psrc[1], psrc[0]);
            psrc += 4;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

//
// RGB24 to RGBX conversions
//

void blipvert::RGB24_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 4;

    if (!in_stride)
        in_stride = width * 3;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        int32_t hcount = width / 4;
        do
        {
            *pdst++ = *reinterpret_cast<uint32_t*>(psrc) | 0xFF000000;
            psrc += 3;

            *pdst++ = *reinterpret_cast<uint32_t*>(psrc) | 0xFF000000;
            psrc += 3;

            *pdst++ = *reinterpret_cast<uint32_t*>(psrc) | 0xFF000000;
            psrc += 3;

            *pdst++ = *reinterpret_cast<uint32_t*>(psrc) | 0xFF000000;
            psrc += 3;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB24_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 3;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            PackRGB565Word(*pdst++, psrc[2], psrc[1], psrc[0]);
            psrc += 3;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB24_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 3;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            PackRGB555Word(*pdst++, psrc[2], psrc[1], psrc[0]);
            psrc += 3;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

//
// RGB555 to RGBX conversions
//

void blipvert::RGB555_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 4;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        do
        {
            uint16_t source = *psrc;
            UnpackRGB555Word(source, pdst[2], pdst[1], pdst[0])
                pdst[3] = 0xFF;

            psrc++;
            pdst += 4;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB555_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 3;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        do
        {
            uint16_t source = *psrc;
            UnpackRGB555Word(source, pdst[2], pdst[1], pdst[0])

                psrc++;
            pdst += 3;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB555_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            *pdst++ = ((*psrc & (RGB555_RED_MASK | RGB555_GREEN_MASK)) << 1) | (*psrc & RGB555_BLUE_MASK);
            psrc++;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

//
// RGB565 to RGBX conversions
//

void blipvert::RGB565_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 4;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        do
        {
            uint16_t source = *psrc;
            UnpackRGB565Word(source, pdst[2], pdst[1], pdst[0])
                pdst[3] = 0xFF;

            psrc++;
            pdst += 4;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB565_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 3;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        do
        {
            uint16_t source = *psrc;
            UnpackRGB565Word(source, pdst[2], pdst[1], pdst[0])

                psrc++;
            pdst += 3;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}


void blipvert::RGB565_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width * 2;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint16_t* psrc = reinterpret_cast<uint16_t*>(in_buf);
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            *pdst++ = 0x8000 | ((*psrc & (RGB565_RED_MASK | RGB565_GREEN_MASK)) >> 1) | (*psrc & RGB565_BLUE_MASK);
            psrc++;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

//
// RGB8 to RGBX conversions
//

void blipvert::RGB8_to_RGB32(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 4;

    if (!in_stride)
        in_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint32_t* pdst = reinterpret_cast<uint32_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            *pdst++ = (*reinterpret_cast<uint32_t*>(&in_palette[*psrc++]) | 0xFF000000);
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB8_to_RGB24(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 3;

    if (!in_stride)
        in_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint8_t* pdst = out_buf;
        int32_t hcount = width;
        do
        {
            pdst[0] = in_palette[*psrc].rgbBlue; // Blue
            pdst[1] = in_palette[*psrc].rgbGreen; // Green
            pdst[2] = in_palette[*psrc].rgbRed; // Red

            psrc++;
            pdst += 3;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB8_to_RGB565(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            PackRGB565Word(*pdst++, in_palette[*psrc].rgbRed,
                in_palette[*psrc].rgbGreen,
                in_palette[*psrc].rgbBlue);
            psrc++;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

void blipvert::RGB8_to_RGB555(int32_t width, int32_t height,
    uint8_t* out_buf, int32_t out_stride,
    uint8_t* in_buf, int32_t in_stride,
    bool flipped, RGBQUAD* in_palette)
{
    if (!out_stride)
        out_stride = width * 2;

    if (!in_stride)
        in_stride = width;

    if (flipped)
    {
        out_buf += (out_stride * (height - 1));
        out_stride = -out_stride;
    }

    do
    {
        uint8_t* psrc = in_buf;
        uint16_t* pdst = reinterpret_cast<uint16_t*>(out_buf);
        int32_t hcount = width;
        do
        {
            PackRGB555Word(*pdst++, in_palette[*psrc].rgbRed,
                in_palette[*psrc].rgbGreen,
                in_palette[*psrc].rgbBlue);
            psrc++;
        } while (--hcount);

        in_buf += in_stride;
        out_buf += out_stride;
    } while (--height);
}

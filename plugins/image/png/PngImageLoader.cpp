/*
 * Copyright (c) 2015 Nikolay Zapolnov (zapolnov@gmail.com).
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "PngImageLoader.h"
#include "engine/image/Image.h"
#include "engine/core/Log.h"
#include <png.h>
#include <setjmp.h>

#ifdef _MSC_VER
#pragma warning(disable:4611)   // interaction between _strjmp and C++ object destruction is non-portable
#endif

namespace Engine
{
    static const size_t PNG_SIGNATURE_SIZE = 8;

    static void pngError(png_structp pngp, png_const_charp message)
    {
        IFile* file = reinterpret_cast<IFile*>(png_get_error_ptr(pngp));
        B3D_LOGE("Unable to decode PNG file \"" << file->name() << "\": " << message);

        (void)file;
        (void)message;

        longjmp(png_jmpbuf(pngp), 1);
    }

    static void pngWarning(png_structp pngp, png_const_charp message)
    {
        IFile* file = reinterpret_cast<IFile*>(png_get_error_ptr(pngp));
        B3D_LOGW("In PNG file \"" << file->name() << "\": warning: " << message);

        (void)file;
        (void)message;
    }

    static void pngRead(png_structp pngp, png_bytep data, png_size_t length)
    {
        IFile* file = reinterpret_cast<IFile*>(png_get_io_ptr(pngp));
        size_t bytesRead = file->read(data, length);
        if (bytesRead != length) {
            B3D_LOGE("Unable to decode PNG file \"" << file->name() << "\": unexpected end of stream.");
            longjmp(png_jmpbuf(pngp), 1);
        }
    }

    bool PngImageLoader::canLoadImage(IFile* file)
    {
        uint64_t pos = file->position();
        png_byte buf[PNG_SIGNATURE_SIZE];
        if (file->read(buf, sizeof(buf)) != sizeof(buf) || png_sig_cmp(buf, 0, sizeof(buf)) != 0) {
            file->seek(pos);
            return false;
        }
        return true;
    }

    ImagePtr PngImageLoader::loadImage(IFile* file)
    {
        struct Context
        {
            png_structp pngp;
            png_infop infop;
            ~Context() { png_destroy_read_struct(&pngp, &infop, nullptr); }
        };

        if (!file)
            return std::make_shared<Image>();

        volatile Context context;
        png_structp pngp = context.pngp = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        if (!pngp) {
            B3D_LOGE("Unable to decode PNG file \"" << file->name() << "\": out of memory.");
            return std::make_shared<Image>();
        }

        if (setjmp(png_jmpbuf(pngp)))
            return std::make_shared<Image>();

        png_set_error_fn(pngp, file, pngError, pngWarning);
        png_set_read_fn(pngp, file, pngRead);
        png_set_sig_bytes(pngp, PNG_SIGNATURE_SIZE);

        png_infop infop = context.infop = png_create_info_struct(pngp);
        if (!infop) {
            B3D_LOGE("Unable to decode PNG file \"" << file->name() << "\": out of memory.");
            return std::make_shared<Image>();
        }

        png_read_info(pngp, infop);

        int bitdepth = png_get_bit_depth(pngp, infop);
        switch (png_get_color_type(pngp, infop))
        {
        default:
            break;

        case PNG_COLOR_TYPE_PALETTE:
            png_set_palette_to_rgb(pngp);
            break;

        case PNG_COLOR_TYPE_GRAY:
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            if (bitdepth < 8)
                png_set_expand_gray_1_2_4_to_8(pngp);
            break;
        }

        if (png_get_valid(pngp, infop, PNG_INFO_tRNS))
            png_set_tRNS_to_alpha(pngp);

        if (bitdepth == 16)
            png_set_scale_16(pngp);
        else if (bitdepth < 8)
            png_set_packing(pngp);

        double gamma = 1.0;
        if (png_get_gAMA(pngp, infop, &gamma))
            png_set_gamma(pngp, 2.2, gamma);

        int numPasses = 1;
        if (png_get_interlace_type(pngp, infop) != PNG_INTERLACE_NONE)
            numPasses = png_set_interlace_handling(pngp);

        png_read_update_info(pngp, infop);

        png_uint_32 width = png_get_image_width(pngp, infop);
        png_uint_32 height = png_get_image_height(pngp, infop);
        png_size_t rowbytes = png_get_rowbytes(pngp, infop);
        png_byte depth = png_get_bit_depth(pngp, infop);
        png_byte channels = png_get_channels(pngp, infop);

        if (depth != 8) {
            B3D_LOGE("Unable to decode PNG file \"" << file->name()
                << "\": image has invalid bit depth (" << depth << ").");
            return std::make_shared<Image>();
        }

        PixelFormat format;
        switch (channels)
        {
        case 1: format = PixelFormat::Luminance8; break;
        case 2: format = PixelFormat::LuminanceAlpha16; break;
        case 3: format = PixelFormat::RGB24; break;
        case 4: format = PixelFormat::RGBA32; break;
        default:
            B3D_LOGE("Unable to decode PNG file \"" << file->name()
                << "\": image has unsupported number of channels (" << channels << ").");
            return std::make_shared<Image>();
        }

        auto image = std::make_shared<Image>(format, width, height);
        image->setDataSize((width * height + (rowbytes - width)) * channels);
        uint8_t* imageData = image->data();

        for (int i = 0; i < numPasses; i++) {
            uint8_t* row = imageData;
            for (png_uint_32 y = 0; y < height; y++) {
                png_read_row(pngp, reinterpret_cast<png_bytep>(row), nullptr);
                row += width * channels;
            }
        }

        png_read_end(pngp, nullptr);

        return image;
    }
}

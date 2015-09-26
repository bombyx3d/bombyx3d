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
#include "JpegImageLoader.h"
#include "engine/core/Log.h"
#include "engine/utility/StringUtils.h"
#include <jerror.h>
#include <jpeglib.h>
#include <setjmp.h>

#ifdef _MSC_VER
#pragma warning(disable:4611)   // interaction between _strjmp and C++ object destruction is non-portable
#endif

namespace Engine
{
    namespace
    {
        struct JpegErrorMgr : jpeg_error_mgr
        {
            IFile* file;
            jmp_buf jmpbuf;
        };

        struct JpegSourceMgr : jpeg_source_mgr
        {
            IFile* file;
            JOCTET* buffer;
            bool atStart;
        };

        static const size_t INPUT_BUFFER_SIZE = 4096;

        static void jpegErrorExit(j_common_ptr cinfo)
        {
            JpegErrorMgr* error = reinterpret_cast<JpegErrorMgr*>(cinfo->err);
            error->output_message(cinfo);
            longjmp(error->jmpbuf, 1);
        }

        static void jpegOutputMessage(j_common_ptr cinfo)
        {
            JpegErrorMgr* error = reinterpret_cast<JpegErrorMgr*>(cinfo->err);

            char buffer[JMSG_LENGTH_MAX];
            error->format_message(cinfo, buffer);

            Z_LOGW("In JPEG file \"" << error->file->name() << "\": " << buffer);
        }

        static void jpegInitSource(j_decompress_ptr cinfo)
        {
            JpegSourceMgr* src = reinterpret_cast<JpegSourceMgr*>(cinfo->src);
            src->atStart = true;
        }

        static boolean jpegFillInputBuffer(j_decompress_ptr cinfo)
        {
            JpegSourceMgr* src = reinterpret_cast<JpegSourceMgr*>(cinfo->src);

            size_t bytesRead = src->file->read(src->buffer, INPUT_BUFFER_SIZE);
            if (bytesRead == 0) {
                if (src->atStart)
                    ERREXIT(cinfo, JERR_INPUT_EMPTY);
                WARNMS(cinfo, JWRN_JPEG_EOF);
                // Insert a fake EOI marker
                src->buffer[0] = (JOCTET)0xFF;
                src->buffer[1] = (JOCTET)JPEG_EOI;
                bytesRead = 2;
            }

            src->atStart = false;
            src->next_input_byte = src->buffer;
            src->bytes_in_buffer = bytesRead;

            return TRUE;
        }

        static void jpegSkipInputData(j_decompress_ptr cinfo, long bytes)
        {
            JpegSourceMgr* src = reinterpret_cast<JpegSourceMgr*>(cinfo->src);

            if (bytes > 0) {
                while (bytes > long(src->bytes_in_buffer)) {
                    bytes -= long(src->bytes_in_buffer);
                    // note we assume that fill_input_buffer will never return FALSE,
                    // so suspension need not be handled.
                    src->fill_input_buffer(cinfo);
                }
                src->next_input_byte += size_t(bytes);
                src->bytes_in_buffer -= size_t(bytes);
            }
        }

        static void jpegTermSource(j_decompress_ptr)
        {
        }
    }

    bool JpegImageLoader::canLoadImage(IFile* file)
    {
        return file && (StringUtils::endsWith(file->name(), ".jpg") || StringUtils::endsWith(file->name(), ".jpeg"));
    }

    ImagePtr JpegImageLoader::loadImage(IFile* file)
    {
        if (!file)
            return std::make_shared<Image>();

        jpeg_decompress_struct cinfo;
        JpegErrorMgr jerr;

        cinfo.err = jpeg_std_error(&jerr);
        jerr.file = file;
        jerr.error_exit = jpegErrorExit;
        jerr.output_message = jpegOutputMessage;
        if (setjmp(jerr.jmpbuf) != 0) {
            jpeg_destroy_decompress(&cinfo);
            return std::make_shared<Image>();
        }

        jpeg_create_decompress(&cinfo);

        JpegSourceMgr jsrc;
        jsrc.file = file;
        jsrc.buffer = reinterpret_cast<JOCTET*>(cinfo.mem->alloc_small
            (reinterpret_cast<j_common_ptr>(&cinfo), JPOOL_PERMANENT, INPUT_BUFFER_SIZE));
        jsrc.init_source = jpegInitSource;
        jsrc.fill_input_buffer = jpegFillInputBuffer;
        jsrc.skip_input_data = jpegSkipInputData;
        jsrc.resync_to_restart = jpeg_resync_to_restart;
        jsrc.term_source = jpegTermSource;
        jsrc.bytes_in_buffer = 0;
        jsrc.next_input_byte = nullptr;
        cinfo.src = &jsrc;

        jpeg_read_header(&cinfo, TRUE);
        jpeg_start_decompress(&cinfo);

        size_t width = cinfo.output_width;
        size_t height = cinfo.output_height;
        size_t numChannels = cinfo.output_components;

        PixelFormat format;
        switch (cinfo.out_color_space)
        {
        case JCS_RGB:
            format = PixelFormat::RGB24;
            if (numChannels != 3) {
                Z_LOGE("Unable to decompress JPEG file \"" << file->name()
                    << "\": image has invalid number of color channels.");
                longjmp(jerr.jmpbuf, 1);
            }
            break;

        case JCS_GRAYSCALE:
            format = PixelFormat::Luminance8;
            if (numChannels != 1) {
                Z_LOGE("Unable to decompress JPEG file \"" << file->name()
                    << "\": image has invalid number of color channels.");
                longjmp(jerr.jmpbuf, 1);
            }
            break;

        case JCS_CMYK:
        case JCS_YCbCr:
        case JCS_YCCK:
        case JCS_UNKNOWN:
        default:
            Z_LOGE("Unable to decompress JPEG file \"" << file->name() << "\": image has unsupported color space.");
            longjmp(jerr.jmpbuf, 1);
        }

        auto image = std::make_shared<Image>(format, width, height);

        size_t rowStride = width * numChannels;
        image->setDataSize(rowStride * height);

        uint8_t* imageData = image->data();
        while (cinfo.output_scanline < cinfo.output_height) {
            JSAMPROW line[1] = { reinterpret_cast<JSAMPLE*>(imageData + cinfo.output_scanline * rowStride) };
            jpeg_read_scanlines(&cinfo, line, 1);
        }

        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);

        return image;
    }
}

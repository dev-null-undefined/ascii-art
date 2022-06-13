#include "PNGImage.h"
#include "../../../container/Matrix.h"
#include "../BufferedFrame.h"
#include "../../logging/Logger.h"
#include <png.h>
#include <stdexcept>

PNGImage::PNGImage(std::string filename) : m_filename(std::move(filename)) {

}

void pngErrorHandler(png_structp,
                     png_const_charp error_msg) {
    throw std::invalid_argument("[read_png] " + std::string(error_msg));
}

void pngWarnHandler(png_structp,
                    png_const_charp error_msg) {
    Logger::log(error_msg, LogLevel::WARN);
}

// TODO: link source of this snippet
void PNGImage::Load() {
    char header[8];    // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE * fp = fopen(m_filename.c_str(), "rb");
    if (!fp)
        throw std::invalid_argument("[read_png_file] File " + m_filename + " could not be opened for reading");
    fread(header, 1, 8, fp);
    if (png_sig_cmp(reinterpret_cast<png_bytep>(header), 0, 8))
        throw std::invalid_argument("[read_png_file] File " + m_filename + " is not recognized as a PNG file");

    png_uint_32 height = 0;
    png_bytep * row_pointers = nullptr;
    png_structp png_ptr = nullptr;
    png_infop end_ptr = nullptr;
    try {
        /* initialize stuff */
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, pngErrorHandler, pngWarnHandler);

        if (!png_ptr)
            throw std::invalid_argument("[read_png_file] png_create_read_struct failed");

        end_ptr = png_create_info_struct(png_ptr);
        if (!end_ptr)
            throw std::invalid_argument("[read_png_file] png_create_info_struct failed");

        png_init_io(png_ptr, fp);
        png_set_sig_bytes(png_ptr, 8);

        png_read_info(png_ptr, end_ptr);

        png_uint_32 width = png_get_image_width(png_ptr, end_ptr);
        png_uint_32 height_tmp = png_get_image_height(png_ptr, end_ptr);
        png_byte color_type = png_get_color_type(png_ptr, end_ptr);
        png_byte bit_depth = png_get_bit_depth(png_ptr, end_ptr);
        if (bit_depth == 16)
            png_set_strip_16(png_ptr);

        png_set_interlace_handling(png_ptr);
        png_read_update_info(png_ptr, end_ptr);
        png_byte channels = png_get_channels(png_ptr, end_ptr);

        height = height_tmp;
        row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * (height));
        for (png_uint_32 y = 0; y < height; y++)
            row_pointers[y] = (png_byte *) malloc((png_get_rowbytes(png_ptr, end_ptr)));


        png_read_image(png_ptr, row_pointers);
        if (color_type == PNG_COLOR_TYPE_RGBA || color_type == PNG_COLOR_TYPE_RGB) {
            Matrix<Color> pixels = Matrix<Color>(height, width);
            for (png_uint_32 y = 0; y < height; y++) {
                png_byte * row = row_pointers[y];
                for (png_uint_32 x = 0; x < width; x++) {
                    png_byte * ptr = &(row[x * channels]);
                    Color color{ptr[0], ptr[1], ptr[2]};
                    if (color_type == PNG_COLOR_TYPE_RGBA) {
                        color.setAlpha(ptr[3]);
                    }
                    pixels.at(x, y) = color;
                }
            }
            m_frame = std::shared_ptr<Frame>(new BufferedFrame(std::move(pixels)));
        } else {
            Logger::log("Non-standard color type(" + std::to_string(color_type) + ") " + m_filename, LogLevel::INFO);
        }

        png_free_data(png_ptr, end_ptr, PNG_FREE_ALL, -1);

        png_destroy_read_struct(&png_ptr, nullptr,
                                &end_ptr);

        /* cleanup heap allocation */
        for (png_uint_32 y = 0; y < height; y++) {
            free(row_pointers[y]);
            row_pointers[y] = nullptr;
        }
        free(row_pointers);
        row_pointers = nullptr;
        free(png_ptr);
        png_ptr = nullptr;
        free(end_ptr);
        end_ptr = nullptr;
        fclose(fp);

    } catch (std::exception & exception) {

        png_free_data(png_ptr, end_ptr, PNG_FREE_ALL, -1);

        png_destroy_read_struct(&png_ptr, nullptr,
                                &end_ptr);

        for (png_uint_32 y = 0; y < height; y++)
            free(row_pointers[y]);
        free(row_pointers);
        free(png_ptr);
        free(end_ptr);
        fclose(fp);
        throw exception;
    }

    if (!m_frame) {
        throw std::invalid_argument("[read_png_file] Error during read_image");
    }
}

std::string_view PNGImage::filename() const {
    return std::string_view(m_filename);
}

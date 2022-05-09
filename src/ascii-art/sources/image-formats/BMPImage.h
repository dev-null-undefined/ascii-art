#ifndef ASCII_ART_BMPIMAGE_H_9E7C9A6C4B8143CC953406E1C6E307F1
#define ASCII_ART_BMPIMAGE_H_9E7C9A6C4B8143CC953406E1C6E307F1


#include "../Image.h"

class BMPImage : public Image {
    /**
     * @brief The BMPHeader structures are sourced from:
     * https://en.wikipedia.org/wiki/BMP_file_format
     */
    struct BMPFileHeader {
        uint16_t m_file_type{};                 // The header field used to identify the BMP and DIB file is 0x42 0x4D in hexadecimal
        uint32_t m_file_size{};                 // Size of the file (in bytes)
        uint16_t m_reserved1{};                 // Reserved; actual value depends on the application that creates the image, if created manually can be 0
        uint16_t m_reserved2{};
        uint32_t m_offset_data{};               // The offset, i.e. starting address, of the byte where the bitmap image data (pixel array) can be found.
    };
    struct BMPInfoHeader {
        uint32_t m_size{};                      // 	the size of this header, in bytes (40)
        int32_t m_width{};                      // 	the bitmap width in pixels (signed integer)
        int32_t m_height{};                     // the bitmap height in pixels (signed integer)
        uint16_t m_planes{};                    // the number of color planes (must be 1)
        uint16_t m_bit_count{};                 // 	the number of bits per pixel, which is the color depth of the image. Typical values are 1, 4, 8, 16, 24 and 32.
        uint32_t m_compression{};               // 	the compression method being used. See the next table for a list of possible values
        uint32_t m_size_image{};                // the image size. This is the size of the raw bitmap data; a dummy 0 can be given for BI_RGB bitmaps.
        int32_t m_x_pixels_per_meter{};         //	the horizontal resolution of the image. (pixel per metre, signed integer)
        int32_t m_y_pixels_per_meter{};         //the vertical resolution of the image. (pixel per metre, signed integer)
        uint32_t m_colors_used{};               // the number of colors in the color palette, or 0 to default to 2^n
        uint32_t m_important_colors{};          // the number of important colors used, or 0 when every color is important; generally ignored
    };
    struct BMPColorHeader {
        uint32_t red_mask{};
        uint32_t green_mask{};
        uint32_t blue_mask{};
        uint32_t alpha_mask{};
        uint32_t color_space_type{};
        uint32_t unused[16]{0};
    };

    static constexpr uint16_t BMP_FILE_TYPE = 0x4D42;
    BMPFileHeader m_file_header;
    BMPInfoHeader m_info_header;
    BMPColorHeader m_color_header;
    std::string m_file_name;
  public:
    BMPImage(std::string filename);

    ~BMPImage() override = default;

  protected:
    void Load() override;
};


#endif //ASCII_ART_BMPIMAGE_H_9E7C9A6C4B8143CC953406E1C6E307F1

#include <fstream>
#include <utility>
#include "BMPImage.h"

BMPImage::BMPImage(std::string  filename) : m_file_name (std::move( filename)){
}

void BMPImage::Load() {
    std::ifstream file(m_file_name, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file " + m_file_name);
    }

    // Read the file header
    if(!file.read(reinterpret_cast<char *>(&m_file_header), sizeof(BMPFileHeader))){
        throw std::runtime_error("Could not read file header from file " + m_file_name);
    }

    // Check the magic number
    if (m_file_header.m_file_type != BMP_FILE_TYPE) {
        throw std::runtime_error("File " + m_file_name + " is not a BMP file");
    }

    // Read the info header
    if(!file.read(reinterpret_cast<char *>(&m_info_header), sizeof(BMPInfoHeader))){
        throw std::runtime_error("Could not read info header from file " + m_file_name);
    }

    // Check the bit count
    if (m_info_header.m_bit_count != 24) {
        throw std::runtime_error("File " + m_file_name + " is not a 24-bit BMP file");
    }

    // Check compression
    if (m_info_header.m_compression != 0) {
        throw std::runtime_error("File " + m_file_name + " is compressed BMP file");
    }

    // Read the palette
    if(!file.read(reinterpret_cast<char *>(&m_color_header), sizeof (BMPColorHeader))){
        throw std::runtime_error("Could not read palette from file " + m_file_name);
    }

    // Read the image data
//    file.read(reinterpret_cast<char *>(imageData.data()), infoHeader.imageSize);

    // Close the file
    file.close();

}

#include "ImageWriter.h"
#include <fstream>
#include <iostream>

void ImageWriter::writePPM(const std::string& filename, int width, int height, const std::vector<uint8_t>& pixels) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return;
    }
    
    // PPM header
    file << "P6\n";
    file << width << " " << height << "\n";
    file << "255\n";
    
    // Write pixel data (RGB)
    file.write(reinterpret_cast<const char*>(pixels.data()), pixels.size());
    file.close();
}

#pragma once
#include <cstdint>
#include <string>
#include <vector>

class ImageWriter {
public:
    static void writePPM(const std::string& filename, int width, int height, const std::vector<uint8_t>& pixels);
};

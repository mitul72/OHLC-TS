#ifndef PARSEDATA_HPP
#define PARSEDATA_HPP

#include <iostream>
#include <fstream>
#include <filesystem>
#include <ArduinoJson.h>
#include <vector>
#include <stdexcept>

namespace fs = std::filesystem;

class ParseData
{
private:
    double price = 0;
    double quantity = 0;
    auto ParseFile(const std::string &filePath) -> void;

public:
    ParseData(const std::string &filePath);
    std::vector<double> prices;
    std::vector<double> volumes;

    auto ParseDirectory(const std::string &directoryPath) -> void;
};
#endif // PARSEDATA_HPP

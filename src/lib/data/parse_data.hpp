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
    double volume = 0;

    /**
     * The function `ParseFile` reads a file line by line, parses each line as JSON, extracts specific
     * values from the JSON, and stores them in vectors.
     *
     * @param filePath The `filePath` parameter is a `std::string` that represents the path to the file
     * that needs to be parsed.
     *
     * @return The function is not returning anything. The return type of the function is `void`, which
     * means it does not return any value.
     */
    auto ParseFile(const std::string &filePath) -> void;

public:
    std::vector<double> prices;
    std::vector<double> volumes;

    /**
     * The ParseData constructor takes a file path as input and calls the ParseDirectory function.
     *
     * @param filePath The `filePath` parameter is a reference to a constant `std::string` object. It
     * represents the path to a file that needs to be parsed.
     */
    ParseData(const std::string &filePath);

    /**
     * The function `ParseDirectory` takes a directory path as input and parses all the ".ndjson" files in
     * that directory.
     *
     * @param directoryPath A string representing the path to the directory that needs to be parsed.
     */
    auto ParseDirectory(const std::string &directoryPath) -> void;
};
#endif // PARSEDATA_HPP

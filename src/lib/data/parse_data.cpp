#include "parse_data.hpp"

ParseData::ParseData(const std::string &filePath)
{
    ParseDirectory(filePath);
}

auto ParseData::ParseFile(const std::string &filePath) -> void
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    json jsonData;
    try
    {
        auto it = jsonData.find("price");
        file >> jsonData;
        if (it != jsonData.end())
        {
            price = jsonData["price"].get<double>();
            quantity = jsonData["quantity"].get<double>();
        }
        else
        {
            price = jsonData["executed_price"].get<double>();
            quantity = jsonData["executed_quantity"].get<double>();
        }
        prices.push_back(price);
        volumes.push_back(quantity);

        // reset prices

        price = 0;
        quantity = 0;
    }
    catch (const json::parse_error &e)
    {
        std::cerr << "Error parsing JSON in file " << filePath << ": " << e.what() << std::endl;
        return;
    }
    catch (const std::exception &e)
    {
        // Catch any exception derived from std::exception
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
    catch (...)
    {
        // Catch any other unknown exceptions
        std::cerr << "Unknown exception caught." << std::endl;
    }
}

auto ParseData::ParseDirectory(const std::string &directoryPath) -> void
{
    for (const auto &entry : fs::directory_iterator(directoryPath))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".ndjson")
        {
            ParseFile(entry.path().string());
        }
    }
}

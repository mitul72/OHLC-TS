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
    // Use nlohmann::json to parse each line separately
    std::string line;
    while (std::getline(file, line))
    {
        try
        {
            StaticJsonDocument<1024> jsonData;
            DeserializationError error = deserializeJson(jsonData, line);
            if (jsonData.containsKey("price"))
            {
                price = jsonData["price"].as<double>();
                quantity = jsonData["quantity"].as<double>();
                prices.push_back(price);
                volumes.push_back(quantity * price);
            }
            else if (jsonData.containsKey("execution_price"))
            {

                price = jsonData["execution_price"].as<double>();
                quantity = jsonData["executed_quantity"].as<double>();
                prices.push_back(price);
                volumes.push_back(quantity * price);
            }

            // reset prices

            price = 0;
            quantity = 0;
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
    file.close();
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

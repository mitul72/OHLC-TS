#include "parse_data.hpp"

/**
 * The ParseData constructor takes a file path as input and calls the ParseDirectory function.
 *
 * @param filePath The `filePath` parameter is a reference to a constant `std::string` object. It
 * represents the path to a file that needs to be parsed.
 */
ParseData::ParseData(const std::string &filePath)
{
    ParseDirectory(filePath);
}

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
                volumes.push_back(quantity);
                volume += quantity;
            }
            else if (jsonData.containsKey("execution_price"))
            {

                price = jsonData["execution_price"].as<double>();
                quantity = jsonData["executed_quantity"].as<double>();
                prices.push_back(price);
                volumes.push_back(quantity);
                volume += quantity;
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

/**
 * The function `ParseDirectory` takes a directory path as input and parses all the ".ndjson" files in
 * that directory.
 *
 * @param directoryPath A string representing the path to the directory that needs to be parsed.
 */
auto ParseData::ParseDirectory(const std::string &directoryPath) -> void
{
    for (const auto &entry : fs::directory_iterator(directoryPath))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".ndjson")
        {
            // reset volume
            volume = 0;
            ParseFile(entry.path().string());
            volumes.push_back(volume);
        }
    }
}

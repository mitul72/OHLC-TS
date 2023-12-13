#include "config_json.hpp"
namespace ConfigKF
{

    json GetConfigJson(std::string path)
    {
        // Read JSON configuration
        std::ifstream configFile(path);
        if (!configFile.is_open())
        {
            std::cerr << "Failed to open configuration file." << std::endl;
            return 1;
        }

        json configJson;
        configFile >> configJson;
        return configJson;
    }
}
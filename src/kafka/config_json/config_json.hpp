#ifndef CONFIG_JSON_H
#define CONFIG_JSON_H

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

namespace ConfigKF
{
    json GetConfigJson(std::string path);
}
#endif // CONFIG_JSON_H

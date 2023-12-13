#include <cppkafka/cppkafka.h>
#include <string>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "config_json.hpp"
#include <vector>

using namespace cppkafka;
using json = nlohmann::json;

int main(int argc, char *argv[])
{
    double operand1 = 0;
    double operand2 = -1;
    int _operator = 0;

    std::cout << "Enter Operand1: ";
    std::cin >> operand1;
    try
    {
        std::cout << "Enter Operator (0 for add, 1 for subtract, 2 for multiply, 3 for divide, 4 for factorial): ";
        std::cin >> _operator;
        if (_operator < 0 || _operator > 4)
        {
            throw std::out_of_range("Invalid operator. Please enter a number between 0 and 4.");
        }
    }
    catch (const std::out_of_range &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    if (_operator != 4)
    {
        std::cout << "Enter Operand2: ";
        std::cin >> operand2;
    }
    try
    {
        auto configJson = ConfigKF::GetConfigJson("config.json");
        Configuration config = {
            {"metadata.broker.list", configJson["kafka"]["broker"].get<std::string>()}};

        // Create the producer
        Producer producer(config);

        // Create a JSON object
        json message = {
            {"operand1", operand1},
            {"operator", _operator},
            {"operand2", operand2}};

        // Convert JSON to string
        std::string jsonString = message.dump();

        producer.produce(MessageBuilder(configJson["kafka"]["topic"].get<std::string>()).partition(0).payload(jsonString));

        producer.flush();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 1;
    }
}

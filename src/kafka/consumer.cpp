#include <cppkafka/cppkafka.h>
#include <iostream>
#include <fstream>
#include "config_json.hpp"
#include <vector>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>

int main()
{

    auto configJson = ConfigKF::GetConfigJson("config.json");

    cppkafka::Consumer consumer({{"metadata.broker.list", configJson["kafka"]["broker"].get<std::string>()},
                                 {"group.id", configJson["kafka"]["group"].get<std::string>()}});

    consumer.subscribe({configJson["kafka"]["topic"].get<std::string>()});
    while (true)
    {
        // Open the file in append mode

        cppkafka::Message msg = consumer.poll();
        if (msg)
        {
            if (msg.get_error())
            {
                std::cout << "Error: " << msg.get_error() << std::endl;
            }
            else
            {
                std::cout << msg.get_payload() << std::endl;
                try
                {
                    std::ofstream outputFile("output.txt", std::ios::app);

                    std::string payload = msg.get_payload();

                    // Parse the JSON
                    json messageJson = json::parse(payload);

                    if (outputFile.is_open())
                    {
                        // Write data to the file
                        outputFile << msg.get_payload() << "\n";
                        // Close the file
                        std::cout << "written" << std::endl;
                    }

                    else
                    {
                        std::cerr << "Unable to open the file." << std::endl;
                        return 1;
                    }
                    outputFile.close();
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Exception caught: " << e.what() << std::endl;
                    return 1;
                }
            }
        }
    }

    std::cout << "Data appended to the file successfully." << std::endl;

    return 0;
}

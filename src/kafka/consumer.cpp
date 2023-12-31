#include <cppkafka/cppkafka.h>
#include <iostream>
#include <fstream>
#include "config_json.hpp"
#include <vector>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <sw/redis++/redis++.h>
#include <string>
#include <grpcpp/grpcpp.h>
#include "vector_message.grpc.pb.h"
#include "vector_message.pb.h"
#include "grpc-lib/grpc-lib.hpp"

int main()
{

    // Connect to Redis
    sw::redis::Redis redis("tcp://127.0.0.1:6379");

    auto configJson = ConfigKF::GetConfigJson("config.json");

    cppkafka::Consumer consumer({{"metadata.broker.list", configJson["kafka"]["broker"].get<std::string>()},
                                 {"group.id", configJson["kafka"]["group"].get<std::string>()}});

    consumer.subscribe({configJson["kafka"]["topic"].get<std::string>()});

    while (true)
    {

        cppkafka::Message msg = consumer.poll();

        if (msg)
        {
            std::cout << configJson["kafka"]["broker"] << std::endl;

            if (msg.get_error())
            {
                std::cout << "Error: " << msg.get_error() << std::endl;
            }
            else
            {
                try
                {

                    std::string payload = msg.get_payload();
                    // Store the serialized vector
                    redis.set("OHLC", payload);
                    vector_message_list list;
                    if (!list.ParseFromString(payload))
                    {
                        std::cerr << "Failed to parse MyStructList." << std::endl;
                        return -1;
                    }

                    GRPCLib::RunServer();
                    GRPCLib::TestServer(list);
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Exception caught: " << e.what() << std::endl;
                    return 1;
                }
            }
        }
    }
}
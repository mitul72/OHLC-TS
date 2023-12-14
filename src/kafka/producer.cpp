#include <cppkafka/cppkafka.h>
#include <string>
#include <iostream>
#include <fstream>
#include "config_json.hpp"
#include <vector>
#include "parse_data.hpp"
#include "ohlc_calculator.hpp"
#include "vector_message.pb.h"

using namespace cppkafka;

int main(int argc, char *argv[])
{
    try
    {
        ParseData dataparser("rawdata/subsetdata");
        OHLCCalculator mycalc;
        auto ohlc_vector = mycalc.GetOHLC(dataparser.prices, dataparser.volumes);
        // Create an instance of your protobuf message
        vector_message_list my_struct_list;

        for (const auto &ohlc : ohlc_vector)
        {
            // Create a new MyStruct message
            vector_message *my_struct = my_struct_list.add_messages();

            // Set the fields
            my_struct->set_open(ohlc.open);
            my_struct->set_high(ohlc.high);
            my_struct->set_low(ohlc.low);
            my_struct->set_close(ohlc.close);
            my_struct->set_volume(ohlc.volume);
            my_struct->set_value(ohlc.value);
        }
        std::string serialized_string = my_struct_list.SerializeAsString();

        auto configJson = ConfigKF::GetConfigJson("config.json");
        Configuration config = {
            {"metadata.broker.list", configJson["kafka"]["broker"].get<std::string>()},
            {"message.max.bytes", 5725714},
        };

        // Create the producer
        Producer producer(config);

        producer.produce(MessageBuilder(configJson["kafka"]["topic"].get<std::string>()).partition(0).payload(serialized_string));

        producer.flush();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 1;
    }
}

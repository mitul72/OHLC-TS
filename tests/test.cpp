#include "gtest/gtest.h"
#include "parse_data.hpp"
#include "ohlc_calculator.hpp"
#include "vector_message.pb.h"
#include <google/protobuf/util/message_differencer.h>

TEST(Protobuf, SerializationDeserialization)
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
    vector_message_list received_struct_list;
    received_struct_list.ParseFromString(serialized_string);
    auto res = google::protobuf::util::MessageDifferencer::Equals(my_struct_list, received_struct_list);

    ASSERT_TRUE(res);
}
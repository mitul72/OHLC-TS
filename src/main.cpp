#include <iostream>
#include "parse_data.hpp"

int main()
{
    ParseData dataparser("rawdata/subsetdata");
    for (auto i : dataparser.prices)
    {
        std::cout << i << std::endl;
    }
}
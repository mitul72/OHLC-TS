#include <iostream>
#include <vector>

struct OHLC
{
    double open;
    double high;
    double low;
    double close;
    double volume;
    double value;
};

class OHLCCalculator
{
public:
    /**
     * The GetOHLC function calculates OHLC (Open, High, Low, Close) data from given price and volume data
     * and prints the results.
     *
     * @return The function `GetOHLC()` returns a vector of OHLC (Open, High, Low, Close) data.
     */
    auto GetOHLC() -> std::vector<OHLC>;

    void convert_to_protobuf();
    void send_protobuf_to_kafka();

private:
    /**
     * The function calculates OHLC (Open, High, Low, Close) values for a given set of prices and volumes.
     *
     * @param prices A vector of double values representing the prices of a financial instrument at
     * different time intervals.
     * @param volumes The `volumes` parameter is a vector of double values representing the trading volumes
     * corresponding to each price in the `prices` vector. Each element in the `volumes` vector should
     * correspond to the same index in the `prices` vector.
     * @param ohlcData A vector of OHLC (Open, High, Low, Close) data structures. Each OHLC structure
     * contains the open, high, low, close, volume, and value of a particular data point.
     */
    auto CalculateOHLC(const std::vector<double> &prices, const std::vector<double> &volumes, std::vector<OHLC> &ohlcData) -> void;
};

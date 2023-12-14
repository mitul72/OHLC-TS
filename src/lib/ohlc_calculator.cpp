#include "ohlc_calculator.hpp"

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
auto OHLCCalculator::CalculateOHLC(const std::vector<double> &prices, const std::vector<double> &volumes, std::vector<OHLC> &ohlcData) -> void
{
    int dataSize = prices.size();
    ohlcData.resize(dataSize);

    for (int i = 0; i < dataSize; ++i)
    {
        OHLC &ohlc = ohlcData[i];
        ohlc.open = prices[i];
        ohlc.high = prices[i];
        ohlc.low = prices[i];
        ohlc.close = prices[i];
        ohlc.volume = volumes[i];
        ohlc.value = volumes[i] * prices[i];
    }

    for (int i = 1; i < dataSize; ++i)
    {
        OHLC &ohlc = ohlcData[i];
        OHLC &prevOHLC = ohlcData[i - 1];

        if (ohlc.open > prevOHLC.close)
            ohlc.open = prevOHLC.close;

        if (ohlc.high < prevOHLC.close)
            ohlc.high = prevOHLC.close;

        if (ohlc.low > prevOHLC.close)
            ohlc.low = prevOHLC.close;

        if (ohlc.close < prevOHLC.close)
            ohlc.close = prevOHLC.close;
    }
}

/**
 * The GetOHLC function calculates OHLC (Open, High, Low, Close) data from given price and volume data
 * and prints the results.
 *
 * @return The function `GetOHLC()` returns a vector of OHLC (Open, High, Low, Close) data.
 */
auto OHLCCalculator::GetOHLC(std::vector<double> &prices, std::vector<double> &volumes) -> std::vector<OHLC>
{

    std::vector<OHLC> ohlcData;
    CalculateOHLC(prices, volumes, ohlcData);

    // // Print the calculated OHLC data
    // for (const OHLC &ohlc : ohlcData)
    // {
    //     std::cout << "Open: " << ohlc.open << ", High: " << ohlc.high << ", Low: " << ohlc.low << ", Close: " << ohlc.close
    //               << ", Volume: " << ohlc.volume << ", Value: " << ohlc.value << std::endl;
    // }
    return ohlcData;
}
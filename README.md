# Bitwyre Technical Test

## Challenge 1: OHLC with Volume & Value

In modern online trading, the concept of OHLC with Volume & Value is used to indicate a stock's performance in real-time. This concept is highly leveraged by investors to guide their investment decisions. The nature of the data is small but moves ultra-fast, we can't afford to delay a single second. In this challenge, we only provide you a tiny subset of the real data.

### Terminologies

- **Stock**: A tradeable equity
- **OHLC**: Stands for Open High Low Close Price
- **Close Price**: The very last price of today
- **Previous Price**: The Close of the previous day
- **Open Price**: The first price of today
- **Lowest Price**: The lowest price that is ever archived today
- **Transaction**: Elements of a transaction are Quantity and Price. Quantity and Executed Quantity are the same. Price and ExecutionPrice are the same
- **Volume**: The sum of quantity
- **Value**: The sum of (quantity \* price)
- **Average Price**: Is value / volume

### Task Breakdown

1. Given Transaction inside a bunch of .ndjson from rawdata.zip. Calculate OHLC with Volume and Value
2. Convert OHLC to protobuf and send the protobuf to another service called consume OHLC.
3. In consumer you can save in database like redis or whatever
4. And inside the consumer create the gRPC server that will give client data based on stock

## Building Instructions

Follow these steps to build and run the project:

```bash
cmake -B build
cd build
make
./consumer-kafka
./producer-kafka
```

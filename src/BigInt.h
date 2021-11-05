#include <algorithm>
#include <cmath>
#include <cstdint>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define BASE 2
#define POSITIVE 1
#define NEGATIVE -1

const vector<vector<uint8_t>> HexToBin{
    {0, 0, 0, 0},
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 1, 0},
    {1, 0, 1, 0},
    {0, 1, 1, 0},
    {1, 1, 1, 0},
    {0, 0, 0, 1},
    {1, 0, 0, 1},
    {0, 1, 0, 1},
    {1, 1, 0, 1},
    {0, 0, 1, 1},
    {1, 0, 1, 1},
    {0, 1, 1, 1},
    {1, 1, 1, 1},
};

const vector<char> BinToHex{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

typedef deque<int8_t> deqBin;

class BigInt {
   private:
    int8_t sign = POSITIVE;
    deqBin data;

    void resize(const size_t& size, const int8_t& val);
    void trim();
    BigInt shift(BigInt data, uint16_t amount);
    BigInt karatsubaMultiply(BigInt a, BigInt b);

   public:
    BigInt(){}
    // only for 0 | 1
    BigInt(uint8_t n) {
        this->data.clear();
        this->data.push_back(n);
    }
    BigInt(deqBin data) {
        this->data = data;
    }
    size_t size();
    void fromHex(const string& hex);
    string toHex();
    void printRawBin();
    BigInt abs();

    BigInt operator+(BigInt rhs);
    BigInt operator-(BigInt rhs);
    BigInt operator*(BigInt rhs);

    BigInt operator-();

    bool operator<(BigInt rhs);
    bool operator>(BigInt rhs);
    bool operator<=(BigInt rhs);
    bool operator>=(BigInt rhs);
};

#include "BigInt.h"

void BigInt::fromHex(const string& hex) {
    this->data.clear();
    int16_t pos = 0;
    if (hex[0] == '-') {
        this->sign = NEGATIVE;
        ++pos;
    } else if (hex[0] == '+') {
        ++pos;
    }
    uint8_t d;
    for (int16_t i = hex.length() - 1; i >= pos; --i) {
        if (isdigit(hex[i])) {
            d = hex[i] - '0';
            this->data.insert(this->data.end(), HexToBin[d].begin(), HexToBin[d].end());
        } else if (isxdigit(hex[i])) {
            d = tolower(hex[i]) - 'a' + 10;
            this->data.insert(this->data.end(), HexToBin[d].begin(), HexToBin[d].end());
        }
    }
    this->trim();
}

string BigInt::toHex() {
    size_t actualSize = ceil(this->data.size() / 4.0) * 4;
    this->data.resize(actualSize, 0);
    if (this->sign == NEGATIVE) {
        cout << "-";
    }
    string result = "";
    uint8_t c;
    for (int16_t i = this->data.size() - 1; i >= 0;) {
        c = this->data[i--] * 8 + this->data[i--] * 4 + this->data[i--] * 2 + this->data[i--];
        result += BinToHex[c];
    }

    return result;
}

// mainly for testing...
void BigInt::printRawBin() {
    for (size_t i = 0; i < this->data.size(); i++) {
        cout << (int)this->data[i];
    }
    cout << "\n";
}

BigInt BigInt::abs() {
    BigInt res = *this;
    res.sign = POSITIVE;
    return res;
}

void BigInt::trim() {
    while (!this->data.empty() && !this->data.back()) {
        this->data.pop_back();
    }

    if (this->data.empty()) {
        this->sign = POSITIVE;
        this->data.push_back(0);
    }
}

void BigInt::resize(const size_t& size, const int8_t& val) {
    this->data.resize(size, val);
}

size_t BigInt::size() {
    return this->data.size();
}

// shift left
BigInt BigInt::shift(BigInt bn, uint16_t amount) {
    BigInt res = bn;
    // cout << 0 << endl;
    for (size_t i = 0; i < amount; ++i) {
        res.data.push_front(0);
    };
    return res;
}

BigInt BigInt::karatsubaMultiply(BigInt a, BigInt b) {
    while (a.size() < b.size()) {
        a.data.push_back(0);
    }
    while (a.size() > b.size()) {
        b.data.push_back(0);
    }

    size_t n = a.size();

    if (n == 1) {
        return (a.data[0] == 1 && b.data[0] == 1) ? BigInt(1) : BigInt(0);
    }

    size_t k = ceil(n / 2.0);
    BigInt al(deqBin(a.data.begin(), a.data.begin() + k));
    BigInt ah(deqBin(a.data.begin() + k, a.data.end()));
    BigInt bl(deqBin(b.data.begin(), b.data.begin() + k));
    BigInt bh(deqBin(b.data.begin() + k, b.data.end()));

    BigInt p3 = karatsubaMultiply(al + ah, bl + bh);
    BigInt p2 = karatsubaMultiply(al, bl);  // d
    BigInt p1 = karatsubaMultiply(ah, bh);  // a
    BigInt ans = shift(p1, n) + shift(p3 - (p1 + p2), k) + p2;
    ans.trim();

    return ans;
}

/* OPERATOR */

BigInt BigInt::operator-() {
    BigInt res = *this;
    res.sign = -res.sign;
    return res;
}

BigInt BigInt::operator+(BigInt rhs) {
    if (this->sign == rhs.sign) {
        BigInt res = *this;
        uint8_t carry = 0;
        for (size_t i = 0; i < max(res.size(), rhs.size()) || carry; i++) {
            // when carry = 1 and
            if (i == res.size()) {
                res.data.push_back(0);
            }
            res.data[i] += carry + (i < rhs.size() ? rhs.data[i] : 0);
            carry = res.data[i] >= BASE;
            if (carry) {
                res.data[i] -= BASE;
            }
        }

        res.trim();
        return res;
    }
    return *this - (-rhs);
}

BigInt BigInt::operator*(BigInt rhs) {
    BigInt l = *this, r = rhs;
    l.sign = POSITIVE;
    r.sign = POSITIVE;
    BigInt res = karatsubaMultiply(l, r);
    res.sign = this->sign * rhs.sign;
    res.trim();
    return res;
}

BigInt BigInt::operator-(BigInt rhs) {
    if (this->sign == rhs.sign) {
        if (this->abs() >= rhs.abs()) {
            BigInt res = *this;
            uint8_t borrow = 0;
            for (size_t i = 0; i < rhs.size() || borrow; ++i) {
                res.data[i] -= borrow + (i < rhs.size() ? rhs.data[i] : 0);
                borrow = res.data[i] < 0;
                if (borrow) {
                    res.data[i] += BASE;
                }
            }
            res.trim();
            return res;
        }
        return -(rhs - *this);
    }
    return *this + (-rhs);
}

/* COMPARATOR */

bool BigInt::operator<(BigInt rhs) {
    if (this->sign != rhs.sign) {
        return this->sign < rhs.sign;
    }
    if (this->data.size() != rhs.data.size()) {
        return this->data.size() * this->sign < rhs.data.size() * rhs.sign;
    }
    for (int16_t i = this->data.size() - 1; i >= 0; --i) {
        if (this->data[i] != rhs.data[i]) {
            return this->data[i] * this->sign < rhs.data[i] * rhs.sign;
        }
    }
    return false;
}
bool BigInt::operator>(BigInt rhs) {
    return rhs < *this;
}
bool BigInt::operator<=(BigInt rhs) {
    return !(rhs < *this);
}
bool BigInt::operator>=(BigInt rhs) {
    return !(*this < rhs);
}

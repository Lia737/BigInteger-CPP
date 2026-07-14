#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "BigInteger.h"
using namespace std;


const int BigInteger::max_size = 100000;

// Fixed: Initialized m_size to 0
BigInteger::BigInteger(int num) {
    m_size = 0;
    if (num == 0) {
        m_size = 1;
        m_digits = new char[2];
        m_digits[0] = '0';
        m_digits[1] = '\0';
        m_isnegative = false;
    }
    else {
        long long temp = (num < 0) ? -static_cast<long long>(num) : static_cast<long long>(num);
        long long count_temp = temp;
        while (count_temp) {
            m_size++;
            count_temp /= 10;
        }
        m_isnegative = (num < 0);
        m_digits = new char[m_size + 1];
        for (int i = m_size - 1; i >= 0; i--) {
            m_digits[i] = '0' + (temp % 10);
            temp /= 10;
        }
        m_digits[m_size] = '\0';
    }
}

BigInteger::BigInteger(int* arr, int n, bool isneg) {
    m_isnegative = isneg;
    m_size = n;
    m_digits = new char[m_size + 1];
    for (int i = 0; i < m_size; i++) {
        m_digits[i] = '0' + arr[i];
    }
    m_digits[m_size] = '\0';
}

BigInteger::BigInteger(const BigInteger& ob) {
    m_isnegative = ob.m_isnegative;
    m_size = ob.m_size;
    m_digits = new char[m_size + 1];
    for (int i = 0; i < m_size; i++) {
        m_digits[i] = ob.m_digits[i];
    }
    m_digits[m_size] = '\0';
}

BigInteger::~BigInteger() {
    delete[] m_digits;
}

// Fixed: Corrected self-assignment check & proper reallocation
BigInteger& BigInteger::operator=(const BigInteger& ob) {
    if (this != &ob) {
        char* temp_digits = new char[ob.m_size + 1];
        for (int i = 0; i < ob.m_size; i++) {
            temp_digits[i] = ob.m_digits[i];
        }
        temp_digits[ob.m_size] = '\0';

        delete[] m_digits;
        m_digits = temp_digits;
        m_size = ob.m_size;
        m_isnegative = ob.m_isnegative;
    }
    return *this;
}

// Fixed: Safe string parsing instead of unsafe buffer reads
istream& operator>>(istream& in, BigInteger& ob) {
    string temp;
    if (!(in >> temp)) return in;

    delete[] ob.m_digits;
    ob.m_isnegative = false;
    ob.m_size = 0;

    int start = 0;
    if (temp[0] == '-') {
        ob.m_isnegative = true;
        start = 1;
    }

    int actual_size = 0;
    for (size_t i = start; i < temp.length(); i++) {
        if (isdigit(temp[i])) {
            actual_size++;
        }
        else {
            cout << "Invalid character encountered. Setting to 0." << endl;
            ob = BigInteger(0);
            return in;
        }
    }

    if (actual_size == 0) {
        ob = BigInteger(0);
        return in;
    }

    if (actual_size > BigInteger::max_size) {
        cout << "Number truncated to max allowed digits." << endl;
        actual_size = BigInteger::max_size;
    }

    ob.m_size = actual_size;
    ob.m_digits = new char[ob.m_size + 1];

    for (int i = 0; i < ob.m_size; i++) {
        ob.m_digits[i] = temp[start + i];
    }
    ob.m_digits[ob.m_size] = '\0';

    return in;
}

ostream& operator<<(ostream& out, const BigInteger& ob) {
    if (ob.m_isnegative) out << "-";
    out << ob.m_digits;
    return out;
}

bool BigInteger::operator==(const BigInteger& ob) const {
    if (m_isnegative != ob.m_isnegative || m_size != ob.m_size) return false;
    for (int i = 0; i < m_size; i++) {
        if (m_digits[i] != ob.m_digits[i]) return false;
    }
    return true;
}

bool BigInteger::operator!=(const BigInteger& ob) const {
    return !(*this == ob);
}

bool BigInteger::operator<(const BigInteger& ob) const {
    if (m_isnegative != ob.m_isnegative) return m_isnegative;
    if (m_size != ob.m_size)
        return m_isnegative ? m_size > ob.m_size : m_size < ob.m_size;
    for (int i = 0; i < m_size; i++) {
        if (m_digits[i] != ob.m_digits[i])
            return m_isnegative ? m_digits[i] > ob.m_digits[i] : m_digits[i] < ob.m_digits[i];
    }
    return false;
}

bool BigInteger::operator>(const BigInteger& ob) const {
    return ob < *this;
}

bool BigInteger::operator<=(const BigInteger& ob) const {
    return !(ob < *this);
}

bool BigInteger::operator>=(const BigInteger& ob) const {
    return !(*this < ob);
}

BigInteger BigInteger::addAbsolute(const BigInteger& a, const BigInteger& b) const {
    int max_len = std::max(a.m_size, b.m_size);
    int* result = new int[max_len + 1];
    int carry = 0;
    int ai = a.m_size - 1;
    int bi = b.m_size - 1;
    int ri = max_len;

    while (ai >= 0 || bi >= 0 || carry) {
        int digitA = (ai >= 0) ? (a.m_digits[ai] - '0') : 0;
        int digitB = (bi >= 0) ? (b.m_digits[bi] - '0') : 0;
        int sum = digitA + digitB + carry;
        result[ri--] = sum % 10;
        carry = sum / 10;
        ai--;
        bi--;
    }

    int start = ri + 1;
    int result_size = max_len + 1 - start;
    BigInteger res(result + start, result_size, false);
    delete[] result;
    return res;
}

BigInteger BigInteger::subtractAbsolute(const BigInteger& a, const BigInteger& b) const {
    int* result = new int[a.m_size];
    int ai = a.m_size - 1;
    int bi = b.m_size - 1;
    int ri = a.m_size - 1;
    int borrow = 0;

    while (ai >= 0) {
        int digitA = a.m_digits[ai] - '0';
        int digitB = (bi >= 0) ? b.m_digits[bi] - '0' : 0;
        int diff = digitA - digitB - borrow;

        if (diff < 0) {
            diff += 10;
            borrow = 1;
        }
        else {
            borrow = 0;
        }

        result[ri--] = diff;
        ai--;
        bi--;
    }
    int start = 0;
    while (start < a.m_size - 1 && result[start] == 0)
        ++start;

    int result_size = a.m_size - start;
    BigInteger res(result + start, result_size, false);
    delete[] result;
    return res;
}

BigInteger BigInteger::operator-() const {
    BigInteger copy(*this);
    if (m_size != 1 || m_digits[0] != '0')
        copy.m_isnegative = !m_isnegative;
    return copy;
}

BigInteger BigInteger::operator+(const BigInteger& ob) const {
    if (!m_isnegative && !ob.m_isnegative) {
        return addAbsolute(*this, ob);
    }
    else if (m_isnegative && ob.m_isnegative) {
        BigInteger res = addAbsolute(*this, ob);
        res.m_isnegative = true;
        return res;
    }
    else if (!m_isnegative && ob.m_isnegative) {
        return *this - (-ob);
    }
    else {
        return ob - (-*this);
    }
}

BigInteger BigInteger::operator-(const BigInteger& ob) const {
    if (!m_isnegative && !ob.m_isnegative) {
        if (*this >= ob) {
            return subtractAbsolute(*this, ob);
        }
        else {
            BigInteger res = subtractAbsolute(ob, *this);
            res.m_isnegative = true;
            return res;
        }
    }
    else if (m_isnegative && ob.m_isnegative) {
        return (-ob) - (-*this);
    }
    else if (!m_isnegative && ob.m_isnegative) {
        return addAbsolute(*this, ob);
    }
    else {
        BigInteger res = addAbsolute(*this, ob);
        res.m_isnegative = true;
        return res;
    }
}

BigInteger BigInteger::operator*(const BigInteger& ob) const {
    if (*this == BigInteger(0) || ob == BigInteger(0)) {
        return BigInteger(0);
    }
    int resultSize = m_size + ob.m_size;
    int* result = new int[resultSize]();
    for (int i = m_size - 1; i >= 0; i--) {
        int digit1 = m_digits[i] - '0';
        for (int j = ob.m_size - 1; j >= 0; j--) {
            int digit2 = ob.m_digits[j] - '0';
            int mul = digit1 * digit2;
            int posLow = i + j + 1;
            int posHigh = i + j;

            int sum = mul + result[posLow];
            result[posLow] = sum % 10;
            result[posHigh] += sum / 10;
        }
    }
    int start = 0;
    while (start < resultSize - 1 && result[start] == 0) {
        ++start;
    }

    int finalSize = resultSize - start;
    BigInteger res(result + start, finalSize, m_isnegative != ob.m_isnegative);

    delete[] result;
    return res;
}

BigInteger BigInteger::abs() const {
    BigInteger copy = *this;
    copy.m_isnegative = false;
    return copy;
}

// Fixed: Optimized Division using faster estimation (prevents timeout crashes)
BigInteger BigInteger::operator/(const BigInteger& divisor) const {
    if (divisor == BigInteger(0)) {
        throw runtime_error("Division by zero is undefined.");
    }

    BigInteger dividend = this->abs();
    BigInteger divisorAbs = divisor.abs();

    if (dividend < divisorAbs) {
        return BigInteger(0);
    }

    int* result = new int[m_size];
    int result_index = 0;

    BigInteger current(0);

    for (int i = 0; i < m_size; ++i) {
        current = current * BigInteger(10) + BigInteger(m_digits[i] - '0');

        // Instead of repeated -1 subtraction, use safe single digit trial estimation
        int low = 0, high = 9, count = 0;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (divisorAbs * BigInteger(mid) <= current) {
                count = mid;
                low = mid + 1;
            }
            else {
                high = mid - 1;
            }
        }
        current = current - (divisorAbs * BigInteger(count));
        result[result_index++] = count;
    }
    int start = 0;
    while (start < result_index - 1 && result[start] == 0) {
        ++start;
    }

    int final_size = result_index - start;
    BigInteger res(result + start, final_size, m_isnegative != divisor.m_isnegative);
    delete[] result;
    return res;
}

BigInteger BigInteger::operator%(const BigInteger& divisor) const {
    if (divisor == BigInteger(0)) {
        throw runtime_error("Modulo by zero is undefined.");
    }

    BigInteger dividend = this->abs();
    BigInteger divisorAbs = divisor.abs();

    if (dividend < divisorAbs) {
        return *this;
    }

    BigInteger quotient = dividend / divisorAbs;
    BigInteger product = quotient * divisorAbs;
    BigInteger remainder = dividend - product;

    remainder.m_isnegative = m_isnegative;
    return remainder;
}

BigInteger& BigInteger::operator+=(const BigInteger& ob) {
    *this = *this + ob;
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& ob) {
    *this = *this - ob;
    return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& ob) {
    *this = *this * ob;
    return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& ob) {
    *this = *this / ob;
    return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& ob) {
    *this = *this % ob;
    return *this;
}

BigInteger& BigInteger::operator++() {
    *this = *this + BigInteger(1);
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger temp = *this;
    *this = *this + BigInteger(1);
    return temp;
}

BigInteger BigInteger::operator^(int exponent) const {
    if (exponent < 0) {
        throw runtime_error("Negative exponent is not supported.");
    }
    BigInteger result(1);
    BigInteger base(*this);
    // Optimized: Binary Exponentiation (O(log N) instead of O(N))
    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = result * base;
        }
        base = base * base;
        exponent /= 2;
    }
    return result;
}

BigInteger::operator int() const {
    int result = 0;
    int sign = m_isnegative ? -1 : 1;

    for (int i = 0; i < m_size; ++i) {
        result = result * 10 + (m_digits[i] - '0');
    }
    return sign * result;
}

BigInteger BigInteger::factorial(int n) {
    BigInteger result = 1;
    for (int i = 2; i <= n; ++i)
        result *= BigInteger(i);
    return result;
}

BigInteger BigInteger::sumArray(BigInteger* arr, int size) {
    BigInteger sum = 0;
    for (int i = 0; i < size; ++i)
        sum += arr[i];
    return sum;
}

BigInteger BigInteger::productArray(BigInteger* arr, int size) {
    BigInteger product = 1;
    for (int i = 0; i < size; ++i)
        product *= arr[i];
    return product;
}

BigInteger BigInteger:: maxArray(BigInteger* arr, int size) {
    BigInteger max = arr[0];
    for (int i = 1; i < size; ++i)
        if (arr[i] > max)
            max = arr[i];
    return max;
}


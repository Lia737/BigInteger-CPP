#pragma once
#include <iostream>
using namespace std;
class BigInteger {
private:
	int m_size;
	char* m_digits;
	bool m_isnegative;
	static const int max_size;
	BigInteger addAbsolute(const BigInteger&, const BigInteger&)const;
	BigInteger subtractAbsolute(const BigInteger&, const BigInteger&)const;
	BigInteger operator-()const;
	BigInteger abs() const;
public:
	BigInteger(int = 0);
	BigInteger(int*, int, bool = false);
	BigInteger(const BigInteger&);
	~BigInteger();
	BigInteger& operator=(const BigInteger&);
	friend istream& operator>>(istream&, BigInteger&);
	friend ostream& operator<<(ostream&, const BigInteger&);
	bool operator==(const BigInteger&)const;
	bool operator!=(const BigInteger&)const;
	bool operator<(const BigInteger&)const;
	bool operator>(const BigInteger&)const;
	bool operator<=(const BigInteger&)const;
	bool operator>=(const BigInteger&)const;
	BigInteger operator+(const BigInteger&)const;
	BigInteger operator-(const BigInteger&)const;
	BigInteger operator*(const BigInteger&)const;
	BigInteger operator/(const BigInteger&)const;
	BigInteger operator%(const BigInteger&)const;
	BigInteger& operator+=(const BigInteger&);
	BigInteger& operator-=(const BigInteger&);
	BigInteger& operator *= (const BigInteger&);
	BigInteger& operator/=(const BigInteger&);
	BigInteger& operator%=(const BigInteger&);
	BigInteger& operator++();
	BigInteger operator++(int);
	BigInteger operator^(int)const;
	operator int()const;
	BigInteger factorial(int n);
	BigInteger sumArray(BigInteger* arr, int size);
	BigInteger productArray(BigInteger* arr, int size);
	BigInteger maxArray(BigInteger* arr, int size);
};
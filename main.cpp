#include <iostream>
#include "BigInteger.h"

using namespace std;

int main() {
    BigInteger a, b, c;
    cout << "Enter numbers a b c \n";
    if (!(cin >> a >> b >> c)) return 0;

    cout << "a = " << a << endl;
    cout << "b = " << b << endl;
    cout << "c = " << c << endl;

    cout << "a + b = " << (a + b) << endl;
    cout << "a - c = " << (a - c) << endl;
    cout << "a * b = " << (a * b) << endl;
    cout << "a / b = " << (a / b) << endl;
    cout << "a % b = " << (a % b) << endl;
    cout << "++a = " << (++a) << endl;
    cout << "a^3 = " << (a ^ 3) << endl;

    int n;
    cout << "\nEnter a natural number to compute factorial: ";
    cin >> n;

    if (n < 0) {
        cout << "Factorial is not defined for negative numbers." << endl;
        return 1;
    }

    BigInteger fact = factorial(n);
    cout << n << "! = " << fact << endl;

    int size;
    cout << "\nEnter number of BigInteger elements in the array: ";
    cin >> size;

    BigInteger* arr = new BigInteger[size];
    cout << "Enter " << size << " BigInteger values:\n";
    for (int i = 0; i < size; ++i) {
        cout << "arr[" << i << "] = ";
        cin >> arr[i];
    };

    cout << "\nSum of array: " << sumArray( arr,  size) << endl;
    cout << "Product of array: " << productArray(arr, size) << endl;
    cout << "Maximum element: " <maxArray(arr, size) << endl;

    delete[] arr;
    return 0;
}
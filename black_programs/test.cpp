#include <sstream>
#include <cstring>
#include <iostream>
#include <stdlib.h>

using namespace std;

short get_digit(unsigned long long number, int digit_position) {
    
    if (digit_position < 0 || digit_position > 20) {
        return -1; 
    }

    unsigned long long shifted = number;
    for (int i = 0; i < digit_position; i++) {
        shifted /= 10;
        //cout << shifted << endl;
    }
    unsigned long long digit = shifted % 10;

    return digit;
}



int main() {
    unsigned long long	 i = 11;

    cout << get_digit(i, 2) << endl;
    
}
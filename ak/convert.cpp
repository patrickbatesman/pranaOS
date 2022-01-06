#include "convert.h"
#include "string.h"

using namespace pranaOS::ak;

int isspace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

char convert::intToString(int n) {
    static char ret[24];
    int numChars = 0;
    bool isNegative = false;

    if (n < 0) {
        n = -n;
        isNegative = true;
        numChars++;
    }
    int temp = n 

    do {
        numChars++;
        temp /= 10;
    } while (temp);

    ret[numChars] = 0;

    
}
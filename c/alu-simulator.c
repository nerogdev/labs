#include <stdio.h>

const int FLOAT_BITS = 32;
const int MANTISSA_BITS = 23;
const int EXPONENT_BITS = 8;

typedef union {
    float f;
    struct {
        unsigned int mantissa: 23;
        unsigned int exponent: 8;
        unsigned int sign: 1;
    } parts;
} Float_t;

const unsigned int SIGN_MASK = 0x80000000;
const unsigned int MANTISSA_MASK = 0x007FFFFF;
const unsigned int EXPONENT_MASK = 0x7F800000;

Float_t parseIntToFloat_t(int num) {
    Float_t fnum;
    fnum.parts.sign = (num < 0) ? 1 : 0;
    num = (num < 0) ? -num : num; // negative number to absolute value

    int mostSignificantBit = 0;
    for (int i = 31; i >= 0; i--) {
        if ((num >> i) & 1) { // getting the most significant bit '0010010' in this case, the bit position would be 5
            mostSignificantBit = i;
            break;
        }
    }

    fnum.parts.exponent = mostSignificantBit + 127; // bias to represent -126 to 127 in binary mode.
    fnum.parts.mantissa = (num << (MANTISSA_BITS - mostSignificantBit)) & 0x7FFFFF; // shift to the left due to the most significant bit and apply a mask to ensure that we keep the less significant 23 bits 
    return fnum;
}

Float_t parseFloatToFloat_t(float num) {
    Float_t fnum;
    unsigned int *ptr = (unsigned int *)&num; // access to the memory direction

    fnum.parts.sign = (*ptr & SIGN_MASK) >> FLOAT_BITS - 1;
    fnum.parts.exponent = (*ptr & EXPONENT_MASK) >> MANTISSA_BITS;
    fnum.parts.mantissa = *ptr & MANTISSA_MASK;

    return fnum;
}

void printFloat(Float_t *fnum) {
    if(fnum == NULL) {
        printf("{}\n");   
        return;
    }

    printf("{\n");
    printf("  \"number\": %f,\n", fnum->f);
    printf("  \"sign\": %d,\n", fnum->parts.sign);
    
    printf("  \"exponent\": ");
    for (int i = EXPONENT_BITS - 1; i >= 0; i--)
    {
        printf("%d", fnum->parts.exponent >> i & 1);
    }
    
    printf(",\n  \"mantissa\": ");
    for (int i = MANTISSA_BITS - 1; i >= 0; i--)
    {
        printf("%d", fnum->parts.mantissa >> i & 1);
    }
    printf("\n}\n");
}

int main(int argc, char *argv[]) {
    Float_t inum = parseIntToFloat_t(5);
    printFloat(&inum);
    Float_t fnum = parseFloatToFloat_t(-5.0);
    printFloat(&fnum);
    return 0;
}

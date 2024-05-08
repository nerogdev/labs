#include <stdio.h>

typedef union {
    float f;
    struct {
        unsigned int mantissa: 23;
        unsigned int exponent: 8;
        unsigned int sign: 1;
    } parts;
} Float_t;

Float_t parseFloat(int num) {
    Float_t fnum;
    fnum.parts.sign = (num < 0) ? 1 : 0;
    num = (num < 0) ? -num : num;

    int mostSignificantBit = 0;
    for (int i = 31; i >= 0; i--) {
        if ((num >> i) & 1) {
            mostSignificantBit = i;
            break;
        }
    }

    fnum.parts.exponent = mostSignificantBit + 127;
    fnum.parts.mantissa = (num << (23 - mostSignificantBit)) & 0x7FFFFF;
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
    printf("  \"exponent\": %d,\n", fnum->parts.exponent);
    printf("  \"mantissa\": %d\n", fnum->parts.mantissa);
    printf("}\n");
}

int main(int argc, char *argv[]) {
    Float_t fnum = parseFloat(60);
    printFloat(&fnum);

    fnum = parseFloat(10);
    printFloat(&fnum);
    fnum = parseFloat(4005);
    printFloat(&fnum);
    return 0;
}

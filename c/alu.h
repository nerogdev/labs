
#ifndef ALU_H
#define ALU_H

typedef union {
    float f;
    struct {
        unsigned int mantissa: 23;
        unsigned int exponent: 8;
        unsigned int sign: 1;
    } parts;
} Float_t;

void printFloat(const Float_t *num);
Float_t parseFloat(float num);
Float_t add(Float_t op1, Float_t op2);
Float_t subtract(Float_t op1, Float_t op2);

#endif

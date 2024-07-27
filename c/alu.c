#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "alu.h"

const int FLOAT_BITS = 32;
const int MANTISSA_BITS = 23;
const int EXPONENT_BITS = 8;
const int NON_MANTISSA_BITS = FLOAT_BITS - MANTISSA_BITS;

const unsigned int SIGN_MASK = 0x80000000;
const unsigned int MANTISSA_MASK = 0x007FFFFF;
const unsigned int EXPONENT_MASK = 0x7F800000;

void errorAluHandler(char *error) {
    printf("Error: %s\n", error);
    exit(1);
}

void errorAluMemory() {
    errorAluHandler("Assign Alu memory.");
}

bool debugMode() {
    char *mode = getenv("MODE");
    return mode && strcmp(mode, "DEBUG") == 0;
}

/**
 * free memory required
*/
char *formatTobits(int num, const int mask) {
    char *result = (char *)malloc((mask + 1) * sizeof(char));
    if (result == NULL) errorAluMemory();

    // null terminator
    result[mask] = '\0';

    for (int i = mask - 1; i >= 0; i--) {
        result[i] = (num & 1) ? '1' : '0';
        num >>= 1;
    }

    return result;
}

/**
 * free memory required
*/
char *formatToExponent(int num) {
    return formatTobits(num, EXPONENT_BITS);
}

/**
 * free memory required
*/
char *formatToMantissa(int num) {
    return formatTobits(num, MANTISSA_BITS);
}

/**
 * free memory required
*/
char *formatFloatAsString(const Float_t *num) {
    size_t len;
    char *exp;
    char *man;
    if (num == NULL) len = 2;
    else {
        exp = formatToExponent(num->parts.exponent);
        man = formatToMantissa(num->parts.mantissa);
        len = snprintf(NULL, 0, "{\n  \"number\": %f,\n  \"sign\": %d,\n  \"exponent\": %s,\n  \"mantissa\": %s\n}", num->f, num->parts.sign, exp, man);
    }
    
    char *result = (char*)malloc(len + 1);
    if (result == NULL) errorAluMemory();

    if (num == NULL) snprintf(result, len, "{}");
    else {
        snprintf(result, len, "{\n  \"number\": %f,\n  \"sign\": %d,\n  \"exponent\": %s,\n  \"mantissa\": %s\n}", num->f, num->parts.sign, exp, man);
        free(exp);
        free(man);
    }
    return result;
}

void printFloat(const Float_t *num) {
    char *floatstr = formatFloatAsString(num);
    printf("%s\n", floatstr);
    free(floatstr);
}

Float_t parseFloat(float num) {
    Float_t fnum;
    unsigned int *ptr = (unsigned int *)&num; // access to the memory direction
    fnum.parts.sign = (*ptr & SIGN_MASK) >> FLOAT_BITS - 1;
    fnum.parts.exponent = (*ptr & EXPONENT_MASK) >> MANTISSA_BITS;
    fnum.parts.mantissa = ((*ptr << NON_MANTISSA_BITS) >> NON_MANTISSA_BITS) & MANTISSA_MASK;

    if (debugMode()) printFloat(&fnum);

    return fnum;
}

Float_t add(Float_t op1, Float_t op2) {
    if (op1.parts.exponent == 0 && op1.parts.mantissa == 0)
        return op2;
    if (op2.parts.exponent == 0 && op2.parts.mantissa == 0)
        return op1;

    // Implicit theoretical bits
    unsigned int mantissa1_tmp = op1.parts.mantissa | 0x00800000;
    unsigned int mantissa2_tmp = op2.parts.mantissa | 0x00800000;

    // Normalize
    // TODO: check exponent masks: exmaple 6
    // exponent: 10000001
    // how to get the exponent result? 6 and 3
    if (op1.parts.exponent < op2.parts.exponent) {
        mantissa1_tmp >>= op2.parts.exponent - op1.parts.exponent;
        op1.parts.exponent = op2.parts.exponent;
    } else if (op2.parts.exponent < op1.parts.exponent) {
        mantissa2_tmp >>= op1.parts.exponent - op2.parts.exponent;
        op2.parts.exponent = op1.parts.exponent;
    }

    unsigned int mantissa;
    unsigned int exponent = op1.parts.exponent;
    unsigned int sign;
    if (op1.parts.sign == op2.parts.sign) {
        mantissa = mantissa1_tmp + mantissa2_tmp;
        sign = op1.parts.sign;
        // shift to the right til least significant bit
        // BUGFIX: "dbe dejar passar cuando las mantisas exponentes son igules y el bit mas importatnte es igual 1"
        while (mantissa && (mantissa & 0x00800000) == 0) {
            printf("entrooooooooooooooooooooooooooooooooooooo");
            exponent++;
            mantissa >>= 1;
        }
    } else { 
        if (mantissa1_tmp >= mantissa2_tmp) {
            mantissa = mantissa1_tmp - mantissa2_tmp;
            sign = op1.parts.sign;
        } else {
            mantissa = mantissa2_tmp - mantissa1_tmp;
            sign = op2.parts.sign;
        }
        // shift to the left til least significant bit
        while (mantissa && (mantissa & 0x00800000) == 0) {
            mantissa <<= 1;
            exponent--;
        }
        if (mantissa == 0) exponent = 0;
    }

    Float_t result;
    result.parts.sign = sign;
    result.parts.exponent = exponent;
    result.parts.mantissa = mantissa & MANTISSA_MASK; // omitting implicit bit
    return result;
}

Float_t subtract(Float_t op1, Float_t op2) {
    op2.parts.sign ^= 1;
    return add(op1, op2);
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define COMMANDS_SIZE 3
#define EXIT_IDX 2
#define EXIT "x"
#define EXIT_DSC "exit"
#define ADD "add"
#define ADD_DSC "Add"
#define SUBTRACT "sub"
#define SUBTRACT_DSC "Subtract"

const int FLOAT_BITS = 32;
const int MANTISSA_BITS = 23;
const int EXPONENT_BITS = 8;
const int NON_MANTISSA_BITS = FLOAT_BITS - MANTISSA_BITS;

typedef struct {
    char code[4];
    char dsc[9];
} Command_t;

typedef union {
    float f;
    struct {
        unsigned int mantissa: 23;
        unsigned int exponent: 8;
        unsigned int sign: 1;
    } parts;
} Float_t;

/**
 * The order is important to check every command individually
*/
const Command_t COMMANDS[COMMANDS_SIZE] = {
    {ADD, ADD_DSC},
    {SUBTRACT, SUBTRACT_DSC},
    {EXIT, EXIT_DSC},
};

const unsigned int SIGN_MASK = 0x80000000;
const unsigned int MANTISSA_MASK = 0x007FFFFF;
const unsigned int EXPONENT_MASK = 0x7F800000;

bool debugMode() {
    char *mode = getenv("MODE");
    return mode && strcmp(mode, "DEBUG") == 0;
}

void errorHandler(char *error) {
    printf("Error: %s\n", error);
    exit(1);
}

void printBits(const int num, const int mask) {
    for (int i = mask - 1; i >= 0; i--)
        printf("%d", num >> i & 1);
}

void printFloat(const Float_t *fnum) {
    if(fnum == NULL) {
        printf("{}\n");   
        return;
    }

    printf("{\n");
    printf("  \"number\": %f,\n", fnum->f);
    printf("  \"sign\": %d,\n", fnum->parts.sign);
    
    printf("  \"exponent\": ");
    printBits(fnum->parts.exponent, EXPONENT_BITS);
    
    printf("\n  \"mantissa\": ");
    printBits(fnum->parts.mantissa, MANTISSA_BITS);

    printf("\n}\n");
}

Float_t parseFloat(float num) {
    Float_t fnum;
    unsigned int *ptr = (unsigned int *)&num; // access to the memory direction
    fnum.parts.sign = (*ptr & SIGN_MASK) >> FLOAT_BITS - 1;
    fnum.parts.exponent = (*ptr & EXPONENT_MASK) >> MANTISSA_BITS;
    fnum.parts.mantissa = ((*ptr << NON_MANTISSA_BITS) >> NON_MANTISSA_BITS) & MANTISSA_MASK;

    if(debugMode()) printFloat(&fnum);

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
        while (mantissa && (mantissa & 0x00800000) == 0) {
            mantissa >>= 1;
            exponent++;
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

char *readInputInteractive() {
    static char input[8];
    
    printf("Commands:\n");
    for (int i = 0; i < COMMANDS_SIZE; i++)
        printf("- %s: %s\n", COMMANDS[i].dsc, COMMANDS[i].code);
    printf("\n\nType a command: ");
    
    scanf("%8s", input);
    return input;
}

float readNumberInteractive(char *msg) {
    float input;
    printf("%s", msg);
    if (scanf("%f", &input) == 1)
        return input;
    
    errorHandler("needed a float");
}

bool isEquivalent(char *value, Command_t cmd) {
    return strcasecmp(value, cmd.code) == 0 || strcasecmp(value, cmd.dsc) == 0;
}

bool isExit(char *value) {
    return isEquivalent(value, COMMANDS[EXIT_IDX]);
}

bool isAdd(char *value) {
    return isEquivalent(value, COMMANDS[0]);
}

bool isSubtract(char *value) {
    return isEquivalent(value, COMMANDS[1]);
}


void exitIfCmd(char *cmd) {
    if (!isExit(cmd)) return;
    printf("\n");
    sleep(1);
    printf("Bye then...\n");
    exit(0);
}

bool cmdExist(char *cmd) {
    for (int i = 0; i < COMMANDS_SIZE; i++) {
        if (isEquivalent(cmd, COMMANDS[i]))
            return true;
    }
    return false;
}

char *readCommand(int cmdSize, char *cmds[]) {
    if (cmdSize >= 2) return cmds[1];
    return readInputInteractive();
}

int main(int argc, char *argv[]) {
    char *cmd;
    bool cmdok = true;
    do {
        if (!cmdok) printf("Wrong command. Try again!\n\n");
        cmd = readCommand(argc--, argv); // decreases in case argument is wrong, avoid infinitive loop
        exitIfCmd(cmd);
        cmdok = cmdExist(cmd);
    } while (!cmdok);

    float op1 = readNumberInteractive("Operand 1: ");
    float op2 = readNumberInteractive("Operand 2: ");
    Float_t result;

    if(isAdd(cmd))
        result = add(parseFloat(op1), parseFloat(op2));
    if (isSubtract(cmd))
        result = subtract(parseFloat(op1), parseFloat(op2));

    printf("Result:\n");
    printFloat(&result);
    return 0;
}

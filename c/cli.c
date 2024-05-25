#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "alu.h"

#define COMMANDS_SIZE 3
#define EXIT_IDX 2
#define EXIT "x"
#define EXIT_DSC "exit"
#define ADD "add"
#define ADD_DSC "Add"
#define SUBTRACT "sub"
#define SUBTRACT_DSC "Subtract"

typedef struct {
    char code[4];
    char dsc[9];
} Command_t;

/**
 * The order is important to check every command individually
*/
const Command_t COMMANDS[COMMANDS_SIZE] = {
    {ADD, ADD_DSC},
    {SUBTRACT, SUBTRACT_DSC},
    {EXIT, EXIT_DSC},
};

void errorHandler(char *error) {
    printf("Error: %s\n", error);
    exit(1);
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
    char *floatstr = formatFloatAsString(&result); 
    printf("%s\n", floatstr);
    free(floatstr);
  
    return 0;
}

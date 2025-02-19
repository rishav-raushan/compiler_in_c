#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define maximum_vari_size 10
#define maximum_coding_length 100

typedef struct {
    char name;int addr;
} Variable;

Variable variables[maximum_vari_size];
int variableCount = 0;
int next_add = 100;
int no_of_labels = 0;
int gettingaddrOfvar(char name) {
    for (int i = 0; i < variableCount; i++) {
        if (variables[i].name == name) {
            return variables[i].addr;
        }
    }
    variables[variableCount].name = name;
    variables[variableCount].addr = next_add++;
    return variables[variableCount++].addr;
}
void generatingassemblycode(const char *inputFile, const char *outputFile) {
    FILE *input = fopen(inputFile, "r");
    FILE *output = fopen(outputFile, "w");
    char line[maximum_coding_length];
    int if_labels = no_of_labels++; 
    int else_labels = no_of_labels++; 

    while (fgets(line, sizeof(line), input)) {
        if (strstr(line, "int")) {
            continue;  
        } 
        else if (strstr(line, "if")) {
            char var;
            int value;
            if (sscanf(line, "if (%c == %d)", &var, &value) == 2) {
                int addr = gettingaddrOfvar(var);
                fprintf(output, "mov A M %d\n", addr);  
                fprintf(output, "cmp A %d\n", value);     
                fprintf(output, "jne end_if_%d\n", if_labels); 
                fprintf(output, "jmp end_else_%d\n", else_labels); 
            }
        }
        else if (strstr(line, "else")) {
            fprintf(output, "end_if_%d:\n", if_labels);  
            fprintf(output, "jmp end_else_%d\n", else_labels); 
        }
        else if (strchr(line, '=')) {
            char var1, var2, op;
            int value;
            if (sscanf(line, "%c = %d", &var1, &value) == 2) {
                fprintf(output, "ldi A %d\n", value);           
                fprintf(output, "mov M A %d\n", gettingaddrOfvar(var1));  
            } 
            else if (sscanf(line, "%c = %c %c 1", &var1, &var1, &op) == 3) {
                int addr = gettingaddrOfvar(var1);
                fprintf(output, "mov A M %d\n", addr);  
                if (op == '+') {
                    fprintf(output, "add M 101\n");  
                } else if (op == '-') {
                    fprintf(output, "sub M 101\n");  
                }
                fprintf(output, "mov M A %d\n", addr);  
            }
else if (sscanf(line, "%c = %c %c %d", &var1, &var2, &op, &value) == 4) {
                fprintf(output, "mov A M %d\n", gettingaddrOfvar(var2));  
                if (op == '+') {
                    fprintf(output, "add M %d\n", value);  
                } else if (op == '-') {
                    fprintf(output, "sub M %d\n", value);  
                }
                fprintf(output, "mov M A %d\n", gettingaddrOfvar(var1));  
            }
        }
    }
    fprintf(output, "end_else_%d:\n", else_labels);
    fprintf(output, "hlt\n");
    fclose(input);
    fclose(output);
}
int main() {
    generatingassemblycode("input.txt", "output.asm");
    return 0;
}
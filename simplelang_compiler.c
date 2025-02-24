#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define maximum_token_length 100
#define maximum_variable 10
#define maximum_code_length 100

// Token Types
typedef enum {
    T_INT, T_IDENTIFIER, T_NUMBER, T_ASSIGN,
    T_PLUS, T_MINUS, T_MULT, T_DIV,
    T_IF, T_ELSE, T_EQUAL, T_LBRACE,
    T_RBRACE, T_SEMICOLON, T_UNKNOWN, T_EOF
} TokenType;

// Token Structure
typedef struct {
    TokenType type;
    char text[maximum_token_length];
} Token;

// Variable Structure
typedef struct {
    char name;
    int addr;
} Variable;

Variable variables[maximum_variable];
int varCount = 0;
int nextAddr = 100;

// Get Variable Address
int getVarAddress(char name) {
    for (int i = 0; i < varCount; i++) {
        if (variables[i].name == name) {
            return variables[i].addr;
        }
    }
    variables[varCount].name = name;
    variables[varCount].addr = nextAddr++;
    return variables[varCount++].addr;
}

// Lexer: Get Next Token
void tokenizeFile(FILE *file, FILE *tokensFile) {
    Token token;
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (isspace(c)) continue;

        if (isalpha(c)) {
            int len = 0;
            token.text[len++] = c;
            while (isalnum(c = fgetc(file))) {
                if (len < maximum_token_length - 1) token.text[len++] = c;
            }
            ungetc(c, file);
            token.text[len] = '\0';

            if (strcmp(token.text, "int") == 0) token.type = T_INT;
            else if (strcmp(token.text, "if") == 0) token.type = T_IF;
            else if (strcmp(token.text, "else") == 0) token.type = T_ELSE;
            else token.type = T_IDENTIFIER;
            fprintf(tokensFile, "Token: %s\n", token.text);
        } else {
            char symbol[3] = {c, '\0'};
            fprintf(tokensFile, "Token: %s\n", symbol);
        }
    }
}

// Code Generation
void generateAssembly(const char *inputFile, const char *outputFile) {
    FILE *input = fopen(inputFile, "r");
    FILE *output = fopen(outputFile, "w");
    char line[maximum_code_length];

    if (!input || !output) {
        perror("Error opening file in generateAssembly");
        return;
    }

    while (fgets(line, sizeof(line), input)) {
        if (strstr(line, "int")) {
            continue;
        } else if (strstr(line, "if")) {
            char var;
            int value;
            if (sscanf(line, "if (%c == %d)", &var, &value) == 2) {
                fprintf(output, "cmp M %d %d\n", getVarAddress(var), value);
                fprintf(output, "jne else_block\n");
            }
        } else if (strchr(line, '=')) {
            char var1, var2, op;
            int value;
            if (sscanf(line, "%c = %d", &var1, &value) == 2) {
                fprintf(output, "ldi A %d\n", value);
                fprintf(output, "mov M A %d\n", getVarAddress(var1));
            } else if (sscanf(line, "%c = %c %c %c", &var1, &var2, &op, &var1) == 4) {
                fprintf(output, "mov A M %d\n", getVarAddress(var2));
                fprintf(output, "add M %d\n", getVarAddress(var1));
                fprintf(output, "mov M A %d\n", getVarAddress(var1));
            }
        } else if (strstr(line, "else")) {
            fprintf(output, "else_block:\n");
        }
    }
    fprintf(output, "end:\nhlt\n");
    fclose(input);
    fclose(output);
}

// AST Generation
void generateAST_node(FILE *inputFile, FILE *astFile) {
    char line[maximum_code_length];
    while (fgets(line, sizeof(line), inputFile)) {
        if (strstr(line, "if")) {
            fprintf(astFile, "AST Node: if statement {\n");
        } else if (strstr(line, "else")) {
            fprintf(astFile, "} else {\n");
        } else {
            fprintf(astFile, "    AST Node: %s", line);
        }
    }
    fprintf(astFile, "}\n");
}

// Main Function
int main() {
    printf("Starting Compilation...\n");
    FILE *inputFile = fopen("input.txt", "r");
    if (!inputFile) {
        perror("Error: Could not open input.txt");
        return 1;
    }

    FILE *tokensFile = fopen("tokens.txt", "w");
    FILE *astFile = fopen("ast_output.txt", "w");
    if (!tokensFile || !astFile) {
        perror("Error: Could not open output files");
        return 1;
    }

    printf("Generating Tokens...\n");
    tokenizeFile(inputFile, tokensFile);
    rewind(inputFile);

    printf("Generating AST...\n");
    generateAST_node(inputFile, astFile);

    fclose(tokensFile);
    fclose(astFile);
    fclose(inputFile);

    printf("Generating Assembly...\n");
    generateAssembly("input.txt", "output.asm");
    
    printf("Compilation Completed Successfully!\n\n\n\n");
printf("For Tokens : cat tokens.txt\n");
printf("For AST nodes: cat ast_output.txt\n");
printf("For Assembly code: cat output.asm\n");

    return 0;
}

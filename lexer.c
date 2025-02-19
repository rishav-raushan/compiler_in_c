#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define MAXIMUM_TOKEN_LENGTH 100
typedef enum {
     T_ELSE, T_EQUAL,T_NUMBER, T_ASSIGN,
    T_PLUS, T_MINUS, T_MULT,T_INT, T_IDENTIFIER, T_DIV,
    T_IF,  T_LBRACE,T_SEMICOLON,
    T_RBRACE,  T_UNKNOWN, T_EOF
} TokenType;
typedef struct {
    TokenType type;
    char texting[MAXIMUM_TOKEN_LENGTH];
} Token;
void getNextToken(FILE *file, Token *token) {
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (isspace(c)) continue;

        if (isalpha(c)) {
            int len = 0;
            token->texting[len++] = c;
            while (isalnum(c = fgetc(file))) {
                if (len < MAXIMUM_TOKEN_LENGTH - 1) token->texting[len++] = c;
            }
            ungetc(c, file);
            token->texting[len] = '\0';

            if (strcmp(token->texting, "int") == 0) token->type = T_INT;
            else if (strcmp(token->texting, "if") == 0) token->type = T_IF;
            else if (strcmp(token->texting, "else") == 0) token->type = T_ELSE;
            else token->type = T_IDENTIFIER;
            return;
        }

        if (isdigit(c)) {
            int len = 0;
            token->texting[len++] = c;
            while (isdigit(c = fgetc(file))) {
                if (len < MAXIMUM_TOKEN_LENGTH - 1) token->texting[len++] = c;
            }
            ungetc(c, file);
            token->texting[len] = '\0';
            token->type = T_NUMBER;
            return;
        }

        switch (c) {
            case '=': token->type = T_ASSIGN; strcpy(token->texting, "="); return;
            case '+': token->type = T_PLUS; strcpy(token->texting, "+"); return;
            case '-': token->type = T_MINUS; strcpy(token->texting, "-"); return;
            case '*': token->type = T_MULT; strcpy(token->texting, "*"); return;
            case '/': token->type = T_DIV; strcpy(token->texting, "/"); return;
            case '{': token->type = T_LBRACE; strcpy(token->texting, "{"); return;
            case '}': token->type = T_RBRACE; strcpy(token->texting, "}"); return;
            case ';': token->type = T_SEMICOLON; strcpy(token->texting, ";"); return;
            default: token->type = T_UNKNOWN; strcpy(token->texting, "UNKNOWN"); return;
        }
    }
    token->type = T_EOF;
    token->texting[0] = '\0';
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Sorry Failed to open file");
        return 1;
    }

    Token token;
    do {
        getNextToken(file, &token);
        printf("Token: %d, texting: %s\n", token.type, token.texting);
    } while (token.type != T_EOF);

    fclose(file);
    return 0;
}

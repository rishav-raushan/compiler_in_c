#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define maximum_T_length 100

typedef enum {
    T_INT, T_IDENTIFIER, T_NUMBER, T_ASSIGN,
    T_PLUS, T_MINUS, T_MULT, T_DIV,
    T_IF, T_ELSE, T_EQUAL, T_LBRACE,
    T_RBRACE, T_SEMICOLON, T_UNKNOWN, T_EOF
} TokenType;

typedef struct {
    TokenType type;
    char texting[maximum_T_length];
} Token;

void getting_next_token(FILE *file, Token *token) {
    int c;
    while ((c = fgetc(file)) != EOF) {
        if (isspace(c)) continue;

        if (isalpha(c)) {
            int len = 0;
            token->texting[len++] = c;
            while (isalnum(c = fgetc(file))) {
                if (len < maximum_T_length - 1) token->texting[len++] = c;
            }
            ungetc(c, file);
            token->texting[len] = '\0';

            if (strcmp(token->texting, "int") == 0) token->type = T_INT;
 else if (strcmp(token->texting, "else") == 0) token->type = T_ELSE;
            else if (strcmp(token->texting, "if") == 0) token->type = T_IF;  
            else token->type = T_IDENTIFIER;
            return;
        }

        if (isdigit(c)) {
            int len = 0;
            token->texting[len++] = c;
            while (isdigit(c = fgetc(file))) {
                if (len < maximum_T_length - 1) token->texting[len++] = c;
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
typedef struct ASTNode {
    TokenType type;
    char value[maximum_T_length];
    struct ASTNode *left, *right;
} ASTNode;

ASTNode* createNode(TokenType type, const char* value) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    strcpy(node->value, value);
    node->left = node->right = NULL;
    return node;
}
ASTNode* parseExpression(FILE* file, Token* token);
ASTNode* parseStatement(FILE* file, Token* token) {
    if (token->type == T_INT) {
        getting_next_token(file, token);
        if (token->type == T_IDENTIFIER) {
            ASTNode* node = createNode(T_IDENTIFIER, token->texting);
            getting_next_token(file, token);
            if (token->type == T_ASSIGN) {
                getting_next_token(file, token);
                node->right = parseExpression(file, token);
            }
            getting_next_token(file, token); // Consume ';'
            return node;
        }
    } else if (token->type == T_IF) {
        ASTNode* node = createNode(T_IF, "if");
        getting_next_token(file, token);
        node->left = parseExpression(file, token);
        getting_next_token(file, token);
        if (token->type == T_LBRACE) {
            getting_next_token(file, token);
            node->right = parseStatement(file, token);
            getting_next_token(file, token);
        }
        return node;
    }
    return parseExpression(file, token);
}

ASTNode* parseExpression(FILE* file, Token* token) {
    ASTNode* left = createNode(token->type, token->texting);
    getting_next_token(file, token);
    if (token->type == T_PLUS || token->type == T_MINUS || token->type == T_MULT || token->type == T_DIV || token->type == T_EQUAL) {
        ASTNode* node = createNode(token->type, token->texting);
        node->left = left;
        getting_next_token(file, token);
        node->right = parseExpression(file, token);
        return node;
    }
    return left;
}

void printAST(ASTNode* node) {
    if (!node) return;
    printAST(node->left);
    printf("%s ", node->value);
    printAST(node->right);
}

int main() {
    FILE *file = fopen("input.txt", "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    Token tokens;
    getting_next_token(file, &tokens);
    while (tokens.type != T_EOF) {
        ASTNode* root = parseStatement(file, &tokens);
        printf("Parsed AST: ");
        printAST(root);
        printf("\n");
        getting_next_token(file, &tokens);
    }

    fclose(file);
    return 0;
}

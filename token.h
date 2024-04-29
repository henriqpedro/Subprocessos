#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

enum tokenType
{
    NUMBER = 256,
    MATHFUNCTION,
    INCOGNITA
};

typedef struct token
{
    int type;
    int value;
    char *input;
    struct token *next;
} token;

struct tokenList
{
    int count;
    token *head;
    token *tail;
};

int isTokenType(int type);
int isNormalizedToken(char);
int isOperator(char);
int isMathFunction(char **, char **);

int getTokenValue(char **, char **);
token *getToken(char **);

void showTokens(struct tokenList);
void getTokens(struct tokenList *, char *);
void addToken(struct tokenList *, char **);
void nextToken();

int getTokenValue(char **input, char **tokenInput)
{
    int number = 0, counter = 0;
    do
    {
        number *= 10;
        number += ((**input) - '0');
        (*input)++;
        counter++;
    } while (isdigit(**input));

    (*input) -= counter;
    *tokenInput = (char *)malloc(sizeof(char) * (counter + 1));

    strncpy(*tokenInput, *input, counter);

    *(*tokenInput + counter) = '\0';
    (*input) += counter - 1;

    return number;
}

int isNormalizedToken(char input)
{
    int *normalizedTokens = (int[]){'x', 'X', '+', '-', '*', '/', '^', '(', ')'};
    while (*normalizedTokens)
    {
        if (*normalizedTokens == input)
            return 1;
        normalizedTokens++;
    }
    return 0;
}

int isIncognita(char input)
{
    return input == 'x' || input == 'X';
}

int isMathFunction(char **input, char **tokenInput)
{
    int counter = 0;
    char **mathFunctions = (char *[]){"sin", "cos", "tan", "log10", "log", '\0'};

    if (isalpha(**input))
    {
        while (*mathFunctions)
        {
            while (**input)
            {
                if (**mathFunctions != **input)
                    break;

                (*input)++;
                (*mathFunctions)++;
                counter++;

                if (!(**mathFunctions))
                {
                    (*input)--;
                    *mathFunctions -= counter;

                    *tokenInput = (char *)malloc(sizeof(char) * (counter + 1));
                    strcpy(*tokenInput, *mathFunctions);

                    return 1;
                }
            }

            (*input) -= counter;
            counter = 0;
            mathFunctions++;
        }
    }

    return 0;
}

token *getToken(char **input)
{
    token *token;
    token = (struct token *)malloc(sizeof(struct token));

    if (token == NULL)
    {
        printf("Memory's full!\n");
        exit(0);
    }

    while (isspace(**input))
        (*input)++;

    token->type = **input;
    token->value = **input;

    if (isMathFunction(input, &token->input))
        token->type = MATHFUNCTION;
    else if (isNormalizedToken(**input))
    {
        token->input = (char *)malloc(sizeof(char) * 2);
        *token->input = **input;
        *(token->input + 1) = '\0';
        
        if (isIncognita(**input))
            token->type = INCOGNITA;
    }
    else if (isdigit(**input))
    {
        token->type = NUMBER;
        token->value = getTokenValue(input, &token->input);
    }
    else
    {
        printf("Invalid token '%c'\n", **input);
        exit(0);
    }

    return token;
}

void addToken(struct tokenList *tokenList, char **input)
{
    if (tokenList->head == NULL)
    {
        tokenList->head = getToken(input);
        tokenList->tail = tokenList->head;
    }
    else
    {
        tokenList->tail->next = getToken(input);
        tokenList->tail = tokenList->tail->next;
    }

    tokenList->tail->next = NULL;
    tokenList->count++;
}

void showTokens(struct tokenList tokenList)
{
    token *token;
    token = tokenList.head;
    while (token != NULL)
    {
        printf("Token { input: %s, value: %d, type: %d }\n", token->input, token->value, token->type);
        token = token->next;
    }
}

void getTokens(struct tokenList *tokenList, char *input)
{
    tokenList->head = NULL;
    tokenList->count = 0;

    while (*input)
    {
        addToken(tokenList, &input);
        input++;
    }
}
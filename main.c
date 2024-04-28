#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

enum tokenType
{
    NUMBER = 256,
    OPERATOR,
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

typedef struct tokenList
{
    int count;
    token *head;
    token *tail;
} tokenList;

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
    int *normalizedTokens = (int[]){'(', ')'};
    while (*normalizedTokens)
    {
        if (*normalizedTokens == input)
            return 1;
        normalizedTokens++;
    }
    return 0;
}

int isOperator(char input)
{
    int *operators = (int[]){'+', '-', '*', '/', '^'};
    while (*operators)
    {
        if (input == *operators)
            return 1;
        operators++;
    }
    return 0;
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

    token->type = **input;
    token->value = **input;

    while (isspace(**input))
        (*input)++;

    if (isdigit(**input))
    {
        token->type = NUMBER;
        token->value = getTokenValue(input, &token->input);
    }
    else if (isMathFunction(input, &token->input))
        token->type = MATHFUNCTION;
    else
    {
        token->input = (char *)malloc(sizeof(char) * 2);
        *token->input = **input;
        *(token->input + 1) = '\0';

        if (isOperator(**input))
            token->type = OPERATOR;
        else if (!isNormalizedToken(**input))
        {
            token->value = -1;
            token->type = -1;
        }
    }

    return token;
}

void addToken(tokenList *tokenList, char **input)
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

void showTokens(tokenList tokenList)
{
    token *token;
    token = tokenList.head;
    while (token != NULL)
    {
        printf("Token { input: %s, value: %d, type: %d }\n", token->input, token->value, token->type);
        token = token->next;
    }
}

tokenList getTokens(char *input)
{
    tokenList tokenList;
    tokenList.head = NULL;

    while (*input)
    {
        addToken(&tokenList, &input);
        input++;
    }

    return tokenList;
}

int main()
{
    char input[10];
    tokenList tokenList;

    printf("f(x) = ");
    gets(input);

    tokenList = getTokens(input);
    showTokens(tokenList);

    return 0;
}
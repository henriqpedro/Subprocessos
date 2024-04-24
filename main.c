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
    char *input;
    int type;
    int value;
} token;

int getTokenValue(char *input, char* tokenInput)
{
    int number = 0, counter = 0;
    do
    {
        *tokenInput = *input;
        number *= pow(10, counter);
        number += (*input - '0');
        input++; counter++; tokenInput++;
    } while (isdigit(*input));

    *tokenInput = '\0';
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

int isMathFunction(char *input, char  *tokenInput)
{
    int counter = 0;
    char **mathFunctions = (char *[]){"sin", "cos", "tan", "log10", "log", '\0'};

    if (isalpha(*input))
    {
        while (*mathFunctions)
        {
            while (*input)
            {
                if (**mathFunctions != *input)
                    break;

                input++;
                (*mathFunctions)++;
                counter++;

                if (!(**mathFunctions)) {
                    *mathFunctions -= counter;
                    strcpy(tokenInput, *mathFunctions);
                    return 1;
                }
            }

            input -= counter;
            counter = 0;
            mathFunctions++;
        }
    }

    return 0;
}

token getToken(char *input)
{
    token token;

    token.input = malloc(sizeof(char));
    *token.input = *input;

    token.value = *input;

    if (isdigit(*input))
    {
        token.type = NUMBER;
        token.value = getTokenValue(input, token.input);
    }
    else if (isOperator(*input))
        token.type = OPERATOR;
    else if (isMathFunction(input, token.input))
        token.type = MATHFUNCTION;
    else if (!isNormalizedToken(*input))
    {
        token.input = "";
        token.value = -1;
        token.type = -1;
    }

    return token;
}

int main()
{
    token token;
    char cc[10];

    printf("f(x) = ");
    scanf("%s", cc);

    token = getToken(cc);
    printf("Token: value: %s; type: %d;\n", token.input, token.type);

    return 0;
}
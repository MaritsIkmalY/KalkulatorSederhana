
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma region Function Prototype

void startCalculator();
int Validate(char nStr[], int len);

const char * addition(char result[], char a[], char b[], int digita, int digitb, int * digitResult);
const char * subtraction(char result[], char a[], char b[], int digita, int digitb, int * digitResult);
const char * multiplication(char result[], char a[], char b[], int digita, int digitb, int * digitResult);
const char * division(char result[], char a[], char b[], int digita, int digitb, int * digitResult);
const char * power(char result[], char a[], char b[], int digita, int digitb, int * digitResult);

const char * Calculate(char nStr[], int len);
const char * ParenthesisCalculation(char nStr[], int len);
const char * PowCalculation(char nStr[], int len, int index);
const char * MultDivCalculation(char nStr[], int len);
const char * AddSubtractCalculation(char nStr[], int len);

int isNumber(char value);
int findDigitFromString(char nStr[], int startIndex);
void revstr(char * nStr, int len);
const char * removeStrElement(char nStr[], int *len, int index);
const char * insertStrElement(char nStr[], int *len, int index, char value);
const char * insertStrElementWithInt(char nStr[], int *len, int index, int value);
const char * insertStrElementWithStr(char nStr[], int *len, int index, char value[]);
const char * removeSpaces(char nStr[], int *len);
const char * removeZeroInfront(char nStr[], int *len);

#pragma endregion Function Prototype


int main()
{
    startCalculator();

    return 0;
}

// Start Calculator
void startCalculator()
{
    char str[500];
    int lenght;

    printf("Enter Operation: \n");
    scanf("%[^\n]s", str);

    lenght = strlen(str);

    if(!Validate(str, lenght))
        return;

    printf("Result:\n");
    Calculate(str, lenght);
    if(str[0] == 'n') str[0] = '-';

    printf("%s", str);
}

//Validation
// This funcion validate symbols and format
// current valid symbol is: '+', '-'
//
// format should look like this:
// <Number><Space><Operator><Space><Number><Space><Operator>...
int Validate(char nStr[], int len)
{
    int isSpace = 0; // 0 is non space, 1 is space
    int state = 0; // state 0 is number, state 1 is operator
    int parenthesisCount; // counter for parenthesis, +1 if '(' and -1 if ')'. by the end of validation, this value should be 0
    int isPassFirstNumber = 0; // is Validation already check first number in string

    for(int i = 0; i < len; i++)
    {
        if(nStr[0] == ' ')
        {
            removeStrElement(nStr, &len, 0);
            continue;
        }

        if(nStr[i] == '-' && !isPassFirstNumber)
        {
            removeStrElement(nStr, &len, i);
            insertStrElement(nStr, &len, i, 'n');
            isSpace = 1;
            continue;
        }

        if(isSpace)
        {
            isSpace = 0;

            if(nStr[i] == ' ')
                continue;
            else
            {
                printf("ERROR: Invalid Spacing");
                return 0;
            }
        }
        else
        {
            isSpace = 1;

            if(state == 0)
            {
                // Parenthesis check
                if (nStr[i] == '(')
                {
                    parenthesisCount++;
                    continue;
                }

                state = 1;

                //Number check
                if(isNumber(nStr[i]))
                {
                    if(!isPassFirstNumber) isPassFirstNumber = 1; // Set PassFirstNumber to true

                    // check if there is still number after this (more digit)
                    if(isNumber(nStr[i + 1]))
                    {
                        state = 0;
                        isSpace = 0;
                    }

                    continue;
                }
                else if(nStr[i] == ' ')
                {
                    printf("ERROR: Invalid Spacing");
                    return 0;
                }
                else
                {
                    printf("ERROR: Invalid Number");
                    return 0;
                }
            }
            else if (state == 1)
            {
                // Parenthesis check
                if(nStr[i] == ')')
                {
                    parenthesisCount--;
                    continue;
                }

                state = 0;

                // Operator check
                switch (nStr[i])
                {
                    case ' ':
                        printf("ERROR: Invalid Spacing");
                        return 0;
                    case '+':
                        continue;
                    case '-':
                        continue;
                    case '*':
                        continue;
                    case '/':
                        continue;
                    case '^':
                        continue;
                    default:
                        printf("ERROR: Unknown Operator");
                        return 0;
                }
            }
        }
    }

    return 1;
}

#pragma region String Arithmetic

// String Addition
// Add two string containing numbers
const char * addition(char result[], char a[], char b[], int digita, int digitb, int * digitResult)
{

    if(a[0] == 'n' && b[0] == 'n')
    {
        removeStrElement(a, &digita, 0);
        removeStrElement(b, &digitb, 0);
        addition(result, a, b, digita, digitb, &*digitResult);
        insertStrElement(result, &*digitResult, 0, 'n');
        return result;
    }
    else if(a[0] == 'n')
    {
        removeStrElement(a, &digita, 0);
        subtraction(result, b, a, digitb, digita, &*digitResult);
        return result;
    }
    else if(b[0] == 'n')
    {
        removeStrElement(b, &digitb, 0);
        subtraction(result, a, b, digita, digitb, &*digitResult);
        return result;
    }

    char ra[digita];
    char rb[digitb];

    strcpy(ra, a);
    strcpy(rb, b);

    revstr(ra, digita);
    revstr(rb, digitb);

    int remainder = 0;
    int i = 0;
    while(isNumber(ra[i]) || ra[i] == 'n' || isNumber(rb[i]) || rb[i] == 'n' || remainder == 1)
    {
        int n = 0;
        int na, nb;
        if(i < digita && isNumber(ra[i])) na = ra[i] - '0';
        else na = 0;
        if(i < digitb && isNumber(rb[i])) nb = rb[i] - '0';
        else nb = 0;

        n = na + nb + remainder;

        if(n > 9) {
            remainder = 1;
        }
        else {
            remainder = 0;
        }

        result[i] = (n % 10) + '0';

        i++;
    }
    if(i == 0) i = 1;

    *digitResult = i;

    revstr(result, *digitResult);

    removeZeroInfront(result, &*digitResult);

    return result;
}

// String Subtraction
// Subtract two string containing integer
const char * subtraction(char result[], char a[], char b[], int digita, int digitb, int * digitResult)
{
    if(a[0] == 'n' && b[0] == 'n')
    {
        removeStrElement(a, &digita, 0);
        removeStrElement(b, &digitb, 0);
        subtraction(result, b, a, digitb, digita, &*digitResult);
        return result;
    }
    else if(a[0] == 'n')
    {
        removeStrElement(a, &digita, 0);
        addition(result, b, a, digitb, digita, &*digitResult);
        insertStrElement(result, &*digitResult, 0, 'n');
        return result;
    }
    else if(b[0] == 'n')
    {
        removeStrElement(b, &digitb, 0);
        addition(result, b, a, digitb, digita, &*digitResult);
        return result;
    }

    char ra[digita];
    char rb[digitb];

    strcpy(ra, a);
    strcpy(rb, b);

    revstr(ra, digita);
    revstr(rb, digitb);

    int remainder = 0;
    int i = 0;
    while(isNumber(ra[i]) || ra[i] == 'n' || isNumber(rb[i]) || rb[i] == 'n')
    {
        int n = 0;
        int na, nb;
        if(i < digita && isNumber(ra[i])) na = ra[i] - '0';
        else na = 0;
        if(i < digitb && isNumber(rb[i])) nb = rb[i] - '0';
        else nb = 0;

        n = na - nb - remainder;

        if(n < 0) {
            n += 10;
            remainder = 1;
        }
        else {
            remainder = 0;
        }

        result[i] = n + '0';

        i++;
    }
    *digitResult = i;

    if(remainder == 1)
    {
        int remain = 0;
        for(int j = 0; j < *digitResult; j++)
        {
            if(isNumber(result[j])) result[j] = 10 - (result[j] - '0') + '0' - remain;
            if(result[j] > '9') result[j] = '0';
            if(remain == 0) remain = 1;
        }
        insertStrElement(result, &*digitResult, *digitResult, 'n');
    }

    revstr(result, *digitResult);

    removeZeroInfront(result, &*digitResult);

    return result;
}

// String Multiplication
// Multiplication of two string containing number
// Mulltiply string a by string b
const char * multiplication(char result[], char a[], char b[], int digita, int digitb, int * digitResult)
{
    char tempa[digita];
    char tempb[digitb];

    strcpy(tempa, a);
    strcpy(tempb, b);

    *digitResult = 1;
    strcpy(result, "0");

    int isNegative = 0;

    if(tempa[0] == 'n')
    {
        isNegative = !isNegative;
        removeStrElement(tempa, &digita, 0);
    }
    if(tempb[0] == 'n')
    {
        isNegative = !isNegative;
        removeStrElement(tempb, &digitb, 0);
    }

    while(1)
    {
        subtraction(tempb, tempb, "1", digitb, 1, &digitb);

        if(tempb[0] == 'n') break;

        addition(result, result, tempa, *digitResult, digita, &*digitResult);

    }

    if(isNegative) insertStrElement(result, &*digitResult, 0, 'n');

    return result;
}

// String Division
// Division of two string containing number
// Divide string a by string b
const char * division(char result[], char a[], char b[], int digita, int digitb, int * digitResult)
{
    char tempa[digita];
    char tempb[digitb];

    strcpy(tempa, a);
    strcpy(tempb, b);

    *digitResult = 1;
    strcpy(result, "0");

    int isNegative = 0;

    if(tempa[0] == 'n')
    {
        isNegative = !isNegative;
        removeStrElement(tempa, &digita, 0);
    }
    if(tempb[0] == 'n')
    {
        isNegative = !isNegative;
        removeStrElement(tempb, &digitb, 0);
    }

    while(1)
    {
        subtraction(tempa, tempa, tempb, digita, digitb, &digita);
        if(tempa[0] == 'n') break;

        addition(result, result, "1", *digitResult, 1, &*digitResult);
    }

    if(isNegative) insertStrElement(result, &*digitResult, 0, 'n');

    return result;
}

// String Exponent
// Exponent of two string containing number
// Power string a by string b
const char * power(char result[], char a[], char b[], int digita, int digitb, int * digitResult)
{
    char tempa[digita];
    char tempb[digitb];

    strcpy(tempa, a);
    strcpy(tempb, b);

    *digitResult = 1;
    strcpy(result, "1");

    int isNegative = 0;

    if(tempa[0] == 'n')
    {
        isNegative = !isNegative;
        removeStrElement(tempa, &digita, 0);
    }
    if(tempb[0] == 'n')
    {
        isNegative = !isNegative;
        removeStrElement(tempb, &digitb, 0);
    }

    if(tempb[0] == 'n') strcpy(result, "0");
    while(1)
    {
        subtraction(tempb, tempb, "1", digitb, 1, &digitb);
        if(tempb[0] == 'n') break;
        puts(result);
        multiplication(result, result, tempa, *digitResult, digita, &*digitResult);
    }

    return result;
}

#pragma endregion String Arithmetic


#pragma region Calculations

// Calculate
// Main Calculation Function
// Calculate various operations
const char * Calculate(char nStr[], int len)
{
    removeSpaces(nStr, &len);

    ParenthesisCalculation(nStr, len);

    PowCalculation(nStr, len, 0);
    MultDivCalculation(nStr, len);
    AddSubtractCalculation(nStr, len);

    return nStr;
}

// Parenthesis Calculation
// Calculating operations inside of a parenthesis
const char * ParenthesisCalculation(char nStr[], int len)
{
    for(int i = 0; i < len; i++)
    {
        if(nStr[i] == '(')
        {
            int index = i;

            int parenthesisCount = 0;

            i++;

            int sLen;
            for(int j = 0; i < len; i++, j++)
            {
                if(nStr[i] == ')' && parenthesisCount == 0)
                {
                    sLen = j;
                    break;
                }

                if(nStr[i] == '(')
                    parenthesisCount++;
                else if(nStr[i] == ')')
                    parenthesisCount--;
            }

            i -= sLen;

            char intStr[sLen];
            for(int j = 0; j < sLen; i++, j++)
            {
                intStr[j] = nStr[i];
            }

            for(int k = 0; k < sLen + 2; k++)
            {
                removeStrElement(nStr, &len, index);
            }

            Calculate(intStr, strlen(intStr));

            insertStrElementWithStr(nStr, &len, index, intStr);

            i = 0;
        }
    }

    return nStr;
}

// Power Calculation
// Calculation of Exponent in a string of operation
const char * PowCalculation(char nStr[], int len, int index)
{
    for (int i = index; i < len; i++)
    {
        if(nStr[i] == '^')
        {
            while(nStr[i - 1] >= '0' && nStr[i - 1] <= '9')
                i--;

            int index1 = i;
            int digit1 = findDigitFromString(nStr, i);

            char intStr1[digit1];
            for(int j = 0; j < digit1; j++, i++)
                intStr1[j] = nStr[i];

            i++;

            int index2 = i;
            int digit2 = findDigitFromString(nStr, i);

            if(nStr[i + digit2] == '^')
            {
                PowCalculation(nStr, len, i);
                digit2 = findDigitFromString(nStr, i);
            }

            char intStr2[digit2];
            for(int j = 0; j < digit2; j++, i++)
                intStr2[j] = nStr[i];

            for(int k = 0; k <= digit1 + digit2; k++)
            {
                removeStrElement(nStr, &len, index1);
            }

            int digitn;
            if(digit1 > digit2) digitn = digit1;
            else digitn = digit2;
            char n[digitn];

            power(n, intStr1, intStr2, digit1, digit2, &digitn);

            insertStrElementWithStr(nStr, &len, index1, n);

            i = index;
        }
    }

    return nStr;
}

// Multiplication / Division Calculation
// Calculation of Multiplication and Division in a string of operation
const char * MultDivCalculation(char nStr[], int len)
{
    for (int i = 0; i < len; i++)
    {
        if(nStr[i] == '*' || nStr[i] == '/')
        {
            int mode;
            if(nStr[i] == '*')
                mode = 0;
            else if(nStr[i] == '/')
                mode = 1;

            while(nStr[i - 1] >= '0' && nStr[i - 1] <= '9')
                i--;

            int index1 = i;
            int digit1 = findDigitFromString(nStr, i);

            char intStr1[digit1];
            for(int j = 0; j < digit1; j++, i++)
                intStr1[j] = nStr[i];

            i++;

            int index2 = i;
            int digit2 = findDigitFromString(nStr, i);

            char intStr2[digit2];
            for(int j = 0; j < digit2; j++, i++)
                intStr2[j] = nStr[i];

            for(int k = 0; k <= digit1 + digit2; k++)
                removeStrElement(nStr, &len, index1);

            int digitn;
            if(digit1 > digit2) digitn = digit1;
            else digitn = digit2;
            char n[digitn];
            if(mode == 0)
                multiplication(n, intStr1, intStr2, digit1, digit2, &digitn);
            else if(mode == 1)
                division(n, intStr1, intStr2, digit1, digit2, &digitn);

            insertStrElementWithStr(nStr, &len, index1, n);

            i = 0;
        }
    }

    return nStr;
}

// Addition / Subtraction Calculation
// Calculation of Addition and Subtraction in a string of operation
const char * AddSubtractCalculation(char nStr[], int len)
{
    for (int i = 0; i < len; i++)
    {
        if(nStr[i] == '+' || nStr[i] == '-')
        {
            int mode;
            if(nStr[i] == '+')
                mode = 0;
            else if(nStr[i] == '-')
                mode = 1;

            while(isNumber(nStr[i - 1]) || nStr[i - 1] == 'n')
                i--;

            int index1 = i;
            int digit1 = findDigitFromString(nStr, i);

            char intStr1[digit1];
            for(int j = 0; j < digit1; j++, i++)
                intStr1[j] = nStr[i];

            i++;

            int index2 = i;
            int digit2 = findDigitFromString(nStr, i);


            char intStr2[digit2];
            for(int j = 0; j < digit2; j++, i++)
                intStr2[j] = nStr[i];

            for(int k = 0; k <= digit1 + digit2; k++)
            {
                removeStrElement(nStr, &len, index1);
            }

            int digitn;
            if(digit1 > digit2) digitn = digit1;
            else digitn = digit2;
            char n[digitn];
            if(mode == 0)
                addition(n, intStr1, intStr2, digit1, digit2, &digitn);
            else if(mode == 1)
                subtraction(n, intStr1, intStr2, digit1, digit2, &digitn);

            insertStrElementWithStr(nStr, &len, index1, n);

            i = 0;
        }
    }

    return nStr;
}

#pragma endregion Calculations


#pragma region Custom Functions

// Check is Number
// Check if given char is a number
int isNumber(char value)
{
    if(value >= '0' && value <= '9') return 1;
    else return 0;
}

// Find Digit From String
// Find digit of integer from string
int findDigitFromString(char nStr[], int startIndex)
{
    int dit;
    int len = strlen(nStr);
    for (int i = 0, j = startIndex; i <= len; i++, j++)
    {
        if(!(isNumber(nStr[j]) || nStr[j] == 'n'))
        {
            dit = i;
            break;
        }
    }

    return dit;
}
//

// Reverse String
// Reverse a String
void revstr(char * nStr, int len)
{
    for(int i = 0; i < len/2; i++)
    {
        int temp = nStr[i];
        nStr[i] = nStr[len - i - 1];
        nStr[len - i - 1] = temp;
    }
}

// Remove String Element
// Remove element in an index of a string
const char * removeStrElement(char nStr[], int *len, int index)
{
    for(int i = index; i < *len; i++)
        nStr[i] = nStr[i + 1];

    *len -= 1;

    return nStr;
}

// Insert String Element
// Insert element in an index of a string
const char * insertStrElement(char nStr[], int *len, int index, char value)
{

    for(int i = *len; i >= index; i--)
    {
        if(i == index) nStr[i] = value;
        else nStr[i] = nStr[i - 1];
    }
    if(*len == index) nStr[index] = value;

    *len += 1;


    return nStr;
}

// Insert String Element with Integer
// Insert integer to a string
const char * insertStrElementWithInt(char nStr[], int *len, int index, int value)
{
    char intStr[*len];
    sprintf(intStr, "%d", value);

    for (int i = index, j = 0; j < findDigitFromString(intStr, 0); i++, j++)
    {
        insertStrElement(nStr, &*len, i, intStr[j]);
    }

    return nStr;
}

// Insert String Element
// Insert string into another string
const char * insertStrElementWithStr(char nStr[], int *len, int index, char value[])
{
    for (int i = index, j = 0; j < findDigitFromString(value, 0); i++, j++)
        insertStrElement(nStr, &*len, i, value[j]);

    return nStr;
}

// Remove Spaces
// Remove Spaces from a string
const char * removeSpaces(char nStr[], int *len)
{
    for(int i = 0; i < *len; i++)
    {
        if (nStr[i] == ' ')
        {
            removeStrElement(nStr, &*len, i);
        }
    }
    return nStr;
}

// Remove Zero In Front of Integer
// Remove zeros in front of an integer
const char * removeZeroInfront(char nStr[], int *len)
{
    int index = 0;
    while(1)
    {
        if(index + 1 == *len) break;

        if(nStr[index] == 'n') index++;
        if(nStr[index] == '0') removeStrElement(nStr, &*len, index);
        else if(isNumber(nStr[index])) break;
    }
    return nStr;
}

#pragma endregion Custom Functions

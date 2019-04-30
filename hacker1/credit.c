#include <stdio.h>
#include <cs50.h>

long long GetPositiveLong(void);

int HowManyDigits(long long number);

int main (void)
{
    long long number = GetPositiveLong();
    
    // count how many digit has the number
    int digit;
    digit = HowManyDigits(number);
    
    // we are going to put in a variable the first 2 numbers of the credit number
    digit = digit - 1;
    int i;
    long long potencia = 1;
    for (i = 1; i < digit; i++)
    {
        potencia *= 10;
    }
    number = number/potencia;
    int company = (int)number;
    
    // check of what card it is
    if (company == 34 || company == 37)
    printf("AMEX\n");
    else if (company >= 51 && company <= 55)
    printf("MASTERCARD\n");
    else if (company >= 40 && company <= 49)
    printf("VISA\n");
    else
    printf("INVALID\n");
    return 0;
    
}

long long GetPositiveLong(void)
{
    long long positive;
    do
    {
        printf("Please, enter the credit card number:\n");
        positive = GetLongLong();
    }
    while(positive <= 0);
    return positive;
}

int HowManyDigits(long long x)
{
    int digit = 0;
    long long i = 1;
    do
    {
        i *= 10;
        digit++;
    }
    while ( (x - i) > 0);
    return digit;
}
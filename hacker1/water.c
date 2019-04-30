#include <stdio.h>
#include <cs50.h>

int  getpositiveint (void);

int main (void)
{
printf("How many minutes do you spend in the shower?\n");
int minutes = getpositiveint();
printf("the number of bottles you spend is: %d\n", minutes*12);
}

int getpositiveint()
{
   int n;
do
{
printf("Give a positive number: ");
   n = GetInt();
}
   while (n <= 0);
return n;
}

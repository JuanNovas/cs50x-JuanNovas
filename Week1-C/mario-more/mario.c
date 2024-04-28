#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int number;
    do
    {
        number = get_int("Height: ");
    }
    while (number < 1 || number > 8);
    for (int n = 1; n <= number; n++)
    {

        for (int x = number; x > n; x--)
        {
            printf(" ");
        }

        for (int x = 1; x <= n; x++)
        {
            printf("#");
        }

        printf("  ");
        for (int x = 1; x <= n; x++)
        {
            printf("#");
        }

        printf("\n");
    }
}

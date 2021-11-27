#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // get the height from user input
    int height;
    do
    {
        height = get_int("Height:\n");
    }
    while (height < 1 || height > 8);

    // print the pyramid
    for (int i = 1; i <= height; i++)
    {
        //print the space on the left
        for (int m = height - i - 1; m >= 0; m--)
        {
            printf("%c", ' ');
        }

        // print the # on the left
        for (int n = 1; n <= i; n++)
        {
            printf("%c", '#');
        }

        // line break
        printf("\n");

    }
}
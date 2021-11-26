#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    // TODO: Prompt for start size
    int start_size;
    do
    {
        start_size = get_int("Please enter a starting population size (more than 9):\n");
    }
    while (start_size < 9);

    // TODO: Prompt for end size
    int end_size;
    do
    {
        end_size = get_int("Please then enter an ending population size (larger than the starting size):\n");
    }
    while (end_size < start_size);

    // TODO: Calculate number of years until we reach threshold
    int year_num = 0;
    int population = start_size;
    // keep the year rolling until reach the end size
    while (population < end_size)
    {
        population = population + round((population / 3)) - round((population / 4));
        year_num++;
    }

    // TODO: Print number of years
    printf("Years: %i\n", year_num);
}
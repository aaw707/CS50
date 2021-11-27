#include <stdio.h>
#include <cs50.h>

// take user input as the name and say hello to that name
int main(void)
{
    string name = get_string("What is your name?\n");
    printf("hello, %s\n", name);
}
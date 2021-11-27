#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
// helper function
char cipher(char x, char arr[], int key);

// copied from Lab2: Scrabble
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
char UPPER[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

int main(int argc, char*argv[])
{
    // check if there's only one input
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // check if every char in the input is a digit i.e. the input is an int
    string key_str = argv[1];
    int input_length = strlen(key_str);
    for (int i = 0; i < input_length; i++)
    {
        if (!isdigit(key_str[i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    //printf("Success\n");
    //printf("%s\n", key_str);

    // the lower case alphabets
    char LOWER[26];
    for (int i = 0; i < 26; i++)
    {
        LOWER[i] = tolower(UPPER[i]);
    }

    // convert the key into an int
    int key = atoi(key_str);

    // get user input as the plaintext
    string text = get_string("plaintext: ");
    int text_length = strlen(text);

    // print the ciphertext
    printf("ciphertext: ");

    // loop through each char in text to cipher it
    for (int i = 0; i < text_length; i++)
    {
        char cipher_char;
        // find out if the char is upper case
        char x = text[i];
        char x_upper = toupper(text[i]);

        if (x == x_upper)
        {
            // char is in upper case
            cipher_char = cipher(x, UPPER, key);
            printf("%c", cipher_char);
        }
        else
        {
            // char is in lower case
            cipher_char = cipher(x, LOWER, key);
            printf("%c", cipher_char);
        }
    }

    printf("\n");
    return 0;
}

char cipher(char x, char arr[], int key)
{
    int position = -1;
    // loop through the array to find the position of the char in alphabets
    for (int i = 0; i < 26; i++)
    {
        if (x == arr[i])
        {
            position = i;
            break;
        }
    }

    // if char is not a letter i.e. position is not reassigned
    if (position == -1)
    {
        return x;
    }

    // find the new position with the key
    int new_position = (position + key) % 26;

    // return the ciphered char
    return arr[new_position];
}
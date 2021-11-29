#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
// helper function
char cipher(char x, char key[], char alphabet[]);

int main(int argc, char *argv[])
{
    // check if there's only one input
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    // check if the key has length of 26
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // need 3 arrays: uppercase alphabets, lowercase alphabets, key
    string alphabets_str = "abcdefghijklmnopqrstuvwxyz";
    char UPPER[26];
    char LOWER[26];
    char KEY[26];

    // loop through each char in key
    for (int i = 0; i < 26; i++)
    {
        // current char
        char key_char = argv[1][i];

        // check if each char is a letter
        if (isalpha(key_char) == 0)
        {
            printf("Key must be alphabetic letters.\n");
            return 1;
        }

        // check for duplicates in key
        for (int j = 0; j < i; j++)
        {
            if (key_char == KEY[j])
            {
                printf("Key must contain each letter exactly once.");
                return 1;
            }
        }

        // if the key passed all the tests above, assign the arrays at this position
        UPPER[i] = toupper(alphabets_str[i]);
        LOWER[i] = alphabets_str[i];
        KEY[i] = argv[1][i];
    }

    // debug
    /*
    printf("UPPER: ");
    for (int i = 0; i < 26; i++)
    {
        printf("%c ", UPPER[i]);
    }
    printf("\n");

    printf("LOWER: ");
    for (int i = 0; i < 26; i++)
    {
        printf("%c ", LOWER[i]);
    }
    printf("\n");

    printf("KEY:   ");
    for (int i = 0; i < 26; i++)
    {
        printf("%c ", KEY[i]);
    }
    printf("\n");
    */

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
            cipher_char = cipher(x, KEY, UPPER);
            printf("%c", cipher_char);
        }
        else
        {
            // char is in lower case
            cipher_char = cipher(x, KEY, LOWER);
            printf("%c", cipher_char);
        }
    }

    printf("\n");
    return 0;
}

char cipher(char x, char key[], char alphabet[])
{

    // if the alphabet is uppercase
    int alphabet_upper;
    if (alphabet[0] == 'A')
    {
        alphabet_upper = 1;
    }
    else
    {
        alphabet_upper = 0;
    }

    // loop through the key to find the position of the char
    for (int i = 0; i < 26; i++)
    {
        if (x == alphabet[i])
        {
            // find out if the key at this position is uppercase
            char key_upper = toupper(key[i]);
            if (key[i] == key_upper && alphabet_upper == 0)
            {
                return tolower(key[i]);
            }
            else if (key[i] != key_upper && alphabet_upper == 1)
            {
                return toupper(key[i]);
            }
            else
            {
                return key[i];
            }
        }
    }
    // if char is not a letter i.e. not in the key
    return x;
}
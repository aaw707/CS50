#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// to avoid counting special symbols, compare each char with the alphabets
// alphabets copied from Lab 2: Scrabble
char ALPHABETS[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

// write a function that takes a string input and rate it based on readability
int main(void)
{
    // get user input
    string text = get_string("Text: ");

    // declare the variables
    int text_length = strlen(text);
    // number of letters in the text, excluding spaces, punctuations, symbols, etc
    int num_letters = 0;
    // number of spaces in the text, which equals to number of words - 1
    int num_spaces = 0;
    int num_words;
    // number of sentences in the text, indicated by a period, exclamation point, or question mark
    int num_sentences = 0;

    // loop through the text and count the letters, words, and sentences
    for (int i = 0; i < text_length; i++)
    {
        char x = text[i];
        //printf("%c\n", x)
        if (x == ' ')
        {
            num_spaces++;
        }
        else if (x == '.' || x == '?' || x == '!')
        {
            num_sentences++;
        }
        else
        {
            x = toupper(x); // to be consistent with the alphabets

            // loop through the alphabets to see if the char is a letter
            for (int j = 0; j < 26/* length of alphabets*/; j++)
            {
                if (x == ALPHABETS[j])
                {
                    num_letters++;
                    break; // break the current loop and count the next char
                }
            }
        }
    }

    num_words = num_spaces + 1;

    // debug
    // printf("num_letters: %i\n", num_letters);
    // printf("num_words: %i\n", num_words);
    // printf("num_sentences: %i\n", num_sentences);

    // calculate the Coleman-Liau index
    // index = 0.0588 * L - 0.296 * S - 15.8
    // L is the average number of letters per 100 words in the text
    // S is the average number of sentences per 100 words in the text.
    float L = (float) num_letters / (float) num_words * 100;
    float S = (float) num_sentences / (float) num_words * 100;
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    // debug
    // printf("L: %f\n", L);
    // printf("S: %f\n", S);

    // report the readability
    if (index >= 16) // a senior undergraduate reading level
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

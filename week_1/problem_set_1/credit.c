#include <cs50.h>
#include <stdio.h>
#include <math.h>

// check if a credit card is valid
int main(void)
{
    // get the credit card number from user input
    long credit_card;
    do
    {
        credit_card = get_long("Number: \n");
    }
    while (credit_card <= 0);

    // make a copy of card_num
    long card_num = credit_card;
    // sum of digits, with every other digit doubled
    int dig_sum = 0;
    // count which digit we are at. starting from the ending one
    int digit_count = 0;

    while (card_num > 0)
    {
        // debug
        //printf("card_num: %ld\n", card_num);
        //printf("digit_count: %i\n", digit_count);
        digit_count++;

        if (digit_count % 2 == 1)
        {
            // add this digit to the sum
            dig_sum += card_num % 10;
        }
        else
        {
            // add this digit * 2

            // if this digit * 2 > 9, add up the two digits of this number
            if (card_num % 10 * 2 > 9)
            {
                dig_sum += card_num % 10 * 2 - 9;
            }
            else
            {
                // else just add the digit * 2
                dig_sum += card_num % 10 * 2;
            }
        }
        //printf("digit_sum: %i\n", dig_sum);
        card_num = floor(card_num / 10);

    }

    // report INVALID
    if (dig_sum % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    // report the credit card company if the number is valid

    int first_digit = floor(credit_card / pow(10, digit_count - 1));
    int second_digit = floor(credit_card / pow(10, digit_count - 2));
    second_digit = second_digit % 10;

    //printf("%i\n", first_digit);
    //printf("%i\n", second_digit);
    //printf("%i\n", digit_count);
    //printf("%f\n", pow(10, digit_count - 1));
    //printf("%ld\n", credit_card);


    if (digit_count == 15 && first_digit == 3 && (second_digit == 4 || second_digit == 7))
    {
        printf("AMEX\n");
    }
    else if (digit_count == 16 && first_digit == 5 && second_digit > 0 && second_digit < 6)
    {
        printf("MASTERCARD\n");
    }
    else if ((digit_count == 13 || digit_count == 16) && first_digit == 4)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
    return 0;
}

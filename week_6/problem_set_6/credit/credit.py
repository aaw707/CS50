from cs50 import get_int
import re
import sys

# get credit card number from user input
card = -1
while card < 0:
    card = get_int("Number: ")

# get a copy of the number as a string
card_number = str(card)

# use sum and count to keep track through the validation
digit_sum = 0
digit_count = 0

# loop through each digit from back to end
while card > 0:
    digit_count += 1
    current_digit = card % 10
    # print("digit_count:", digit_count)
    # print("current_digit: ", current_digit)

    if digit_count % 2 == 1:
        digit_sum += current_digit
    else:
        if current_digit * 2 < 9:
            digit_sum += current_digit * 2
        else:
            digit_sum += current_digit * 2 - 9

    # print("digit_sum:", digit_sum)
    card = card // 10

# invalid by digit check
if digit_sum % 10 != 0:
    print("INVALID")
    sys.exit()

## company check

# american express: 15 digits, starting with 34/37
if re.search(r'^3[47][0-9]{13}$', card_number):
    print("AMEX")
    sys.exit()
# mastercard: 16 digits, starting with 51-55
elif re.search(r'^5[1-5][0-9]{14}$', card_number):
    print("MASTERCARD")
    sys.exit()
# visa: 13/16 digits, start with 4
elif re.search(r'^4[0-9]{12}$', card_number) or re.search(r'^4[0-9]{15}$', card_number):
    print("VISA")
    sys.exit()
# invalid
else:
    print("INVALID")
    sys.exit()
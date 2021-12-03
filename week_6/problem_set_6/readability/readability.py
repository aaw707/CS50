from cs50 import get_string
import re

text = get_string("Text: ")

# set counters for letters, words, sentences
num_letters = 0
num_words = 1
num_sentences = 0

# loop through the text
for i in range(len(text)):

    # count letters
    if re.search('^[a-zA-Z]$', text[i]):
        num_letters += 1

    # count words. total num of words = num of spaces + 1
    if text[i] == ' ':
        num_words += 1

    # count sentences
    if text[i] in ['.', '!', '?']:
        num_sentences += 1

# average number of letters per 100 words
L = num_letters / num_words * 100

# average number of sentences per 100 words
S = num_sentences / num_words * 100

# calculate the coleman-liau index
index = 0.0588 * L - 0.296 * S - 15.8

if index >= 16:
    print("Grade 16+")

elif index < 1:
    print("Before Grade 1")

else:
    print(f"Grade {round(index)}")
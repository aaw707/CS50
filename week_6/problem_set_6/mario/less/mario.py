from cs50 import get_int

# get a positive int from input as the height of the pyramid
height = -1
while height <= 0 or height > 8:
    height = get_int("Height: ")

# build the pyrimad
for i in range(height):
    print(' ' * (height - i - 1) + '#' * (i + 1))
from cs50 import get_float

# get user input as the change owed in dolloars
change = -1
while change < 0:
    change = get_float("Change owed: ")

# number of cents owed calculated from change
cents = change * 100

# calculate the number of coins
quarters = cents // 25
cents -= quarters * 25

dimes = cents // 10
cents -= dimes * 10

nickels = cents // 5
cents -= nickels * 5

pennies = cents

print(int(quarters + dimes + nickels + pennies))
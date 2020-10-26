from cs50 import get_int

number = get_int("Please insert credit card number: ")
number_string = str(number)

if (len(number_string) != 13 and len(number_string) != 15 and len(number_string) != 16):
    print("INVALID")
    exit(1)

# get first sum
first_sum = 0
i = len(number_string) - 2
while (i >= 0):
    num = 2 * int(number_string[i])
    if (num >= 10):
        first_sum += 1 + (num - 10)
    else:
        first_sum += num
    i -= 2

#get second sum
second_sum = 0
j = len(number_string) - 1
while (j >= 0):
    second_sum += int(number_string[j])
    j -= 2

# get total of the two sums
total_sum = first_sum + second_sum

# invalid if last digit is not a zero
if total_sum % 10 != 0:
    print("INVALID")

if int(number_string[0:2]) == 34 or int(number_string[0:2]) == 37:
    print("AMEX")
elif int(number_string[0:2]) >= 51 and int(number_string[0:2]) <= 55:
    print("MASTERCARD")
elif int(number_string[0]) == 4:
    print("VISA")
else:
    print("INVALID")
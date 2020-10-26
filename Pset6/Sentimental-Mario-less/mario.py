from cs50 import get_int

while True:
    height = get_int("Please insert height of pyramid: ")
    if (height > 0 and height < 9):
        break

for i in range(height):
    print(" " * (height - i - 1), end="")
    print("#" * (i + 1))



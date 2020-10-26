#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    int get_length(long num); // stores length of number in an int

    // gets added digits from first list of digits
    long get_digit_from_last(long num, int n);

    // gets added digits from second list of digits
    int get_first_digits(long number, int n);

    //sums of lists of digits respectfully
    int get_first_sum(long number, int length);
    int get_second_sum(long number, int length);

    // asks user for credit card number
    long number = get_long("Please insert credit card number: ");
    int length = get_length(number);

    // if not right length: invalid!
    if (length != 13 && length != 15 && length != 16)
    {
        printf("INVALID\n");
        return 0;
    }
    int first_sum = get_first_sum(number, length);
    int second_sum = get_second_sum(number, length);
    int total_sum = first_sum + second_sum;

    // if last digit is not a zero: invalid!
    if (total_sum % 10 != 0)
    {
        printf("INVALID\n");
    }
    else
    {
        int digits = get_first_digits(number, 2); // get first 2 digits of credit card number
        if (digits == 34 || digits == 37)
        {
            printf("AMEX\n");
        }
        else if (digits >= 51 && digits <= 55)
        {
            printf("MASTERCARD\n");
        }
        else
        {
            int digit = get_first_digits(number, 1); // get first digit of credit card number
            if (digit == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
    }
}

// gets n'th digit (from ending to beginning)
long get_digit_from_last(long num, int n)
{
    long digit = num / pow(10, n - 1);
    digit %= 10;
    return digit;
}

int get_length(long num)
{
    int count = 0;
    while (num != 0)
    {
        num /= 10;
        count++;
    }
    return count;
}

int get_first_sum(long number, int length)
{
    int total_of_digits = 0;
    for (int i = 2; i <= length; i += 2)
    {
        long digit = get_digit_from_last(number, i); // get digit needed
        digit *= 2; // multiple by 2

        if (digit >= 10)
        {
            total_of_digits += (digit / 10) + (digit % 10);
        }
        else
        {
            total_of_digits += digit;
        }
    }
    return total_of_digits;
}

int get_second_sum(long number, int length)
{
    int total_of_digits = 0;
    for (int i = 1; i <= length; i += 2)
    {
        long digit = get_digit_from_last(number, i); // get digit needed
        total_of_digits += digit;

    }
    return total_of_digits;
}

int get_first_digits(long number, int n)
{
    if (n == 2)
    {
        while (number > 99)
        {
            number /= 10;
        }
    }
    if (n == 1)
    {
        while (number > 9)
        {
            number /= 10;
        }
    }
    return number;
}
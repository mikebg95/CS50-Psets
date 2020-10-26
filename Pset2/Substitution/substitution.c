#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cs50.h>

int main(int argc, char *argv[])
{
    bool correct_input(int argc, char *argv[]);
    bool contains_letter(char *input, char letter);

    if (!correct_input(argc, argv))
    {
        return 1;
    }

    // store input in variable
    char *input = argv[1];
    printf("input is: %s\n", input);

    int keys[strlen(input)]; // array to store separate keys

    // check for double letters; save key if okay
    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (contains_letter(input, input[i]))
        {
            printf("Key can only contain each letter exactly once!\n");
            return 1;
        }

        keys[i] = toupper(input[i]) - 65 - i;
    }

    // prompt user for plaintext
    char *plaintext;
    plaintext = get_string("plaintext: ");

    // variable to store ciphertext
    char ciphertext[strlen(plaintext) + 1];

    // loop over symbols in plaintext
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        int letter = (int) plaintext[i];

        // for capital letters
        if (letter >= 65 && letter <= 90)
        {
            int key = keys[letter - 65]; // works
            // ciphertext[i] = plaintext[i] + key;

            // wraps around backwards
            if ((plaintext[i] + key) < 65)
            {
                ciphertext[i] = plaintext[i] + key + 26;
            }

            // wraps around
            else if ((plaintext[i] + key) > 90)
            {
                ciphertext[i] = plaintext[i] + key - 26;
            }

            // no wrap-around necessary
            else
            {
                ciphertext[i] = plaintext[i] + key;
            }
        }

        // for small letters
        else if (letter >= 97 && letter <= 122)
        {
            int key = keys[letter - 97]; // works

            // wraps around backwards
            if ((plaintext[i] + key) < 97)
            {
                ciphertext[i] = plaintext[i] + key + 26;
            }

            // wraps around
            else if ((plaintext[i] + key) > 122)
            {
                ciphertext[i] = plaintext[i] + key - 26;
            }

            // no wrap-around necessary
            else
            {
                ciphertext[i] = plaintext[i] + key;
            }
        }

        // for non-alphabetic symbols
        else
        {
            ciphertext[i] = plaintext[i];
            printf("ciphertext char is: %c\n\n", ciphertext[i]);
        }

        // string has ended
        if (i == n - 1)
        {
            ciphertext[i + 1] = '\0';
        }
    }

    printf("ciphertext: %s\n", ciphertext);
}

// returns whether input contains letter more than once
bool contains_letter(char *input, char letter)
{
    int count = 0;

    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (input[i] == letter)
        {
            count++;
        }
    }

    return (count > 1);
}

// returns whether input is integer
bool correct_input(int argc, char *argv[])
{
    // make sure user insert only one argument
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return false;
    }

    char *inp = argv[1]; // save input

    // make sure input is letters
    for (int i = 0, n = strlen(inp); i < n; i++)
    {
        if (!isalpha(inp[i]))
        {
            printf("Usage: ./substitution key\n");
            return false;
        }
    }

    // if too many letters
    if (strlen(inp) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return false;
    }

    return true; // if everything is okay
}
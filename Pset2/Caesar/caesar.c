#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cs50.h>

int main(int argc, char *argv[])
{
    bool correct_input(int argc, char *argv[]);

    // check if input is correct
    bool is_correct = correct_input(argc, argv);
    if (!is_correct)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // store input and convert to (long) integer
    char *input = argv[1];
    char *ptr = NULL;
    int key = (int) strtol(input, &ptr, 10);

    // make sure input is non-negative integer
    if (key <= 0)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    key %= 26; // when key is unnecessarily high

    // prompt user for plaintext
    char *plaintext;
    plaintext = get_string("plaintext: ");

    char ciphertext[strlen(plaintext + 1)]; // variable to store ciphertext

    // loop through chars of plaintext
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {

        // make sure it wraps around alphabet
        if ((plaintext[i] + key >= 91 && plaintext[i] + key < 97) || plaintext[i] + key > 122)
        {
            ciphertext[i] = plaintext[i] + key - 26;
            // printf("%c", plaintext[i] + key - 26);
        }

        // if char isn't a letter
        else if (plaintext[i] + key < 65 || plaintext[i] + key > 122 || (plaintext[i] + key >= 91 && plaintext[i] + key <= 96))
        {
            ciphertext[i] = plaintext[i];
            // printf("%c", plaintext[i]);
        }

        // if it's a letter that doesn't need to wrap around
        else
        {
            ciphertext[i] = plaintext[i] + key;
            // printf("%c", plaintext[i] + key);
        }
    }

    printf("ciphertext: %s\n", ciphertext); // print ciphertext
    return 0;

}

// returns whether input is integer
bool correct_input(int argc, char *argv[])
{
    // make sure user insert only one argument
    if (argc != 2)
    {
        return false;
    }

    char *inp = argv[1]; // save input

    // make sure input is number
    for (int i = 0, n = strlen(inp); i < n; i++)
    {
        if (!isdigit(inp[i]))
        {
            return false;
        }
    }
    return true;
}
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(void)
{
    // prompt user for text
    string text;
    text = get_string("Please insert text: ");

    // create variables to keep track of words, letters and sentences
    int words = 0;
    int letters = 0;
    int sentences = 0;

    for (int i = 0, n = strlen(text); i <= n; i++)
    {
        // get number of words in text
        if (text[i] == ' ')
        {
            words++;
        }

        // get number of letters in text
        if (isalpha(text[i]))
        {
            letters++;
        }

        // get number of sentences in text
        if ((text[i] == '.' || text[i] == '!' || text[i] == '?') && text[i + 1] == ' ')
        {
            sentences++;
        }
    }

    // to account for last word/sentence
    words++;
    sentences++;

    // calculate average letters and sentences per 100 words
    float letters_100_words = (100 / (float) words) * (float) letters; // works
    float sentences_100_words = (100 / (float) words) * (float) sentences; // works

    // calculate index
    float index = 0.0588 * letters_100_words - 0.296 * sentences_100_words - 15.8;

    // print output
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %1.0f\n", index);
    }
}
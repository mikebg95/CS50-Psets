#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // ask for height and store in int variable
    int height;

    do
    {
        height = get_int("Please insert height of pyramid: ");
    }
    while (height < 1 || height > 8);

    // draw pyramid
    for (int i = 1; i <= height; i++)
    { 

        // draw spaces
        for (int j = height - i; j > 0; j--)
        {
            printf(" ");
        }

        // draw hashtags
        for (int k = 1; k <= i; k++)
        {
            printf("#");
        }    
        printf("\n"); // new line after each row
    }

}

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    void print_char(char c, int num); // initialize function

    // ask user for height
    int height;
    do
    {
        height = get_int("Please insert height of pyramid: ");
    }
    while (height < 1 || height > 8);

    for (int i = 1; i <= height; i++)
    {   
        // number of spaces and hashtags per row
        int space_num =  height - i;
        int hashtag_num = i;

        // call function print_char
        print_char(' ', space_num);
        print_char('#', hashtag_num);
        printf("  ");
        print_char('#', hashtag_num);
        printf("\n");
    }
}

void print_char(char c, int num) {
    for (int i = 0; i < num; i++) {
        printf("%c", c);
    }
}

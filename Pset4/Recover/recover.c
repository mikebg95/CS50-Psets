#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <cs50.h>
#include <string.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // make sure user enters correct arguments
    if (argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }

    // open image file and check for null
    FILE *infile = fopen(argv[1], "r");
    if (infile == NULL)
    {
        printf("ERROR: File cannot be opened\n");
        return 1;
    }

    BYTE buffer[512]; // buffer to read 512 bytes from file at a time
    bool first_jpeg_found = false; // to know whether found jpg is first or not
    int jpeg_number = 0; // to keep track of number of jpg's found
    char title[8]; // array of chars to store title for new jpg files
    FILE *outfile;

    // keep reading 512 bytes into buffer until EOF is reached
    while (fread(buffer, 1, 512, infile) != 0)
    {
        // if JPEG header found
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // if its the very first JPEG, open new file
            if (!first_jpeg_found)
            {
                first_jpeg_found = true;
                sprintf(title, "%03i.jpg", jpeg_number);
                outfile = fopen(title, "w");
                fwrite(buffer, 1, 512, outfile);
                jpeg_number++;

            }
            // if its not the very first JPEG, close previous file and open new one
            else
            {
                fclose(outfile);
                sprintf(title, "%03i.jpg", jpeg_number);
                outfile = fopen(title, "w");
                fwrite(buffer, 1, 512, outfile);
                jpeg_number++;

            }
        }
        // if not JPEG header found
        else
        {
            // if already found JPEG, write buffer to file
            if (first_jpeg_found)
            {
                fwrite(buffer, 1, 512, outfile);
            }
        }
    }

    // close files and free memory
    fclose(infile);
    fclose(outfile);


}

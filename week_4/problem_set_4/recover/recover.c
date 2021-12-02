#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t BYTE;
const int BLOCK_SIZE = 512;
BYTE buffer[BLOCK_SIZE];
bool is_first_jpeg = true;
bool found_jpeg = false;
char outfile[8];
int file_counter = 0;
size_t bytes_read;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    // open memory card
    char *infile = argv[1];
    FILE *inptr = fopen(infile, "r");
    // if memory card can't be read
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }
    // open output image
    sprintf(outfile, "%03i.jpg", file_counter);
    FILE *outptr = fopen(outfile, "w");

    // repeat until end of card
    while (true)
    {
        // read 512 bytes into a buffer
        bytes_read = fread(buffer, sizeof(BYTE), BLOCK_SIZE, inptr);
        // if reach the end of file, end
        if (bytes_read == 0)
        {
            break;
        }

        // if it's the start of a new jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // if it's the first jpeg
            if (is_first_jpeg)
            {
                found_jpeg = true;
                is_first_jpeg = false;
            }
            // else: close the previous file before opening a new one
            else
            {
                fclose(outptr);
            }
            sprintf(outfile, "%03i.jpg", file_counter);
            outptr = fopen(outfile, "w");
            fwrite(buffer, sizeof(BYTE), bytes_read, outptr);
            file_counter++;
        }
        // else: if already found a jpeg, keep writing to the current file
        else if (found_jpeg)
        {
            fwrite(buffer, sizeof(BYTE), bytes_read, outptr);
        }
    }
    // close file
    fclose(inptr);
    fclose(outptr);
    return 0;
}






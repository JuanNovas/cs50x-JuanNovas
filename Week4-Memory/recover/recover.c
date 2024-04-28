#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int BLOCK_SIZE = 512;

typedef uint8_t BYTE;

BYTE buffer[BLOCK_SIZE];

int n = 0;

char filename[8];

FILE *fotito = NULL;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover Image\n");
        return 1;
    }
    FILE *image = fopen(argv[1], "r");

    while (fread(buffer, 1, BLOCK_SIZE, image) == BLOCK_SIZE)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (n == 0)
            {
                sprintf(filename, "%03i.jpg", n);
                fotito = fopen(filename, "w");
                fwrite(buffer, 1, BLOCK_SIZE, fotito);
                n++;
            }
            else if (n > 0)
            {
                fclose(fotito);
                sprintf(filename, "%03i.jpg", n);
                fotito = fopen(filename, "w");
                fwrite(buffer, 1, BLOCK_SIZE, fotito);
                n++;
            }
        }
        else if (n > 0)
        {
            fwrite(buffer, 1, BLOCK_SIZE, fotito);
        }
    }

    fclose(image);
    fclose(fotito);
}

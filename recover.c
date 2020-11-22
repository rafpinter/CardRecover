//
//
//  CardRecover
//  
//  This is a tool that allows you to recover deleted jpeg from a memory card
// 
//

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "r");
    if (!card)
    {
        return 1;
    }

    unsigned char buffer[512];
    char filename[7];
    int n = 0; //for checking the size of buffer
    int i = 0; //for counting the files
    int k = 0; //prevents writing before finding the first jpeg
    sprintf(filename, "%03i.jpg", i);
    FILE *img = fopen(filename, "w");

    do
    {
        n = fread(buffer, 1, 512, card);
        if (n == 0)
        {
            break;
        }

        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff
            && (buffer[3] & 0xf0) == 0xe0) //checking if buffer is the start of a new jpeg
        {
            k = 1;

            if (i == 0) //if first JPEG
            {
                fwrite(buffer, 1, 512, img);
            }
            else //close JPEG and start new one
            {
                fclose(img); //closing previous file
                sprintf(filename, "%03i.jpg", i);
                img = fopen(filename, "w"); //opening new file
                buffer[0] = 0xff;
                fwrite(buffer, 1, 512, img);
            }
            i++; //updating file's count

        }
        else
        {
            if (k != 0) //prevents writing before finding the first jpeg
            {
                //continue printing
                fwrite(buffer, 1, 512, img);
            }
            if (n != 512)
            {
                fwrite(buffer, 1, n, img);
            }
        }
    }
    while (n == 512);

    fclose(card);
    fclose(img);
    return 0;
}
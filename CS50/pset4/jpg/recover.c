/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Author: Anaum Riaz
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BLOCK 512
typedef unsigned char BYTE;


int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 1)
    {
        printf("Usage: ./recover\n");
        return 1;
    }


    // open input file 
    FILE* inptr = fopen("card.raw", "r");
    if (inptr == NULL)
    {
        printf("Could not open card.raw.\n");
        return 2;
    }

    int jpeg_no = 0; 
    char jpgpatt1[4] = {0xff, 0xd8, 0xff, 0xe0};
    char jpgpatt2[4] = {0xff, 0xd8, 0xff, 0xe1};
    
    BYTE buff[BLOCK];
    FILE* outptr = NULL;
   
    while (fread(buff, BLOCK, 1, inptr) > 0)
    {
          
        if (memcmp(jpgpatt1, buff, 4) == 0 || memcmp(jpgpatt2, buff, 4) == 0 )
        {
            
            // close already open file before creating new
            if (outptr != NULL)
            {
                fclose(outptr);
            }
            
            // making new file for writing
            char *namefile = malloc(3 + sizeof(".jpg") + 1);
            sprintf(namefile, "%03d.jpg", jpeg_no);
           
          
            outptr = fopen(namefile, "w");
            free(namefile);
            
            if (outptr == NULL)
            {
                fclose(inptr);
                fprintf(stderr, "Could not create %s.\n", namefile);
                return 3;
            }
            
            jpeg_no++;
        }
       
        // Writing 512 Bytes now  
        if (outptr != NULL)
        {
            fwrite(buff, BLOCK, 1, outptr);  
        }
    }
   
    // closing file pointers
    fclose(outptr);  
    fclose(inptr);

    // Adios
    return 0;
}


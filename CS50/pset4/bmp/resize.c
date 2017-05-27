/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Anaum Riaz.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

void pix_inter(int n, RGBTRIPLE, FILE* outptr);
void copy_rows(int, int wp, int new_padding, FILE *);

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];
    int res_n = atoi(argv[1]);


    if (res_n < 0 || res_n > 100)
    {
        printf("Enter  0 < 'n' < 101.\n");
        return 2;
    }

  
    // open input file 
    FILE* inptr = fopen(infile, "r");
    

    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w+");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

// to change  DWORD  biSizeImage;
    BITMAPFILEHEADER new_bf;
    BITMAPINFOHEADER new_bi;
    new_bf = bf;
    new_bi = bi;
    new_bi.biWidth = bi.biWidth * res_n;
    new_bi.biHeight = bi.biHeight * res_n;
    
    // determine padding for scanlines
    int old_padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int new_padding =  (4 - (new_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    new_bf.bfSize = 54 + (new_bi.biWidth * abs(new_bi.biHeight) * 3) + abs(new_bi.biHeight) * new_padding;
    
    new_bi.biSizeImage = abs(new_bi.biHeight) * (new_bi.biWidth * 3 + new_padding);
     
    // write outfile's BITMAPFILEHEADER/infoHeader
    fwrite(&new_bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&new_bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    RGBTRIPLE n_pix;
   
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
       
        // iterate over pixels in scanline
        for (int j = 1; j <= bi.biWidth; j++)
        {
        
            fread(&n_pix, sizeof(RGBTRIPLE), 1, inptr);
            // write RGB triple to outfile
            fwrite(&n_pix, sizeof(RGBTRIPLE), 1, outptr);
            
            pix_inter(res_n, n_pix, outptr);
            
        }

       
        // skip over padding, if any
        fseek(inptr, old_padding, SEEK_CUR);
        
        for (int k = 0; k < new_padding; k++)
        {
            fputc(0x00, outptr);
        }
    
        copy_rows(res_n, new_bi.biWidth, new_padding, outptr);
         
}
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // Bye bye
    return 0;
}


void pix_inter(int n, RGBTRIPLE n_pix, FILE* outptr)
{
  
    // n-1 to fill
    for (int i = 1; i < n; i++)
    {
        // write RGB triple to outfile
        fwrite(&n_pix, sizeof(RGBTRIPLE), 1, outptr);
    }
}

void copy_rows(int n, int wp, int new_padding, FILE*  outptr)
{
    int nbytes = new_padding + wp * 3;
    BYTE *row = malloc(nbytes);
    fseek(outptr, -nbytes, SEEK_CUR);
    fread(row, 1, nbytes, outptr);
    
    for (int m = 0 ; m < (n - 1); m++)
    {
        fwrite(row, 1, nbytes, outptr);
    } 
    free (row);
}  
             

/**
 *Joshua Ferdaszewski
 *ferdaszewski@gmail.com 
 *
 *resize.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Resizes a BMP by a factor of n
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./copy n infile outfile\n");
        return 1;
    }
    
    // check for appropriate resize factor
    int n = atoi(argv[1]);
    if (n < 1 || n > 100)
    {
        printf("Usage: ./copy n infile outfile\n");
        return 2;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];    

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
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
    
    // create info and file header for output
    BITMAPFILEHEADER bfout = bf;
    BITMAPINFOHEADER biout = bi;
    
    // adjust output BITMAPFILEHEADER to reflect larger size
    // new image width and height in pixels
    biout.biWidth = bi.biWidth * n;
    biout.biHeight = bi.biHeight * n;
        
    // determine padding for input scanlines
    int paddingin =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // determine padding for output scanlines
    int paddingout = (4 - (biout.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // output image size in bytes
    biout.biSizeImage = (biout.biWidth * sizeof(RGBTRIPLE) + paddingout) * abs(biout.biHeight);
    
    // output file size in bytes
    bfout.bfSize = biout.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bfout, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&biout, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // create array to hold scanline
        RGBTRIPLE buffer[biout.biWidth];
        int counter = 0;
        
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;
            
            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
    
            // write RGB triple to output n times
            for (int k = 0; k < n; k++)
            {
                // write pixels to buffer
                buffer[counter] = triple;
                counter++;
            }
        }
        
        // skip over input padding, if any
        fseek(inptr, paddingin, SEEK_CUR);
        
        // write buffer to output file n times
        for (int p = 0; p < n; p++)
        {
            // write scanline to file
            fwrite(&buffer, sizeof(RGBTRIPLE), biout.biWidth, outptr);
        
            // then add output file padding
            for (int l = 0; l < paddingout; l++)
            {
                fputc(0x00, outptr);
            }
        }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}

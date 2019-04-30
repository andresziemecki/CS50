/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./copy infile outfile\n");
        return 1;
    }

    // remember filenames
    int n = atoi(argv[1]);
    if (n<1 || n>100)
    {
        printf("you put a number too big!\n");
        return -1;
    }
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf; //llama una varialbe tipo BITMAPFILEHEADER como bf
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    
    /*fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
    ptr − This is the pointer to a block of memory with a minimum size of size*nmemb bytes.
    size − This is the size in bytes of each element to be read.
    nmemb − This is the number of elements, each one with a size of size bytes.
    stream − This is the pointer to a FILE object that specifies an input stream.*/
    
    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    int padding =  ((4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4);
    bi.biWidth = bi.biWidth*n;
    bi.biHeight = bi.biHeight*n;
    int padding2 =  ((4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4); //la cantidad de relleno se eneucnetra en bytes
    bi.biSizeImage = bi.biWidth*abs(bi.biHeight)*3 + abs(bi.biHeight)*padding2;
    bf.bfSize = (bi.biSizeImage + 54);
    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    
    RGBTRIPLE* array = malloc(bi.biWidth*sizeof(RGBTRIPLE) + 1);
    if (array == NULL)
    return 1;

    for (int i = 0, biHeight = abs(bi.biHeight)/n; i < biHeight; i++)
    {

        for (int j = 0; j < (bi.biWidth)/n; j++)
        {

            RGBTRIPLE triple;


            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            *(array+j) = triple;
            for (int k = 0; k < n ; k++)
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
        }
        
        fseek(inptr, 54 + (1+i)*(sizeof(RGBTRIPLE)*bi.biWidth/n) + (1+i)*padding, SEEK_SET);
        
        for (int k = 0; k < padding2; k++)
        {
            fputc(0x00, outptr);
        }
        
        for (int x = 1; x < n; x++)
        {
            for (int j = 0; j < (bi.biWidth)/n; j++)
            {
                for (int k = 0; k < n ; k++)
                {
                    fwrite((array+j), sizeof(RGBTRIPLE), 1, outptr);
                }
            }
        
            fseek(inptr, 54 + (1+i)*(sizeof(RGBTRIPLE)*bi.biWidth/n) + (1+i)*padding, SEEK_SET);
            for (int k = 0; k < padding2; k++)
            {
                fputc(0x00, outptr);
            }
        }
        
    }
    free(array);


    fclose(inptr);

    fclose(outptr);

    return 0;
}

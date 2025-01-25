// LSB_Steg Header File
//

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define SUCCESS 0
#define FAILURE -1

#define ACTION_HIDE		1
#define ACTION_EXTRACT	2

// the following structure information is taken from wingdi.h

/* constants for the biCompression field */
//#define BI_RGB        0L	// (no compression)
//#define BI_RLE8       1L	// (run-length encoding, 8 bits)
//#define BI_RLE4       2L	// (run-length encoding, 4 bits)
//#define BI_BITFIELDS  3L
//#define BI_JPEG       4L
//#define BI_PNG        5L

// this structure defines the file header format for a bitmap file
//typedef struct tagBITMAPFILEHEADER // (14 bytes)
//{
//        WORD    bfType;					// ASCII "BM"
//        unsigned int   bfSize;					// total length of bitmap file
//        WORD    bfReserved1;			// reserved
//        WORD    bfReserved2;			// reserved
//        unsigned int   bfOffBits;				// offset to start of actual pixel data
//} BITMAPFILEHEADER;

// this structure defines the header which describes the bitmap itself
//typedef struct tagBITMAPINFOHEADER // (40 bytes)
//{
//        unsigned int      biSize;				// size of BITMAPINFOHEADER
//        LONG       biWidth;				// width in pixels
//        LONG       biHeight;			// height in pixels
//        WORD       biPlanes;			// always 1
//        WORD       biBitCount;			// color bits per pixel
//        unsigned int      biCompression;		// BI_RGB, BI_RLE8, BI_RLE4
//        unsigned int      biSizeImage;			// total bytes in image
//        LONG       biXPelsPerMeter;		// 0, or optional horizontal resolution
//        LONG       biYPelsPerMeter;		// 0, or optional vertical resolution
//        unsigned int      biClrUsed;			// colors actually used (normally zero, can be lower than biBitCount)
//        unsigned int      biClrImportant;		// important colors actualy used (normally zero)
//} BITMAPINFOHEADER;	

//typedef struct tagRGBQUAD
//{
//        BYTE    rgbBlue;
//        BYTE    rgbGreen;
//        BYTE    rgbRed;
//        BYTE    rgbReserved;
//} RGBQUAD;

/*
* txtFile struct stores the content of the file and the length of message
*/
typedef struct txtFile{
	unsigned char *pFile;
	size_t size;
} txtFile;

// Functions in Bitmap reader
void initGlobals();
void displayFileInfo(char *pFileName, BITMAPFILEHEADER *pFileHdr, BITMAPINFOHEADER *pFileInfo, RGBQUAD *ptrPalette, unsigned char *pixelData);
void createDupePal(char* pFileName, BITMAPFILEHEADER* pFileHdr, BITMAPINFOHEADER* pFileInfo, RGBQUAD* ptrPalette, unsigned char* pixelData);
bool isValidBitMap(char *filedata);
unsigned char *readBitmapFile(char *fileName, unsigned int *fileSize);
txtFile *readTextFile(char *fileName, unsigned int *fileSize);
int writeFile(char *filename, int fileSize, unsigned char *pFile);
unsigned int total_bytes_available(BITMAPINFOHEADER *pFileInfo);
void charToBinary(txtFile *text_file, char *binaryStr, char *bitArray);
void binaryToBitArray(const char *binaryStr, char *bitArray);
void textToBinary(txtFile *text_file, char *binaryStr);
void bitArrayToBinary(const char *bitArray, int bitArraySize, unsigned char *binaryStr);
char* binaryToText(unsigned char* binaryStr, int size);


// Bitmap Reader
//
#include "BitmapReader.h"

extern BITMAPFILEHEADER *gpCoverFileHdr, *gpStegoFileHdr;
extern BITMAPINFOHEADER *gpCoverFileInfoHdr, *gpStegoFileInfoHdr;
extern RGBQUAD *gpCoverPalette, *gpStegoPalette;
extern unsigned int gCoverFileSize, gMsgFileSize, gStegoFileSize;

// Command Line Global Variables
extern char gCoverPathFileName[MAX_PATH], *gCoverFileName;
extern char gMsgPathFileName[MAX_PATH], *gMsgFileName;
extern char gStegoPathFileName[MAX_PATH], *gStegoFileName;
extern char gInputPathFileName[MAX_PATH], *gInputFileName;
extern char gOutputPathFileName[MAX_PATH], *gOutputFileName;
extern char gAction;						// typically hide (1), extract (2), wipe (3), randomize (4), but also specifies custom actions for specific programs
extern char gNumBits2Hide;


// Global Variables for File Data Pointers
/*
BITMAPFILEHEADER *gpCoverFileHdr, *gpStegoFileHdr;
BITMAPINFOHEADER *gpCoverFileInfoHdr, *gpStegoFileInfoHdr;
RGBQUAD *gpCoverPalette, *gpStegoPalette;
unsigned int gCoverFileSize, gMsgFileSize, gStegoFileSize;

// Command Line Global Variables
char gCoverPathFileName[MAX_PATH], *gCoverFileName;
char gMsgPathFileName[MAX_PATH], *gMsgFileName;
char gStegoPathFileName[MAX_PATH], *gStegoFileName;
char gInputPathFileName[MAX_PATH], *gInputFileName;
char gOutputPathFileName[MAX_PATH], *gOutputFileName;
char gAction;						// typically hide (1), extract (2), wipe (3), randomize (4), but also specifies custom actions for specific programs
char gNumBits2Hide;
*/
void initGlobals(){
	gpCoverFileHdr = NULL;
	gpStegoFileHdr = NULL;
	gpCoverFileInfoHdr = NULL;
	gpStegoFileInfoHdr = NULL;
	gpCoverPalette = NULL;
	gpStegoPalette = NULL;
	gCoverFileSize = gMsgFileSize = gStegoFileSize = 0;

	// Command Line Global Variables
	gCoverPathFileName[0] = 0;
	gCoverFileName = NULL;
	gMsgPathFileName[0] = 0;
	gMsgFileName = NULL;
	gStegoPathFileName[0] = 0;
	gStegoFileName = NULL;
	gAction = 0;						// typically hide (1), extract (2)
	gNumBits2Hide = 1;

	return;
} // initGlobals


void displayFileInfo(char *pFileName,
					 BITMAPFILEHEADER *pFileHdr, 
					 BITMAPINFOHEADER *pFileInfo,
					 RGBQUAD *ptrPalette,
					 unsigned char *pixelData)
{
	int numColors, i;

	printf("\nFile Information for %s: \n\n", pFileName);
	printf("File Header Info:\n");
	printf("File Type: %c%c\nFile Size:%ld bytes\nData Offset:%ld\n\n", 
		(pFileHdr->bfType & 0xFF), (pFileHdr->bfType >> 8), pFileHdr->bfSize, pFileHdr->bfOffBits);

	switch(pFileInfo->biBitCount)
	{
	case 1:
		numColors = 2;
		break;
	case 4:
		numColors = 16;
		break;
	case 8:
		numColors = 256;
		break;
	case 16:
		numColors = 65536;
		break;
	case 24:
		numColors = 16777216;
		break;
	default:
		numColors = -1;
	}

	printf("Bit Map Image Info:\n\nSize Info Header:%ld\nWidth:%ld\nHeight:%ld\nPlanes:%d\n"
		"Bits/Pixel:%d ==> %d colors\n"
		"Compression:%ld\nImage Size:%ld\nRes X:%ld\nRes Y:%ld\nColors:%ld\nImportant Colors:%ld\n\n",
		pFileInfo->biSize, 
		pFileInfo->biWidth, 
		pFileInfo->biHeight, 
		pFileInfo->biPlanes, 
		pFileInfo->biBitCount, numColors,
		pFileInfo->biCompression, 
		pFileInfo->biSizeImage, 
		pFileInfo->biXPelsPerMeter,
		pFileInfo->biYPelsPerMeter,
		pFileInfo->biClrUsed,
		pFileInfo->biClrImportant);

	//	There are no palettes
	if(pFileInfo->biBitCount > 16 || numColors == -1)
	{
		printf("\nNo Palette\n\n");
	}
	else
	{
		printf("Palette:\n\n");

		for(i = 0; i < numColors; i++)
		{
			printf("R:%02x   G:%02x   B:%02x\n", ptrPalette->rgbRed, ptrPalette->rgbGreen, ptrPalette->rgbBlue);
			ptrPalette++;
		}
	}

	// print first 24 bytes of pixel data
	printf("\n Pixel Data: \n\n");
	for(int i = 0; i < 24; i++)
	{
		printf("%02X ", *(pixelData + i));
	}
	printf("\n\n");
	return;
} // displayFileInfo

// quick check for bitmap file validity - you may want to expand this or be more specfic for a particular bitmap type
bool isValidBitMap(char *filedata)
{
	if( filedata[0] != 'B' || filedata[1] != 'M') return false;

	return true;
} // isValidBitMap


// reads specified bitmap file from disk
unsigned char *readBitmapFile(char *fileName, unsigned int *fileSize)
{
	FILE *ptrFile;
	unsigned char *pFile;

	ptrFile = fopen(fileName, "rb");	// specify read only and binary (no CR/LF added)

	if(ptrFile == NULL)
	{
		printf("Error in opening file: %s.\n\n", fileName);
		exit(-1);
	}

	fseek(ptrFile, 0, SEEK_END);
	*fileSize = ftell(ptrFile);
	fseek(ptrFile, 0, SEEK_SET);

	// malloc memory to hold the file, include room for the header and color table
	pFile = (unsigned char *) malloc(*fileSize);

	if(pFile == NULL)
	{
		printf("Error - Could not allocate %d bytes of memory for bitmap file.\n\n", *fileSize);
		exit(-1);
	}

	// Read in complete file
	// buffer for data, size of each item, max # items, ptr to the file
	fread(pFile, sizeof(unsigned char), *fileSize, ptrFile);
	fclose(ptrFile);

	return(pFile);
} // readBitmapFile

// writes modified bitmap file to disk
// gMask used to determine the name of the file
int writeFile(char *filename, int fileSize, unsigned char *pFile)
{
	FILE *ptrFile;
	int x;

	// open the new file, MUST set binary format (text format will add line feed characters)
	ptrFile = fopen(filename, "wb+");
	if(ptrFile == NULL)
	{
		printf("Error opening file (%s) for writing.\n\n", filename);
		exit(-1);
	}

	// write the file
	x = (int) fwrite(pFile, sizeof(unsigned char), fileSize, ptrFile);

	// check for success
	if(x != fileSize)
	{
		printf("Error writing file %s.\n\n", filename);
		exit(-1);
	}
	fclose(ptrFile); // close file
	return(SUCCESS);
} // writeFile


// Calulate total availble bytes if only 3 MSB are used
unsigned int total_bytes_available(BITMAPINFOHEADER *pFileInfo){
	return (unsigned int)((pFileInfo->biHeight * pFileInfo->biWidth) * 3)/8;
}

// Function to convert a character to its binary representation
void charToBinary(unsigned char c, char *binaryStr) {
    for (int i = 7; i >= 0; i--) {
        binaryStr[7 - i] = (c & (1 << i)) ? '1' : '0';
    }
    binaryStr[8] = '\0';
}


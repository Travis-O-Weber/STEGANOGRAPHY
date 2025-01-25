#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "hide-extract.h"
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
extern unsigned char g_mask[9];
extern int B;

/*
* Hide function
* input: hide and cover file descriptors
* output: final image file descriptor
*/
int hide(char *coverFile, char *hidFile) {
    char *finalFile = prependHide(coverFile, FILE_NAME_SIZE);
    char *binaryStr;
    unsigned int hide_space, messageLength = 0;
    unsigned int coverFileSize;
    unsigned char *pixelData, *coverData = readBitmapFile(coverFile, &coverFileSize);
    txtFile *text_file;
    BYTE maskedByte, hiddenMD, xorMD, indx1, indx2, indx3, indx4 = 0;

    initGlobals();

    if (coverFile == NULL) {
        printf("[ERROR] NULL %s\n", coverFile);
        return -1;
    }

    if (!isValidBitMap((char *)coverData)) {
        printf("Invalid cover image file.\n");
        return -1;
    }

    // Set bmp data to coverData
    coverData = readBitmapFile(coverFile, &gCoverFileSize);
    gpCoverFileHdr = (BITMAPFILEHEADER *) coverData;
    gpCoverFileInfoHdr = (BITMAPINFOHEADER *) (coverData + sizeof(BITMAPFILEHEADER));
    gpCoverPalette = (RGBQUAD *) ((char *) coverData + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER));
    pixelData = coverData + gpCoverFileHdr->bfOffBits;

    createDupePal(coverFile, gpCoverFileHdr, gpCoverFileInfoHdr, gpCoverPalette, pixelData);

    // Check available bytes 
    hide_space = total_bytes_available(gpCoverFileInfoHdr);


    // READ TEXT FILE 
    text_file = readTextFile(hidFile, &messageLength);
    //printf(".txt Content: \n%s\n\nBinary Representation:\n", text_file->pFile);
    
    if ((messageLength+5) > hide_space){
        printf("\n\nWARNING: the message being hid is too big for the given file, only %d bytes can be hidden out of %d, %d%% of message.\n", (hide_space-5), messageLength, (hide_space*100/messageLength));
        messageLength = hide_space-5;
        text_file->size = hide_space-5;
    }

    else{
        printf("\n\nThe message will fit in the given file! %d bytes can be hidden within the %d bytes of available capacity, %d%% of message.\n", messageLength,(hide_space-5), (messageLength*100/hide_space));
    }

    binaryStr = (char *)malloc(sizeof(char) * text_file->size * 9 + 1);

    if (binaryStr == NULL) {
        printf("[ERROR] hide-extra.c | binaryStr Malloc\n");
        return 1;
    }

    // Convert text to binary representation
    textToBinary(text_file, binaryStr);
    //printf("%s\n", binaryStr);

    // Allocate memory for the bit array (each 3-bit packed into a byte)
    size_t bitArraySize = ((text_file->size * 8) + 2) / 3;
    //unsigned char *bitArray = (unsigned char *)malloc(bitArraySize);
	char *bitArray = (char *)malloc(bitArraySize);
	
    if (bitArray == NULL) {
        printf("[ERROR] hide-extra.c | bitArray Malloc\n");
        free(binaryStr);
        return 1;
    }

    // Convert binary string to bit array
    binaryToBitArray(binaryStr, bitArray);

    // Print the bit array
//    printf("Bit Array:\n");
//    for (size_t i = 0; i < bitArraySize; i++) {
//        printf("%02X ", bitArray[i]);
//    }
//    printf("\n");
	
    
    //Setting the 5th bit from the end to represent how many bytes take up the hidden file size
    if (messageLength < 0xFF){
        coverData[gpCoverFileHdr->bfSize -5] = 0x01;
        coverData[gpCoverFileHdr->bfSize - 1] = messageLength;
        } 

    else if (messageLength < 0xFFFF){
        coverData[gpCoverFileHdr->bfSize -5] = 0x02;
        indx1 = (BYTE)text_file->size;
        indx2 = (BYTE)(text_file->size>> 8);
        coverData[gpCoverFileHdr->bfSize - 1] = indx1;
        coverData[gpCoverFileHdr->bfSize - 2] = indx2;
        } 

    else if (messageLength < 0xFFFFFF){
        coverData[gpCoverFileHdr->bfSize -5] = 0x03;
        indx1 = (BYTE)text_file->size;
        indx2 = (BYTE)(text_file->size>> 8);
        indx3 = (BYTE)(text_file->size>> 16);
        coverData[gpCoverFileHdr->bfSize - 1] = indx1;
        coverData[gpCoverFileHdr->bfSize - 2] = indx2;
        coverData[gpCoverFileHdr->bfSize - 3] = indx3;
        } 

    else if (messageLength < 0xFFFFFFFF){
        coverData[gpCoverFileHdr->bfSize -5] = 0x04;
        indx1 = (BYTE)text_file->size;
        indx2 = (BYTE)(text_file->size>> 8);
        indx3 = (BYTE)(text_file->size>> 16);
        indx3 = (BYTE)(text_file->size>> 24);
        coverData[gpCoverFileHdr->bfSize - 1] = indx1;
        coverData[gpCoverFileHdr->bfSize - 2] = indx2;
        coverData[gpCoverFileHdr->bfSize - 3] = indx3;
        coverData[gpCoverFileHdr->bfSize - 4] = indx4;
        } 

    else {
        printf("Error: unkown error occured assigning message length to last bytes.");
        return -1;
    } 
    
//    DEBUG
//    printf("\nmessageLength: %d\n",messageLength);
//    printf("\nhide space available: %d\n",hide_space);
//    printf("\nbyte rep of message length: %02x\n", (BYTE)text_file->size);
    //displayFileInfo(coverFile, gpCoverFileHdr, gpCoverFileInfoHdr, gpCoverPalette, pixelData);

    for (size_t i = 0; i < bitArraySize; i++) {
    	// masking og data
        maskedByte = coverData[1078 + i] & 0x1F;
        // assigning hidden message data
        hiddenMD = (BYTE)bitArray[i];
        // shifting hidden message data to 3 MSB
        xorMD = hiddenMD << 5;
		// assigning hidden message data in MSB of pixel data
		maskedByte = maskedByte ^ xorMD;
		//printf("%02x ", maskedByte);    
		coverData[1078 + i] = maskedByte;    
    }
    //printf("\n");
    
    // Save the final image
    int result = writeFile(finalFile, coverFileSize, coverData);

    // Free allocated memory
    free(binaryStr);
    free(bitArray);
    free(text_file->pFile);
    free(text_file);
    free(coverData);
    return result;
}

/*
* Extract function
* input: cover file descriptors
* output: final image file descriptor
*/
int extract(char *cover, char *outfile){
	
    unsigned int coverFileSize, bytesOfMessageLength = 0, indxToMulti2 = 0, indxToMulti3= 0, indxToMulti4 = 0, messageLength = 0;
    unsigned char *coverData, *messageData, *binaryStr;
    BYTE maskedByte, indx1, indx2, indx3, indx4;
    char *bitArray;
	
    initGlobals();

    // Read the cover image
    coverData = readBitmapFile(cover, &coverFileSize);

    //Error Checking
    if (coverData == NULL) {
        printf("Error reading cover image file: %s\n", cover);
        return -1;
    }

    //Set globals, create space in memory for message data
	gpCoverFileHdr = (BITMAPFILEHEADER *) coverData;
    gpCoverFileInfoHdr = (BITMAPINFOHEADER *) (coverData + sizeof(BITMAPFILEHEADER));

    //Reading the 5th byte from the end to represent how many bytes take up the hidden file size.
    bytesOfMessageLength = coverData[gpCoverFileHdr->bfSize -5];

	switch (bytesOfMessageLength) {
	    case 0x1:
	        messageLength = coverData[gpCoverFileHdr->bfSize - 1];
	        break;
	
	    case 0x2:
	        indx1 = coverData[gpCoverFileHdr->bfSize - 1];
	        indx2 = coverData[gpCoverFileHdr->bfSize - 2];
	        indxToMulti2 = indx2 << 8;
	        messageLength = indxToMulti2 + indx1;
	        break;
	
	    case 0x3:
	        indx1 = coverData[gpCoverFileHdr->bfSize - 1];
	        indx2 = coverData[gpCoverFileHdr->bfSize - 2];
	        indx3 = coverData[gpCoverFileHdr->bfSize - 3];
	        indxToMulti2 = indx2 << 8;
	        indxToMulti3 = indx3 << 16;
	        messageLength = indxToMulti3 + indxToMulti2 + indx1;
	        break;
	
	    case 0x4:
	        indx1 = coverData[gpCoverFileHdr->bfSize - 1];
	        indx2 = coverData[gpCoverFileHdr->bfSize - 2];
	        indx3 = coverData[gpCoverFileHdr->bfSize - 3];
	        indx4 = coverData[gpCoverFileHdr->bfSize - 4];
	        indxToMulti2 = indx2 << 8;
	        indxToMulti3 = indx3 << 16;
	        indxToMulti4 = indx4 << 24;
	        messageLength = indxToMulti4 + indxToMulti3 + indxToMulti2 + indx1;
	        break;
	
	    default:
	        printf("Error: Unknown error occurred reading message length from fifth to last byte. ");
	        return -1;
	}

	//printf("\nmessage length = %u Bytes\n", messageLength);
    messageData = (unsigned char *)malloc(sizeof(unsigned char) * messageLength + 1);

	//Error Checking
	if (messageData == NULL){
	    printf("error malloc\n");
	    return 1;
	}
	
	int bitArraySize = (messageLength * 8 / 3) + 1;
	bitArray = (char *)malloc(bitArraySize);
	binaryStr = (unsigned char *)malloc(messageLength * 8 + 1);
	
    for (int i = 0; i < bitArraySize; i++) {
        maskedByte = coverData[1078 + i] & 0xE0;  
        maskedByte = maskedByte >> 5; 
		bitArray[i] = maskedByte;  
		//printf("%02x ", bitArray[i]);
    }
    //printf("\n");
    
    // Convert BitArray to binary
    bitArrayToBinary(bitArray, bitArraySize, binaryStr);
    //printf("BINARY: \n%s\n", binaryStr);
   // writeHiddenMessageToFile(outfile, binaryStr, messageLength);

	char* textStr = binaryToText(binaryStr, messageLength * 8);
    writeHiddenMessageToFile(outfile, textStr, messageLength);

    //DEBUG
	//printf("\nExtracted Message:\n%s\n\n", textStr);
	
    // Free allocated memory
    free(bitArray);
    free(coverData);
    free(messageData);
	return 0;
}
	

/*
* Create Extracted Message File
* Writes the extracted message to a new file in the directory.
* Input: output file pointer
*/
int writeHiddenMessageToFile(char *outfile, char* messageData, unsigned int messageLength){

    FILE *ptrFile;
	//unsigned char *pFile;
    int x;

	ptrFile = fopen(outfile, "wb+");	

	if(ptrFile == NULL)
	{
		printf("Error in creating file: %s.\n\n", outfile);
		exit(-1);
	}

    x = (int) fwrite(messageData, sizeof(unsigned char), messageLength, ptrFile);

	// check for success
	if(x != messageLength)
	{
		printf("Error writing file %s.\n\n", outfile);
		exit(-1);
	}
	fclose(ptrFile); // close file
	return(SUCCESS);

}



/*
* Error Input Syntax
* Prints example syntax
*/
void error_syntax(){
	printf("\n[ERROR] Invalid syntax\nHiding: prog -h -c <cover_image.bmp> <hid_text.txt> [-b <bit number>]\n\
            Extracting: prog -e <cover_image.bmp> [-o <output file> ]\n\
            Where options within [] are optional\n\n");
}

/*
* Check file type
* input: file name
* output: int flag: 1 == incorrect file type 0 == correct file type
*/
int check_file_type(char *file_name, char *extension){
	
	size_t len = strlen(file_name);
	char file_type[5];
	
	// file too short / incorrect filetype
	if(len < 4){
		return 1;
	}
	
	strcpy(file_type, file_name + len -4);
	
	if(strcmp(file_type, extension) != 0){
		return 1;
	}else{
		// correct file type
		return 0;
	}
	printf("\n[ERROR] Check file syntax function \n");
	return 1;
}

/*
* Create Name of final File
*
*/
char* prependHide(const char *str, size_t size) {
    const char *word = "hide_";
    size_t wordLen = strlen(word);
    size_t strLen = strlen(str);

    // Ensure there is enough space
    if (size < wordLen + strLen + 1) {
        printf("Not enough space to prepend the word.\n");
        return NULL;
    }

    // Allocate memory for the final string
    char *finalName = malloc(wordLen + strLen + 1);

    if (finalName == NULL) {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    // Copy the new word to the beginning
    strcpy(finalName, word);

    // Concatenate the original string after the new word
    strcat(finalName, str);

    return finalName;
}






#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "hide-extract.h"
#include "BitmapReader.h"
#include "hashmap.h"
#include "linkedlist.h"

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

/*
* Hide function
* input: hide and cover file descriptors
* output: final image file descriptor
*/
int hide(char *coverFile, char *hidFile) {
	
	char *finalFile = prependHide(coverFile, FILE_NAME_SIZE);
	linked_list_T *list = NewLinkedList();
	color *temp;
	unsigned int *hide_space_ptr, hide_space, i;
	char binaryStr[9];
	
	initGlobals();
	// for testing hashmap and PQ
	int flag = 1;

    // Read the cover image
    unsigned int coverFileSize;
    unsigned char *coverData = readBitmapFile(coverFile, &coverFileSize);
    unsigned char *pixelData;
    
    if(coverFile == NULL){
    	printf("[ERROR] NULL %s\n", coverFile);
    	return -1;
	}

    if (!isValidBitMap((char *)coverData)) {
        printf("Invalid cover image file.\n");
        return -1;
    }

    // Read the hidden image
	

	coverData = readBitmapFile(coverFile, &gCoverFileSize);

	gpCoverFileHdr = (BITMAPFILEHEADER *) coverData;

	gpCoverFileInfoHdr = (BITMAPINFOHEADER *) (coverData + sizeof(BITMAPFILEHEADER) );

	// there might not exist a palette - I don't check here, but you can see how in display info
	gpCoverPalette = (RGBQUAD *) ( (char *) coverData + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) );

	pixelData = coverData + gpCoverFileHdr->bfOffBits;
	
	// Troubleshoot
	if(flag == 1){
		// Initialize hashmap
		init_hash_table(gpCoverFileInfoHdr, pixelData);
		//print_table();
		
		// Pass hashmap to Priority Queue
		for(int i = 0; i < TABLE_SIZE; i++){
			EnlistPR(list, hash_table[i]);
		}
		
		// Get top 32 colors
		for(int j = 0; j < TABLE_SIZE; j++){
			if(j >= 32){
				temp = Delist(list);
				free(temp);
			}
		}
		
		// Print Queue
		PrintAllRecords(list);
	}
	
	//displayFileInfo(coverFile, gpCoverFileHdr, gpCoverFileInfoHdr, gpCoverPalette, pixelData);

    // Save the final image
    int result = writeFile((char *)finalFile, coverFileSize, coverData);

    // Free allocated memory
    free(coverData);
    return result;
}


/*
* Extract function
* input: cover file descriptors
* output: final image file descriptor
*/
int extract(char *cover){
	if(cover != NULL){
		; // temp
	}
	printf("[SUCCES] Extrancting image\n");
	return 0;
}
	
/*
* Error Input Syntax
* Prints example syntax
*/
void error_syntax(){
	printf("[ERROR] Invalid syntax\nHiding: prog -h -c <cover_image.bmp> <hid_image.bmp>\nExtracting: prog -e <cover_image.bmp>\n");
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
	
	//printf("FILE TYPE: %s\n", file_type);
	
	if(strcmp(file_type, extension) != 0){
		return 1;
	}else{
		// correct file type
		return 0;
	}
	printf("[ERROR] Check file syntax function \n");
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
    char *finalName = malloc(wordLen + strLen + 1); // +1 for the null terminator

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






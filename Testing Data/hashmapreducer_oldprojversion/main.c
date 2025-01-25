#include <fcntl.h> 
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include "hide-extract.h"
#include "BitmapReader.h"
#include "hashmap.h"

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

/*
* Syntax:
*
* Hiding:
* prog -h -c <cover_image.bmp> <hid_image.bmp> 
*
* Extraction:
* prog -e <cover_image.bmp> <new_extract_image.bmp>
*
*/

int main(int argc, char *argv[]){
	
	// Initialize vars
	char cover[FILE_NAME_SIZE], hid[FILE_NAME_SIZE];
	
	if(argc < 4 ){
		// Print error syntax
		error_syntax();
		return 1;
	}
	
	switch(argc){
		// HIDE
		case 5:
			// Check flags
			if(strcmp(argv[1], "-h") != 0 || strcmp(argv[2], "-c") != 0){
				error_syntax();
				return 1;				
			}
			// Check if right file type
			if(check_file_type(argv[3], ".bmp") != 0){
				printf("Invalid file type: Try '.bmp' file type instead of %s\n", argv[3]);
				return 1;
			}
			
			// Check if right file type
			if(check_file_type(argv[4], ".txt") != 0){
				printf("Invalid file type: Try '.txt' file type instead of %s\n", argv[4]);
				return 1;
			}
			
			// Assign each filename to file descriptor
			strcpy(cover, argv[3]);
			strcpy(hid, argv[4]);
			
			// Begin hiding
			if(hide(cover, hid) != 0){
				printf("Error Hiding data");
				return 1;
			}
			break;
			
			
		// EXTRACT
		case 4:
			
			// Check flags
			if(strcmp(argv[1], "-e") != 0){
				error_syntax();
				return 1;				
			}
			
			// Check if right file type
			if(check_file_type(argv[2], ".bmp") != 0){
				printf("Invalid file type: Try '.bmp' file type instead of %s\n", argv[2]);
				return 1;
			}
			
			// Assign each filename to file descriptor
			strcpy(cover, argv[2]);
			
			// Begin extraction
			if(extract(cover) != 0){
				printf("Error Hiding data");
				return 1;
			}
			
			break;
		default:
			// Print error syntax
			error_syntax();
			return 1;
			break;
	}
	
	return 0;
}

#include <fcntl.h> 
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include "hide-extract.h"
#include "BitmapReader.h"

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
unsigned char g_mask[9] = { 0, 1, 3, 7, 15, 31, 63, 127, 255 };
int B = 0;

/*
* Syntax:
*
* Hiding:
* prog -h -c <cover_image.bmp> <hid_text.txt> 
*
* Extraction:
* prog -e <cover_image.bmp> <new_extract_image.bmp>
*
*/

int main(int argc, char *argv[]){
	
	// Initialize vars
	char cover[FILE_NAME_SIZE], hid[FILE_NAME_SIZE], output[FILE_NAME_SIZE];
	
	if(argc < 3 || argc > 7){
		// Print error syntax
		error_syntax();
		return 1;
	}
	
	switch(argc){
		// HIDE
		case 7:
			// Check flags
			if(strcmp(argv[1], "-h") != 0 || strcmp(argv[2], "-c") != 0){
				error_syntax();
				return 1;				
			}
			// Check if right file type
			if(check_file_type(argv[3], ".bmp") != 0){
				printf("Invalid file type: Try '.bmp' file type instead of %s\n", argv[3]);
				printf("\n\n");
				return 1;
			}


			if(strcmp(argv[5], "-b") != 0){
				printf("Invalid option. This spot is reserved '-b' and %s was entered\n", argv[5]);
				printf("\n\n");
				return 1;				
			}

			if( atoi(argv[6]) < 0 || atoi(argv[6]) > 8 ){
				printf("Invalid bit count. You may select between 0(default) and 8 bits\n");
				printf("\n\n");
				return 1;				
			}
			
			// Assign each filename to file descriptor
			strcpy(cover, argv[3]);
			strcpy(hid, argv[4]);
			B = atoi(argv[6]);
			
			// Begin hiding
			if(hide(cover, hid) != 0){
				printf("Error Hiding data");
				printf("\n\n");
				return 1;
			}
			
			printf("\n[SUCCESS] Text file '%s' has been hidden in '%s' and saved to the current directory as 'hide_%s'\n", argv[4], argv[3], argv[3]);
            printf("\n\n***************************************************************************\n");
				printf("*          8-bit, 32 Color Bitmap Duplicate Palette Hiding 2.0 V          *\n");
				printf("*  Program Created by  Kip Roberts-Lemus, Rodrigo Nunez and Travis Weber  *\n");
				printf("*                    Last Modified: August 6th, 2024                      *\n");
				printf("*                  University of Texas at San Antonio                     *\n");
				printf("***************************************************************************\n");
			printf("\n\n");
			break;
			
			case 5:
		/////////////////////////////////////////////////   HIDE   ///////////////////////////////////////////////////////////////

			if(strcmp(argv[1], "-h") == 0){

				// Check flags
				if(strcmp(argv[1], "-h") != 0 || strcmp(argv[2], "-c") != 0){
					error_syntax();
					return 1;				
				}
				// Check if right file type
				if(check_file_type(argv[3], ".bmp") != 0){
					printf("Invalid file type: Try '.bmp' file type instead of %s\n", argv[3]);
					printf("\n\n");
					return 1;
				}
				
				// Assign each filename to file descriptor
				strcpy(cover, argv[3]);
				strcpy(hid, argv[4]);
				
				// Begin hiding
				if(hide(cover, hid) != 0){
					printf("Error Hiding data");
					printf("\n\n");
					return 1;
				}
				
				printf("\n[SUCCESS] Data from '%s' has been hidden in '%s' and saved to the current directory as 'hide_%s'\n", argv[4], argv[3], argv[3]);
				printf("\n\n***************************************************************************\n");
				printf("*          8-bit, 32 Color Bitmap Duplicate Palette Hiding 2.0 V          *\n");
				printf("*  Program Created by  Kip Roberts-Lemus, Rodrigo Nunez and Travis Weber  *\n");
				printf("*                    Last Modified: August 6th, 2024                      *\n");
				printf("*                  University of Texas at San Antonio                     *\n");
				printf("***************************************************************************\n");
				printf("\n\n");

		}
		///////////////////////////////////////////////// EXTRACT ///////////////////////////////////////////////////////////////
		if(strcmp(argv[1], "-e") == 0){
			


			// Check flags
			if(strcmp(argv[1], "-e") != 0){
				error_syntax();
				return 1;				
			}
			
			// Check if right file type
			if(check_file_type(argv[2], ".bmp") != 0){
				printf("Invalid file type: Try '.bmp' file type instead of %s\n", argv[2]);
				printf("\n\n");
				return 1;
			}
			
			// Check flags
			if(strcmp(argv[3], "-o") != 0 ){
				printf("Invalid option. This spot is reserved '-o' and %s was entered\n", argv[5]);
				printf("\n\n");
				return 1;				
			}
			// Assign each filename to file descriptor
			strcpy(cover, argv[2]);
			strcpy(output, argv[4]);
			strcat(output, ".bin");
			
			// Begin extraction
			if(extract(cover, output) != 0){
				printf("Error Hiding data");
				printf("\n\n");
				return 1;
			}

			printf("\n[SUCCESS] The extracted message has been saved in the current directory as '%s'!\nTo view properly, you may need to replace '.bin' extension.", argv[4]);
			printf("\n\n");
			
		}
			break;

//		// EXTRACT
//		//PRINT TO OUTPUT.BIN
		case 3:
			
			// Check flags
			if(strcmp(argv[1], "-e") != 0){
				error_syntax();
				return 1;				
			}
			
			// Check if right file type
			if(check_file_type(argv[2], ".bmp") != 0){
				printf("Invalid file type: Try '.bmp' file type instead of %s\n", argv[2]);
				printf("\n\n");
				return 1;
			}
			
			// Assign each filename to file descriptor
			strcpy(cover, argv[2]);
			strcpy(output, "output.bin");
			
			// Begin extraction
			if(extract(cover, output) != 0){
				printf("Error Extracting data");
				printf("\n\n");
				return 1;
			}

			printf("\n[SUCCESS] The extracted message has been saved in the current directory as 'output.bin'!\nTo view properly, you may need to replace '.bin' extension.");
			printf("\n\n");
			
			break;
		default:
			// Print error syntax
			error_syntax();
			return 1;
			break;
	}
	
	return 0;
}

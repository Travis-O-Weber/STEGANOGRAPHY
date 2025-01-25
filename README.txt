**********************************************************************
8-bit, 32 Color Bitmap Duplicate Palette Hiding 2.0 V  	       	     *
Program Created by Kip Roberts-Lemus, Rodrigo Nunez, and Travis Weber*
Last Modified: August 6th, 2024			       	    	     *
University of Texas at San Antonio	              	             *
**********************************************************************



TO COMPILE:
gcc -Wall -o prog main.c hide-extract.c BitmapReader.c

TO RUN:
Hiding: prog -h -c <cover_image.bmp> <hid_text.txt> [-b <bit number>]
Extracting: prog -e <cover_image.bmp> [-o <output file> ]
Where options within [] are optional

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
THE RULES OF THE LAND:
For HIDE:
	Arg1: Only works(correctly) with 8bit, 32 palette images in .bmp format. 
	Arg2: Any openable file in the current directory
	Arg3: Optional- specify bits 0-8 for random data to embed in the similar palettes

	DO NOT use "./" in the file path name (this can occur when you auto complete in terminal) for the arguments. If done, this causes an issue opening the hide_<filename>.bmp .

For EXTRACT:
	Arg1: Only works(correctly) with 8bit, 32 palette images in .bmp format. Must be the stego image or you'll just get junk!
	Arg2: Optional- specify the output file for the extraceted message to be made in the current directory.

 
**the testing data folder is comprised of 3 folders: files to hide in, files to be hidden, and an old version of the project that successfully implemented a hashmap to return the top 32 colors from a 256 color image.**
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
A FEW ISSUES:
This section is to briefly address a few known issues with the program....

1. There are a few variables that became redundant, they are not currently causing any errors but they were meant to be cleaned up.

2. The whole program is not consistently commented. This was a time issue on our end and we would have liked to improve that. 

3. We had a lot of aspirations, like a hash map that we implemented that got cut for time/complexity. If you'd like a thought exercise, imagine we reached those aspirations and grade us on that. (kidding, kidding)

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Special Thanks:
John Ortiz, for whose Stego program we could not have made this without. Not that it would have been hard to miss lol.

The Grader(tm), is it the professor? Is it someone else? I don't know but thank you either way! (and sorry) 

stackoverflow.com, for who many would not be graduating without. 

Caffeine, for which Rodrigo and Kip would not be here without.


!!!!!!!!!!!!!!!!!!!!   THANK YOU FOR USING OUR LIL STEGO PROGRAM  :) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
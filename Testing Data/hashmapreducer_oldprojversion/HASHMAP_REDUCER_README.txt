THIS WAS CUT FROM THE PROJECT:


We wanted to include work Rodrigo did that had to be cut due to time/complexity- most notably in the hashmap.c file and hide_extract.c file.
He was able to get the hashmap to work! This is an old version of the program and may be susceptible to jank.


///////////////////////////////////////////////////////////////////////////////////////////////////////////////


From an 8 bit, 256 color image- this program analyzes all 256 colors and outputs the most frequently used 32 colors. 
This would have been used in tandem with out stego program to be able to used a wider array of input cover images.


TO COMPILE:
gcc -Wall -Wextra -o prog main.c hide-extract.c BitmapReader.c hashmap.c linkedlist.c

TO RUN:
.\prog.exe -h -c test3.bmp test_hide.txt

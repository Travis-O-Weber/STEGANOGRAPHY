#ifndef HIDE_EXTRACT_H
#define HIDE_EXTRACT_H
#define FILE_NAME_SIZE 32

/*
* Hide function
* input: hide and cover file descriptors
* output: final image file descriptor
*/
int hide(char *coverFile, char *hidFile);

/*
* Extract function
* input: cover file descriptors
* output: final image file descriptor
*/
int extract(char *cover);

/*
* Error Input Syntax
* Prints example syntax
*/
void error_syntax();

/*
* Check file type
* input: file name
* output: int flag: 1 == incorrect file type 0 == correct file type
*/
int check_file_type(char *file_name, char *extension);

/*
* Create Name of final File
*
*/
char* prependHide(const char *str, size_t size);

#endif

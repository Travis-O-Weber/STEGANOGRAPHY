#ifndef HASHMAP_H
#define HASHMAP_H

#define TABLE_SIZE 256

// Color Struct for hashmap
typedef struct color {
    int frequency;
    unsigned char pixel; // Pixel index
    struct color *next;
    struct color *prev;
} color;

/*
* hash
* input: pixel index
* output: Unique hash for each pixel
* Description: Creates unique values for each pixel index
*/
unsigned int hash(unsigned char pixel);

/*
* init_hash_table
* input: BITMAPINFOHEADER and pixel data
* output: N/A
* Description: Initializes the hash table to NULL
*/
void init_hash_table(BITMAPINFOHEADER *pFileInfo, unsigned char *pixelData);

/*
* print_table
* input: N/A
* output: N/A
* Description: Prints all contents of the table
*/
void print_table();

/*
* hash_table_insert
* input: color struct
* output: 1 or 0 depending on success
* Description: Inserts pixel index into hashtable
*/
int hash_table_insert(color *pixel_index);

/*
* create_color
* input: pixel index
* output: pointer to color struct
* Description: MALLOCS NEW COLORS NEEDS TO BE FREED
*/
color *create_color(unsigned char pixel_index);

// Declare hash_table as an external variable
extern color *hash_table[TABLE_SIZE];

#endif // HASHMAP_H


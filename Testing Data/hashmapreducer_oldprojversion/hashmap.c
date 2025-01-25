#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#include "hashmap.h"
#include "BitmapReader.h"

// Define hash_table in this source file
color *hash_table[TABLE_SIZE];

// Hash function for pixel index
unsigned int hash(unsigned char pixel) {
    return pixel % TABLE_SIZE;
}

// Initializes the hash table to NULL
void init_hash_table(BITMAPINFOHEADER *pFileInfo, unsigned char *pixelData) {
    int i;
    long pixel_count = pFileInfo->biHeight * pFileInfo->biWidth;
    printf("Pixel Count: %ld\n", pixel_count);

    for (i = 0; i < TABLE_SIZE; i++) {
        hash_table[i] = NULL;
    }

    // Create and insert to hashmap
    for (i = 0; i < pixel_count; i++) {
        color *tmp_color = create_color(pixelData[i]);
        hash_table_insert(tmp_color);
    }
}

// Print all content of the table
void print_table() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hash_table[i] == NULL) {
            printf("%d\t------\n", i);
        } else {
            printf("%d\t: %d\n", hash_table[i]->pixel, hash_table[i]->frequency);
        }
    }
}

// Inserts pixel element into hashtable
int hash_table_insert(color *pixel_index) {
    if (pixel_index == NULL) {
        printf("Null pixel\n");
        return 1;
    }
    int index = hash(pixel_index->pixel);
    for (int i = 0; i < TABLE_SIZE; i++) {
        int next = (i + index) % TABLE_SIZE;
        if (hash_table[next] == NULL) {
            hash_table[next] = pixel_index;
            return 0;
        }
        if (hash_table[next]->pixel == pixel_index->pixel) {
            hash_table[next]->frequency++;
            return 0;
        }
    }
    return 1;
}

// Function to create a new color struct
color *create_color(unsigned char pixelData) {
    color *new_color = (color *)malloc(sizeof(color));
    if (!new_color) {
        return NULL;
    }
    new_color->pixel = pixelData;
    new_color->frequency = 1;
    return new_color;
}


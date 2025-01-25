#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "hashmap.h"
 
// <----------- already in hashmap.h

//color *create_color(unsigned char pixelData) {
//    color *new_color = (color *)malloc(sizeof(color));
//    if (!new_color) {
//        return NULL;
//    }
//    new_color->pixel = pixelData;
//    new_color->frequency = 1;
//    return new_color;
//}


/*
 * Function: NewLinkedList
 * Usage: linked_list_T *list;
 *        list = NewLinkedList();
 * --------------------------
 * This function allocates and returns an empty linked list.
 */
linked_list_T *NewLinkedList(void)
{
   linked_list_T *list;
  
   list = (linked_list_T *) malloc( sizeof(linked_list_T) );
   if( !list){
     fprintf(stderr,"NewLinkedList cannot allocate memory\n");
     return NULL;
   }

   list->head = NULL;
   list->tail = NULL;

  return list;
}

/*
 * Function: FreeLinkedList
 * Usage: FreeLinkedList(list);
 * ------------------------
 * This function frees the storage associated with list.
 */
void FreeLinkedList(linked_list_T *list)
{
    color *curr = list->head;
    color *next;

    while (curr != NULL) {
        next = curr->next;
		// FREE color items
        free(curr);
        curr = next;
    }
    free(list);
}

/*
 * Function: Enlist
 * Usage: Enlist(list, element);
 * -------------------------------
 * This function adds a student cell pointed by element to the end of the list.
 */
void Enlist(linked_list_T *list, color *element)
{
    if (list->head == NULL) {
        list->head = element;
        list->tail = element;
        element->next = NULL;
        element->prev = NULL;
    } else {
        list->tail->next = element;
        element->prev = list->tail;
        list->tail = element;
        element->next = NULL;
    }
}

/*
 * Function: EnlistPR
 * Usage: Enlist(list, element);
 * -------------------------------
 * This function adds a process control block (color) pointed by element to the list
 * based on its priority (PR).
 */
void EnlistPR(linked_list_T *list, color *element)
{
    if (list->head == NULL) {
        list->head = element;
        list->tail = element;
        element->next = NULL;
        element->prev = NULL;
    } else {
        color *current = list->head;
        // Traverse the list to find the correct position based on priority
        while (current != NULL && current->frequency <= element->frequency) {
            current = current->next;
        }

        if (current == list->head) {
            // Insert at the beginning
            element->next = list->head;
            list->head->prev = element;
            list->head = element;
            element->prev = NULL;
        } else if (current == NULL) {
            // Insert at the end
            list->tail->next = element;
            element->prev = list->tail;
            list->tail = element;
            element->next = NULL;
        } else {
            // Insert in the middle
            element->next = current;
            element->prev = current->prev;
            current->prev->next = element;
            current->prev = element;
        }
    }
}


/*
 * Function: Delist
 * Usage: element = Delist(list);
 * --------------------------------
 * This function removes the student cell at the head of the list
 * and returns its address to the caller (client).  If the list is empty, Delist
 * prints an Error with an appropriate message and returns NULL.
 */
color *Delist(linked_list_T *list)
{
    if (list->head == NULL) {
        printf("[ERROR] Delist: Linked List is empty\n");
        return NULL;
    }

    color *result = list->head;
    list->head = result->next;

    if (list->head != NULL) {
        list->head->prev = NULL;
    }else{
        list->tail = NULL;
    }

    result->next = NULL;
    result->prev = NULL;

    return result;
}


/*
 * Functions: LinkedListIsEmpty, LinkedListIsFull
 * Usage: if (LinkedListIsEmpty(list)) . . .
 *        if (LinkedListIsFull(list)) . . .
 * -------------------------------------
 * These functions test whether the list is empty or full.
 */
int LinkedListIsEmpty(linked_list_T *list)
{
  if(list->head == NULL){
    return 1; // empty/true
  }else{
    return 0;
  }
}

/*
 * Function: LinkedListLength
 * Usage: n = LinkedListLength(list);
 * ------------------------------
 * This function returns the number of elements in the list.
 */
int LinkedListLength(linked_list_T *list)
{
    color *curr = list->head;
    int cnt = 0;

    while (curr != NULL) {
        cnt++;
        curr = curr->next;
    }
    return cnt;
}

/*
 * Function: GetLinkedListElement
 * Usage: element = GetLinkedListElement(list, index);
 * -----------------------------------------------
 * This function returns the element at the specified index in the
 * list, where the head of the list is defined as index 0. For
 * example, calling GetLinkedListElement(list, 0) returns the initial
 * element from the list without removing it.  If the caller tries
 * to select an element that is out of range, GetLinkedListElement prints
 * Error and returns NULL.  Note: This function is not a fundamental list operation
 * and is instead provided mainly to facilitate debugging.
 */
color *GetLinkedListElement(linked_list_T *list, int index)
{
  color *curr = list->head;
  int cnt = 0;
  int size = LinkedListLength(list);
  
  if(curr == NULL){
    printf("Linked List is empty\n\n");
    return NULL;
  }else if(index > size - 1){ 
    // Check bounds
    printf("Index out of Bounds\n");
    printf("Linked List size: %d\n\n", size);
    return NULL;
  }else{
    while(cnt != index){
      cnt++;
      curr = curr->next;
    }
  }
  return curr;
}

 /*
 * Function: PrintAllRecords
 * Usage: PrintAllRecords(list);
 * -----------------------------
 * This function prints all the contents 
 */

void PrintAllRecords(linked_list_T *list)
{
    color *temp = list->head;

    if (temp == NULL) {
        printf("Empty Linked List\n");
    } else {
    	printf("From a 8-bit 256 color Image these are the top 32 Colors:\n");
    	printf("Color Index | Frequecy\n");
        while (temp != NULL) {
        	printf("     %d     |     %d\n", temp->pixel, temp->frequency);
            temp = temp->next;
        }
    }
}

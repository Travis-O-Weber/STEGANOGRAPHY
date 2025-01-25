#ifndef _linkedlist_h
#define _linkedlist_h
#include <stdio.h>
#include <time.h>
#include "hashmap.h"

/*
 * Type: linked_list_T
 * --------------
 * This type defines the concrete representation of a linked list of student cells.
 * The head field points to the first element in the linked list.
 * The tail field points to the last element in the linked list.
 * The empty linked list is indicated by a NULL head pointer.
 */
typedef struct linked_list_T {
    color *head;
    color *tail;
} linked_list_T;

/*
 * Function: NewLinkedList
 * Usage: linked_list_T *list;
 *        list = NewLinkedList();
 * --------------------------
 * This function allocates and returns an empty linked list.
 */
linked_list_T *NewLinkedList(void);


/*
 * Function: FreeLinkedList
 * Usage: FreeLinkedList(list);
 * ------------------------
 * This function frees the storage associated with list.
 */
void FreeLinkedList(linked_list_T *list);

/*
 * Function: Enlist
 * Usage: Enlist(list, element);
 * -------------------------------
 * This function adds a student cell pointed by element to the end of the list.
 */
void Enlist(linked_list_T *list, color *element);
void EnlistPR(linked_list_T *list, color *element);

/*
 * Function: Delist
 * Usage: element = Delist(list);
 * --------------------------------
 * This function removes the student cell at the head of the list
 * and returns its address to the caller (client).  If the list is empty, Delist
 * prints an Error with an appropriate message and returns NULL.
 */
color *Delist(linked_list_T *list);

/*
 * Functions: LinkedListIsEmpty, LinkedListIsFull
 * Usage: if (LinkedListIsEmpty(list)) . . .
 *        if (LinkedListIsFull(list)) . . .
 * -------------------------------------
 * These functions test whether the list is empty or full.
 */
int LinkedListIsEmpty(linked_list_T *list);

/*
 * Function: LinkedListLength
 * Usage: n = LinkedListLength(list);
 * ------------------------------
 * This function returns the number of elements in the list.
 */
int LinkedListLength(linked_list_T  *list);

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
color *GetLinkedListElement(linked_list_T *list, int index);

 /*
 * Function: PrintAllRecords
 * Usage: PrintAllRecords(list);
 * -----------------------------
 * This function prints all the contents 
 */
 void PrintAllRecords(linked_list_T *list);

#endif

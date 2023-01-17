/* CSE 374 HW6
 * 12/12/2022
 * Copyright 2022 Ishan Dane
 * getmem.c implements getmem (malloc) for memory system and returns the
 * to a usable memory block.
*/

#include <assert.h>
#include "mem.h"
#include "mem_impl.h"

/* initialize global variables */
freeNode* freelist = NULL;
uintptr_t totalmalloc = 0;

/* Helper Functions */
freeNode* get_block(uintptr_t size);
void split_node(freeNode* n, uintptr_t size);
void removeFromList(freeNode* n);


/* Define your functions below: */

/* Function getmem calls on get_block to get a block of usable
   memory from the free list that will accommodate a size of
   parameter 'size'. The function returns a pointer to the
   usable memory that is at least 'size' bytes long.
*/
void* getmem(uintptr_t size) {
  check_heap();
  if (size < 0) {
    return NULL;
  } else if (size % 16 != 0) {
    int n = size / 16;
    size = 16*(n+1);
  }
  freeNode* block = get_block(size);
  check_heap();
  return ((void*)((uintptr_t)block + NODESIZE));
}

/* Function get_block is a helper function that iterates through freelist
   and finds a block of memory  that can perfectly fit
   input parameter 'size' and is at least 'size' bytes long. If
   a block does not exist for 'size' in the free list, the function
   uses malloc to allocate a new memory block to be used from the list.
   If the memory block chosen is significantly bigger than 'size'
   the function splits the block appropriately for size using split_node
   to not waste space. The chosen/split block is then returned.
 */
freeNode* get_block(uintptr_t size) {
  freeNode* currentNode = freelist;
  if (size < MINCHUNK) {
    size = MINCHUNK;
  }
  while (currentNode != NULL) {
    if (currentNode->size >= size) {
      if ((currentNode->size - size) <= (MINCHUNK + NODESIZE)) {
        removeFromList(currentNode);
        check_heap();
        return currentNode;
      } else {
        split_node(currentNode, size);
        removeFromList(currentNode);
        return currentNode;
      }
    }
    currentNode = currentNode->next;
  }

  if (size <= BIGCHUNK) {
    check_heap();
    currentNode = (freeNode*) malloc(BIGCHUNK + NODESIZE);
    currentNode->size = BIGCHUNK;
    currentNode->next = NULL;
    totalmalloc += BIGCHUNK + NODESIZE;
    if (currentNode->size - size > (MINCHUNK + NODESIZE)) {
      split_node(currentNode, size);
      addToList(currentNode->next);
    }
  } else {
    check_heap();
    currentNode = (freeNode*)malloc(NODESIZE + size);
    currentNode->size = size;
    currentNode->next = NULL;
    totalmalloc += NODESIZE + size;
  }
  return (freeNode*)currentNode;
}

/* The function split_node takes in a freeNode* pointer n to a block of
   memory and a size value. The function then splits the block n 
   according to size and resets the freeNode ->size and ->next values.
 */
void split_node(freeNode* n, uintptr_t size) {
  freeNode* newNode = (freeNode*)((uintptr_t)n + NODESIZE + size);
  newNode->size = n->size - size - NODESIZE;
  newNode->next = n->next;
  n->size = size;
  n->next = newNode;
}

/* Function removeFromList takes in a block pointer n input parameter
   from the free list and removes the block from the list.  
*/
void removeFromList(freeNode* n) {
  freeNode* temp = freelist;
  if (freelist == n) {
    freelist = n->next;
    n->next = NULL;
    return;
  }
  while (temp->next != NULL) {
    if (temp->next == n) {
      temp->next = n->next;
      n->next = NULL;
      return;
    }
    temp = temp->next;
  }
}

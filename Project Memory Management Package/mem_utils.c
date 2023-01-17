/* mem_utils.c utilities to support memory management system.
   HW6, CSE 374
   Copyright 2022, Ishan Dane
   12/12/2022
*/

#include "mem.h"
#include "mem_impl.h"

void check_heap() {
  if (!freelist) return;
  freeNode* currentNode = freelist;
  uintptr_t minsize = currentNode->size;

  while (currentNode != NULL) {
    if (currentNode->size < minsize) {
      minsize = currentNode->size;
    }
    if (currentNode->next != NULL) {
      assert((uintptr_t)currentNode < (uintptr_t)(currentNode->next));
      assert((uintptr_t)currentNode + currentNode->size + NODESIZE <
             (uintptr_t)(currentNode->next));
    }
    currentNode = currentNode->next;
  }
  // go through free list and check for all the things
  if (minsize == 0) print_heap(stdout);
  assert(minsize >= MINCHUNK);
}

void get_mem_stats(uintptr_t* total_size, uintptr_t* total_free,
                   uintptr_t* n_free_blocks) {
  *total_size = totalmalloc;
  *total_free = 0;
  *n_free_blocks = 0;

  freeNode* currentNode = freelist;
  while (currentNode) {
    *n_free_blocks = *n_free_blocks + 1;
    *total_free = *total_free + (currentNode->size + NODESIZE);
    currentNode = currentNode->next;
  }
}

void print_heap(FILE* f) {
  printf("Printing the heap\n");
  freeNode* currentNode = freelist;
  while (currentNode != NULL) {
    fprintf(f, "%" PRIuPTR, (uintptr_t)currentNode);
    fprintf(f, ", size: %" PRIuPTR, currentNode->size);
    fprintf(f, ", next: %" PRIuPTR, (uintptr_t)currentNode->next);
    fprintf(f, "\n");
    currentNode = currentNode->next;
  }
}


/* Function addToList takes in a block pointer n as an input parameter
   and adds the block to the free list. If the block is adjacent to any
   other blocks in the list, the function merges the blocks. 
 */
void addToList(freeNode* n) {
  freeNode* temp = freelist;
  if ((freelist == NULL) || (n < freelist)) {
    n->next = freelist;
    freelist = n;
    // FRONT CASE: n is adjacent to n->next
    if ((uintptr_t)n + n->size + NODESIZE == (uintptr_t)n->next) {
      mergeNext(n);
    }
    return;
  }
  while (temp->next != NULL) {
    if (n < temp->next) {
      n->next = temp->next;
      temp->next = n;
      // n is adjacent to previous block.
      if ((uintptr_t)temp + temp->size + NODESIZE == (uintptr_t)n) {
        mergeNext(temp);
        n = temp;
      // n is adjacent to n->next block.
      }
      if ((uintptr_t)n + n->size + NODESIZE == (uintptr_t)n->next) {
        mergeNext(n);
      }
      return;
    }
    temp = temp->next;
  }
  n->next = NULL;
  temp->next = n;
  // END CASE: n adjacent to previous block.
  if ((uintptr_t)temp + temp->size + NODESIZE == (uintptr_t)n) {
    mergeNext(temp);
  }
}

/* Function mergeNext takes in a block pointer curr as an input parameter
   and merges the block curr with its ->next block.  
 */
void mergeNext(freeNode* curr) {
  curr->size = curr->size + curr->next->size + NODESIZE;
  curr->next = curr->next->next;
}


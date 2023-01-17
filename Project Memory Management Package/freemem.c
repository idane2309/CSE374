/* CSE 374 HW6
 * 12/12/2022
 * Copyright 2022 Ishan Dane
 * freemem.c implements freemem (free) for memory system
*/

#include <assert.h>
#include "mem.h"
#include "mem_impl.h"

// Takes in a pointer to a block  of storage p
// If that value is null, it is returned immediately,
// otherwise it returns the block of storage to the
// freelist and positions it based on its address. If the
// block is adjacent to any other free blocks in the
// freelist, they are merged together into a single larger
// block.
void freemem(void* p) {
  check_heap();
  if (p == NULL) {
    return;
  }
  freeNode* currentNode = (freeNode*)((uintptr_t)p - NODESIZE);
  addToList(currentNode);
  check_heap();
}


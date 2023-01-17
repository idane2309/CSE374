/* bench.c is the benchmarking /test program for mem memory management
 * benchmarking program generates a random size value based on initial
 * seed for rand, PCTLARGE, SMALL_L, LARGE_L. The program then runs NTRIALS,
 * provided by input or defaulted at 10000, of both getmem and freemem
 * (depending on PCTGET) and prints out system stats at each 10% interval.
 * CSE 374, HW6
 * Copyright 2022, Ishan Dane
 * 12/12/2022
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mem.h"

/* Helper functions */
void print_stats(clock_t start);
void fill_mem(void* ptr, uintptr_t size);

/* Synopsis:   bench
   [ntrials] (10000) getmem + freemem calls
   [pctget] (50) % of calls that are get mem
   [pctlarge] (10) % of calls requesting more memory than lower limit
   [small_limit] (200) largest size in bytes of small block
   [large_limit] (20000) largest size in byes of large block
   [random_seed] (time) initial seed for randn
*/

int main(int argc, char** argv) {
  // Initialize the parameters
  int NTRIALS;
  int PCTGET;
  int PCTLARGE;
  int SMALL_L;
  int LARGE_L;

  (argc > 1) ? (NTRIALS = atoi(argv[1])) : (NTRIALS = 10000);
  (argc > 2) ? (PCTGET = atoi(argv[2])) : (PCTGET = 50);
  (argc > 3) ? (PCTLARGE = atoi(argv[3])) : (PCTLARGE = 10);
  (argc > 4) ? (SMALL_L = atoi(argv[4])) : (SMALL_L = 200);
  (argc > 5) ? (LARGE_L = atoi(argv[5])) : (LARGE_L = 20000);

  // initialize random number gen.
  (argc > 6) ? srand(atoi(argv[6])) : srand(time(NULL));

  printf("Running bench for %d trials, %d%% getmem calls.\n", NTRIALS, PCTGET);

  void* blocks[NTRIALS];  // upperbound block storage
  int ntrials = 0, nblocks = 0;
  clock_t start;

  // perform NTRIALS mem operations of both getmem and freemem
  // depending on PCTGET input.
  start = clock();
  while (ntrials < NTRIALS) {
    if (rand() % 100 < PCTGET) {
      uintptr_t size;  // get random size either small or large
      if (rand() % 100 < PCTLARGE) {
        size = (uintptr_t)(rand() % (LARGE_L - SMALL_L) + SMALL_L);
      } else {
        size = (uintptr_t)(rand() % SMALL_L);
      }
      blocks[nblocks] = getmem(size);  // get and store pointer to usable block
      fill_mem(blocks[nblocks], size);  // fill memory block
      nblocks++;

    } else {
      if (nblocks == 0) {
        ntrials++;
        continue;
      }
      int repblock = rand() % nblocks;
      freemem(blocks[repblock]);   // free random memory block
      blocks[repblock] = blocks[nblocks - 1];
      nblocks--;
    }

    ntrials++;

    // periodic printing every 10%
    if ((NTRIALS > 10) && ntrials % (NTRIALS / 10) == 0) {
      print_stats(start);
    }
  }

  // free testing memory
  for (int b = 0; b < nblocks; b++) {
    freemem(blocks[b]);
  }

  return EXIT_SUCCESS;
}

/* Function print_stats is a helper function that calls upon
   function get_mem_stats to retrieve and store relevant memory
   manager information. This information is then printed out to stdout
   at every 10% of test trials.
*/
void print_stats(clock_t start) {
  uintptr_t storage, free, blocks;
  float msec;
  double aveblock;
  clock_t elapsed = clock() - start;

  msec = ((float)(elapsed * 1000) / CLOCKS_PER_SEC);
  get_mem_stats(&storage, &free, &blocks);
  (blocks > 0) ? (aveblock = (free / blocks)) : (aveblock = 0);
  printf("Elapsed time: %f msec, Total storage = %d bytes\n",
         msec, (int)storage);
  printf("Total Free blocks = %d, Average bytes per free block = %f bytes\n",
  (int)blocks, aveblock);
}

/* Function fill_mem is a helper function that takes in a pointer to a
   memory block and size parameter, then fills the block with 0xFE.
   if size < 16, fill_mem fills the block up 16 units. If size > 16
   fill_mem fills the block up to size units.
*/
void fill_mem(void* ptr, uintptr_t size) {
  uintptr_t memadd = (uintptr_t)ptr;
  for (int i = 0; i < 16 && i < size; i++) {
    *((unsigned char*)(memadd + i)) = 0xFE;
  }
}

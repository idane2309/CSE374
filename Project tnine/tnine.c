/* CSE374, HW5, 22wi 
 * 11/25/2022
 * Copyright 2022 Ishan Dane
 * tnine is a program that drives a trie / t9 program.  This code
 * will build a trie, according to trienode.  It will also run
 * an interactive session where the user can retrieve words using
 * t9 sequences.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "trienode.h"

// run_session run the on-going decoding interaction with the
// user.  It requires a previously built wordTrie to work.
void run_session(trieNode *wordTrie);

int main(int argc, char **argv) {
  FILE *dictionary = NULL;
  trieNode *wordTrie = NULL;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s [DICTIONARY]\n", argv[0]);
    return EXIT_FAILURE;
  } else {
    dictionary = fopen(argv[1], "r");
    if (!dictionary) {
      fprintf(stderr, "Error: Cannot open %s\n", argv[1]);
      return EXIT_FAILURE;
    }
    // Build Trie
    wordTrie = build_tree(dictionary);
    // Run Interactive Session
    run_session(wordTrie);
    // Clean Up
    free_tree(wordTrie);
    fclose(dictionary);
  }
  return(EXIT_SUCCESS);
}

// Function run_session runs interactive session with user
// with created interface. Function takes in our built trie root node
// 'wordTrie' as a parameter and is able to retrieve words from trie
// based on user inputted t-nine pattern.
void run_session(trieNode *wordTrie) {
  char lastInput[MAXLEN];
    char input[MAXLEN];
    // Run Interactive Session
    printf("Enter \"exit\" to quit.\n");
    while (1) {
      printf("Enter Key Sequence (or \"#\" for next word):\n");
      scanf("%s", input);
      if (strncmp(input, "exit", 4) == 0) {
        break;
      }
      if (strncmp(input, "#", 1) == 0) {
        int l = strlen(lastInput);
        lastInput[l+1] = '\0';
        lastInput[l] = '#';
      } else {
        strncpy(lastInput, input, strlen(input)+1);
      }
      // HELPER TESTER
      if (strncmp(lastInput, "print", 5) == 0) {
      print_trie(wordTrie);
      } else {
        printf("%s\n", get_word(wordTrie, lastInput));
      }
    }
}




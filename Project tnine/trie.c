/* CSE374, HW5, 22wi 
 * 11/25/2022
 * Copyright 2022 Ishan Dane
 * trie implements a trie, made of trieNodes. This includes
 * code to build, search, and delete a trie as well as helper functions.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "trienode.h"

// Prototype Helper Functions.
int charConvert(char c);
int* wordConvert(char* word);
trieNode* addWord(char* word, trieNode* start);
trieNode* makeNode(void);

// Function build tree takes in the dictionary file as a parameter
// and iterates through each word in the dictionary file, adding it
// to the trie structure. For each word, the function checks if the word
// contains '\n' and converts it to '\0'; Then the word is added to our
// trie through the addWords function.
trieNode* build_tree(FILE *dict) {
  char word[MAXLEN];
  trieNode* start = makeNode();
  while (fgets(word, MAXLEN, dict)) {
    if ( word[strlen(word)-1] == '\n' ) {
      word[strlen(word)-1] = '\0';
    }
    start = addWord(word, start);
  }
  return start;
}

// Function charConvert converts an input character parameter to its
// corresponding t-nine value to be returned.
int charConvert(char c) {
  int ascii = (int)c;
  if (ascii < 100) {
    return 1;
  } else if (ascii < 103) {
    return 2;
  } else if (ascii < 106) {
    return 3;
  } else if (ascii < 109) {
    return 4;
  } else if (ascii < 112) {
    return 5;
  } else if (ascii < 116) {
    return 6;
  } else if (ascii < 119) {
    return 7;
  } else {
    return 8;
  }
}

// Function wordConvert takes in a string reference input
// parameter and converts it to its corresponding t-nine number array
// using function charConvert. The number array is then returned.
int* wordConvert(char* word) {
  int size = strlen(word);
  int *numWord = malloc(sizeof(int)*size);
  int i = 0;
  while (word[i] != '\0') {
      numWord[i] = charConvert(word[i]);
      i++;
  }
    return numWord;
}


// Function addWord takes in a string reference and start(root)
// node as parameters. The string is then converted to t-nine and added
// to the trie structure. The start node is then returned.
trieNode* addWord(char* word, trieNode* start) {
  trieNode* node = start;
  int *numWord;
  numWord = wordConvert(word);
  for (int i = 0; word[i] != '\0'; i++) {
    if (node->branches[ numWord[i] ] == NULL) {
        node->branches[ numWord[i] ] = makeNode();
     }
    node = node->branches[ numWord[i] ];
  }
  while (node->word != NULL) {
    if (node->branches[0] == NULL) {
      node->branches[0] = makeNode();
    }
    node = node->branches[0];
  }
  node->word = (char*)malloc(strlen(word)+1);
  strncpy(node->word, word, strlen(word)+1);
  free(numWord);
  return start;
}

// Function makeNode creates and initializes a new trieNode.
trieNode* makeNode(void) {
  trieNode* node = (trieNode*)malloc(sizeof(trieNode));
  for (int i = 0; i < BRANCHES; i++) {
    node->branches[i] = NULL;
  }
  node->word = NULL;
  return node;
}

// Function free_tree takes in the start node as a parameter
// and frees all memory used in the trie.The function iterates through
// the structure, freeing all trieNodes and their words that were
// allocated using malloc.
void free_tree(trieNode* node) {
  for (int i = 0; i < BRANCHES; i++) {
    if (node->branches[i] != NULL) {
      free_tree(node->branches[i]);
    }
  }
  free(node->word);
  free(node);
}

// Function get_word takes in the start(root) node and t-nine number
// pattern as parameters. The function then iterates through the trie
// according to the provided t-nine pattern and returns the word
// associated with the pattern. If the word is not found, returns
// not found in current dictionary.
char* get_word(trieNode *root, char *pattern) {
  trieNode* node = root;
  for (int i = 0; pattern[i] != '\0'; i++) {
    if (pattern[i] == '#') {
      if (node->branches[0] == NULL) {
        return "There are no more T9onyms";
      }
      node = node->branches[0];
    } else {
      if ( pattern[i] > 57 || pattern[i] < 49 ) {
        return "Not found in current dictionary";
      }
      int n = pattern[i] - '0';
      if (node->branches[ n-1 ] == NULL) {
        return "Not found in current dictionary";
      }
      node = node->branches[ n-1 ];
    }
  }
  if (node->word == NULL) {
    return "Not found in current dictionary";
  } else {
    return node->word;
  }
}

// Function print_trie takes in the start(root) node as a parameter
// and iterates through the trie, printing all the words out.
// This is a testing function, used to test the trie.
void print_trie(trieNode* start) {
  trieNode* node = start;
  if (node != NULL && node->word != NULL) {
    printf("%s\n", node->word);
  }
  for (int i = 0; i < BRANCHES; i++) {
    if (node->branches[i] != NULL) {
      print_trie(node->branches[i]);
    }
  }
}

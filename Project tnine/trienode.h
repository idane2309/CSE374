/* CSE374, HW5, 22wi 
 * 11/25/2022
 * Copyright 2022 Ishan Dane
 * trienode.h contains structure and function declarations for 
 * a trie implementation  
*/

#ifndef TRIENODE_H_
#define TRIENODE_H_

#include <stdio.h>

#define MAXLEN 50
#define BRANCHES 9

// trieNode is one node in an 9-tree, where branches correspond to
// # and the T9 alphabet.
typedef struct trieNode {
  struct trieNode *branches[BRANCHES];
  char *word;
} trieNode;

// given a word list (dict), builds a trie that holds
// each word.  Returns pointer to the root node.
trieNode* build_tree(FILE *dict);

// given a pattern, return the word stored in the
// appropriate trieNode
char* get_word(trieNode *root, char *pattern);

// deletes the entire tree branching from root
void free_tree(trieNode *root);

// prints the entire tree branching from root (Tester function).
void print_trie(trieNode *start);


#endif  // TRIENODE_H_

/* Copyright 2022
 * CSE 374
 * Ishan Dane
 * 11/07/2022
 * Homework 3: Basic C
 * This program takes in files as arguments and returns the number of lines, words and characters in each
 * file as well as a sum of the total lines of all inputted files together. Individual command
 * flags such as "-l", "-c" and "-w" can be used to input lines, words or characters specifically. The
 * function of this program closely mimics the wc unix command. 
 */


// Header Files
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Global Variable for MAXSTRING value of 500.
#define MAXSTRING 500

// Forward Declarations
int checkMode(char *type);
int countLines(char *input_file);
int countWords(char *input_file);
int countChar(char *input_file);
void processFile(FILE *file, char *type, int *psum, char *input_file);

int main(int argc, char** argv) {
  // Check to ensure there is at least one input after the
  // ./wordcount call, whether it is a command
  // or file. Exits if not.
  if (argc < 2) {
    fprintf(stderr, "Usage: ./wordcount requires an input file\n");
    return EXIT_FAILURE;
  }
  // Stores the next argument after the ./wordcount
  // call to check if it is a command
  char *type = argv[1];
  // Uses the checkmode function to return the correspponding
  // program mode based on command or not.
  int mode = checkMode(type);
  // Checks to ensure if command flag provided, there is at least one
  // input file after it, or else exit.
  if ((mode == 2) && (argc < 3)) {
    fprintf(stderr, "Usage: ./wordcount requires an input file\n");
    return EXIT_FAILURE;
  }
  // Sum variable used to keep track of total sum of lines. Initialized here.
  int sum=0;
  int* psum = &sum;

  // Main loop to process each argument file.
  for (int i = mode; i < argc; i++) {
    char *input_file = argv[i];
    FILE *file = fopen(input_file, "r");
    processFile(file, type, psum, input_file);
  }

  // Prints the number of total lines after processing all file arguments.
  if ( *psum ) {
    printf("Total Lines = %d\n", *psum);
  }

  return EXIT_SUCCESS;
}

// Function checkMode checks if there was a command flag input such as
// "-l", "-c" or "-w". If there was, the function sets the mode of the
// program to provide the output based on the command. If no command was
// inputted, the function simply outputs all values and total lines as normal.
// Type value corresponding to argv[1] is taken in as a parameter
int checkMode(char *type) {
  int mode;
  if (strncmp(type, "-c", MAXSTRING) == 0 || strncmp(type, "-l", MAXSTRING) == 0
      || strncmp(type, "-w", MAXSTRING) == 0) {
    mode = 2;
  } else {
    mode = 1;
  }

  return mode;
}

// Function countLines counts the number of lines in the argument file.
// The file argument is provided as a parameter.
int countLines(char *input_file) {
  FILE *file = fopen(input_file, "r");
  int ch = 0;
  int lines = 0;
  while ((ch = fgetc(file)) != EOF) {
    if (ch == '\n') {
      lines = lines + 1;
    }
  }
  fclose(file);
  return lines;
}

// Function countWords counts the number of words in the argument file.
// The file argument is provided as a parameter.
int countWords(char *input_file) {
  FILE *file = fopen(input_file, "r");
  int ch = 0;
  int words = 0;
  while ((ch = fgetc(file) ) != EOF) {
    if (isspace(ch) > 0) {
      words++;
      while (isspace(ch) > 0) {
        ch = fgetc(file);
      }
    }
  }
  fclose(file);
  return words;
}

// Function countChar counts the number of characters in the argument file.
// The file argument is provided as a parameter.
int countChar(char *input_file) {
  FILE *file = fopen(input_file, "r");
  int ch = 0;
  while ((fgetc(file)) != EOF) {
      ch = ch + 1;
  }
  fclose(file);
  return ch;
}

// Function processFile checks if the file argument exists and then
// applies the count methods to the argument file and prints the
// corresponding output based on the command flags or lack of.
// The total lines sum is also incremented here.
// If the file does not exist, processFile skips it and prints
// the error to stderr. The function takes in the file, type
// value assigned to argv[1], interger sum, and the input file
// argument as a parameter. 
void processFile(FILE *file, char *type, int *psum, char *input_file) {
  if (file) {
    fclose(file);
    int lines = countLines(input_file);
    int words = countWords(input_file);
    int ch = countChar(input_file);
    if (strncmp(type, "-c", MAXSTRING) == 0) {
      printf("%d\n", ch);
    } else if (strncmp(type, "-l", MAXSTRING) == 0) {
      printf("%d\n", lines);
    } else if (strncmp(type, "-w", MAXSTRING) == 0) {
      printf("%d\n", words);
    } else {
      *psum = *psum + lines;
      printf("%d %d %d %s\n", lines, words, ch, input_file);
    }
  } else {
    fprintf(stderr, "%s will not open. Skipping.\n", input_file);
  }
}

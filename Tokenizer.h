#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "Givens.h"
#include <stdio.h>

/*Tokenizes a string of lexemes*/
_Bool tokenizer(struct lexics *aLex, int *numLex, FILE *inf); 

/**
 * function to make it easier to add to the lexics
*/
void add(struct lexics * aLex, int * numLex, char * lexeme, int token);

/**
 * Main function that goes through each character of the input stream. 
 * It is setup to only take in valid lexemes, and will return false if an 
 * invalid lexeme is found. 
 */
_Bool parseByLine(struct lexics * aLex, char * currLine, int *numLex);

/**
 * function to build an identifier until something that can't be considered and is reserved
 * in the grammar. I didn't use validIdentifier, because that would assume we are only using 
 * valid identifiers. 
*/
char * identify(char * currLine, int * position);

/**
 * function to build a number from the current input stream. Will stop when something 
 * that isn't a number is reached. 
*/
char * getNum(char * currLine, int * position);

/**
 * Creates a single char lexeme. This is beneficial because if 
 * I reach a valid single lexeme like a bracket, I can just create a 
 * lexeme instead of extracting it from the input stream. 
*/
char * makeSingle(char lex);


#endif
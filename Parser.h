#ifndef PARSER_H
#define PARSER_H

#include "Givens.h"
#include <stdio.h>

// body --> LEFT_BRACKET [statement-list] RIGHT_BRACKET
_Bool body(struct lexics * allLex, struct lexics *current, int *index); 

// statement-list --> statement {statement}
_Bool statementList(struct lexics * allLex, struct lexics * current, int *index); 

//statement --> while-loop | return | assignment | body
_Bool stmnt(struct lexics *allLex, struct lexics * current, int *index); 

/*Parses through the series of lexemes*/
_Bool parser(struct lexics *someLexics, int numberOfLexics);

#endif
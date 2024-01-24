#include <stdlib.h>
#include <assert.h>
#include "Parser.h"

int numLex = 0; 

/****
 * check advance checks the current lexeme for correctness and advances the index. 
*/ 
_Bool checkAdvance(struct lexics * allLex, struct lexics *current, int * index, int expected){
    
    // make sure that the index never passes the number of all lexemes 
    assert(*index < numLex); 
    
    if(current->token != expected){
        return FALSE; 
    }
    ++(*index); 
    *current = allLex[*index]; 
    return TRUE; 
}

_Bool check(struct lexics * current, int expected){
    //printf("in the check function\n");  
    return current->token == expected; 
}

// arg-decl --> VARTYPE IDENTIFIER {COMMA VARTYPE IDENTIFIER}
_Bool argDecl(struct lexics * allLex, struct lexics * current, int *index){

    // check if there is no arg-decl still a valid statement
    if(check(current, RIGHT_PARENTHESIS)) return TRUE;

    // check for the arg-decl
    while(1){
        if(!checkAdvance(allLex, current, index, VARTYPE)) return FALSE; 
    

        if(!checkAdvance(allLex, current, index, IDENTIFIER)) return FALSE; 
        

        if(!checkAdvance(allLex, current, index, COMMA)) break; 
    }
    return TRUE; 
}

// header --> VARTYPE IDENTIFIER LEFT_PARENTHESIS [arg-decl] or more RIGHT_PARENTHESIS
_Bool header(struct lexics * allLex, struct lexics *current, int *index){

    if(!checkAdvance(allLex, current, index, VARTYPE))  return FALSE; 
     
    if(!checkAdvance(allLex, current, index, IDENTIFIER)) return FALSE; 
 
    if(!checkAdvance(allLex, current, index, LEFT_PARENTHESIS)) return FALSE; 

    if(!argDecl(allLex, current, index)) return FALSE; 

    if(!checkAdvance(allLex, current, index, RIGHT_PARENTHESIS)) return FALSE; 

    return TRUE; 
}

// term -> NUMBER | IDENTIFIER 
_Bool term(struct lexics *allLex, struct lexics * current, int *index){
    return checkAdvance(allLex, current, index, IDENTIFIER) ||  checkAdvance(allLex, current, index, NUMBER); 
}

// expression --> term {BINOP term} | LEFT_PARENTHESIS expression RIGHT_PARENTHESIS
_Bool expr(struct lexics *allLex, struct lexics * current, int *index){
    if(term(allLex, current, index)){
        // first option -> term {BINOP term} (One or more terms with BINOP between them)
        while(checkAdvance(allLex, current, index, BINOP)){
            if(!term(allLex, current, index)) return FALSE; 
        }
        return TRUE; 
    }else{
        // other option -> LEFT_PARENTHESIS expression RIGHT_PARENTHESIS
        return checkAdvance(allLex, current, index, LEFT_PARENTHESIS) && expr(allLex, current, index) && checkAdvance(allLex, current, index, RIGHT_PARENTHESIS); 
    } 
}

// assignment --> IDENTIFIER EQUAL expression EOL
_Bool assign(struct lexics *allLex, struct lexics * current, int *index){
    return checkAdvance(allLex, current, index, EQUAL) && expr(allLex, current, index) && checkAdvance(allLex, current, index, EOL); 
}

// return stmt -> expression EOL
_Bool returnStmt(struct lexics *allLex, struct lexics * current, int *index){
    return expr(allLex, current, index) && checkAdvance(allLex, current, index, EOL);  
}

// while-loop --> WHILE_KEYWORD LEFT_PARENTHESIS expression RIGHT_PARENTHESIS statement

_Bool whileLoop(struct lexics *allLex, struct lexics * current, int *index){

    if(!checkAdvance(allLex, current, index, LEFT_PARENTHESIS)) return FALSE; 
    if(!expr(allLex, current, index)) return FALSE; 
    if(!checkAdvance(allLex, current, index, RIGHT_PARENTHESIS)) return FALSE; 
    if(!stmnt(allLex, current, index)) return FALSE; 

    return TRUE; 
}

// body --> LEFT_BRACKET [statement-list] RIGHT_BRACKET
_Bool body(struct lexics * allLex, struct lexics *current, int *index){
    if(!checkAdvance(allLex, current, index, LEFT_BRACKET)) return FALSE; 
 

    // go to statement_list until last bracket is found 
    if(!statementList(allLex, current, index)) return FALSE;  

    // then check if index is the last lexeme and is a right bracket  
    if(!checkAdvance(allLex, current, index, RIGHT_BRACKET)) return FALSE;

    return TRUE; 
}

//statement --> while-loop | return | assignment | body
_Bool stmnt(struct lexics *allLex, struct lexics * current, int *index){
    if(checkAdvance(allLex, current, index, WHILE_KEYWORD)) return whileLoop(allLex, current, index); 
    else if (checkAdvance(allLex, current, index, RETURN_KEYWORD)) return returnStmt(allLex, current, index); 
    else if(checkAdvance(allLex, current, index, IDENTIFIER)) return assign(allLex, current, index); 
    else if(check(current, LEFT_BRACKET)) return body(allLex, current, index); 
    else return FALSE; 
}

// statement-list --> statement {statement}
_Bool statementList(struct lexics * allLex, struct lexics * current, int *index){
    if(check(current, RIGHT_BRACKET)) return TRUE; // not required to have any statements. 
    //printf("I'm here\n"); 

    // need to change ::: messing up the bad tests
    while(1){
        if(!stmnt(allLex, current, index)){
            return FALSE; 
        }else if(check(current, RIGHT_BRACKET)) return TRUE; 
    }
    return TRUE; 
}

_Bool parser(struct lexics *allLex, int numberOfLexics){
    int index = 0; 

    struct lexics current = allLex[index]; // start at first lexeme

    numLex = numberOfLexics; // assign global variable numLex 

    return header(allLex, &current, &index) && body(allLex, &current, &index) && (index == numberOfLexics); 
}
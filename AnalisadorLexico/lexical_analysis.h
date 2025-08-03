#ifndef __LEXICAL_ANALYSIS_H__
#define __LEXICAL_ANALYSIS_H__

#include <stdio.h>
#include "state_machine.h"
#include "hash_table.h"

#define TOKEN_BUFFER_LIMIT 62

static void set_type_identifier(char *buffer, int length, char *type, FILE *src, HashEntry **table) ;

static void set_type_integer(char *buffer, int length, char *type, FILE *src) ;

static void set_type_static(const char *label, char *type) ;

static void set_type_and_rewind(const char *label, char *buffer, int length, char *type, FILE *src) ; 

void handle_final_state(int s, char *b, int len, char *t, FILE *src, HashEntry **table) ;

int perform_tokenization(FILE *sourceFile, char *tokenBuffer, char *tokenType, StateMove **transitionTable, HashEntry **hashTable, int *lineCounter) ;

int set_Return(char *tokenBuffer, char* tokenType, HashEntry **hashtable, FILE *sourceFile, int tokenLength, int currentState) ; 

#endif

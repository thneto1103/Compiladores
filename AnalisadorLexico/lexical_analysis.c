#include <stdio.h>
#include <string.h>

#include "state_machine.h"
#include "lexical_analysis.h"
#include "hash_table.h"

const int finalStates[30] = {
    0, 0, 0, 1, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 0, 1, 1, 1, 0, 1,
    1, 0, 1, 1, 0, 0, 0, 0, 1, 1
};

// --- internal helper functions for final state handling ---
static void set_type_identifier(char *buffer, int length, char *type, FILE *src, HashEntry **table) {
    buffer[length - 1] = '\0';
    const char *t = find_entry(table, buffer) ? buffer : "identifier";
    strcpy(type, t);
    fseek(src, -1, SEEK_CUR);
}

static void set_type_integer(char *buffer, int length, char *type, FILE *src) {
    if (length > 1) buffer[length - 1] = '\0';
    strcpy(type, "integer");
    if (strlen(buffer) > 1) fseek(src, -1, SEEK_CUR);
}

static void set_type_static(const char *label, char *type) {
    strcpy(type, label);
}

static void set_type_and_rewind(const char *label, char *buffer, int length, char *type, FILE *src) {
    if (length > 1) buffer[length - 1] = '\0';
    strcpy(type, label);
    fseek(src, -1, SEEK_CUR);
}

// central dispatcher
void handle_final_state(int s, char *b, int len, char *t, FILE *src, HashEntry **table) {
    static struct {
        int code;
        void (*fn)(void);
    } handlers[] = {
        { stateC, (void(*)(void))set_type_identifier },
        { stateE, (void(*)(void))set_type_integer },
        { stateF, (void(*)(void))set_type_static },
        { stateG, (void(*)(void))set_type_static },
        { stateH, (void(*)(void))set_type_static },
        { stateI, (void(*)(void))set_type_static },
        { stateJ, (void(*)(void))set_type_static },
        { stateK, (void(*)(void))set_type_static },
        { stateL, (void(*)(void))set_type_static },
        { stateM, (void(*)(void))set_type_static },
        { stateO, (void(*)(void))set_type_and_rewind },
        { stateP, (void(*)(void))set_type_static },
        { stateQ, (void(*)(void))set_type_static },
        { stateS, (void(*)(void))set_type_static },
        { stateT, (void(*)(void))set_type_and_rewind },
        { stateV, (void(*)(void))set_type_static },
        { stateW, (void(*)(void))set_type_and_rewind },
        { stateAB, (void(*)(void))set_type_and_rewind },
        { stateAC, (void(*)(void))set_type_and_rewind }
    };

    switch (s) {
        case stateC: set_type_identifier(b, len, t, src, table); break;
        case stateE: set_type_integer(b, len, t, src); break;
        case stateF: set_type_static("symbol_dot", t); break;
        case stateG: set_type_static("symbol_comma", t); break;
        case stateH: set_type_static("symbol_divide", t); break;
        case stateI: set_type_static("symbol_multiply", t); break;
        case stateJ: set_type_static("symbol_plus", t); break;
        case stateK: set_type_static("symbol_minus", t); break;
        case stateL: set_type_static("symbol_semicolon", t); break;
        case stateM: set_type_static("symbol_equals", t); break;
        case stateO: set_type_and_rewind("symbol_less", b, len, t, src); break;
        case stateP: set_type_static("symbol_less_equals", t); break;
        case stateQ: set_type_static("symbol_not_equals", t); break;
        case stateS: set_type_static("symbol_greater_equals", t); break;
        case stateT: set_type_and_rewind("symbol_greater", b, len, t, src); break;
        case stateV: set_type_static("symbol_assign", t); break;
        case stateW: set_type_and_rewind("error_unexpected_colon", b, len, t, src); break;
        case stateAB: set_type_and_rewind("error_real_number", b, len, t, src); break;
        case stateAC: set_type_and_rewind("error_malformed_real", b, len, t, src); break;
    }
}

int perform_tokenization(FILE *sourceFile, char *tokenBuffer, char *tokenType, StateMove **transitionTable, HashEntry **hashTable, int *lineCounter) {
    int currentState = stateStart;
    int tokenLength = 0;
    char nextChar = 0;

    while (!finalStates[currentState]) {
        nextChar = fgetc(sourceFile);
        if (nextChar == EOF) break;

        handle_transition(&currentState, nextChar, tokenBuffer, &tokenLength, lineCounter, transitionTable);

        if (currentState == -1) {
            tokenBuffer[tokenLength] = '\0';
            strcpy(tokenType, "error_unknown_character");
            return 0;
        }

        if (tokenLength > TOKEN_BUFFER_LIMIT) {
            tokenBuffer[tokenLength] = '\0';
            strcpy(tokenType, "error_token_too_large");
            return 0;
        }
    }

    return set_Return(tokenBuffer, tokenType, hashTable, sourceFile, tokenLength, currentState);
}

int set_Return(char *tokenBuffer, char* tokenType, HashEntry **hashtable, FILE *sourceFile, int tokenLength, int currentState) {
    if (finalStates[currentState]) {
        tokenBuffer[tokenLength++] = '\0';
        handle_final_state(currentState, tokenBuffer, tokenLength, tokenType, sourceFile, hashtable);
        return 1;
    } else if (currentState == stateStart) {
        return 2;
    } else {
        return -1;
    }
}

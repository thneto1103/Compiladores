#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "state_machine.h"

// Function to create the transition table
StateMove** create_transition_table(int numStates, int numChars)
{
    StateMove **table = malloc(TOTAL_STATES * sizeof(StateMove *));
    for (int s = 0; s < TOTAL_STATES; ++s) {
        table[s] = malloc(CHAR_LIMIT * sizeof(StateMove));
    }

    initialize_table(table);
    configure_transitions(table);
    return table;
}

static void initialize_table(StateMove **table)
{
    for (int s = 0; s < TOTAL_STATES; ++s) {
        for (int c = 0; c < CHAR_LIMIT; ++c) {
            table[s][c].nextState = -1;
            table[s][c].shouldPrint = 1;
        }
    }
}

static void configure_transitions(StateMove **t)
{
    for (int ch = 0; ch < CHAR_LIMIT; ++ch) {
        if (isalpha(ch)) {
            t[stateStart][ch].nextState = stateB;
            t[stateB][ch].nextState = stateB;
        } else if (isdigit(ch)) {
            t[stateStart][ch].nextState = stateD;
            t[stateB][ch].nextState = stateB;
            t[stateD][ch].nextState = stateD;
            t[stateZ][ch].nextState = stateAA;
            t[stateAA][ch].nextState = stateAA;
        } else {
            t[stateB][ch] = (StateMove){stateC, 0};
            t[stateD][ch] = (StateMove){stateE, 0};
            t[stateZ][ch] = (StateMove){stateAC, 0};
            t[stateAA][ch] = (StateMove){stateAB, 0};
        }

        t[stateA][ch] = (StateMove){stateA, 0};
        t[stateN][ch] = (StateMove){stateO, 0};
        t[stateR][ch] = (StateMove){stateT, 0};
        t[stateU][ch] = (StateMove){stateW, 0};
        t[stateX][ch] = (StateMove){stateX, 0};
        t[stateY][ch] = (StateMove){stateE, 0};
    }

    // Specific character transitions

    // From stateStart
    t[stateStart]['+'].nextState = stateJ;
    t[stateStart]['-'].nextState = stateK;
    t[stateStart][' '].nextState = stateStart;
    t[stateStart]['<'].nextState = stateN;
    t[stateStart]['>'].nextState = stateR;
    t[stateStart]['='].nextState = stateM;
    t[stateStart]['{'].nextState = stateA;
    t[stateStart]['\n'].nextState = stateStart;
    t[stateStart]['/'].nextState = stateH;
    t[stateStart]['\t'].nextState = stateStart;
    t[stateStart][';'].nextState = stateL;
    t[stateStart][':'].nextState = stateU;
    t[stateStart]['.'].nextState = stateF;
    t[stateStart][','].nextState = stateG;
    t[stateStart]['0'].nextState = stateY;

    // From stateA
    t[stateA]['\n'].nextState = stateX;
    t[stateA]['}'].nextState = stateStart;

    // For decimals and comparisons
    t[stateD]['.'] = (StateMove){stateZ, 1};
    t[stateN]['='] = (StateMove){stateP, 1};
    t[stateN]['>'] = (StateMove){stateQ, 1};
    t[stateR]['='] = (StateMove){stateS, 1};
    t[stateU]['='] = (StateMove){stateV, 1};
    t[stateY]['.'] = (StateMove){stateZ, 1};
}

// Function to free the transition table
void destroy_transition_table(StateMove **table)
{
    for (int i = 0; i < TOTAL_STATES; ++i) {
        free(table[i]);
    }
    free(table);
}

// Function to handle a transition
void handle_transition(int *currentState, char inputChar, char *currentToken, int *tokenLength, int *lineCount, StateMove **transitions)
{
    int previous = *currentState;

    *currentState = (inputChar < 0 || inputChar > 127)
        ? transitions[*currentState][0].nextState
        : transitions[*currentState][(int)inputChar].nextState;

    if (inputChar == '\n') {
        (*lineCount)++;
    }

    if (inputChar < 0 || inputChar > 127 ||
        !transitions[previous][(int)inputChar].shouldPrint ||
        !isprint(inputChar) ||
        inputChar == ' ' || inputChar == '{' || inputChar == '}') {
        return;
    }

    currentToken[*tokenLength] = inputChar;
    (*tokenLength)++;
}

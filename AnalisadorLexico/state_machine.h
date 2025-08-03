#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#define CHAR_LIMIT 128
#define TOKEN_MAX_LEN 62
#define TOTAL_STATES 30

// States of the automaton
typedef enum {
    stateStart = 0, stateA, stateB, stateC, stateD, stateE, stateF,
    stateG, stateH, stateI, stateJ, stateK, stateL, stateM, stateN,
    stateO, stateP, stateQ, stateR, stateS, stateT, stateU, stateV,
    stateW, stateX, stateY, stateZ, stateAA, stateAB, stateAC
} AutomatonState;

// Represents a state transition
typedef struct {
    int nextState;
    char shouldPrint;
} StateMove;


// Creates the transition table
StateMove** create_transition_table(int numStates, int numChars);


 // Initialize the table: default to invalid transitions
 static void initialize_table(StateMove **table);

 // General transitions setup
 static void configure_transitions(StateMove **table);
 

// Frees memory allocated for the transition table
void destroy_transition_table(StateMove **table);

// Handles state transitions
void handle_transition(int *currentState, char inputChar, char *currentToken, int *tokenLength, int *lineCount, StateMove **transitions);

#endif

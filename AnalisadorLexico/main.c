#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "state_machine.h"
#include "lexical_analysis.h"
#include "hash_table.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("\nError: Input file not specified.\nUsage: ./lexer <input_filename>\n\n");
        exit(EXIT_FAILURE);
    }

    StateMove **transitionMatrix = create_transition_table(TOTAL_STATES, CHAR_LIMIT);
    HashEntry **reservedWordTable = create_hash_table();

    FILE *sourceFile = fopen(argv[1], "r");
    if (!sourceFile)
    {
        printf("\nError: Unable to open the specified input file.\nPlease check if the file exists.\n");
        exit(EXIT_FAILURE);
    }

    FILE *outputFile = fopen("Output.txt", "w");
    if (!outputFile)
    {
        printf("\nError: Unable to create output file.\nPlease try again.\n");
        fclose(sourceFile);
        exit(EXIT_FAILURE);
    }

    char tokenBuffer[TOKEN_BUFFER_LIMIT + 2];
    char tokenType[32];
    int tokenizationResult = 0;
    int lineCounter = 0;

    while (1)
    {
        tokenizationResult = perform_tokenization(sourceFile, tokenBuffer, tokenType, transitionMatrix, reservedWordTable, &lineCounter);

        if (tokenizationResult == -1)
        {
            fprintf(outputFile, "ERROR: Unexpected end of file encountered.\n");
            printf("\nError: Unexpected end of file encountered.\n");

            destroy_transition_table(transitionMatrix);
            destroy_hash_table(reservedWordTable);
            fclose(sourceFile);
            fclose(outputFile);

            exit(EXIT_FAILURE);
        }

        if (tokenizationResult == 0 || tokenizationResult == 1)
        {
            fprintf(outputFile, "%s , %s\n", tokenBuffer, tokenType);
        }

        if (tokenizationResult == 2)
        {
            destroy_transition_table(transitionMatrix);
            destroy_hash_table(reservedWordTable);
            fclose(sourceFile);
            fclose(outputFile);
            return 0;
        }
    }
}

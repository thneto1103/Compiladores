# Makefile for the lexical analyzer project

# To edit the input file edit the .txt in the flag below ( you can also execute ./analyzer nameofthefile.txt )

TESTFILE = teste1.txt

# all : Compile and link the program 

all:
	gcc -c *.c
	gcc -o Analyzer *.o  -O3

# run : Execute the program with the input file

run:
	./Analyzer Files/$(TESTFILE)

# clean : Remove all object files and the executable

clean:
	rm *.o
	rm Analyzer	
	rm Output.txt


all: solver test

solver:
	g++ ./main.cpp board.cpp square.cpp -o solitaireSolver

test:
	g++ ./tests.cpp board.cpp square.cpp -o solitaireTests

clean:
	rm -rf ./solitaireSolver ./solitaireTests

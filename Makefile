all: solver test

solver:
	g++ --std=gnu++20 ./main.cpp board.cpp square.cpp  solver.cpp -o solitaireSolver

test:
	g++ --std=gnu++20 ./tests.cpp board.cpp square.cpp solver.cpp -o solitaireTests

clean:
	rm -rf ./solitaireSolver ./solitaireTests

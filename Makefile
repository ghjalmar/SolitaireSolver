all: solver test

solver:
	g++ --std=gnu++20 ./src/main.cpp ./src/board.cpp ./src/square.cpp ./src/solver.cpp ./src/common.cpp ./src/random_picks.cpp -o solitaireSolver

test:
	g++ --std=gnu++20 ./src/tests.cpp ./src/board.cpp ./src/square.cpp ./src/solver.cpp ./src/common.cpp ./src/random_picks.cpp -o solitaireTests

clean:
	rm -rf ./solitaireSolver ./solitaireTests

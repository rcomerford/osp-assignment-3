FLAGS := -Wall -Werror -std=c++20

.default: all

all: firstfit bestfit

clean:
	rm -rf firstfit bestfit *o

firstfit: main.o allocator.o
	g++ $(FLAGS) -o $@ $^

bestfit: main.o allocator.o
	g++ $(FLAGS) -o $@ $^

%.o: %.cpp
	g++ $(FLAGS) -c $^

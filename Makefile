FLAGS := -Wall -Werror -std=c++20

.default: all

all: firstfit bestfit

clean:
	rm -rf firstfit bestfit *o

firstfit: main.o loader.o allocator.o
	g++ $(FLAGS) -o $@ $^

bestfit: main.o loader.o allocator.o
	g++ $(FLAGS) -o $@ $^

%.o: %.cpp
	g++ $(FLAGS) -c $^

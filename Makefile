FIRSTFIT := firstfit
BESTFIT := bestfit

FIRSTFIT_DIR 	 := ./$(FIRSTFIT)_files
BESTFIT_DIR := ./$(BESTFIT)_files

FLAGS := -Wall -Werror -std=c++20

.default: all

all: firstfit bestfit

clean:
	rm -rf $(FIRSTFIT) $(BESTFIT) $(FIRSTFIT_DIR)/*o $(BESTFIT_DIR)/*o

firstfit: $(FIRSTFIT_DIR)/main.o
	g++ $(FLAGS) $@ $^

bestfit: $(BESTFIT_DIR)/main.o
	g++ $(FLAGS) $@ $^

$(FIRSTFIT_DIR)/%.o: %.cpp
	g++ $(FLAGS) $^

$(BESTFIT_DIR)/%.o: %.cpp
	g++ $(FLAGS)c $^
SRC=$(wildcard *.cpp)
TARGETS=$(SRC:.cpp=)

% : %.cpp
	g++  -g -O0  $< -o $@

all : $(TARGETS)

clean:
	rm -rfv $(TARGETS)

.PHONY: all clean
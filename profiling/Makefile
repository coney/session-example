TARGETS = io-bound cpu-bound cpu-bound-opti

all: $(TARGETS)

clean:
	rm -fv $(TARGETS)

$(TARGETS): $(addsuffix .cpp,$(TARGETS))
	$(CXX) -o $@ $@.cpp -O2


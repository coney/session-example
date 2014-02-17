TARGETS = daemon signal_receiver

all: $(TARGETS)

clean:
	rm -fv $(TARGETS)

$(TARGETS): $(addsuffix .cpp,$(TARGETS))
	$(CXX) -o $@ $@.cpp


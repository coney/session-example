TARGETS = daemon signal_receiver daemon-nodup

all: $(TARGETS)

clean:
	rm -fv $(TARGETS)

$(TARGETS): $(addsuffix .cpp,$(TARGETS))
	$(CXX) -o $@ $@.cpp


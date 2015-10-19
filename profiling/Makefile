TARGET = example
GPROF_TARGET=example-gprof
GPERF_TARGET=example-gperf
FLAGS = -g -O2

all: $(TARGET) $(GPROF_TARGET) $(GPERF_TARGET)

clean:
	rm -fv $(TARGET) $(GPROF_TARGET) $(GPERF_TARGET)

$(TARGET): example.cpp
	$(CXX) $(FLAGS) -o $@ $<

$(GPROF_TARGET): example.cpp
	$(CXX) $(FLAGS) -pg -o $@ $<

$(GPERF_TARGET): example.cpp
	$(CXX) $(FLAGS) -DGPERF -o $@ $< -lprofiler

CMOCK=$(shell readlink -f ..)
CWD=$(shell pwd)

#GMOCK_DIR=../gmock
GMOCK_INC=-I$(GMOCK_DIR)
GMOCK_SRC=$(GMOCK_DIR)/*.cc

CFLAGS+=$(shell $(CMOCK)/bin/cmock-config --cflags) $(GMOCK_INC)
#CPPFLAGS+=$(GMOCK_INC)
LDFLAGS+=$(shell $(CMOCK)/bin/cmock-config --libs $(GMOCK) $(GTEST))

SRCS=$(shell ls *.cc)
OBJS=$(SRCS:.cc=.o)

all: libfoo.so cmock_test

foo.o: foo.c
	$(CC) -c -fPIC $^ -o $@

libfoo.so: foo.o
	$(CC) -shared -Wl,-soname,$(CWD)/$@ -o $@ $^

%.o: %.cc
	$(CXX) -c $(CFLAGS) $^ -o $@

cmock_test: $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) -pthread -L$(CWD) -lfoo $(GMOCK_SRC) $(GMOCK_INC)

clean:
	rm -f $(OBJS) cmock_test foo.o libfoo.so

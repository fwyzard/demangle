.PHONY: all clean

TARGETS := bin/demangleTest

all: $(TARGETS)

clean:
	rm -f $(TARGETS)

bin/demangleTest: test/test.cc include/demangle.h
	mkdir -p $(dir $(TARGETS))
	$(CXX) -std=c++20 -Iinclude $< -o $@

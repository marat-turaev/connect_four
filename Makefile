CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
APPNAME = bot
srcdir = src
builddir = bin

all: $(builddir)/main.o $(builddir)/field.o $(builddir)/strategy.o
	$(CXX) $(builddir)/main.o $(builddir)/field.o $(builddir)/strategy.o -o $(APPNAME)

$(builddir)/main.o: $(srcdir)/main.cpp
	mkdir $(builddir)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(builddir)/field.o: $(srcdir)/field.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(builddir)/strategy.o: $(srcdir)/strategy.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(APPNAME)
	rm -r bin/

run:
	$(APPNAME)

.PHONY: clean
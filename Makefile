PROJECTNAME := strawberry
CC := g++
SRCDIR := src
BINDIR := bin
BUILDDIR := build
INCDIR := include
TESTDIR := test
TARGET := $(BINDIR)/$(PROJECTNAME)

SOURCES := $(shell find $(SRCDIR) -name *.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.cpp=.o))
CFLAGS := -Wall -O2 -std=c++11

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -I $(INCDIR) -c -o $@ $<

$(BUILDDIR)/%.test.o: $(TESTDIR)/%.test.cpp
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -I $(INCDIR) -c -o $@ $<

$(BINDIR)/%.test: $(BUILDDIR)/%.test.o $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $@

%.test: $(BINDIR)/%.test
	$<

clean:
	@echo " Cleaning..."
	$(RM) -r $(BUILDDIR) $(BINDIR)

.PHONY: clean
.PRECIOUS: $(BUILDDIR)/%.o $(BUILDDIR)/%.test.o $(BINDIR)/%.test

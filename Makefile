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
CFLAGS := -O2 -std=c++11
CCC := $(CC) $(CFLAGS) -I $(INCDIR)

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp include/ast.h
	@mkdir -p $(BUILDDIR)
	$(CCC) -c -o $@ $<

$(BUILDDIR)/%.test.o: $(TESTDIR)/%.test.cpp
	@mkdir -p $(BUILDDIR)
	$(CCC) -c -o $@ $<

$(BINDIR)/%.test: $(BUILDDIR)/%.test.o $(filter-out $(BUILDDIR)/main.o, $(OBJECTS))
	@mkdir -p $(BINDIR)
	$(CC) $^ -o $@

%.test: $(BINDIR)/%.test
	$<
assembler.test: codegen.test
disassembler.test: assembler.test
vm.test: assembler.test
clean:
	@echo " Cleaning..."
	$(RM) -rf .depend $(BUILDDIR) $(BINDIR)

depend: .depend

.depend: $(SOURCES)
	@rm -f .depend
	@$(foreach src,$(SOURCES),$(CCC) -MM $(src) -MT $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(src:.cpp=.o)) >> .depend;)


-include .depend


.PHONY: clean
.PRECIOUS: $(BUILDDIR)/%.o $(BUILDDIR)/%.test.o $(BINDIR)/%.test

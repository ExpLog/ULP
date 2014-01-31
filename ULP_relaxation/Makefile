# Combinatorial Optimization - Uncapacitated Facility Location
#
# Authors: Ian Herszterg, Leonardo Fontoura, Pedro Grojsgold, Rafael Rocha
#
# Makefile


TARGET=ulp

SRCDIR=src
OBJDIR=obj
BINDIR=bin

#CXX=g++

#CFLAGS = -Wall -ansi -pedantic

CDEBUG = -g -DDEBUG

ifeq ($(DEBUG),yes)
	CFLAGS += $(CDEBUG)
	LDFLAGS += -g
endif

ifeq ($(COVERAGE),yes)
	CFLAGS += -fprofile-arcs -ftest-coverage
endif


all: $(BINDIR)/$(TARGET) $(BINDIR)/convert

$(OBJDIR):
	mkdir -p $@

$(BINDIR):
	mkdir -p $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) -c $(CFLAGS) -o $@ $^

$(BINDIR)/$(TARGET): $(OBJDIR)/main.o $(OBJDIR)/solver.o $(OBJDIR)/input.o $(OBJDIR)/matrix.o | $(BINDIR)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BINDIR)/convert: $(OBJDIR)/convert.o $(OBJDIR)/matrix.o $(OBJDIR)/input.o | $(BINDIR)
	$(CXX) -o $@ $^ $(LDFLAGS)

debug:
	$(MAKE) $(MAKEFILE) DEBUG=yes

coverage:
	$(MAKE) $(MAKEFILE) COVERAGE=yes

clean:
	$(RM) -r $(OBJDIR) $(BINDIR)
	$(RM) *.gcda *.gcno *.gcov

.PHONY: debug clean test


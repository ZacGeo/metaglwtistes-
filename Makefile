CC ?= gcc
BISON ?= bison
FLEX ?= flex
CFLAGS ?= -std=c11 -Wall -Wextra -pedantic -g

TARGET = cpp_stage2
SOURCES = parser.tab.c lex.yy.c ast.c types.c semantic.c symtab.c diagnostics.c

.PHONY: all clean test analysis

all: $(TARGET)

parser.tab.c: parser.y
	$(BISON) -d -v -o parser.tab.c parser.y

parser.tab.h: parser.tab.c
	@test -f parser.tab.h

lex.yy.c: lexer.l parser.tab.h
	$(FLEX) -o lex.yy.c lexer.l

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $(SOURCES)

test: $(TARGET)
	./$(TARGET) -q tests/valid_scalar.cpp
	@if ./$(TARGET) -q tests/error_undeclared.cpp; then exit 1; else echo "expected semantic rejection: OK"; fi
	@if ./$(TARGET) -q tests/error_type.cpp; then exit 1; else echo "expected semantic rejection: OK"; fi
	@if ./$(TARGET) -q tests/error_break.cpp; then exit 1; else echo "expected semantic rejection: OK"; fi

analysis: parser.tab.c lex.yy.c
	@echo "Generated parser.output, parser.tab.c/h and lex.yy.c"

clean:
	rm -f $(TARGET) parser.tab.c parser.tab.h parser.output lex.yy.c

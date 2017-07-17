AST_OBJ = ast.o
PROG_OBJ = program.o
PROC_OBJ = procedure.o
OBJ_CORE = error-display.o main.o user-options.o
OBJ_GEN = scanner.o parser.o

HEADERS_CORE = error-display.hh user-options.hh ast.hh program.hh procedure.hh common-classes.hh
HEADERS_GEN = parser.h parser.ih parserbase.h scanner.h scanner.ih scannerbase.h

HEADERS = $(HEADERS_CORE) $(HEADERS_GEN)
OBJ = $(OBJ_GEN) $(OBJ_CORE) $(AST_OBJ) $(PROC_OBJ) $(PROG_OBJ)

CPP = g++
BISONCPP = bisonc++
FLEXCPP = flexc++
TGT = sclp

option = -DCOMPILE
CFLAGS = -g

$(TGT): $(OBJ) 
	$(CPP) --std=c++0x $(option) $(OBJ) -pg -o $(TGT)

%.o: %.cc $(HEADERS)
	$(CPP) --std=c++0x $(option) $(CFLAGS) -c $<

scanner.cc scannerbase.h: scanner.ll scanner.ih scanner.h
	$(FLEXCPP) scanner.ll

parser.cc parserbase.h: parser.yy parser.ih parser.h
	$(BISONCPP) --verbose parser.yy

clean :
	rm -f *.o scannerbase.h parserbase.h scanner.cc parser.cc parser.yy.output
	rm -f sclp
	rm -f cscope.out tags a.out

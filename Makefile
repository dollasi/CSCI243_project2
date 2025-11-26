#
# Created by gmakemake (Ubuntu Apr 19 2024) on Tue Nov 25 19:17:06 2025
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak

CFLAGS = -std=c99 -ggdb -Wall -Wextra -pedantic

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	interp.c parser.c stack.c symtab.c test_stack.c test_symtab.c tree_node.c
PS_FILES =	
S_FILES =	
H_FILES =	interp.h parser.h stack.h stack_node.h symtab.h tree_node.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	parser.o stack.o symtab.o tree_node.o 

#
# Main targets
#

all:	interp test_stack test_symtab 

interp:	interp.o $(OBJFILES)
	$(CC) $(CFLAGS) -o interp interp.o $(OBJFILES) $(CLIBFLAGS)

test_stack:	test_stack.o $(OBJFILES)
	$(CC) $(CFLAGS) -o test_stack test_stack.o $(OBJFILES) $(CLIBFLAGS)

test_symtab:	test_symtab.o $(OBJFILES)
	$(CC) $(CFLAGS) -o test_symtab test_symtab.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

interp.o:	interp.h parser.h stack.h stack_node.h symtab.h tree_node.h
parser.o:	parser.h stack.h stack_node.h symtab.h tree_node.h
stack.o:	stack.h stack_node.h
symtab.o:	symtab.h
test_stack.o:	stack.h stack_node.h
test_symtab.o:	symtab.h
tree_node.o:	symtab.h tree_node.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) interp.o test_stack.o test_symtab.o core

realclean:        clean
	-/bin/rm -f interp test_stack test_symtab 

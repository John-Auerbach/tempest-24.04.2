# MAKE FILE OF ATTITUDE ROUTINES.
#
# Apostolos Lerios - TOLIS@NOVA.


# CONSTANT DEFINITIONS.

# Include dbx debugging symbols.

DEBUG=-g

# Directories for targets and dependents.

OBJ=../obj/$(ARCH)/
TARGET=../lib/$(ARCH)/

# Header and make files.

ATTH=attitude.h
ATTMAKE=Makefile

# Shorthand for compiler invocation.

CC=cc
INCDIRS=-I../math
DEFINES=`/usr/bin/arch | awk '/sun4/ {print "-D_NO_PROTO"}'`
COMPILE=$(CC) $(INCDIRS) $(DEFINES) $(DEBUG) -c -o $@


# LIBRARY CREATION.

$(TARGET)libatt.a: $(OBJ)pyr.o $(OBJ)nasa.o
	ar ru $@ $(OBJ)pyr.o $(OBJ)nasa.o
	ranlib $@

# Compilation.

$(OBJ)pyr.o: pyr.c $(ATTH) $(ATTMAKE)
	 $(COMPILE) pyr.c

$(OBJ)nasa.o: nasa.c $(EARTHH) $(EARTHMAKE)
	 $(COMPILE) nasa.c


# TESTING CODE.

test: test.c
	$(CC) test.c $(INCDIRS) $(DEFINES) $(DEBUG) -o $@ -L$(TARGET) \
	-latt -lam -lm

verify: verify.c
	$(CC) verify.c $(INCDIRS) $(DEFINES) $(DEBUG) -o $@ -L$(TARGET) \
	-latt -lam -lm

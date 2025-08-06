# MAKE FILE OF ADVANCED MATH ROUTINES.
#
# Apostolos Lerios - TOLIS@NOVA.


# CONSTANT DEFINITIONS.

# Include dbx debugging symbols.

DEBUG=-g

# Directories for targets and dependents.

OBJ=../obj/$(ARCH)/
TARGET=../lib/$(ARCH)/

# Header and make files.

ORBITH=orbit.h
ORBITMAKE=Makefile

# Shorthand for compiler invocation.

CC=cc
INCDIRS=-I../math -I../earth
DEFINES=`/usr/bin/arch | awk '/sun4/ {print "-D_NO_PROTO"}'`
COMPILE=$(CC) $(INCDIRS) $(DEFINES) $(DEBUG) -c -o $@


# LIBRARY CREATION.

$(TARGET)liborbit.a: $(OBJ)sat.o $(OBJ)sun.o $(OBJ)moon.o
	ar ru $@ $(OBJ)sat.o $(OBJ)sun.o $(OBJ)moon.o
	ranlib $@

# Compilation.

$(OBJ)sat.o: sat.c $(ORBITH) $(ORBITMAKE)
	 $(COMPILE) sat.c

$(OBJ)sun.o: sun.c $(ORBITH) $(ORBITMAKE)
	 $(COMPILE) sun.c

$(OBJ)moon.o: moon.c $(ORBITH) $(ORBITMAKE)
	 $(COMPILE) moon.c


# TESTING CODE.

test: test.c
	$(CC) test.c $(INCDIRS) $(DEFINES) $(DEBUG) -o $@ -L$(TARGET) \
	-lorbit -learth -lam -lm

verify: verify.c
	$(CC) verify.c $(INCDIRS) $(DEFINES) $(DEBUG) -o $@ -L$(TARGET) \
	-lorbit -learth -lam -lm

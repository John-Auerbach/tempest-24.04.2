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

MATHH=advmath.h
USERH=user.h
MATHMAKE=Makefile

# Shorthand for compiler invocation.

CC=cc
INCDIRS=
DEFINES=`/usr/bin/arch | awk '/sun4/ {print "-D_NO_PROTO"}'`
COMPILE=$(CC) $(INCDIRS) $(DEFINES) $(DEBUG) -c -o $@

# LIBRARY CREATION.

$(TARGET)libam.a: $(OBJ)matrix.o $(OBJ)quat.o $(OBJ)schmidt.o $(OBJ)sph.o \
	$(OBJ)eqn.o $(OBJ)alf.o $(OBJ)vector.o $(OBJ)util.o
	ar ru $@ $(OBJ)matrix.o $(OBJ)quat.o $(OBJ)schmidt.o $(OBJ)sph.o \
	$(OBJ)eqn.o $(OBJ)alf.o $(OBJ)vector.o $(OBJ)util.o
	ranlib $@

# Compilation.

$(OBJ)matrix.o: matrix.c $(MATHH) $(MATHMAKE)
	 $(COMPILE) matrix.c

$(OBJ)quat.o: quat.c $(MATHH) $(MATHMAKE)
	 $(COMPILE) quat.c

$(OBJ)schmidt.o: schmidt.c $(MATHH) $(MATHMAKE)
	 $(COMPILE) schmidt.c

$(OBJ)sph.o: sph.c $(MATHH) $(MATHMAKE) $(USERH)
	 $(COMPILE) sph.c

$(OBJ)eqn.o: eqn.c $(MATHH) $(MATHMAKE) $(USERH)
	 $(COMPILE) eqn.c

$(OBJ)alf.o: alf.c $(MATHH) $(MATHMAKE)
	 $(COMPILE) alf.c

$(OBJ)vector.o: vector.c $(MATHH) $(MATHMAKE) $(USERH)
	 $(COMPILE) vector.c

$(OBJ)util.o: util.c $(MATHH) $(MATHMAKE)
	 $(COMPILE) util.c


# TESTING CODE.

verify: verify.c
	$(CC) verify.c $(INCDIRS) $(DEFINES) $(DEBUG) -o $@ -L$(TARGET) \
	-lam -lm

test: test.c
	$(CC) test.c $(INCDIRS) $(DEFINES) $(DEBUG) -o $@ -L$(TARGET) \
	-lam -lm

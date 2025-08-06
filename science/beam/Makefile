# MAKE FILE OF ELECTRON BEAM TRACER.
#
# Apostolos Lerios - TOLIS@NOVA.


# CONSTANT DEFINITIONS.

# Include dbx debugging symbols.

DEBUG=-g

# Directories for targets and dependents.

OBJ=../obj/$(ARCH)/
TARGET=../lib/$(ARCH)/

# Header and make files.

BEAMH=beam.h
USERH=user.h
BEAMMAKE=Makefile

# Shorthand for compiler invocation.

CC=cc
INCDIRS=-I../math -I../solid
DEFINES=`/usr/bin/arch | awk '/sun4/ {print "-D_NO_PROTO"}'`
COMPILE=$(CC) $(INCDIRS) $(DEFINES) $(DEBUG) -c -o $@


# LIBRARY CREATION.

$(TARGET)libbeam.a: $(OBJ)gen.o $(OBJ)beam.o
	ar ru $@ $(OBJ)gen.o $(OBJ)beam.o
	ranlib $@

# Compilation.

$(OBJ)gen.o: gen.c $(BEAMH) $(BEAMMAKE)
	 $(COMPILE) gen.c

$(OBJ)beam.o: beam.c $(BEAMH) $(BEAMMAKE) $(USERH)
	 $(COMPILE) beam.c


# TESTING CODE.

test: test.c
	$(CC) test.c $(INCDIRS) $(DEFINES) $(DEBUG) -o $@ -L$(TARGET) \
	-lbeam -lsolid -lam -lm

verify: verify.c
	$(CC) verify.c $(INCDIRS) $(DEFINES) $(DEBUG) -o $@ -L$(TARGET) \
	-lbeam -lsolid -lam -lm

# MAKE FILE OF CAMERA/GENERATOR/BODY FILE READER.
#
# Apostolos Lerios - TOLIS@NOVA.


# CONSTANT DEFINITIONS.

# Include dbx debugging symbols.

DEBUG=-g

# Directories for targets and dependents.

OBJ=../obj/$(ARCH)/
TARGET=../lib/$(ARCH)/

# Header and make files.

READERH=reader.h
READERMAKE=Makefile

# Shorthand for compiler invocation.

CC=cc
INCDIRS=-I../math -I../solid -I../strsp -I../beam
DEFINES=`/usr/bin/arch | awk '/sun4/ {print "-D_NO_PROTO"}'`
COMPILE=$(CC) $(INCDIRS) $(DEFINES) $(DEBUG) -c -o $@


# LIBRARY CREATION.

$(TARGET)libreader.a: $(OBJ)camera.o $(OBJ)extvertex.o $(OBJ)extsurf.o \
	$(OBJ)extbody.o $(OBJ)extgen.o $(OBJ)reader.o
	ar ru $@ $(OBJ)camera.o $(OBJ)extvertex.o $(OBJ)extsurf.o \
	$(OBJ)extbody.o $(OBJ)extgen.o $(OBJ)reader.o
	ranlib $@

# Compilation.

$(OBJ)camera.o: camera.c $(READERH) $(READERMAKE)
	 $(COMPILE) camera.c

$(OBJ)extvertex.o: extvertex.c $(READERH) $(READERMAKE)
	 $(COMPILE) extvertex.c

$(OBJ)extsurf.o: extsurf.c $(READERH) $(READERMAKE)
	 $(COMPILE) extsurf.c

$(OBJ)extbody.o: extbody.c $(READERH) $(READERMAKE)
	 $(COMPILE) extbody.c

$(OBJ)extgen.o: extgen.c $(READERH) $(READERMAKE)
	 $(COMPILE) extgen.c

$(OBJ)reader.o: reader.c $(READERH) $(READERMAKE)
	 $(COMPILE) reader.c


# TESTING CODE.

test: test.c
	$(CC) test.c $(INCDIRS) $(DEFINES) $(DEBUG) -o $@ -L$(TARGET) \
	-lreader -lbeam -lsolid -lam -lstrsp -lm

verify: verify.c
	$(CC) verify.c $(INCDIRS) $(DEFINES) $(DEBUG) -o $@ -L$(TARGET) \
	-lreader -lbeam -lsolid -lam -lstrsp -lm

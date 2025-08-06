# MAKE FILE OF SOLID OBJECT MANIPULATION.
#
# Apostolos Lerios - TOLIS@NOVA.


# CONSTANT DEFINITIONS.

# Include dbx debugging symbols.

DEBUG=-g

# Directories for targets and dependents.

OBJ=../obj/$(ARCH)/
TARGET=../lib/$(ARCH)/

# Header and make files.

SOLIDH=solid.h
USERH=user.h
SOLIDMAKE=Makefile

# Shorthand for compiler invocation.

CC=cc
INCDIRS=-I../math
DEFINES=`/usr/bin/arch | awk '/sun4/ {print "-D_NO_PROTO"}'`
COMPILE=$(CC) $(INCDIRS) $(DEFINES) $(DEBUG) -c -o $@


# LIBRARY CREATION.

$(TARGET)libsolid.a: $(OBJ)vertex.o $(OBJ)surf.o $(OBJ)body.o $(OBJ)tree.o \
	$(OBJ)planes.o
	ar ru $@ $(OBJ)vertex.o $(OBJ)surf.o $(OBJ)body.o $(OBJ)tree.o \
	$(OBJ)planes.o
	ranlib $@

# Compilation.

$(OBJ)vertex.o: vertex.c $(SOLIDH) $(SOLIDMAKE)
	 $(COMPILE) vertex.c

$(OBJ)surf.o: surf.c $(SOLIDH) $(SOLIDMAKE)
	 $(COMPILE) surf.c

$(OBJ)body.o: body.c $(SOLIDH) $(SOLIDMAKE)
	 $(COMPILE) body.c

$(OBJ)tree.o: tree.c $(SOLIDH) $(SOLIDMAKE) $(USERH)
	 $(COMPILE) tree.c

$(OBJ)planes.o: planes.c $(SOLIDH) $(SOLIDMAKE) $(USERH)
	 $(COMPILE) planes.c


# TESTING CODE.

test: test.c
	$(CC) test.c $(INCDIRS) $(DEFINES) $(DEBUG) -o $@ -L$(TARGET) \
	-lsolid -lam -lm

verify: verify.c
	$(CC) verify.c $(INCDIRS) $(DEFINES) $(DEBUG) -o $@ -L$(TARGET) \
	-lsolid -lam -lm

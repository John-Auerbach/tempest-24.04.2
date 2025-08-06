# MAKE FILE OF STRING SPACE MANIPULATION.
#
# Apostolos Lerios - TOLIS@NOVA.


# CONSTANT DEFINITIONS.

# Include dbx debugging symbols.

DEBUG=-g

# Directories for targets and dependents.

OBJ=../obj/$(ARCH)/
TARGET=../lib/$(ARCH)/

# Header and make files.

STRSPH=strsp.h
STRSPMAKE=Makefile

# Shorthand for compiler invocation.

CC=cc
INCDIRS=
DEFINES=`arch | awk '/sun4/ {print "-D_NO_PROTO"}'`
COMPILE=$(CC) $(INCDIRS) $(DEFINES) $(DEBUG) -c -o $@



# LIBRARY CREATION.

$(TARGET)libstrsp.a: $(OBJ)strsp.o
	ar ru $@ $(OBJ)strsp.o
	ranlib $@

# Compilation.

$(OBJ)strsp.o: strsp.c $(STRSPH) $(STRSPMAKE)
	 $(COMPILE) strsp.c


# TESTING CODE.

test: test.c
	$(CC) test.c $(INCDIRS) $(DEFINES) $(DEBUG) -o $@ -L$(TARGET) \
	-lstrsp

verify: verify.c
	$(CC) verify.c $(INCDIRS) $(DEFINES) $(DEBUG) -o $@ -L$(TARGET) \
	-lstrsp

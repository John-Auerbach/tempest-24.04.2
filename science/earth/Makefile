# MAKE FILE OF EARTH MODELLING ROUTINES.
#
# Apostolos Lerios - TOLIS@NOVA.


# CONSTANT DEFINITIONS.

# Include dbx debugging symbols.

DEBUG=-g

# Directories for targets and dependents.

OBJ=../obj/$(ARCH)/
TARGET=../lib/$(ARCH)/

# Header and make files.

EARTHH=earth.h
USERH=user.h
EARTHMAKE=Makefile

# Shorthand for compiler invocation.

CC=cc
INCDIRS=-I../include/$(ARCH) -I../math
DEFINES=`/usr/bin/arch | awk '/sun4/ {print "-D_NO_PROTO"}'`
COMPILE=$(CC) $(INCDIRS) $(DEFINES) $(DEBUG) -c -o $@


# LIBRARY CREATION.

$(TARGET)libearth.a: $(OBJ)bfield.o $(OBJ)coord.o $(OBJ)eciecr.o \
	$(OBJ)lvlheci.o $(OBJ)lvlhloc.o $(OBJ)eciearth.o $(OBJ)days.o \
	$(OBJ)eciecl.o
	ar ru $@ $(OBJ)bfield.o $(OBJ)coord.o $(OBJ)eciecr.o \
	$(OBJ)lvlheci.o $(OBJ)lvlhloc.o $(OBJ)eciearth.o $(OBJ)days.o \
	$(OBJ)eciecl.o
	ranlib $@

# Compilation.

$(OBJ)bfield.o: bfield.c $(EARTHH) $(EARTHMAKE) $(USERH)
	 $(COMPILE) bfield.c

$(OBJ)coord.o: coord.c $(EARTHH) $(EARTHMAKE)
	 $(COMPILE) coord.c

$(OBJ)eciecr.o: eciecr.c $(EARTHH) $(EARTHMAKE)
	 $(COMPILE) eciecr.c

$(OBJ)eciecl.o: eciecl.c $(EARTHH) $(EARTHMAKE)
	 $(COMPILE) eciecl.c

$(OBJ)lvlheci.o: lvlheci.c $(EARTHH) $(EARTHMAKE)
	 $(COMPILE) lvlheci.c

$(OBJ)lvlhloc.o: lvlhloc.c $(EARTHH) $(EARTHMAKE)
	 $(COMPILE) lvlhloc.c

$(OBJ)eciearth.o: eciearth.c $(EARTHH) $(EARTHMAKE)
	 $(COMPILE) eciearth.c

$(OBJ)days.o: days.c $(EARTHH) $(EARTHMAKE)
	 $(COMPILE) days.c


# TESTING CODE.

test: test.c
	$(CC) test.c $(INCDIRS) $(DEFINES) $(DEBUG) -o $@ -L$(TARGET) \
	-learth -lam -lm

verify: verify.c
	$(CC) verify.c $(INCDIRS) $(DEFINES) $(DEBUG) -o $@ -L$(TARGET) \
	-learth -lam -lm

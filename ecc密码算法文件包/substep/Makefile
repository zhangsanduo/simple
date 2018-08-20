export IDIR LDIR DEFS

IDIR            += ./include
IDIR            += ../dependent/source/include

CFLAGS          := $(addprefix -I, $(IDIR))
CFLAGS          += $(addprefix -L, $(LDIR))
CFLAGS          += $(addprefix -D, $(DEFS))
CFLAGS          += -shared -fPIC -O2


include rule.mk

DIR_OBJ         = ../dependent/.obj

IDIR            += ../dependent/source/include/  ./

CFLAGS		+= $(addprefix -I, $(IDIR))
CFLAGS		+= $(addprefix -D, $(DEFS))

LIBA		:=  ../dependent/.obj/libgmsdf.a 
LIBS		:= pthread  dl

LDFLAGS		:= $(addprefix -L, $(LDIR))
LDFLAGS		+= $(addprefix -l, $(LIBS))

OBJ            = $(patsubst %.c,${DIR_OBJ}/%.o,$(notdir $(wildcard *.c)))

.PHONY: all


all:  $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) $(LIBA) -o ecc

${DIR_OBJ}/%.o:%.c
	test -d $(DIR_OBJ) || mkdir -p $(DIR_OBJ)
	$(CC) $(CFLAGS) -c  $< -o $@

clean:
	$(RM) *.o ecc ./.obj/*.o
